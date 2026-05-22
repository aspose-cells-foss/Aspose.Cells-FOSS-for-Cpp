#include "Support/PackageTestSupport.h"

#include "Support/TestSupport.h"

#include "aspose/cells_foss/XmlDocument.h"
#include "aspose/cells_foss/ZipArchive.h"

#include <algorithm>
#include <cstdint>
#include <stdexcept>

namespace Aspose::Cells_FOSS::Tests::Package {
namespace {

std::string NormalizeEntryPath(std::string_view entryPath)
{
    std::string result(entryPath);
    while (!result.empty() && (result.front() == '/' || result.front() == '\\')) {
        result.erase(result.begin());
    }
    std::replace(result.begin(), result.end(), '\\', '/');
    return result;
}

std::uint16_t ReadUInt16(const std::vector<std::uint8_t>& bytes, std::size_t offset)
{
    if (offset + 2 > bytes.size()) {
        throw std::runtime_error("Invalid zip data while reading uint16.");
    }
    return static_cast<std::uint16_t>(bytes[offset])
        | (static_cast<std::uint16_t>(bytes[offset + 1]) << 8);
}

std::uint32_t ReadUInt32(const std::vector<std::uint8_t>& bytes, std::size_t offset)
{
    if (offset + 4 > bytes.size()) {
        throw std::runtime_error("Invalid zip data while reading uint32.");
    }
    return static_cast<std::uint32_t>(bytes[offset])
        | (static_cast<std::uint32_t>(bytes[offset + 1]) << 8)
        | (static_cast<std::uint32_t>(bytes[offset + 2]) << 16)
        | (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
}

void WriteArchive(const std::filesystem::path& packagePath, const ZipArchive& archive)
{
    WriteAllBytes(packagePath, archive.ToZipBytes());
}

ZipArchive ReadArchive(const std::filesystem::path& packagePath)
{
    return ZipArchive::FromZipBytes(ReadAllBytes(packagePath));
}

void RebuildArchive(const std::filesystem::path& packagePath,
                    const std::function<bool(const std::string&, std::string&)>& mapName)
{
    const auto bytes = ReadAllBytes(packagePath);
    const auto archive = ZipArchive::FromZipBytes(bytes);
    ZipArchive rebuilt;

    for (const auto& name : ListEntryNames(bytes)) {
        std::string destination = name;
        if (!mapName(name, destination)) {
            continue;
        }
        const auto entry = archive.GetEntry(name);
        if (!entry.IsNull()) {
            rebuilt.AddEntry(destination, entry.Open());
        }
    }

    WriteArchive(packagePath, rebuilt);
}

}  // namespace

bool EntryExists(const std::filesystem::path& packagePath, std::string_view entryPath)
{
    return !ReadArchive(packagePath).GetEntry(NormalizeEntryPath(entryPath)).IsNull();
}

std::string ReadEntryText(const std::filesystem::path& packagePath, std::string_view entryPath)
{
    auto archive = ReadArchive(packagePath);
    auto entry = archive.GetEntry(NormalizeEntryPath(entryPath));
    if (entry.IsNull()) {
        throw std::runtime_error("Missing zip entry: " + NormalizeEntryPath(entryPath));
    }
    return BytesToString(entry.Open());
}

void RewriteEntryText(const std::filesystem::path& packagePath,
                      std::string_view entryPath,
                      const std::function<std::string(std::string)>& rewrite)
{
    auto archive = ReadArchive(packagePath);
    const auto normalizedPath = NormalizeEntryPath(entryPath);
    auto entry = archive.GetEntry(normalizedPath);
    if (entry.IsNull()) {
        throw std::runtime_error("Missing zip entry: " + normalizedPath);
    }

    archive.AddEntry(normalizedPath, StringToBytes(rewrite(BytesToString(entry.Open()))));
    WriteArchive(packagePath, archive);
}

void RewriteXmlEntry(const std::filesystem::path& packagePath,
                     std::string_view entryPath,
                     const std::function<void(XmlElement&)>& mutateRoot)
{
    auto archive = ReadArchive(packagePath);
    const auto normalizedPath = NormalizeEntryPath(entryPath);
    auto entry = archive.GetEntry(normalizedPath);
    if (entry.IsNull()) {
        throw std::runtime_error("Missing zip entry: " + normalizedPath);
    }

    auto document = XmlDocument::Load(entry.Open());
    auto root = document.GetRoot();
    mutateRoot(root);
    archive.AddEntry(normalizedPath, XmlDocument::Build(root).SaveToUtf8());
    WriteArchive(packagePath, archive);
}

void DeleteEntry(const std::filesystem::path& packagePath, std::string_view entryPath)
{
    const auto normalizedPath = NormalizeEntryPath(entryPath);
    RebuildArchive(packagePath, [&](const std::string& name, std::string& destination) {
        destination = name;
        return name != normalizedPath;
    });
}

void MoveEntry(const std::filesystem::path& packagePath,
               std::string_view sourceEntryPath,
               std::string_view destinationEntryPath)
{
    const auto source = NormalizeEntryPath(sourceEntryPath);
    const auto destination = NormalizeEntryPath(destinationEntryPath);
    auto found = false;
    RebuildArchive(packagePath, [&](const std::string& name, std::string& mapped) {
        if (name == destination) {
            return false;
        }
        if (name == source) {
            found = true;
            mapped = destination;
            return true;
        }
        mapped = name;
        return true;
    });

    if (!found) {
        throw std::runtime_error("Missing zip entry: " + source);
    }
}

void CreatePackage(const std::filesystem::path& packagePath,
                   const std::map<std::string, std::string>& entries)
{
    ZipArchive archive;
    for (const auto& entry : entries) {
        archive.AddEntry(NormalizeEntryPath(entry.first), StringToBytes(entry.second));
    }
    WriteArchive(packagePath, archive);
}

std::vector<std::string> ListEntryNames(const std::vector<std::uint8_t>& zipBytes)
{
    constexpr std::uint32_t eocdSignature = 0x06054b50;
    constexpr std::uint32_t centralDirectorySignature = 0x02014b50;

    if (zipBytes.size() < 22) {
        throw std::runtime_error("Invalid zip data: missing EOCD.");
    }

    std::size_t eocdOffset = std::string::npos;
    for (std::size_t offset = zipBytes.size() - 22;; --offset) {
        if (ReadUInt32(zipBytes, offset) == eocdSignature) {
            eocdOffset = offset;
            break;
        }
        if (offset == 0) {
            break;
        }
    }

    if (eocdOffset == std::string::npos) {
        throw std::runtime_error("Invalid zip data: EOCD not found.");
    }

    const auto entryCount = ReadUInt16(zipBytes, eocdOffset + 10);
    auto centralOffset = static_cast<std::size_t>(ReadUInt32(zipBytes, eocdOffset + 16));
    std::vector<std::string> names;
    names.reserve(entryCount);

    for (std::uint16_t index = 0; index < entryCount; ++index) {
        if (ReadUInt32(zipBytes, centralOffset) != centralDirectorySignature) {
            throw std::runtime_error("Invalid zip data: central directory signature mismatch.");
        }

        const auto nameLength = ReadUInt16(zipBytes, centralOffset + 28);
        const auto extraLength = ReadUInt16(zipBytes, centralOffset + 30);
        const auto commentLength = ReadUInt16(zipBytes, centralOffset + 32);
        const auto nameOffset = centralOffset + 46;
        if (nameOffset + nameLength > zipBytes.size()) {
            throw std::runtime_error("Invalid zip data: central directory entry name exceeds buffer.");
        }

        names.emplace_back(reinterpret_cast<const char*>(zipBytes.data() + nameOffset), nameLength);
        centralOffset = nameOffset + nameLength + extraLength + commentLength;
    }

    return names;
}

}  // namespace Aspose::Cells_FOSS::Tests::Package
