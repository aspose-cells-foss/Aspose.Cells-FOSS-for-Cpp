#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"

#include "aspose/cells_foss/SaveOptions.h"
#include "aspose/cells_foss/StylesheetSaveContext.h"
#include "aspose/cells_foss/XlsxWorkbookAutoFilter.h"
#include "aspose/cells_foss/XlsxWorkbookConditionalFormatting.h"
#include "aspose/cells_foss/XlsxWorkbookDefinedNames.h"
#include "aspose/cells_foss/XlsxWorkbookHyperlinks.h"
#include "aspose/cells_foss/XlsxWorkbookPageSetup.h"
#include "aspose/cells_foss/XlsxWorkbookProperties.h"
#include "aspose/cells_foss/XlsxWorkbookStyles.h"
#include "aspose/cells_foss/XlsxWorkbookValidations.h"
#include "aspose/cells_foss/XlsxWorkbookWorksheetProtection.h"
#include "aspose/cells_foss/XlsxWorkbookWorksheetViews.h"
#include "aspose/cells_foss/ZipArchive.h"
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
#include "aspose/cells_foss/core/WorkbookModel.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdio>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Local type aliases
// ---------------------------------------------------------------------------

using Attrs = std::vector<std::pair<std::string, std::string>>;
using Children = std::vector<XmlElement>;

using Core::CellAddress;
using Core::CellRecord;
using Core::CellValueKind;
using Core::ColumnRangeModel;
using Core::DateSystem;
using Core::MergeRegion;
using Core::RowModel;
using Core::SharedStringRepository;
using Core::SheetVisibility;
using Core::StyleValue;
using Core::WorkbookModel;
using Core::WorksheetModel;

// ---------------------------------------------------------------------------
// Anonymous helpers
// ---------------------------------------------------------------------------

namespace {

/// Creates a text node (not a named element, just raw text content).
XmlElement MakeTextNode(std::string text)
{
    auto node = std::make_shared<XmlNodeData>();
    node->isTextNode = true;
    node->text = std::move(text);
    return XmlElement::MakeElement(std::move(node));
}

/// Formats a double using round-trip precision (equivalent to C# "R" format).
std::string DoubleToRoundTrip(double value)
{
    char buf[64];
    const auto result = std::to_chars(buf, buf + sizeof(buf) - 1, value);
    std::string s;
    if (result.ec == std::errc()) {
        s.assign(buf, result.ptr);
    } else {
        std::snprintf(buf, sizeof(buf), "%.17g", value);
        s.assign(buf);
    }
    // C# round-trip format uses uppercase 'E' for exponent; match that.
    for (char& c : s) {
        if (c == 'e') {
            c = 'E';
        }
    }
    return s;
}

/// Creates an element with a single text child.
XmlElement ElemText(const std::string& qualifiedName, std::string text)
{
    auto elem = XmlElement::MakeElement(std::string(qualifiedName));
    elem.AddChild(MakeTextNode(std::move(text)));
    return elem;
}

/// Creates an element with attributes and a single text child.
XmlElement ElemAttrText(const std::string& qualifiedName,
                        Attrs attrs,
                        std::string text)
{
    auto elem = XmlElement::MakeElement(std::string(qualifiedName), std::move(attrs), Children{});
    elem.AddChild(MakeTextNode(std::move(text)));
    return elem;
}

int ComparePersistedCells(const std::pair<CellAddress, CellRecord>& left,
                          const std::pair<CellAddress, CellRecord>& right)
{
    const int rowCmp = left.first.GetRowIndex() - right.first.GetRowIndex();
    if (rowCmp != 0) {
        return rowCmp;
    }
    return left.first.GetColumnIndex() - right.first.GetColumnIndex();
}

int CompareMergeRegions(const MergeRegion& left, const MergeRegion& right)
{
    int cmp = left.GetFirstRow() - right.GetFirstRow();
    if (cmp != 0) return cmp;
    cmp = left.GetFirstColumn() - right.GetFirstColumn();
    if (cmp != 0) return cmp;
    cmp = left.GetTotalRows() - right.GetTotalRows();
    if (cmp != 0) return cmp;
    return left.GetTotalColumns() - right.GetTotalColumns();
}

int CompareColumnRangesByBounds(const ColumnRangeModel& left,
                                const ColumnRangeModel& right)
{
    const int minCmp = left.GetMinColumnIndex() - right.GetMinColumnIndex();
    if (minCmp != 0) {
        return minCmp;
    }
    return left.GetMaxColumnIndex() - right.GetMaxColumnIndex();
}

std::vector<std::pair<CellAddress, CellRecord>>
CollectPersistedCells(const WorksheetModel& worksheet,
                      const StyleValue& workbookDefaultStyle)
{
    std::vector<std::pair<CellAddress, CellRecord>> persistedCells;
    for (const auto& [address, record] : worksheet.GetCells()) {
        if (XlsxWorkbookSerializerCommon::ShouldPersistCell(workbookDefaultStyle, record)) {
            persistedCells.emplace_back(address, record);
        }
    }
    std::sort(persistedCells.begin(), persistedCells.end(),
              [](const auto& l, const auto& r) {
                  return ComparePersistedCells(l, r) < 0;
              });
    return persistedCells;
}

std::vector<XmlElement> BuildColumnElements(
    const std::vector<ColumnRangeModel>& columns)
{
    std::vector<XmlElement> columnElements;
    columnElements.reserve(columns.size());
    for (const auto& column : columns) {
        columnElements.push_back(
            XlsxWorkbookSerializerCommon::BuildColumnElement(column));
    }
    return columnElements;
}

std::vector<std::pair<CellAddress, CellRecord>> GetRowCells(
    const std::vector<std::pair<CellAddress, CellRecord>>& persistedCells,
    int rowIndex)
{
    std::vector<std::pair<CellAddress, CellRecord>> rowCells;
    for (const auto& pair : persistedCells) {
        if (pair.first.GetRowIndex() == rowIndex) {
            rowCells.push_back(pair);
        }
    }
    return rowCells;
}

std::vector<MergeRegion> SortMergeRegions(
    const std::vector<MergeRegion>& mergeRegions)
{
    std::vector<MergeRegion> sortedRegions(mergeRegions.begin(), mergeRegions.end());
    std::sort(sortedRegions.begin(), sortedRegions.end(),
              [](const MergeRegion& l, const MergeRegion& r) {
                  return CompareMergeRegions(l, r) < 0;
              });
    return sortedRegions;
}

std::vector<XmlElement> BuildMergeCellElements(
    const std::vector<MergeRegion>& mergeRegions)
{
    std::vector<XmlElement> mergeCellElements;
    mergeCellElements.reserve(mergeRegions.size());
    for (const auto& region : mergeRegions) {
        mergeCellElements.push_back(XmlElement::MakeElement(
            XlsxWorkbookSerializerCommon::MainNs + "mergeCell",
            Attrs{{"ref", XlsxWorkbookSerializerCommon::ToRangeReference(region)}},
            Children{}));
    }
    return mergeCellElements;
}

}  // anonymous namespace

