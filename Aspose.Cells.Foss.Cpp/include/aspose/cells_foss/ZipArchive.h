#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Represents an entry in a ZipArchive. Default-constructed instances are null.
class ZipArchiveEntry {
public:
    ZipArchiveEntry();

    bool IsNull() const noexcept;

    /// Reads the entry content.
    const std::vector<uint8_t>& Open() const;

    /// Gets the full name/path of the entry.
    const std::string& GetFullName() const noexcept;

private:
    std::string _fullName;
    std::vector<uint8_t> _data;
    bool _isNull;

    friend class ZipArchive;
    ZipArchiveEntry(std::string fullName, std::vector<uint8_t> data);
};

/// In-memory representation of a ZIP archive.
class ZipArchive {
public:
    ZipArchive();

    /// Parses a ZIP archive from raw bytes. Throws std::runtime_error on
    /// invalid or corrupt data.
    static ZipArchive FromZipBytes(const std::vector<uint8_t>& zipData);

    /// Serializes this archive to ZIP bytes using STORED (no compression).
    std::vector<uint8_t> ToZipBytes() const;

    /// Gets an entry by URI. The URI is normalized (leading '/' stripped, backslashes replaced).
    /// Returns a null ZipArchiveEntry if not found.
    ZipArchiveEntry GetEntry(std::string_view uri) const;

    /// Adds or replaces an entry.
    void AddEntry(std::string_view uri, std::vector<uint8_t> data);

    /// Gets the number of entries.
    size_t GetEntryCount() const noexcept;

private:
    std::unordered_map<std::string, std::vector<uint8_t>> _entries;
};

}}  // namespace Aspose::Cells_FOSS
