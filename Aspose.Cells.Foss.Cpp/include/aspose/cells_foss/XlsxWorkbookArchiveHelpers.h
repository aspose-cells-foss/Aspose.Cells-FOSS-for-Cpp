#pragma once

#include "aspose/cells_foss/CellValue.h"
#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/XmlDocument.h"
#include "aspose/cells_foss/ZipArchive.h"

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Internal helper methods for reading XLSX workbook parts from a ZIP archive.
class XlsxWorkbookArchiveHelpers {
public:
    XlsxWorkbookArchiveHelpers() = delete;

    /// Ensures the byte buffer is seekable (identity for in-memory data).
    static std::vector<uint8_t> EnsureSeekable(const std::vector<uint8_t>& stream);

    /// Gets a ZIP entry by URI, normalizing leading slashes and backslashes.
    static ZipArchiveEntry GetEntry(const ZipArchive& archive, std::string_view uri);

    /// Loads an XML document from a ZIP entry.
    static XmlDocument LoadDocument(const ZipArchiveEntry& entry);

    /// Loads relationships from a .rels file in the archive.
    static std::unordered_map<std::string, std::string> LoadRelationships(
        const ZipArchive& archive,
        std::string_view relationshipsUri,
        std::string_view sourceUri);

    /// Finds the first relationship target whose resolved path matches expectedPath.
    static std::string FindRelationshipTarget(
        const std::unordered_map<std::string, std::string>& workbookRelationships,
        std::string_view expectedPath);

    /// Finds the first relationship target whose inferred kind matches relationshipType.
    static std::string FindRelationshipTargetByType(
        const std::unordered_map<std::string, std::string>& workbookRelationships,
        std::string_view relationshipType);

    /// Loads the shared strings table from the archive.
    static std::vector<std::string> LoadSharedStrings(
        const ZipArchive& archive,
        const std::unordered_map<std::string, std::string>& workbookRelationships,
        const LoadOptions& options,
        LoadDiagnostics& diagnostics);

    /// Loads the set of cell-format indexes that use a date number format.
    static std::unordered_set<int> LoadDateStyleIndexes(
        const ZipArchive& archive,
        const std::unordered_map<std::string, std::string>& workbookRelationships,
        const LoadOptions& options,
        LoadDiagnostics& diagnostics);

    /// Parses a boolean attribute ("1" or "true" yields true).
    static bool ParseBoolAttribute(const XmlAttribute& attribute);

    /// Parses an integer attribute, returning empty on failure or null attribute.
    static std::optional<int> ParseIntAttribute(const XmlAttribute& attribute);

    /// Parses a double attribute, returning empty on failure or null attribute.
    static std::optional<double> ParseDoubleAttribute(const XmlAttribute& attribute);

    /// Normalizes a formula string by trimming and stripping a leading '='.
    static std::string NormalizeFormula(std::string_view value);

    /// Reads the concatenated text from an inline-string element.
    static std::string ReadInlineString(const XmlElement& inlineStringElement);

    /// Attempts to parse a raw string as a number (int or double).
    static bool TryParseNumber(std::string_view rawValue, CellValue& numberValue);

    /// Records a load issue and notifies the warning callback if present.
    static void AddIssue(LoadDiagnostics& diagnostics, const LoadOptions& options, const LoadIssue& issue);

    /// Resolves the worksheet part URI from a relationship id, falling back to convention.
    static std::string ResolveWorksheetUri(
        int sheetIndex,
        std::string_view relationshipId,
        const std::unordered_map<std::string, std::string>& workbookRelationships,
        const ZipArchive& archive,
        LoadDiagnostics& diagnostics,
        std::string_view sheetName,
        const LoadOptions& options);

    /// Resolves a relative or absolute target against a source URI.
    static std::string ResolvePartUri(std::string_view sourceUri, std::string_view target);

    /// Tests whether a candidate path matches an expected URI (case-insensitive).
    static bool RelationshipPointsTo(std::string_view candidate, std::string_view expectedUri);

    /// Infers the relationship type from a resolved target path.
    static std::string GetRelationshipTargetKind(std::string_view candidate);

    /// Tests whether a number format code looks like a date/time format.
    static bool LooksLikeDateFormat(std::string_view formatCode);
};

}}  // namespace Aspose::Cells_FOSS
