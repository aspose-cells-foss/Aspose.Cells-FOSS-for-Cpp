#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// EnsureSeekable
// ---------------------------------------------------------------------------

std::vector<uint8_t> XlsxWorkbookArchiveHelpers::EnsureSeekable(
    const std::vector<uint8_t>& stream)
{
    return stream;
}

// ---------------------------------------------------------------------------
// GetEntry
// ---------------------------------------------------------------------------

ZipArchiveEntry XlsxWorkbookArchiveHelpers::GetEntry(
    const ZipArchive& archive,
    std::string_view uri)
{
    return archive.GetEntry(uri);
}

// ---------------------------------------------------------------------------
// LoadDocument
// ---------------------------------------------------------------------------

XmlDocument XlsxWorkbookArchiveHelpers::LoadDocument(const ZipArchiveEntry& entry)
{
    if (entry.IsNull()) {
        return XmlDocument();
    }
    const auto& rawData = entry.Open();
    return XmlDocument::Load(rawData);
}

// ---------------------------------------------------------------------------
// LoadRelationships
// ---------------------------------------------------------------------------

std::unordered_map<std::string, std::string>
XlsxWorkbookArchiveHelpers::LoadRelationships(
    const ZipArchive& archive,
    std::string_view relationshipsUri,
    std::string_view sourceUri)
{
    auto entry = GetEntry(archive, relationshipsUri);
    if (entry.IsNull()) {
        return {};
    }

    auto document = LoadDocument(entry);
    std::unordered_map<std::string, std::string> relationships;

    const auto& root = document.GetRoot();
    if (!root.IsNull()) {
        const auto relElements = root.GetElements(
            XlsxWorkbookSerializerCommon::PackageRelationshipNs + "Relationship");
        for (const auto& rel : relElements) {
            const auto idAttr = rel.GetAttribute("Id");
            const auto targetAttr = rel.GetAttribute("Target");
            if (idAttr.IsNull() || idAttr.GetValue().empty() ||
                targetAttr.IsNull() || targetAttr.GetValue().empty()) {
                continue;
            }
            relationships[idAttr.GetValue()] = ResolvePartUri(sourceUri, targetAttr.GetValue());
        }
    }

    return relationships;
}

// ---------------------------------------------------------------------------
// FindRelationshipTarget
// ---------------------------------------------------------------------------

std::string XlsxWorkbookArchiveHelpers::FindRelationshipTarget(
    const std::unordered_map<std::string, std::string>& workbookRelationships,
    std::string_view expectedPath)
{
    for (const auto& pair : workbookRelationships) {
        if (RelationshipPointsTo(pair.second, expectedPath)) {
            return pair.second;
        }
    }
    return {};
}

// ---------------------------------------------------------------------------
// FindRelationshipTargetByType
// ---------------------------------------------------------------------------

std::string XlsxWorkbookArchiveHelpers::FindRelationshipTargetByType(
    const std::unordered_map<std::string, std::string>& workbookRelationships,
    std::string_view relationshipType)
{
    for (const auto& pair : workbookRelationships) {
        if (GetRelationshipTargetKind(pair.second) == relationshipType) {
            return pair.second;
        }
    }
    return {};
}

// ---------------------------------------------------------------------------
// LoadSharedStrings
// ---------------------------------------------------------------------------

