#include "aspose/cells_foss/XlsxWorkbookSerializer.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/InvalidFileFormatException.h"
#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadIssue.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/SaveFormat.h"
#include "aspose/cells_foss/SaveOptions.h"
#include "aspose/cells_foss/StylesheetLoadContext.h"
#include "aspose/cells_foss/StylesheetSaveContext.h"
#include "aspose/cells_foss/UnsupportedFeatureException.h"
#include "aspose/cells_foss/WorkbookSaveException.h"
#include "aspose/cells_foss/WorksheetDefinedNamesState.h"
#include "aspose/cells_foss/XmlDocument.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/XlsxDocumentProperties.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookAutoFilter.h"
#include "aspose/cells_foss/XlsxWorkbookConditionalFormatting.h"
#include "aspose/cells_foss/XlsxWorkbookDefinedNames.h"
#include "aspose/cells_foss/XlsxWorkbookHyperlinks.h"
#include "aspose/cells_foss/XlsxWorkbookPageSetup.h"
#include "aspose/cells_foss/XlsxWorkbookProperties.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"
#include "aspose/cells_foss/XlsxWorkbookStyles.h"
#include "aspose/cells_foss/XlsxWorkbookValidations.h"
#include "aspose/cells_foss/XlsxWorkbookWorksheetProtection.h"
#include "aspose/cells_foss/XlsxWorkbookWorksheetViews.h"
#include "aspose/cells_foss/ZipArchive.h"
#include "aspose/cells_foss/CellValue.h"
#include "aspose/cells_foss/core/CellAddress.h"
#include "aspose/cells_foss/core/CellRecord.h"
#include "aspose/cells_foss/core/CellValueKind.h"
#include "aspose/cells_foss/core/ColumnRangeModel.h"
#include "aspose/cells_foss/core/DateSerialConverter.h"
#include "aspose/cells_foss/core/DateSystem.h"
#include "aspose/cells_foss/core/MergeRegion.h"
#include "aspose/cells_foss/core/RowModel.h"
#include "aspose/cells_foss/core/SharedStringRepository.h"
#include "aspose/cells_foss/core/SheetVisibility.h"
#include "aspose/cells_foss/core/StyleValue.h"
#include "aspose/cells_foss/core/WorkbookModel.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <algorithm>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

using Core::CellAddress;
using Core::CellRecord;
using Core::CellValueKind;
using Core::ColumnRangeModel;
using Core::DateSerialConverter;
using Core::DateSystem;
using Core::MergeRegion;
using Core::RowModel;
using Core::SharedStringRepository;
using Core::SheetVisibility;
using Core::StyleValue;
using Core::WorkbookModel;
using Core::WorksheetModel;

// ---------------------------------------------------------------------------
// Namespace shortcuts
// ---------------------------------------------------------------------------

static const XNamespace& MainNs = XlsxWorkbookSerializerCommon::MainNs;

// ---------------------------------------------------------------------------
// String helpers
// ---------------------------------------------------------------------------

static bool IsNullOrWhiteSpace(std::string_view value)
{
    return value.find_first_not_of(" \t\n\r\f\v") == std::string_view::npos;
}

static bool EqualsIgnoreCase(std::string_view left, std::string_view right)
{
    if (left.size() != right.size()) {
        return false;
    }
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(left[i])) !=
            std::tolower(static_cast<unsigned char>(right[i]))) {
            return false;
        }
    }
    return true;
}

static bool TryParseInt(std::string_view str, int& result)
{
    if (str.empty()) {
        return false;
    }
    std::string copy(str);
    char* end = nullptr;
    errno = 0;
    const long value = std::strtol(copy.c_str(), &end, 10);
    if (end != copy.c_str() + copy.size()) {
        return false;
    }
    if (errno == ERANGE || value < INT_MIN || value > INT_MAX) {
        return false;
    }
    result = static_cast<int>(value);
    return true;
}

static bool TryParseDouble(std::string_view str, double& result)
{
    if (str.empty()) {
        return false;
    }
    std::string copy(str);
    char* end = nullptr;
    errno = 0;
    result = std::strtod(copy.c_str(), &end);
    if (end != copy.c_str() + copy.size()) {
        return false;
    }
    if (errno == ERANGE) {
        return false;
    }
    return true;
}

