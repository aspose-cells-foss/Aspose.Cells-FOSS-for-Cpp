#pragma once

#include "aspose/cells_foss/XmlDocument.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/core/CellAddress.h"
#include "aspose/cells_foss/core/CellRecord.h"
#include "aspose/cells_foss/core/ColumnRangeModel.h"
#include "aspose/cells_foss/core/DateSystem.h"
#include "aspose/cells_foss/core/MergeRegion.h"
#include "aspose/cells_foss/core/RowModel.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

class SaveOptions;
class StylesheetSaveContext;
class ZipArchive;

namespace Core {
class SharedStringRepository;
class WorkbookModel;
class WorksheetModel;
}  // namespace Core

/// Constants and helpers shared across the XLSX workbook serializer.
class XlsxWorkbookSerializerCommon {
public:
    XlsxWorkbookSerializerCommon() = delete;

    // -----------------------------------------------------------------------
    // Relationship type constants
    // -----------------------------------------------------------------------

    static inline const std::string WorksheetRelationshipType =
        "http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet";

    static inline const std::string SharedStringsRelationshipType =
        "http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings";

    static inline const std::string StylesRelationshipType =
        "http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles";

    static inline const std::string CorePropertiesRelationshipType =
        "http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties";

    static inline const std::string ExtendedPropertiesRelationshipType =
        "http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties";

    // -----------------------------------------------------------------------
    // XML namespace constants
    // -----------------------------------------------------------------------

    static inline const XNamespace MainNs =
        XNamespace("http://schemas.openxmlformats.org/spreadsheetml/2006/main");

    static inline const XNamespace RelationshipNs =
        XNamespace("http://schemas.openxmlformats.org/officeDocument/2006/relationships");

    static inline const XNamespace PackageRelationshipNs =
        XNamespace("http://schemas.openxmlformats.org/package/2006/relationships");

    static inline const XNamespace ContentTypeNs =
        XNamespace("http://schemas.openxmlformats.org/package/2006/content-types");

    static inline const XNamespace XmlNs =
        XNamespace("http://www.w3.org/XML/1998/namespace");

    // -----------------------------------------------------------------------
    // Built-in date format set
    // -----------------------------------------------------------------------

    static inline const std::unordered_set<int> BuiltInDateFormats =
        {14, 15, 16, 17, 18, 19, 20, 21, 22, 45, 46, 47};

    // -----------------------------------------------------------------------
    // Cell persistence
    // -----------------------------------------------------------------------

    /// Returns true if the cell record should be persisted to the worksheet XML.
    static bool ShouldPersistCell(const Core::StyleValue& workbookDefaultStyle,
                                  const Core::CellRecord& record);

    // -----------------------------------------------------------------------
    // ZIP / XML entry writing
    // -----------------------------------------------------------------------

    /// Serializes an XmlDocument to UTF-8 and writes it as a ZIP entry.
    static void WriteXmlEntry(ZipArchive& archive,
                              std::string_view path,
                              const XmlDocument& document);

    // -----------------------------------------------------------------------
    // Document builders
    // -----------------------------------------------------------------------

    /// Builds the [Content_Types].xml document.
    static XmlDocument BuildContentTypes(
        const Core::WorkbookModel& model,
        bool hasSharedStrings,
        bool hasDateStyles,
        bool hasCoreProperties,
        bool hasExtendedProperties);

    /// Builds the root _rels/.rels document.
    static XmlDocument BuildRootRelationships(bool hasCoreProperties,
                                              bool hasExtendedProperties);

    /// Builds the xl/workbook.xml document.
    static XmlDocument BuildWorkbook(const Core::WorkbookModel& model);

    /// Builds the xl/_rels/workbook.xml.rels document.
    static XmlDocument BuildWorkbookRelationships(
        const Core::WorkbookModel& model,
        bool hasSharedStrings,
        bool hasDateStyles);