std::vector<std::string> XlsxWorkbookArchiveHelpers::LoadSharedStrings(
    const ZipArchive& archive,
    const std::unordered_map<std::string, std::string>& workbookRelationships,
    const LoadOptions& options,
    LoadDiagnostics& diagnostics)
{
    (void)options;
    (void)diagnostics;

    std::string sharedStringsUri = FindRelationshipTarget(
        workbookRelationships, "/xl/sharedStrings.xml");
    if (sharedStringsUri.empty()) {
        if (workbookRelationships.empty()) {
            sharedStringsUri = "/xl/sharedStrings.xml";
        } else {
            sharedStringsUri = FindRelationshipTargetByType(
                workbookRelationships,
                XlsxWorkbookSerializerCommon::SharedStringsRelationshipType);
        }
    }

    if (sharedStringsUri.empty()) {
        sharedStringsUri = "/xl/sharedStrings.xml";
    }

    auto entry = GetEntry(archive, sharedStringsUri);
    if (entry.IsNull()) {
        return {};
    }

    auto document = LoadDocument(entry);
    std::vector<std::string> items;

    const auto& root = document.GetRoot();
    if (!root.IsNull()) {
        const auto siElements = root.GetElements(
            XlsxWorkbookSerializerCommon::MainNs + "si");
        items.reserve(siElements.size());
        for (const auto& item : siElements) {
            items.push_back(ReadInlineString(item));
        }
    }

    return items;
}

// ---------------------------------------------------------------------------
// LoadDateStyleIndexes
// ---------------------------------------------------------------------------

std::unordered_set<int> XlsxWorkbookArchiveHelpers::LoadDateStyleIndexes(
    const ZipArchive& archive,
    const std::unordered_map<std::string, std::string>& workbookRelationships,
    const LoadOptions& options,
    LoadDiagnostics& diagnostics)
{
    (void)options;
    (void)diagnostics;

    std::string stylesUri = FindRelationshipTarget(workbookRelationships, "/xl/styles.xml");
    if (stylesUri.empty()) {
        stylesUri = "/xl/styles.xml";
    }

    auto entry = GetEntry(archive, stylesUri);
    if (entry.IsNull()) {
        return {};
    }

    auto document = LoadDocument(entry);
    std::unordered_map<int, std::string> customFormats;

    const auto& root = document.GetRoot();
    if (!root.IsNull()) {
        auto numFmtsElement = root.GetElement(
            XlsxWorkbookSerializerCommon::MainNs + "numFmts");
        if (!numFmtsElement.IsNull()) {
            const auto numFmtElements = numFmtsElement.GetElements(
                XlsxWorkbookSerializerCommon::MainNs + "numFmt");
            for (const auto& numFmt : numFmtElements) {
                const auto id = ParseIntAttribute(numFmt.GetAttribute("numFmtId"));
                const auto codeAttr = numFmt.GetAttribute("formatCode");
                if (id.has_value() && !codeAttr.IsNull() && !codeAttr.GetValue().empty()) {
                    customFormats[id.value()] = codeAttr.GetValue();
                }
            }
        }
    }

    std::unordered_set<int> dateStyleIndexes;

    if (!root.IsNull()) {
        auto cellXfsElement = root.GetElement(
            XlsxWorkbookSerializerCommon::MainNs + "cellXfs");
        if (!cellXfsElement.IsNull()) {
            const auto cellXfs = cellXfsElement.GetElements(
                XlsxWorkbookSerializerCommon::MainNs + "xf");
            for (int index = 0; index < static_cast<int>(cellXfs.size()); ++index) {
                const auto numFmtId = ParseIntAttribute(cellXfs[index].GetAttribute("numFmtId"));
                if (!numFmtId.has_value()) {
                    continue;
                }

                if (XlsxWorkbookSerializerCommon::BuiltInDateFormats.count(numFmtId.value()) != 0) {
                    dateStyleIndexes.insert(index);
                    continue;
                }

                const auto it = customFormats.find(numFmtId.value());
                if (it != customFormats.end() && LooksLikeDateFormat(it->second)) {
                    dateStyleIndexes.insert(index);
                }
            }
        }
    }

    return dateStyleIndexes;
}

// ---------------------------------------------------------------------------
// ParseBoolAttribute
// ---------------------------------------------------------------------------

bool XlsxWorkbookArchiveHelpers::ParseBoolAttribute(const XmlAttribute& attribute)
{
    if (attribute.IsNull()) {
        return false;
    }
    const auto& value = attribute.GetValue();
    return value == "1" ||
           (value.size() == 4 &&
            (value[0] == 't' || value[0] == 'T') &&
            (value[1] == 'r' || value[1] == 'R') &&
            (value[2] == 'u' || value[2] == 'U') &&
            (value[3] == 'e' || value[3] == 'E'));
}