static bool TryParseIso8601DateTime(std::string_view str, DateTime& result)
{
    // Expected formats: "YYYY-MM-DD" or "YYYY-MM-DDTHH:MM:SS"
    if (str.size() < 10) {
        return false;
    }
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;

    if (!TryParseInt(str.substr(0, 4), year)) {
        return false;
    }
    if (str[4] != '-') {
        return false;
    }
    if (!TryParseInt(str.substr(5, 2), month)) {
        return false;
    }
    if (str[7] != '-') {
        return false;
    }
    if (!TryParseInt(str.substr(8, 2), day)) {
        return false;
    }

    if (str.size() > 10) {
        if (str[10] != 'T' && str[10] != ' ') {
            return false;
        }
        if (str.size() >= 19) {
            if (!TryParseInt(str.substr(11, 2), hour)) {
                return false;
            }
            if (str[13] != ':') {
                return false;
            }
            if (!TryParseInt(str.substr(14, 2), minute)) {
                return false;
            }
            if (str[16] != ':') {
                return false;
            }
            if (!TryParseInt(str.substr(17, 2), second)) {
                return false;
            }
        }
    }

    result = DateTime(year, month, day, hour, minute, second);
    return true;
}

// ---------------------------------------------------------------------------
// Private: LoadWorksheet
// ---------------------------------------------------------------------------

static bool MergeRegionsOverlap(const MergeRegion& left, const MergeRegion& right)
{
    int leftLastRow = left.GetFirstRow() + left.GetTotalRows() - 1;
    int leftLastColumn = left.GetFirstColumn() + left.GetTotalColumns() - 1;
    int rightLastRow = right.GetFirstRow() + right.GetTotalRows() - 1;
    int rightLastColumn = right.GetFirstColumn() + right.GetTotalColumns() - 1;

    return left.GetFirstRow() <= rightLastRow &&
           right.GetFirstRow() <= leftLastRow &&
           left.GetFirstColumn() <= rightLastColumn &&
           right.GetFirstColumn() <= leftLastColumn;
}

static bool ContainsOverlappingMergeRegion(
    const std::vector<MergeRegion>& mergeRegions,
    const MergeRegion& candidate)
{
    for (std::size_t i = 0; i < mergeRegions.size(); ++i) {
        if (MergeRegionsOverlap(mergeRegions[i], candidate)) {
            return true;
        }
    }
    return false;
}