// ---------------------------------------------------------------------------
// ShouldPersistCell
// ---------------------------------------------------------------------------

bool XlsxWorkbookSerializerCommon::ShouldPersistCell(
    const StyleValue& workbookDefaultStyle,
    const CellRecord& record)
{
    return record.GetIsExplicitlyStored()
        || !record.GetFormula().empty()
        || !record.GetValue().IsEmpty()
        || record.GetKind() != CellValueKind::Blank
        || !XlsxWorkbookStyles::StylesEqual(record.GetStyle(), workbookDefaultStyle);
}

// ---------------------------------------------------------------------------
// WriteXmlEntry
// ---------------------------------------------------------------------------

void XlsxWorkbookSerializerCommon::WriteXmlEntry(
    ZipArchive& archive,
    std::string_view path,
    const XmlDocument& document)
{
    archive.AddEntry(path, document.SaveToUtf8());
}

// ---------------------------------------------------------------------------
// BuildContentTypes
// ---------------------------------------------------------------------------

XmlDocument XlsxWorkbookSerializerCommon::BuildContentTypes(
    const WorkbookModel& model,
    bool hasSharedStrings,
    bool hasDateStyles,
    bool hasCoreProperties,
    bool hasExtendedProperties)
{
    auto root = XmlElement::MakeElement(
        ContentTypeNs + "Types",
        Attrs{},
        Children{
            XmlElement::MakeElement(ContentTypeNs + "Default",
                Attrs{{"Extension", "rels"},
                      {"ContentType", "application/vnd.openxmlformats-package.relationships+xml"}},
                Children{}),
            XmlElement::MakeElement(ContentTypeNs + "Default",
                Attrs{{"Extension", "xml"},
                      {"ContentType", "application/xml"}},
                Children{}),
            XmlElement::MakeElement(ContentTypeNs + "Override",
                Attrs{{"PartName", "/xl/workbook.xml"},
                      {"ContentType", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml"}},
                Children{})
        });

    if (hasCoreProperties) {
        root.AddChild(XmlElement::MakeElement(
            ContentTypeNs + "Override",
            Attrs{{"PartName", "/docProps/core.xml"},
                  {"ContentType", "application/vnd.openxmlformats-package.core-properties+xml"}},
            Children{}));
    }

    if (hasExtendedProperties) {
        root.AddChild(XmlElement::MakeElement(
            ContentTypeNs + "Override",
            Attrs{{"PartName", "/docProps/app.xml"},
                  {"ContentType", "application/vnd.openxmlformats-officedocument.extended-properties+xml"}},
            Children{}));
    }

    const auto& worksheets = model.GetWorksheets();
    for (std::size_t index = 0; index < worksheets.size(); ++index) {
        root.AddChild(XmlElement::MakeElement(
            ContentTypeNs + "Override",
            Attrs{{"PartName", "/xl/worksheets/sheet" + std::to_string(index + 1) + ".xml"},
                  {"ContentType", "application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml"}},
            Children{}));
    }

    if (hasSharedStrings) {
        root.AddChild(XmlElement::MakeElement(
            ContentTypeNs + "Override",
            Attrs{{"PartName", "/xl/sharedStrings.xml"},
                  {"ContentType", "application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml"}},
            Children{}));
    }

    if (hasDateStyles) {
        root.AddChild(XmlElement::MakeElement(
            ContentTypeNs + "Override",
            Attrs{{"PartName", "/xl/styles.xml"},
                  {"ContentType", "application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml"}},
            Children{}));
    }

    return XmlDocument::Build(root);
}

// ---------------------------------------------------------------------------
// BuildRootRelationships
// ---------------------------------------------------------------------------

XmlDocument XlsxWorkbookSerializerCommon::BuildRootRelationships(
    bool hasCoreProperties,
    bool hasExtendedProperties)
{
    auto relationships = XmlElement::MakeElement(
        PackageRelationshipNs + "Relationships",
        Attrs{},
        Children{XmlElement::MakeElement(
            PackageRelationshipNs + "Relationship",
            Attrs{{"Id", "rId1"},
                  {"Type", "http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument"},
                  {"Target", "xl/workbook.xml"}},
            Children{})});

    int relationshipId = 2;
    if (hasCoreProperties) {
        relationships.AddChild(XmlElement::MakeElement(
            PackageRelationshipNs + "Relationship",
            Attrs{{"Id", "rId" + std::to_string(relationshipId)},
                  {"Type", CorePropertiesRelationshipType},
                  {"Target", "docProps/core.xml"}},
            Children{}));
        ++relationshipId;
    }

    if (hasExtendedProperties) {
        relationships.AddChild(XmlElement::MakeElement(
            PackageRelationshipNs + "Relationship",
            Attrs{{"Id", "rId" + std::to_string(relationshipId)},
                  {"Type", ExtendedPropertiesRelationshipType},
                  {"Target", "docProps/app.xml"}},
            Children{}));
    }

    return XmlDocument::Build(relationships);
}

// ---------------------------------------------------------------------------
// BuildWorkbook
// ---------------------------------------------------------------------------

XmlDocument XlsxWorkbookSerializerCommon::BuildWorkbook(
    const WorkbookModel& model)
{
    auto workbook = XmlElement::MakeElement(
        MainNs + "workbook",
        Attrs{{"xmlns:r", RelationshipNs.GetUri()}},
        Children{});

    auto workbookProperties = XlsxWorkbookProperties::BuildWorkbookPropertiesElement(model);
    if (!workbookProperties.IsNull()) {
        workbook.AddChild(std::move(workbookProperties));
    }

    auto workbookProtection = XlsxWorkbookProperties::BuildWorkbookProtectionElement(model);
    if (!workbookProtection.IsNull()) {
        workbook.AddChild(std::move(workbookProtection));
    }

    auto bookViews = XlsxWorkbookProperties::BuildBookViewsElement(model);
    if (!bookViews.IsNull()) {
        workbook.AddChild(std::move(bookViews));
    }

    auto sheets = XmlElement::MakeElement(MainNs + "sheets", Attrs{}, Children{});
    const auto& worksheets = model.GetWorksheets();
    for (std::size_t index = 0; index < worksheets.size(); ++index) {
        const auto& ws = worksheets[index];
        auto sheet = XmlElement::MakeElement(
            MainNs + "sheet",
            Attrs{{"name", ws.GetName()},
                  {"sheetId", std::to_string(index + 1)},
                  {"r:id", "rId" + std::to_string(index + 1)}},
            Children{});

        if (ws.GetVisibility() == SheetVisibility::Hidden) {
            sheet.SetAttribute("state", "hidden");
        } else if (ws.GetVisibility() == SheetVisibility::VeryHidden) {
            sheet.SetAttribute("state", "veryHidden");
        }

        sheets.AddChild(std::move(sheet));
    }

    workbook.AddChild(std::move(sheets));

    auto definedNames = XlsxWorkbookDefinedNames::BuildDefinedNames(model);
    if (!definedNames.IsNull()) {
        workbook.AddChild(std::move(definedNames));
    }

    auto calculationProperties = XlsxWorkbookProperties::BuildCalculationPropertiesElement(model);
    if (!calculationProperties.IsNull()) {
        workbook.AddChild(std::move(calculationProperties));
    }

    return XmlDocument::Build(workbook);
}

// ---------------------------------------------------------------------------
// BuildWorkbookRelationships
// ---------------------------------------------------------------------------

XmlDocument XlsxWorkbookSerializerCommon::BuildWorkbookRelationships(
    const WorkbookModel& model,
    bool hasSharedStrings,
    bool hasDateStyles)
{
    auto relationships = XmlElement::MakeElement(
        PackageRelationshipNs + "Relationships", Attrs{}, Children{});
    int relationshipId = 1;

    const auto& worksheets = model.GetWorksheets();
    for (std::size_t index = 0; index < worksheets.size(); ++index) {
        relationships.AddChild(XmlElement::MakeElement(
            PackageRelationshipNs + "Relationship",
            Attrs{{"Id", "rId" + std::to_string(relationshipId++)},
                  {"Type", WorksheetRelationshipType},
                  {"Target", "worksheets/sheet" + std::to_string(index + 1) + ".xml"}},
            Children{}));
    }

    if (hasSharedStrings) {
        relationships.AddChild(XmlElement::MakeElement(
            PackageRelationshipNs + "Relationship",
            Attrs{{"Id", "rId" + std::to_string(relationshipId++)},
                  {"Type", SharedStringsRelationshipType},
                  {"Target", "sharedStrings.xml"}},
            Children{}));
    }

    if (hasDateStyles) {
        relationships.AddChild(XmlElement::MakeElement(
            PackageRelationshipNs + "Relationship",
            Attrs{{"Id", "rId" + std::to_string(relationshipId++)},
                  {"Type", StylesRelationshipType},
                  {"Target", "styles.xml"}},
            Children{}));
    }

    return XmlDocument::Build(relationships);
}

// ---------------------------------------------------------------------------
// BuildWorksheet
// ---------------------------------------------------------------------------

XmlDocument XlsxWorkbookSerializerCommon::BuildWorksheet(
    WorksheetModel& worksheet,
    const StyleValue& workbookDefaultStyle,
    DateSystem dateSystem,
    SharedStringRepository& sharedStrings,
    const SaveOptions& options,
    StylesheetSaveContext& stylesheet)
{
    auto persistedCells = CollectPersistedCells(worksheet, workbookDefaultStyle);
    auto worksheetElement = XmlElement::MakeElement(
        MainNs + "worksheet",
        Attrs{{"xmlns:r", RelationshipNs.GetUri()}},
        Children{});

    auto sheetProperties = XlsxWorkbookWorksheetViews::BuildWorksheetSheetProperties(worksheet);
    if (!sheetProperties.IsNull()) {
        worksheetElement.AddChild(std::move(sheetProperties));
    }

    auto dimensionReference = CalculateDimensionReference(
        persistedCells, worksheet.GetMergeRegions());
    if (!dimensionReference.empty()) {
        worksheetElement.AddChild(XmlElement::MakeElement(
            MainNs + "dimension",
            Attrs{{"ref", dimensionReference}},
            Children{}));
    }

    auto sheetViews = XlsxWorkbookWorksheetViews::BuildWorksheetViewsElement(worksheet);
    if (!sheetViews.IsNull()) {
        worksheetElement.AddChild(std::move(sheetViews));
    }

    auto normalizedColumns = NormalizeColumnRanges(worksheet.GetColumns());
    if (!normalizedColumns.empty()) {
        worksheetElement.AddChild(XmlElement::MakeElement(
            MainNs + "cols", Attrs{}, BuildColumnElements(normalizedColumns)));
    }

    auto sheetData = XmlElement::MakeElement(MainNs + "sheetData", Attrs{}, Children{});
    const RowModel defaultRowModel;
    const auto rowIndexes = GetWorksheetRowIndexes(persistedCells, worksheet.GetRows());
    for (const int rowIndex : rowIndexes) {
        auto rowCells = GetRowCells(persistedCells, rowIndex);
        auto rowIt = worksheet.GetRows().find(rowIndex);
        const RowModel& rowModel =
            (rowIt != worksheet.GetRows().end()) ? rowIt->second : defaultRowModel;
        if (rowCells.empty() && !HasRowMetadata(rowModel)) {
            continue;
        }
        sheetData.AddChild(BuildRowElement(rowIndex, rowModel, rowCells,
                                           dateSystem, sharedStrings, options, stylesheet));
    }

    worksheetElement.AddChild(std::move(sheetData));

    auto sheetProtection = XlsxWorkbookWorksheetProtection::BuildSheetProtectionElement(worksheet);
    if (!sheetProtection.IsNull()) {
        worksheetElement.AddChild(std::move(sheetProtection));
    }

    auto autoFilter = XlsxWorkbookAutoFilter::BuildAutoFilterElement(
        worksheet, stylesheet.GetDifferentialFormatCount());
    if (!autoFilter.IsNull()) {
        worksheetElement.AddChild(std::move(autoFilter));
    }

    auto mergeRegions = SortMergeRegions(worksheet.GetMergeRegions());
    if (!mergeRegions.empty()) {
        auto mergeCells = XmlElement::MakeElement(
            MainNs + "mergeCells",
            Attrs{{"count", std::to_string(mergeRegions.size())}},
            BuildMergeCellElements(mergeRegions));
        worksheetElement.AddChild(std::move(mergeCells));
    }

    auto condFormattingElements =
        XlsxWorkbookConditionalFormatting::BuildConditionalFormattingElements(
            worksheet, stylesheet);
    for (auto& elem : condFormattingElements) {
        worksheetElement.AddChild(std::move(elem));
    }

    auto dataValidations = XlsxWorkbookValidations::BuildDataValidationsElement(worksheet);
    if (!dataValidations.IsNull()) {
        worksheetElement.AddChild(std::move(dataValidations));
    }

    auto hyperlinks = XlsxWorkbookHyperlinks::BuildHyperlinksElement(worksheet);
    if (!hyperlinks.IsNull()) {
        worksheetElement.AddChild(std::move(hyperlinks));
    }

    auto printOptions = XlsxWorkbookPageSetup::BuildPrintOptionsElement(worksheet.GetPageSetup());
    if (!printOptions.IsNull()) {
        worksheetElement.AddChild(std::move(printOptions));
    }

    auto pageMargins = XlsxWorkbookPageSetup::BuildPageMarginsElement(worksheet.GetPageSetup());
    if (!pageMargins.IsNull()) {
        worksheetElement.AddChild(std::move(pageMargins));
    }

    auto pageSetup = XlsxWorkbookPageSetup::BuildPageSetupElement(worksheet.GetPageSetup());
    if (!pageSetup.IsNull()) {
        worksheetElement.AddChild(std::move(pageSetup));
    }

    auto headerFooter = XlsxWorkbookPageSetup::BuildHeaderFooterElement(worksheet.GetPageSetup());
    if (!headerFooter.IsNull()) {
        worksheetElement.AddChild(std::move(headerFooter));
    }

    auto rowBreaks = XlsxWorkbookPageSetup::BuildRowBreaksElement(worksheet.GetPageSetup());
    if (!rowBreaks.IsNull()) {
        worksheetElement.AddChild(std::move(rowBreaks));
    }

    auto columnBreaks = XlsxWorkbookPageSetup::BuildColumnBreaksElement(worksheet.GetPageSetup());
    if (!columnBreaks.IsNull()) {
        worksheetElement.AddChild(std::move(columnBreaks));
    }

    return XmlDocument::Build(worksheetElement);
}

// ---------------------------------------------------------------------------
// BuildSharedStrings
// ---------------------------------------------------------------------------

XmlDocument XlsxWorkbookSerializerCommon::BuildSharedStrings(
    const SharedStringRepository& sharedStrings)
{
    const auto& values = sharedStrings.GetValues();
    auto root = XmlElement::MakeElement(
        MainNs + "sst",
        Attrs{{"count", std::to_string(values.size())},
              {"uniqueCount", std::to_string(values.size())}},
        Children{});

    for (const auto& value : values) {
        root.AddChild(XmlElement::MakeElement(
            MainNs + "si", Attrs{},
            Children{CreateTextElement(value)}));
    }

    return XmlDocument::Build(root);
}

// ---------------------------------------------------------------------------
// BuildMinimalStylesheet
// ---------------------------------------------------------------------------

XmlDocument XlsxWorkbookSerializerCommon::BuildMinimalStylesheet()
{
    auto stylesheet = XmlElement::MakeElement(
        MainNs + "styleSheet",
        Attrs{},
        Children{
            // fonts
            XmlElement::MakeElement(MainNs + "fonts",
                Attrs{{"count", "1"}},
                Children{XmlElement::MakeElement(MainNs + "font", Attrs{},
                    Children{
                        XmlElement::MakeElement(MainNs + "sz", Attrs{{"val", "11"}}, Children{}),
                        XmlElement::MakeElement(MainNs + "name", Attrs{{"val", "Calibri"}}, Children{})
                    })}),
            // fills
            XmlElement::MakeElement(MainNs + "fills",
                Attrs{{"count", "2"}},
                Children{
                    XmlElement::MakeElement(MainNs + "fill", Attrs{},
                        Children{XmlElement::MakeElement(MainNs + "patternFill",
                            Attrs{{"patternType", "none"}}, Children{})}),
                    XmlElement::MakeElement(MainNs + "fill", Attrs{},
                        Children{XmlElement::MakeElement(MainNs + "patternFill",
                            Attrs{{"patternType", "gray125"}}, Children{})})
                }),
            // borders
            XmlElement::MakeElement(MainNs + "borders",
                Attrs{{"count", "1"}},
                Children{XmlElement::MakeElement(MainNs + "border", Attrs{},
                    Children{
                        XmlElement::MakeElement(MainNs + "left", Attrs{}, Children{}),
                        XmlElement::MakeElement(MainNs + "right", Attrs{}, Children{}),
                        XmlElement::MakeElement(MainNs + "top", Attrs{}, Children{}),
                        XmlElement::MakeElement(MainNs + "bottom", Attrs{}, Children{}),
                        XmlElement::MakeElement(MainNs + "diagonal", Attrs{}, Children{})
                    })}),
            // cellStyleXfs
            XmlElement::MakeElement(MainNs + "cellStyleXfs",
                Attrs{{"count", "1"}},
                Children{XmlElement::MakeElement(MainNs + "xf",
                    Attrs{{"numFmtId", "0"}, {"fontId", "0"}, {"fillId", "0"}, {"borderId", "0"}},
                    Children{})}),
            // cellXfs
            XmlElement::MakeElement(MainNs + "cellXfs",
                Attrs{{"count", "2"}},
                Children{
                    XmlElement::MakeElement(MainNs + "xf",
                        Attrs{{"numFmtId", "0"}, {"fontId", "0"}, {"fillId", "0"},
                              {"borderId", "0"}, {"xfId", "0"}},
                        Children{}),
                    XmlElement::MakeElement(MainNs + "xf",
                        Attrs{{"numFmtId", "14"}, {"fontId", "0"}, {"fillId", "0"},
                              {"borderId", "0"}, {"xfId", "0"}, {"applyNumberFormat", "1"}},
                        Children{})
                }),
            // cellStyles
            XmlElement::MakeElement(MainNs + "cellStyles",
                Attrs{{"count", "1"}},
                Children{XmlElement::MakeElement(MainNs + "cellStyle",
                    Attrs{{"name", "Normal"}, {"xfId", "0"}, {"builtinId", "0"}},
                    Children{})})
        });

    return XmlDocument::Build(stylesheet);
}

// ---------------------------------------------------------------------------
// BuildRowElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookSerializerCommon::BuildRowElement(
    int rowIndex,
    const RowModel& rowModel,
    const std::vector<std::pair<CellAddress, CellRecord>>& rowCells,
    DateSystem dateSystem,
    SharedStringRepository& sharedStrings,
    const SaveOptions& options,
    StylesheetSaveContext& stylesheet)
{
    auto row = XmlElement::MakeElement(
        MainNs + "row",
        Attrs{{"r", std::to_string(rowIndex + 1)}},
        Children{});

    if (rowModel.GetHeight().has_value()) {
        row.SetAttribute("ht", DoubleToRoundTrip(*rowModel.GetHeight()));
        row.SetAttribute("customHeight", "1");
    }

    if (rowModel.GetHidden()) {
        row.SetAttribute("hidden", "1");
    }

    if (rowModel.GetStyleIndex().has_value() && *rowModel.GetStyleIndex() >= 0) {
        row.SetAttribute("s", std::to_string(*rowModel.GetStyleIndex()));
        row.SetAttribute("customFormat", "1");
    }

    for (const auto& pair : rowCells) {
        row.AddChild(BuildCell(pair.first, pair.second, dateSystem,
                               sharedStrings, options, stylesheet));
    }

    return row;
}

// ---------------------------------------------------------------------------
// BuildColumnElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookSerializerCommon::BuildColumnElement(
    const ColumnRangeModel& column)
{
    auto element = XmlElement::MakeElement(
        MainNs + "col",
        Attrs{{"min", std::to_string(column.GetMinColumnIndex() + 1)},
              {"max", std::to_string(column.GetMaxColumnIndex() + 1)}},
        Children{});

    if (column.GetWidth().has_value()) {
        element.SetAttribute("width", DoubleToRoundTrip(*column.GetWidth()));
        element.SetAttribute("customWidth", "1");
    }

    if (column.GetHidden()) {
        element.SetAttribute("hidden", "1");
    }

    if (column.GetStyleIndex().has_value() && *column.GetStyleIndex() >= 0) {
        element.SetAttribute("style", std::to_string(*column.GetStyleIndex()));
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildCell
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookSerializerCommon::BuildCell(
    const CellAddress& address,
    const CellRecord& record,
    DateSystem dateSystem,
    SharedStringRepository& sharedStrings,
    const SaveOptions& options,
    StylesheetSaveContext& stylesheet)
{
    auto cell = XmlElement::MakeElement(
        MainNs + "c",
        Attrs{{"r", address.ToString()}},
        Children{});

    const int styleIndex = stylesheet.GetStyleIndex(record);
    if (styleIndex > 0) {
        cell.SetAttribute("s", std::to_string(styleIndex));
    }

    const bool hasFormula = !record.GetFormula().empty();
    if (hasFormula) {
        cell.AddChild(ElemText(MainNs + "f", record.GetFormula()));
    }

    const auto& value = record.GetValue();
    if (value.IsEmpty()) {
        return cell;
    }

    if (value.IsString()) {
        const auto text = value.AsString();
        if (hasFormula) {
            cell.SetAttribute("t", "str");
            cell.AddChild(ElemText(MainNs + "v", text));
        } else if (options.GetUseSharedStrings()) {
            cell.SetAttribute("t", "s");
            cell.AddChild(ElemText(MainNs + "v",
                std::to_string(sharedStrings.Intern(text))));
        } else {
            cell.SetAttribute("t", "inlineStr");
            cell.AddChild(XmlElement::MakeElement(MainNs + "is", Attrs{},
                Children{CreateTextElement(text)}));
        }
    } else if (value.IsBool()) {
        cell.SetAttribute("t", "b");
        cell.AddChild(ElemText(MainNs + "v", value.AsBool() ? "1" : "0"));
    } else if (value.IsDateTime()) {
        const auto dateTime = value.AsDateTime();
        cell.AddChild(ElemText(MainNs + "v",
            DoubleToRoundTrip(Core::DateSerialConverter::ToSerial(dateTime, dateSystem))));
    } else if (value.IsInteger()) {
        cell.AddChild(ElemText(MainNs + "v", std::to_string(value.AsInteger())));
    } else if (value.IsDouble()) {
        cell.AddChild(ElemText(MainNs + "v", DoubleToRoundTrip(value.AsDouble())));
    }

    return cell;
}

// ---------------------------------------------------------------------------
// CreateTextElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookSerializerCommon::CreateTextElement(
    std::string_view value)
{
    auto text = XmlElement::MakeElement(MainNs + "t");
    text.AddChild(MakeTextNode(std::string(value)));
    if (NeedsPreserveWhitespace(value)) {
        text.SetAttribute("xml:space", "preserve");
    }
    return text;
}

// ---------------------------------------------------------------------------
// HasRowMetadata
// ---------------------------------------------------------------------------

bool XlsxWorkbookSerializerCommon::HasRowMetadata(const RowModel& rowModel)
{
    return rowModel.GetHeight().has_value()
        || rowModel.GetHidden()
        || rowModel.GetStyleIndex().has_value();
}

// ---------------------------------------------------------------------------
// HasColumnMetadata
// ---------------------------------------------------------------------------

bool XlsxWorkbookSerializerCommon::HasColumnMetadata(
    const ColumnRangeModel& column)
{
    return column.GetWidth().has_value()
        || column.GetHidden()
        || column.GetStyleIndex().has_value();
}

// ---------------------------------------------------------------------------
// NeedsPreserveWhitespace
// ---------------------------------------------------------------------------

bool XlsxWorkbookSerializerCommon::NeedsPreserveWhitespace(
    std::string_view value)
{
    if (value.empty()) {
        return false;
    }
    return std::isspace(static_cast<unsigned char>(value.front()))
        || std::isspace(static_cast<unsigned char>(value.back()))
        || value.find('\n') != std::string_view::npos
        || value.find('\r') != std::string_view::npos
        || value.find('\t') != std::string_view::npos;
}

// ---------------------------------------------------------------------------
// GetWorksheetRowIndexes
// ---------------------------------------------------------------------------

std::vector<int> XlsxWorkbookSerializerCommon::GetWorksheetRowIndexes(
    const std::vector<std::pair<CellAddress, CellRecord>>& persistedCells,
    const std::unordered_map<int, RowModel>& rows)
{
    std::set<int> indexSet;
    for (const auto& pair : persistedCells) {
        indexSet.insert(pair.first.GetRowIndex());
    }
    for (const auto& [rowIndex, _] : rows) {
        (void)_;
        indexSet.insert(rowIndex);
    }
    return std::vector<int>(indexSet.begin(), indexSet.end());
}

// ---------------------------------------------------------------------------
// NormalizeColumnRanges
// ---------------------------------------------------------------------------

std::vector<ColumnRangeModel> XlsxWorkbookSerializerCommon::NormalizeColumnRanges(
    const std::vector<ColumnRangeModel>& columns)
{
    std::vector<ColumnRangeModel> ordered;
    for (const auto& column : columns) {
        if (!HasColumnMetadata(column)) {
            continue;
        }
        ColumnRangeModel copy;
        copy.SetMinColumnIndex(column.GetMinColumnIndex());
        copy.SetMaxColumnIndex(column.GetMaxColumnIndex());
        copy.SetWidth(column.GetWidth());
        copy.SetHidden(column.GetHidden());
        copy.SetStyleIndex(column.GetStyleIndex());
        ordered.push_back(std::move(copy));
    }

    std::sort(ordered.begin(), ordered.end(),
              [](const ColumnRangeModel& l, const ColumnRangeModel& r) {
                  return CompareColumnRangesByBounds(l, r) < 0;
              });

    if (ordered.empty()) {
        return ordered;
    }

    std::vector<ColumnRangeModel> normalized;
    normalized.push_back(ordered[0]);
    for (std::size_t index = 1; index < ordered.size(); ++index) {
        auto& current = ordered[index];
        auto& previous = normalized.back();
        if (previous.GetMaxColumnIndex() + 1 >= current.GetMinColumnIndex()
            && ColumnMetadataEqual(previous, current)) {
            previous.SetMaxColumnIndex(
                std::max(previous.GetMaxColumnIndex(), current.GetMaxColumnIndex()));
            continue;
        }
        normalized.push_back(current);
    }

    return normalized;
}

// ---------------------------------------------------------------------------
// ColumnMetadataEqual
// ---------------------------------------------------------------------------

bool XlsxWorkbookSerializerCommon::ColumnMetadataEqual(
    const ColumnRangeModel& left,
    const ColumnRangeModel& right)
{
    return left.GetWidth() == right.GetWidth()
        && left.GetHidden() == right.GetHidden()
        && left.GetStyleIndex() == right.GetStyleIndex();
}

// ---------------------------------------------------------------------------
// CalculateDimensionReference
// ---------------------------------------------------------------------------

std::string XlsxWorkbookSerializerCommon::CalculateDimensionReference(
    const std::vector<std::pair<CellAddress, CellRecord>>& persistedCells,
    const std::vector<MergeRegion>& mergeRegions)
{
    const bool hasCells = !persistedCells.empty();
    const bool hasMerges = !mergeRegions.empty();
    if (!hasCells && !hasMerges) {
        return std::string();
    }

    int minRow = (std::numeric_limits<int>::max)();
    int minColumn = (std::numeric_limits<int>::max)();
    int maxRow = (std::numeric_limits<int>::min)();
    int maxColumn = (std::numeric_limits<int>::min)();

    for (const auto& pair : persistedCells) {
        minRow = (std::min)(minRow, pair.first.GetRowIndex());
        minColumn = (std::min)(minColumn, pair.first.GetColumnIndex());
        maxRow = (std::max)(maxRow, pair.first.GetRowIndex());
        maxColumn = (std::max)(maxColumn, pair.first.GetColumnIndex());
    }

    for (const auto& region : mergeRegions) {
        minRow = (std::min)(minRow, region.GetFirstRow());
        minColumn = (std::min)(minColumn, region.GetFirstColumn());
        maxRow = (std::max)(maxRow, region.GetFirstRow() + region.GetTotalRows() - 1);
        maxColumn = (std::max)(maxColumn, region.GetFirstColumn() + region.GetTotalColumns() - 1);
    }

    const auto firstAddress = CellAddress(minRow, minColumn).ToString();
    const auto lastAddress = CellAddress(maxRow, maxColumn).ToString();
    return firstAddress == lastAddress
        ? firstAddress
        : firstAddress + ":" + lastAddress;
}

// ---------------------------------------------------------------------------
// ToRangeReference
// ---------------------------------------------------------------------------

std::string XlsxWorkbookSerializerCommon::ToRangeReference(
    const MergeRegion& region)
{
    const auto first = CellAddress(region.GetFirstRow(), region.GetFirstColumn()).ToString();
    const auto last = CellAddress(region.GetFirstRow() + region.GetTotalRows() - 1,
                                  region.GetFirstColumn() + region.GetTotalColumns() - 1).ToString();
    return first == last ? first : first + ":" + last;
}

// ---------------------------------------------------------------------------
// TryParseMergeReference
// ---------------------------------------------------------------------------

bool XlsxWorkbookSerializerCommon::TryParseMergeReference(
    std::string_view mergeReference,
    MergeRegion& region)
{
    // Check for all-whitespace input (matches C# IsNullOrWhiteSpace).
    bool allWhitespace = true;
    for (char ch : mergeReference) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            allWhitespace = false;
            break;
        }
    }
    if (allWhitespace) {
        return false;
    }

    // Find the colon separator.
    const auto colonPos = mergeReference.find(':');
    if (colonPos == std::string_view::npos) {
        // Single cell reference.
        CellAddress single;
        if (!TryParseCellReference(mergeReference, single)) {
            return false;
        }
        region = MergeRegion(single.GetRowIndex(), single.GetColumnIndex(), 1, 1);
        return true;
    }

    // Two-part range reference.
    const auto firstPart = mergeReference.substr(0, colonPos);
    const auto lastPart = mergeReference.substr(colonPos + 1);

    CellAddress first, last;
    if (!TryParseCellReference(firstPart, first) ||
        !TryParseCellReference(lastPart, last) ||
        last.GetRowIndex() < first.GetRowIndex() ||
        last.GetColumnIndex() < first.GetColumnIndex()) {
        return false;
    }

    region = MergeRegion(first.GetRowIndex(), first.GetColumnIndex(),
                         last.GetRowIndex() - first.GetRowIndex() + 1,
                         last.GetColumnIndex() - first.GetColumnIndex() + 1);
    return true;
}

// ---------------------------------------------------------------------------
// TryParseCellReference
// ---------------------------------------------------------------------------

bool XlsxWorkbookSerializerCommon::TryParseCellReference(
    std::string_view cellReference,
    CellAddress& address)
{
    try {
        address = CellAddress::Parse(cellReference);
        return true;
    } catch (...) {
        address = CellAddress();
        return false;
    }
}

}}  // namespace Aspose::Cells_FOSS