// ---------------------------------------------------------------------------
// ParseIntAttribute
// ---------------------------------------------------------------------------

std::optional<int> XlsxWorkbookArchiveHelpers::ParseIntAttribute(
    const XmlAttribute& attribute)
{
    if (attribute.IsNull()) {
        return std::nullopt;
    }
    const auto& value = attribute.GetValue();
    if (value.empty()) {
        return std::nullopt;
    }
    try {
        size_t pos = 0;
        const long result = std::stol(value, &pos, 10);
        if (pos != value.size()) {
            return std::nullopt;
        }
        if (result < std::numeric_limits<int>::min() || result > std::numeric_limits<int>::max()) {
            return std::nullopt;
        }
        return static_cast<int>(result);
    } catch (...) {
        return std::nullopt;
    }
}

// ---------------------------------------------------------------------------
// ParseDoubleAttribute
// ---------------------------------------------------------------------------

std::optional<double> XlsxWorkbookArchiveHelpers::ParseDoubleAttribute(
    const XmlAttribute& attribute)
{
    if (attribute.IsNull()) {
        return std::nullopt;
    }
    const auto& value = attribute.GetValue();
    if (value.empty()) {
        return std::nullopt;
    }
    try {
        size_t pos = 0;
        const double result = std::stod(value, &pos);
        if (pos != value.size()) {
            return std::nullopt;
        }
        return result;
    } catch (...) {
        return std::nullopt;
    }
}

// ---------------------------------------------------------------------------
// NormalizeFormula
// ---------------------------------------------------------------------------

std::string XlsxWorkbookArchiveHelpers::NormalizeFormula(std::string_view value)
{
    size_t start = 0;
    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start]))) {
        ++start;
    }
    size_t end = value.size();
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }

    if (start >= end) {
        return {};
    }

    std::string_view trimmed = value.substr(start, end - start);
    if (!trimmed.empty() && trimmed[0] == '=') {
        trimmed.remove_prefix(1);
    }
    return std::string(trimmed);
}

// ---------------------------------------------------------------------------
// ReadInlineString
// ---------------------------------------------------------------------------

std::string XlsxWorkbookArchiveHelpers::ReadInlineString(
    const XmlElement& inlineStringElement)
{
    if (inlineStringElement.IsNull()) {
        return {};
    }

    const auto mainNsT = XlsxWorkbookSerializerCommon::MainNs + "t";
    std::vector<std::string> textNodes;

    const auto descendants = inlineStringElement.GetDescendantsAndSelf();
    for (const auto& element : descendants) {
        if (element.GetName() == mainNsT) {
            textNodes.push_back(element.GetValue());
        }
    }

    if (!textNodes.empty()) {
        std::string result;
        size_t totalSize = 0;
        for (const auto& s : textNodes) {
            totalSize += s.size();
        }
        result.reserve(totalSize);
        for (const auto& s : textNodes) {
            result.append(s);
        }
        return result;
    }

    return inlineStringElement.GetValue();
}

// ---------------------------------------------------------------------------
// TryParseNumber
// ---------------------------------------------------------------------------