static WorksheetModel LoadWorksheet(
    std::string_view sheetName,
    const XmlDocument& worksheetDocument,
    const std::unordered_map<std::string, std::string>& worksheetHyperlinkTargets,
    DateSystem dateSystem,
    const std::vector<std::string>& sharedStrings,
    StylesheetLoadContext& stylesheet,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    const WorksheetDefinedNamesState& definedNamesState)
{
    WorksheetModel worksheetModel(sheetName);
    const auto& worksheetRoot = worksheetDocument.GetRoot();
    if (worksheetRoot.IsNull()) {
        return worksheetModel;
    }

    XlsxWorkbookWorksheetViews::LoadWorksheetViewSettings(
        worksheetModel, worksheetRoot, diagnostics, options, sheetName);
    XlsxWorkbookWorksheetProtection::LoadWorksheetProtection(
        worksheetModel, worksheetRoot, diagnostics, options, sheetName);
    XlsxWorkbookAutoFilter::LoadAutoFilter(
        worksheetModel, worksheetRoot, stylesheet, diagnostics, options, sheetName);

    // LoadColumns
    {
        std::vector<ColumnRangeModel> columns;
        auto colsElement = worksheetRoot.GetElement(MainNs + "cols");
        if (!colsElement.IsNull()) {
            const auto colElements = colsElement.GetElements(MainNs + "col");
            for (const auto& columnElement : colElements) {
                auto minAttr = XlsxWorkbookArchiveHelpers::ParseIntAttribute(
                    columnElement.GetAttribute("min"));
                auto maxAttr = XlsxWorkbookArchiveHelpers::ParseIntAttribute(
                    columnElement.GetAttribute("max"));
                if (!minAttr.has_value() || !maxAttr.has_value() ||
                    minAttr.value() <= 0 || maxAttr.value() <= 0 ||
                    minAttr.value() > maxAttr.value()) {
                    throw InvalidFileFormatException(
                        "Worksheet column metadata in '" + std::string(sheetName) +
                        "' contains an invalid min/max span.");
                }

                auto styleIndex = XlsxWorkbookArchiveHelpers::ParseIntAttribute(
                    columnElement.GetAttribute("style"));
                if (styleIndex.has_value() &&
                    (styleIndex.value() < 0 ||
                     styleIndex.value() >= static_cast<int>(stylesheet.GetCellFormats().size()))) {
                    if (options.GetStrictMode()) {
                        throw InvalidFileFormatException(
                            "The column style index '" +
                            std::to_string(styleIndex.value()) + "' is invalid.");
                    }
                    LoadIssue issue("COL-L001", DiagnosticSeverity::Warning,
                                   "Column style index '" +
                                       std::to_string(styleIndex.value()) +
                                       "' is invalid and was dropped.",
                                   false, true);
                    issue.SetSheetName(sheetName);
                    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                    styleIndex = std::nullopt;
                }

                ColumnRangeModel col;
                col.SetMinColumnIndex(minAttr.value() - 1);
                col.SetMaxColumnIndex(maxAttr.value() - 1);
                col.SetWidth(
                    XlsxWorkbookArchiveHelpers::ParseDoubleAttribute(
                        columnElement.GetAttribute("width")));
                col.SetHidden(
                    XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
                        columnElement.GetAttribute("hidden")));
                col.SetStyleIndex(styleIndex);
                columns.push_back(col);
            }
        }

        // NormalizeLoadedColumns
        std::vector<ColumnRangeModel> ordered(columns.begin(), columns.end());
        std::sort(ordered.begin(), ordered.end(),
                  [](const ColumnRangeModel& l, const ColumnRangeModel& r) {
                      if (l.GetMinColumnIndex() != r.GetMinColumnIndex()) {
                          return l.GetMinColumnIndex() < r.GetMinColumnIndex();
                      }
                      return l.GetMaxColumnIndex() < r.GetMaxColumnIndex();
                  });

        if (!ordered.empty()) {
            std::vector<ColumnRangeModel> normalized;
            normalized.push_back(ordered[0]);
            bool mergeReported = false;
            for (std::size_t idx = 1; idx < ordered.size(); ++idx) {
                auto& current = ordered[idx];
                auto& previous = normalized.back();
                if (current.GetMinColumnIndex() <= previous.GetMaxColumnIndex() + 1 &&
                    XlsxWorkbookSerializerCommon::ColumnMetadataEqual(previous, current)) {
                    if ((current.GetMinColumnIndex() <= previous.GetMaxColumnIndex() ||
                         current.GetMaxColumnIndex() <= previous.GetMaxColumnIndex()) &&
                        !mergeReported) {
                        LoadIssue issue("COL-R001", DiagnosticSeverity::Recoverable,
                                       "Overlapping compatible column metadata was normalized during load.",
                                       true);
                        issue.SetSheetName(sheetName);
                        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                        mergeReported = true;
                    }
                    previous.SetMaxColumnIndex(
                        std::max(previous.GetMaxColumnIndex(), current.GetMaxColumnIndex()));
                    continue;
                }
                normalized.push_back(current);
            }
            auto& cols = worksheetModel.GetColumns();
            cols.insert(cols.end(), normalized.begin(), normalized.end());
        }
    }

    XlsxWorkbookPageSetup::LoadWorksheetPageSetup(
        worksheetModel, worksheetRoot, diagnostics, options, sheetName);
    XlsxWorkbookHyperlinks::LoadHyperlinks(
        worksheetModel, worksheetRoot, worksheetHyperlinkTargets,
        diagnostics, options, sheetName);
    XlsxWorkbookConditionalFormatting::LoadConditionalFormattings(
        worksheetModel, worksheetRoot, stylesheet, diagnostics, options, sheetName);
    XlsxWorkbookValidations::LoadValidations(
        worksheetModel, worksheetRoot, diagnostics, options, sheetName);
    XlsxWorkbookPageSetup::ApplyWorksheetDefinedNames(
        worksheetModel, definedNamesState);

    auto sheetData = worksheetRoot.GetElement(MainNs + "sheetData");
    if (sheetData.IsNull()) {
        LoadIssue issue("ACF-WS-001", DiagnosticSeverity::Recoverable,
                       "Worksheet sheetData is missing; an empty sheet was synthesized.",
                       true);
        issue.SetSheetName(sheetName);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
        return worksheetModel;
    }

    auto rowElements = sheetData.GetElements(MainNs + "row");
    std::unordered_set<int> seenRows;
    std::unordered_set<CellAddress> seenCells;
    int previousRowIndex = -1;
    bool rowsOutOfOrderReported = false;

    for (const auto& rowElement : rowElements) {
        // TryResolveRowIndex
        int rowIndex = -1;
        bool rowIndexResolved = false;
        auto rowIndexAttr = XlsxWorkbookArchiveHelpers::ParseIntAttribute(
            rowElement.GetAttribute("r"));
        if (rowIndexAttr.has_value()) {
            if (rowIndexAttr.value() <= 0) {
                throw InvalidFileFormatException(
                    "Worksheet row index '" + std::to_string(rowIndexAttr.value()) +
                    "' is invalid.");
            }
            rowIndex = rowIndexAttr.value() - 1;
            rowIndexResolved = true;
        } else {
            // Try to infer from cell references
            const auto cellElements = rowElement.GetElements(MainNs + "c");
            for (const auto& cellEl : cellElements) {
                auto cellRefAttr = cellEl.GetAttribute("r");
                std::string cellRef =
                    cellRefAttr.IsNull() ? std::string() : cellRefAttr.GetValue();
                CellAddress inferredAddress;
                if (XlsxWorkbookSerializerCommon::TryParseCellReference(cellRef, inferredAddress)) {
                    rowIndex = inferredAddress.GetRowIndex();
                    LoadIssue issue("WS-R004", DiagnosticSeverity::Recoverable,
                                   "A worksheet row index was missing and was inferred from contained cells.",
                                   true);
                    issue.SetSheetName(sheetName);
                    issue.SetRowIndex(rowIndex);
                    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                    rowIndexResolved = true;
                    break;
                }
            }
            if (!rowIndexResolved) {
                LoadIssue issue("ROW-F001", DiagnosticSeverity::Warning,
                               "A worksheet row without an index and without parseable cells was skipped.");
                issue.SetSheetName(sheetName);
                XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                continue;
            }
        }

        if (!seenRows.insert(rowIndex).second) {
            throw InvalidFileFormatException(
                "Duplicate row index '" + std::to_string(rowIndex + 1) +
                "' was found in worksheet '" + std::string(sheetName) + "'.");
        }

        if (previousRowIndex > rowIndex && !rowsOutOfOrderReported) {
            LoadIssue issue("WS-R002", DiagnosticSeverity::Recoverable,
                           "Worksheet rows were out of order and were normalized during load.",
                           true);
            issue.SetSheetName(sheetName);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            rowsOutOfOrderReported = true;
        }

        previousRowIndex = rowIndex;

        // ApplyRowMetadata
        {
            auto styleIndex = XlsxWorkbookArchiveHelpers::ParseIntAttribute(
                rowElement.GetAttribute("s"));
            if (styleIndex.has_value() &&
                (styleIndex.value() < 0 ||
                 styleIndex.value() >= static_cast<int>(stylesheet.GetCellFormats().size()))) {
                if (options.GetStrictMode()) {
                    throw InvalidFileFormatException(
                        "The row style index '" + std::to_string(styleIndex.value()) +
                        "' is invalid.");
                }
                LoadIssue issue("ROW-L001", DiagnosticSeverity::Warning,
                               "Row style index '" + std::to_string(styleIndex.value()) +
                                   "' is invalid and was dropped.",
                               false, true);
                issue.SetSheetName(sheetName);
                issue.SetRowIndex(rowIndex);
                XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                styleIndex = std::nullopt;
            }

            RowModel rowModel;
            rowModel.SetHeight(
                XlsxWorkbookArchiveHelpers::ParseDoubleAttribute(
                    rowElement.GetAttribute("ht")));
            rowModel.SetHidden(
                XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
                    rowElement.GetAttribute("hidden")));
            rowModel.SetStyleIndex(styleIndex);

            if (rowModel.GetHeight().has_value() &&
                !XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
                    rowElement.GetAttribute("customHeight"))) {
                LoadIssue issue("ROW-R002", DiagnosticSeverity::Recoverable,
                               "Row height metadata was missing customHeight and was normalized during load.",
                               true);
                issue.SetSheetName(sheetName);
                issue.SetRowIndex(rowIndex);
                XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            }

            if (rowModel.GetHeight().has_value() || rowModel.GetHidden() ||
                rowModel.GetStyleIndex().has_value()) {
                worksheetModel.GetRows()[rowIndex] = rowModel;
            }
        }

        // Process cells
        int previousColumnIndex = -1;
        bool cellsOutOfOrderReported = false;
        const auto cellElements = rowElement.GetElements(MainNs + "c");
        for (const auto& cellElement : cellElements) {
            // TryReadCellRecord
            CellRecord record;
            record.SetStyle(stylesheet.GetDefaultCellStyle());
            record.SetIsExplicitlyStored(true);

            auto cellRefAttr = cellElement.GetAttribute("r");
            std::string cellReference =
                cellRefAttr.IsNull() ? std::string() : cellRefAttr.GetValue();
            if (IsNullOrWhiteSpace(cellReference)) {
                LoadIssue issue("CELL-F001", DiagnosticSeverity::Warning,
                               "A cell without a reference was skipped.");
                issue.SetSheetName(sheetName);
                issue.SetRowIndex(rowIndex);
                XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                continue;
            }

            CellAddress address;
            try {
                address = CellAddress::Parse(cellReference);
            } catch (const std::invalid_argument&) {
                if (options.GetStrictMode()) {
                    throw InvalidFileFormatException(
                        "The cell reference '" + cellReference + "' is invalid.");
                }
                LoadIssue issue("CELL-F001", DiagnosticSeverity::Warning,
                               "Cell reference '" + cellReference +
                                   "' is invalid and was skipped.");
                issue.SetSheetName(sheetName);
                issue.SetCellRef(cellReference);
                issue.SetRowIndex(rowIndex);
                XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                continue;
            }

            auto styleIndex = XlsxWorkbookArchiveHelpers::ParseIntAttribute(
                cellElement.GetAttribute("s"));
            bool isDateStyle = styleIndex.has_value() &&
                               stylesheet.GetDateStyleIndexes().count(styleIndex.value()) != 0;
            if (styleIndex.has_value()) {
                if (styleIndex.value() >= 0 &&
                    styleIndex.value() < static_cast<int>(stylesheet.GetCellFormats().size())) {
                    record.SetStyle(stylesheet.GetCellFormats()[styleIndex.value()]);
                } else if (options.GetStrictMode()) {
                    throw InvalidFileFormatException(
                        "The style index '" + std::to_string(styleIndex.value()) +
                        "' is invalid.");
                } else {
                    LoadIssue issue("STYLE-F001", DiagnosticSeverity::Warning,
                                   "Cell style index '" + std::to_string(styleIndex.value()) +
                                       "' is invalid and style 0 was used instead.");
                    issue.SetSheetName(sheetName);
                    issue.SetCellRef(cellReference);
                    issue.SetRowIndex(rowIndex);
                    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                }
            }

            // Formula
            auto formulaElement = cellElement.GetElement(MainNs + "f");
            auto formulaText = XlsxWorkbookArchiveHelpers::NormalizeFormula(
                formulaElement.IsNull() ? std::string_view() : formulaElement.GetValue());
            if (!formulaText.empty()) {
                record.SetFormula(formulaText);
                record.SetKind(CellValueKind::Formula);
            }

            // Cell value
            auto cellTypeAttr = cellElement.GetAttribute("t");
            std::string cellType =
                cellTypeAttr.IsNull() ? std::string() : cellTypeAttr.GetValue();
            auto valueElement = cellElement.GetElement(MainNs + "v");
            std::string rawValue =
                valueElement.IsNull() ? std::string() : valueElement.GetValue();

            // TryReadCellValue
            CellValue value;
            CellValueKind kind = CellValueKind::Blank;
            bool valueRead = false;

            if (cellType == "inlineStr") {
                auto isElement = cellElement.GetElement(MainNs + "is");
                value = CellValue(XlsxWorkbookArchiveHelpers::ReadInlineString(isElement));
                kind = CellValueKind::String;
                valueRead = true;
            } else if (cellType == "s") {
                int sharedStringIndex = 0;
                if (TryParseInt(rawValue, sharedStringIndex) &&
                    sharedStringIndex >= 0 &&
                    sharedStringIndex < static_cast<int>(sharedStrings.size())) {
                    value = CellValue(sharedStrings[sharedStringIndex]);
                    kind = CellValueKind::String;
                    valueRead = true;
                } else {
                    LoadIssue issue("SST-L001", DiagnosticSeverity::LossyRecoverable,
                                   "The cell points to an invalid shared string index.",
                                   false, true);
                    issue.SetSheetName(sheetName);
                    issue.SetCellRef(cellReference);
                    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                    value = CellValue(std::string());
                    kind = CellValueKind::String;
                    valueRead = true;
                }
            } else if (cellType == "b") {
                bool boolValue =
                    (rawValue == "1") || EqualsIgnoreCase(rawValue, "true");
                value = CellValue(boolValue);
                kind = CellValueKind::Boolean;
                valueRead = true;
            } else if (cellType == "d") {
                DateTime dateTime;
                if (TryParseIso8601DateTime(rawValue, dateTime)) {
                    value = CellValue(dateTime);
                    kind = CellValueKind::DateTime;
                    valueRead = true;
                }
            } else if (cellType == "str") {
                value = CellValue(rawValue);
                kind = CellValueKind::String;
                valueRead = true;
            } else if (cellType == "e") {
                value = CellValue(rawValue);
                kind = CellValueKind::String;
                valueRead = true;
            } else if (!rawValue.empty()) {
                if (isDateStyle) {
                    double serial = 0.0;
                    if (TryParseDouble(rawValue, serial)) {
                        value = CellValue(DateSerialConverter::FromSerial(serial, dateSystem));
                        kind = CellValueKind::DateTime;
                        valueRead = true;
                    } else {
                        LoadIssue issue("CELL-R002", DiagnosticSeverity::Recoverable,
                                       "A formula or numeric cell contained an invalid cached date serial and the cached value was cleared.",
                                       true);
                        issue.SetSheetName(sheetName);
                        issue.SetCellRef(cellReference);
                        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                    }
                } else {
                    CellValue numberValue;
                    if (XlsxWorkbookArchiveHelpers::TryParseNumber(rawValue, numberValue)) {
                        value = numberValue;
                        kind = CellValueKind::Number;
                        valueRead = true;
                    }
                }
            }

            if (valueRead) {
                record.SetValue(value);
                if (record.GetFormula().empty()) {
                    record.SetKind(kind);
                }
            }

            if (previousColumnIndex > address.GetColumnIndex() && !cellsOutOfOrderReported) {
                LoadIssue issue("WS-R003", DiagnosticSeverity::Recoverable,
                               "Worksheet cells were out of order within a row and were normalized during load.",
                               true);
                issue.SetSheetName(sheetName);
                issue.SetRowIndex(rowIndex);
                XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                cellsOutOfOrderReported = true;
            }

            previousColumnIndex = address.GetColumnIndex();
            if (!seenCells.insert(address).second) {
                throw InvalidFileFormatException(
                    "Duplicate cell reference '" + address.ToString() +
                    "' was found in worksheet '" + std::string(sheetName) + "'.");
            }

            if (XlsxWorkbookSerializerCommon::ShouldPersistCell(
                    stylesheet.GetDefaultCellStyle(), record)) {
                worksheetModel.GetCells()[address] = record;
            }
        }
    }

    // LoadMergeRegions
    {
        auto mergeCellsElement = worksheetRoot.GetElement(MainNs + "mergeCells");
        if (!mergeCellsElement.IsNull()) {
            const auto mergeElements =
                mergeCellsElement.GetElements(MainNs + "mergeCell");
            for (const auto& mergeElement : mergeElements) {
                auto mergeRefAttr = mergeElement.GetAttribute("ref");
                std::string mergeReference =
                    mergeRefAttr.IsNull() ? std::string() : mergeRefAttr.GetValue();
                MergeRegion region;
                if (!XlsxWorkbookSerializerCommon::TryParseMergeReference(
                        mergeReference, region)) {
                    throw InvalidFileFormatException(
                        "The merge reference '" + mergeReference + "' is invalid.");
                }

                if (ContainsOverlappingMergeRegion(
                        worksheetModel.GetMergeRegions(), region)) {
                    if (options.GetStrictMode()) {
                        throw InvalidFileFormatException(
                            "The merge reference '" + mergeReference +
                            "' overlaps an existing merged range.");
                    }
                    LoadIssue issue("MRG-L001", DiagnosticSeverity::LossyRecoverable,
                                   "Overlapping merge range '" + mergeReference +
                                       "' was dropped during load.",
                                   true, true);
                    issue.SetSheetName(sheetName);
                    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                    continue;
                }

                worksheetModel.GetMergeRegions().push_back(region);
            }

            auto& mergeRegions = worksheetModel.GetMergeRegions();
            std::sort(mergeRegions.begin(), mergeRegions.end(),
                      [](const MergeRegion& left, const MergeRegion& right) {
                          if (left.GetFirstRow() != right.GetFirstRow()) {
                              return left.GetFirstRow() < right.GetFirstRow();
                          }
                          if (left.GetFirstColumn() != right.GetFirstColumn()) {
                              return left.GetFirstColumn() < right.GetFirstColumn();
                          }
                          if (left.GetTotalRows() != right.GetTotalRows()) {
                              return left.GetTotalRows() < right.GetTotalRows();
                          }
                          return left.GetTotalColumns() < right.GetTotalColumns();
                      });
        }
    }

    if (worksheetRoot.GetElement(MainNs + "dimension").IsNull() &&
        (!worksheetModel.GetCells().empty() ||
         !worksheetModel.GetMergeRegions().empty())) {
        LoadIssue issue("WS-R001", DiagnosticSeverity::Recoverable,
                       "Worksheet dimension was missing and was recalculated during load.",
                       true);
        issue.SetSheetName(sheetName);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    }

    return worksheetModel;
}