    /// Builds an xl/worksheets/sheetN.xml document.
    static XmlDocument BuildWorksheet(
        Core::WorksheetModel& worksheet,
        const Core::StyleValue& workbookDefaultStyle,
        Core::DateSystem dateSystem,
        Core::SharedStringRepository& sharedStrings,
        const SaveOptions& options,
        StylesheetSaveContext& stylesheet);

    /// Builds the xl/sharedStrings.xml document.
    static XmlDocument BuildSharedStrings(
        const Core::SharedStringRepository& sharedStrings);

    /// Builds a minimal xl/styles.xml document.
    static XmlDocument BuildMinimalStylesheet();

    // -----------------------------------------------------------------------
    // Element builders
    // -----------------------------------------------------------------------

    /// Builds a row element with its child cells.
    static XmlElement BuildRowElement(
        int rowIndex,
        const Core::RowModel& rowModel,
        const std::vector<std::pair<Core::CellAddress, Core::CellRecord>>& rowCells,
        Core::DateSystem dateSystem,
        Core::SharedStringRepository& sharedStrings,
        const SaveOptions& options,
        StylesheetSaveContext& stylesheet);

    /// Builds a col element for a column range.
    static XmlElement BuildColumnElement(const Core::ColumnRangeModel& column);

    /// Builds a cell element (c element).
    static XmlElement BuildCell(
        const Core::CellAddress& address,
        const Core::CellRecord& record,
        Core::DateSystem dateSystem,
        Core::SharedStringRepository& sharedStrings,
        const SaveOptions& options,
        StylesheetSaveContext& stylesheet);

    /// Creates a text element (t element) with optional xml:space="preserve".
    static XmlElement CreateTextElement(std::string_view value);

    // -----------------------------------------------------------------------
    // Metadata queries
    // -----------------------------------------------------------------------

    /// Returns true if the row model has any non-default metadata.
    static bool HasRowMetadata(const Core::RowModel& rowModel);

    /// Returns true if the column range has any non-default metadata.
    static bool HasColumnMetadata(const Core::ColumnRangeModel& column);

    /// Checks whether a string value needs xml:space="preserve".
    static bool NeedsPreserveWhitespace(std::string_view value);

    // -----------------------------------------------------------------------
    // Collection helpers
    // -----------------------------------------------------------------------

    /// Collects and sorts the union of row indexes from persisted cells and
    /// row metadata.
    static std::vector<int> GetWorksheetRowIndexes(
        const std::vector<std::pair<Core::CellAddress, Core::CellRecord>>& persistedCells,
        const std::unordered_map<int, Core::RowModel>& rows);

    /// Filters, sorts, and merges adjacent column ranges that share the same
    /// metadata.
    static std::vector<Core::ColumnRangeModel> NormalizeColumnRanges(
        const std::vector<Core::ColumnRangeModel>& columns);

    /// Returns true if two column ranges have identical metadata.
    static bool ColumnMetadataEqual(const Core::ColumnRangeModel& left,
                                    const Core::ColumnRangeModel& right);

    /// Calculates the dimension reference string for the sheetData dimension
    /// element.
    static std::string CalculateDimensionReference(
        const std::vector<std::pair<Core::CellAddress, Core::CellRecord>>& persistedCells,
        const std::vector<Core::MergeRegion>& mergeRegions);

    // -----------------------------------------------------------------------
    // Reference utilities
    // -----------------------------------------------------------------------

    /// Converts a merge region to a range reference string (e.g. "A1:B2").
    static std::string ToRangeReference(const Core::MergeRegion& region);

    /// Parses a range reference string into a merge region. Returns true on
    /// success.
    static bool TryParseMergeReference(std::string_view mergeReference,
                                       Core::MergeRegion& region);

    /// Parses a cell reference string into a CellAddress. Returns true on
    /// success.
    static bool TryParseCellReference(std::string_view cellReference,
                                      Core::CellAddress& address);
};

}}  // namespace Aspose::Cells_FOSS