bool XlsxWorkbookArchiveHelpers::TryParseNumber(
    std::string_view rawValue,
    CellValue& numberValue)
{
    const std::string raw(rawValue);

    const bool hasExponent = raw.find('e') != std::string::npos ||
                             raw.find('E') != std::string::npos;

    if (!hasExponent) {
        std::istringstream iss(raw);
        iss.imbue(std::locale::classic());
        long double decValue = 0;
        iss >> decValue;
        if (!iss.fail() && iss.eof()) {
            const bool hasDot = raw.find('.') != std::string::npos;
            const bool hasComma = raw.find(',') != std::string::npos;
            if (!hasDot && !hasComma) {
                const auto truncated = std::trunc(decValue);
                if (truncated == decValue &&
                    decValue >= static_cast<long double>(std::numeric_limits<int>::min()) &&
                    decValue <= static_cast<long double>(std::numeric_limits<int>::max())) {
                    numberValue = CellValue(static_cast<int>(truncated));
                    return true;
                }
            }
            numberValue = CellValue(static_cast<double>(decValue));
            return true;
        }
    }

    try {
        size_t pos = 0;
        const double doubleValue = std::stod(raw, &pos);
        if (pos == raw.size()) {
            const double remainder = std::fmod(doubleValue, 1.0);
            if (std::abs(remainder) < std::numeric_limits<double>::epsilon() &&
                doubleValue >= static_cast<double>(std::numeric_limits<int>::min()) &&
                doubleValue <= static_cast<double>(std::numeric_limits<int>::max())) {
                numberValue = CellValue(static_cast<int>(doubleValue));
                return true;
            }
            numberValue = CellValue(doubleValue);
            return true;
        }
    } catch (...) {
        // Fall through to return false
    }

    return false;
}

// ---------------------------------------------------------------------------
// AddIssue
// ---------------------------------------------------------------------------

void XlsxWorkbookArchiveHelpers::AddIssue(
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    const LoadIssue& issue)
{
    diagnostics.Add(issue);
    const auto& callback = options.GetWarningCallback();
    if (callback) {
        WarningInfo info;
        info.SetCode(issue.GetCode());
        info.SetSeverity(issue.GetSeverity());
        info.SetMessage(issue.GetMessage());
        info.SetDataLossRisk(issue.GetDataLossRisk());
        info.SetPartUri(issue.GetPartUri());
        info.SetSheetName(issue.GetSheetName());
        info.SetCellRef(issue.GetCellRef());
        info.SetRowIndex(issue.GetRowIndex());
        callback->Warning(info);
    }
}

// ---------------------------------------------------------------------------
// ResolveWorksheetUri
// ---------------------------------------------------------------------------

std::string XlsxWorkbookArchiveHelpers::ResolveWorksheetUri(
    int sheetIndex,
    std::string_view relationshipId,
    const std::unordered_map<std::string, std::string>& workbookRelationships,
    const ZipArchive& archive,
    LoadDiagnostics& diagnostics,
    std::string_view sheetName,
    const LoadOptions& options)
{
    if (!relationshipId.empty()) {
        const auto it = workbookRelationships.find(std::string(relationshipId));
        if (it != workbookRelationships.end()) {
            return it->second;
        }
    }

    const std::string conventionalUri =
        "/xl/worksheets/sheet" + std::to_string(sheetIndex + 1) + ".xml";

    if (!GetEntry(archive, conventionalUri).IsNull()) {
        LoadIssue issue("PKG-R001",
                        DiagnosticSeverity::Recoverable,
                        "Worksheet relationship metadata was incomplete; the worksheet part was resolved by convention.",
                        true);
        issue.SetSheetName(sheetName);
        issue.SetPartUri(conventionalUri);
        AddIssue(diagnostics, options, issue);
    }

    return conventionalUri;
}

// ---------------------------------------------------------------------------
// ResolvePartUri
// ---------------------------------------------------------------------------

std::string XlsxWorkbookArchiveHelpers::ResolvePartUri(
    std::string_view sourceUri,
    std::string_view target)
{
    if (!target.empty() && target[0] == '/') {
        return std::string(target);
    }

    const auto lastSlash = sourceUri.rfind('/');
    std::string baseDir;
    if (lastSlash != std::string_view::npos) {
        baseDir = std::string(sourceUri.substr(0, lastSlash + 1));
    } else {
        baseDir = "/";
    }

    std::string combined = baseDir + std::string(target);

    std::vector<std::string> segments;
    std::istringstream iss(combined);
    std::string segment;
    while (std::getline(iss, segment, '/')) {
        if (segment.empty() || segment == ".") {
            continue;
        }
        if (segment == ".." && !segments.empty()) {
            segments.pop_back();
        } else if (segment != "..") {
            segments.push_back(std::move(segment));
        }
    }

    if (segments.empty()) {
        return "/";
    }

    std::string result;
    size_t totalLen = segments.size();
    for (const auto& seg : segments) {
        totalLen += seg.size();
    }
    result.reserve(totalLen);

    for (const auto& seg : segments) {
        result.push_back('/');
        result.append(seg);
    }
    return result;
}