// ---------------------------------------------------------------------------
// XlsxWorkbookSerializer::Save
// ---------------------------------------------------------------------------

void XlsxWorkbookSerializer::Save(
    WorkbookModel& model,
    std::vector<std::uint8_t>& stream,
    const SaveOptions& options)
{
    if (options.GetSaveFormat() != SaveFormat::Xlsx) {
        throw UnsupportedFeatureException("Only XLSX save is supported.");
    }

    if (model.GetWorksheets().empty()) {
        throw WorkbookSaveException("A workbook must contain at least one worksheet.");
    }

    SharedStringRepository sharedStrings;
    auto stylesheet = XlsxWorkbookStyles::BuildStylesheet(model);
    auto corePropertiesDocument =
        XlsxDocumentProperties::BuildCorePropertiesDocument(model);
    auto extendedPropertiesDocument =
        XlsxDocumentProperties::BuildExtendedPropertiesDocument(model);

    for (const auto& worksheet : model.GetWorksheets()) {
        for (const auto& pair : worksheet.GetCells()) {
            if (!XlsxWorkbookSerializerCommon::ShouldPersistCell(
                    model.GetDefaultStyle(), pair.second)) {
                continue;
            }
            if (pair.second.GetKind() == CellValueKind::String &&
                options.GetUseSharedStrings()) {
                if (pair.second.GetValue().IsString()) {
                    sharedStrings.Intern(pair.second.GetValue().AsString());
                }
            }
        }
    }

    ZipArchive archive;

    XlsxWorkbookSerializerCommon::WriteXmlEntry(
        archive, "[Content_Types].xml",
        XlsxWorkbookSerializerCommon::BuildContentTypes(
            model, !sharedStrings.GetValues().empty(),
            stylesheet.GetHasStyles(),
            !corePropertiesDocument.IsNull(),
            !extendedPropertiesDocument.IsNull()));

    XlsxWorkbookSerializerCommon::WriteXmlEntry(
        archive, "_rels/.rels",
        XlsxWorkbookSerializerCommon::BuildRootRelationships(
            !corePropertiesDocument.IsNull(),
            !extendedPropertiesDocument.IsNull()));

    XlsxWorkbookSerializerCommon::WriteXmlEntry(
        archive, "xl/workbook.xml",
        XlsxWorkbookSerializerCommon::BuildWorkbook(model));

    XlsxWorkbookSerializerCommon::WriteXmlEntry(
        archive, "xl/_rels/workbook.xml.rels",
        XlsxWorkbookSerializerCommon::BuildWorkbookRelationships(
            model, !sharedStrings.GetValues().empty(),
            stylesheet.GetHasStyles()));

    for (std::size_t i = 0; i < model.GetWorksheets().size(); ++i) {
        auto& worksheet = model.GetWorksheets()[i];
        XlsxWorkbookSerializerCommon::WriteXmlEntry(
            archive,
            "xl/worksheets/sheet" + std::to_string(i + 1) + ".xml",
            XlsxWorkbookSerializerCommon::BuildWorksheet(
                worksheet, model.GetDefaultStyle(),
                model.GetSettings().GetDateSystem(),
                sharedStrings, options, stylesheet));

        auto hyperlinkRels =
            XlsxWorkbookHyperlinks::BuildWorksheetHyperlinkRelationships(worksheet);
        if (!hyperlinkRels.IsNull()) {
            XlsxWorkbookSerializerCommon::WriteXmlEntry(
                archive,
                "xl/worksheets/_rels/sheet" + std::to_string(i + 1) + ".xml.rels",
                hyperlinkRels);
        }
    }

    if (!sharedStrings.GetValues().empty()) {
        XlsxWorkbookSerializerCommon::WriteXmlEntry(
            archive, "xl/sharedStrings.xml",
            XlsxWorkbookSerializerCommon::BuildSharedStrings(sharedStrings));
    }

    if (stylesheet.GetHasStyles()) {
        XlsxWorkbookSerializerCommon::WriteXmlEntry(
            archive, "xl/styles.xml", stylesheet.GetDocument());
    }

    if (!corePropertiesDocument.IsNull()) {
        XlsxWorkbookSerializerCommon::WriteXmlEntry(
            archive, "docProps/core.xml", corePropertiesDocument);
    }

    if (!extendedPropertiesDocument.IsNull()) {
        XlsxWorkbookSerializerCommon::WriteXmlEntry(
            archive, "docProps/app.xml", extendedPropertiesDocument);
    }

    stream = archive.ToZipBytes();
}