// ---------------------------------------------------------------------------
// RelationshipPointsTo
// ---------------------------------------------------------------------------

bool XlsxWorkbookArchiveHelpers::RelationshipPointsTo(
    std::string_view candidate,
    std::string_view expectedUri)
{
    if (candidate.size() != expectedUri.size()) {
        return false;
    }
    for (size_t i = 0; i < candidate.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(candidate[i])) !=
            std::tolower(static_cast<unsigned char>(expectedUri[i]))) {
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// GetRelationshipTargetKind
// ---------------------------------------------------------------------------

std::string XlsxWorkbookArchiveHelpers::GetRelationshipTargetKind(
    std::string_view candidate)
{
    const std::string lowerCandidate = [&]() {
        std::string s(candidate);
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return s;
    }();

    if (lowerCandidate.size() >= 18 &&
        lowerCandidate.compare(lowerCandidate.size() - 18, 18, "/sharedstrings.xml") == 0) {
        return XlsxWorkbookSerializerCommon::SharedStringsRelationshipType;
    }

    if (lowerCandidate.size() >= 11 &&
        lowerCandidate.compare(lowerCandidate.size() - 11, 11, "/styles.xml") == 0) {
        return XlsxWorkbookSerializerCommon::StylesRelationshipType;
    }

    if (lowerCandidate.find("/worksheets/") != std::string::npos) {
        return XlsxWorkbookSerializerCommon::WorksheetRelationshipType;
    }

    return {};
}

// ---------------------------------------------------------------------------
// LooksLikeDateFormat
// ---------------------------------------------------------------------------

bool XlsxWorkbookArchiveHelpers::LooksLikeDateFormat(std::string_view formatCode)
{
    if (formatCode.empty()) {
        return false;
    }

    bool allWhitespace = true;
    for (char c : formatCode) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            allWhitespace = false;
            break;
        }
    }
    if (allWhitespace) {
        return false;
    }

    const std::string codeStr(formatCode);

    if (codeStr.find("[$-F400]") != std::string::npos ||
        codeStr.find("[$-F800]") != std::string::npos) {
        return true;
    }

    std::string normalized;
    normalized.reserve(formatCode.size());
    bool inQuote = false;
    bool inBracket = false;

    for (size_t index = 0; index < formatCode.size(); ++index) {
        const char ch = formatCode[index];

        if (ch == '"') {
            inQuote = !inQuote;
            continue;
        }

        if (inQuote) {
            continue;
        }

        if (ch == '[') {
            inBracket = true;
            continue;
        }

        if (ch == ']' && inBracket) {
            inBracket = false;
            continue;
        }

        if (inBracket) {
            normalized.push_back(static_cast<char>(
                std::tolower(static_cast<unsigned char>(ch))));
            continue;
        }

        if (ch == '\\' || ch == '_') {
            ++index;
            continue;
        }

        if (ch == '*') {
            continue;
        }

        normalized.push_back(static_cast<char>(
            std::tolower(static_cast<unsigned char>(ch))));
    }

    auto contains = [&](const char* needle) -> bool {
        return normalized.find(needle) != std::string::npos;
    };

    return contains("yy") ||
           contains("yyyy") ||
           contains("dd") ||
           contains("ddd") ||
           contains("mm") ||
           contains("mmm") ||
           contains("m/") ||
           contains("/m") ||
           contains("h") ||
           contains("am/pm") ||
           contains("a/p") ||
           contains("ss");
}

}}  // namespace Aspose::Cells_FOSS