// ---------------------------------------------------------------------------
// XlsxWorkbookSerializer::Load
// ---------------------------------------------------------------------------

WorkbookModel XlsxWorkbookSerializer::Load(
    const std::vector<std::uint8_t>& stream,
    const LoadOptions& options,
    LoadDiagnostics& diagnostics)
{
    try {
        auto archive = ZipArchive::FromZipBytes(stream);

        auto workbookEntry =
            XlsxWorkbookArchiveHelpers::GetEntry(archive, "/xl/workbook.xml");
        if (workbookEntry.IsNull()) {
            throw InvalidFileFormatException(
                "The package does not contain /xl/workbook.xml.");
        }

        auto workbookDocument =
            XlsxWorkbookArchiveHelpers::LoadDocument(workbookEntry);
        auto workbookRelationships =
            XlsxWorkbookArchiveHelpers::LoadRelationships(
                archive, "/xl/_rels/workbook.xml.rels", "/xl/workbook.xml");
        auto sharedStrings =
            XlsxWorkbookArchiveHelpers::LoadSharedStrings(
                archive, workbookRelationships, options, diagnostics);
        auto stylesheet =
            XlsxWorkbookStyles::LoadStylesheet(
                archive, workbookRelationships, options, diagnostics);

        WorkbookModel workbookModel;
        workbookModel.GetWorksheets().clear();
        workbookModel.SetDefaultStyle(stylesheet.GetDefaultCellStyle());

        const auto& workbookRoot = workbookDocument.GetRoot();
        if (workbookRoot.IsNull()) {
            throw InvalidFileFormatException("The workbook XML is empty.");
        }

        auto worksheetDefinedNames =
            XlsxWorkbookPageSetup::LoadWorksheetDefinedNames(
                workbookRoot, diagnostics, options);

        auto sheets = workbookRoot.GetElement(MainNs + "sheets");
        if (sheets.IsNull()) {
            throw InvalidFileFormatException(
                "The workbook XML does not contain a sheets element.");
        }

        auto sheetElements = sheets.GetElements(MainNs + "sheet");
        if (sheetElements.empty()) {
            throw InvalidFileFormatException(
                "The workbook XML does not contain any worksheets.");
        }

        XlsxWorkbookProperties::LoadWorkbookMetadata(
            workbookRoot, workbookModel,
            static_cast<int>(sheetElements.size()), diagnostics, options);
        XlsxDocumentProperties::LoadDocumentProperties(
            archive, workbookModel, diagnostics, options);
        XlsxWorkbookDefinedNames::LoadWorkbookDefinedNames(
            workbookRoot, workbookModel,
            static_cast<int>(sheetElements.size()), diagnostics, options);

        workbookModel.SetActiveSheetIndex(
            workbookModel.GetActiveSheetIndex() <
                    static_cast<int>(sheetElements.size())
                ? workbookModel.GetActiveSheetIndex()
                : 0);

        for (std::size_t index = 0; index < sheetElements.size(); ++index) {
            const auto& sheetElement = sheetElements[index];

            auto nameAttr = sheetElement.GetAttribute("name");
            std::string sheetName =
                nameAttr.IsNull() ? std::string() : nameAttr.GetValue();
            if (IsNullOrWhiteSpace(sheetName)) {
                sheetName = "Sheet" + std::to_string(index + 1);
            }

            std::string resolvedSheetName = sheetName;

            auto relIdAttr = sheetElement.GetAttribute("id");
            std::string relationshipId =
                relIdAttr.IsNull() ? std::string() : relIdAttr.GetValue();

            auto worksheetUri = XlsxWorkbookArchiveHelpers::ResolveWorksheetUri(
                static_cast<int>(index), relationshipId, workbookRelationships,
                archive, diagnostics, resolvedSheetName, options);

            auto worksheetEntry =
                XlsxWorkbookArchiveHelpers::GetEntry(archive, worksheetUri);
            if (worksheetEntry.IsNull()) {
                throw InvalidFileFormatException(
                    "Worksheet part '" + worksheetUri + "' was not found.");
            }

            auto worksheetHyperlinkTargets =
                XlsxWorkbookHyperlinks::LoadWorksheetHyperlinkTargets(
                    archive, worksheetUri);

            WorksheetDefinedNamesState definedNamesState;
            auto dnsIt = worksheetDefinedNames.find(static_cast<int>(index));
            if (dnsIt != worksheetDefinedNames.end()) {
                definedNamesState = dnsIt->second;
            }

            auto worksheetModel = LoadWorksheet(
                resolvedSheetName,
                XlsxWorkbookArchiveHelpers::LoadDocument(worksheetEntry),
                worksheetHyperlinkTargets,
                workbookModel.GetSettings().GetDateSystem(),
                sharedStrings, stylesheet, diagnostics, options,
                definedNamesState);

            auto stateAttr = sheetElement.GetAttribute("state");
            std::string state =
                stateAttr.IsNull() ? std::string() : stateAttr.GetValue();
            if (state == "hidden") {
                worksheetModel.SetVisibility(SheetVisibility::Hidden);
            } else if (state == "veryHidden") {
                worksheetModel.SetVisibility(SheetVisibility::VeryHidden);
            } else {
                worksheetModel.SetVisibility(SheetVisibility::Visible);
            }

            workbookModel.GetWorksheets().push_back(std::move(worksheetModel));
        }

        return workbookModel;
    } catch (const CellsException&) {
        throw;
    } catch (const std::exception& ex) {
        throw InvalidFileFormatException(
            "The workbook is not a valid XLSX zip package.", ex);
    }
}

}}  // namespace Aspose::Cells_FOSS
