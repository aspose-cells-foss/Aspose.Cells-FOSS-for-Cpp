#include <gtest/gtest.h>

#include "Support/PackageTestSupport.h"
#include "Support/ScenarioFactories.h"
#include "Support/TestSupport.h"

#include "aspose/cells_foss/CellValueType.h"
#include "aspose/cells_foss/Color.h"
#include "aspose/cells_foss/FilterOperatorType.h"
#include "aspose/cells_foss/FormatConditionType.h"
#include "aspose/cells_foss/OperatorType.h"
#include "aspose/cells_foss/Workbook.h"

using namespace Aspose::Cells_FOSS;
using namespace Aspose::Cells_FOSS::Tests;

TEST(OpenXmlFeatureGoldenTests, worksheet_settings_roundtrip_and_emit_expected_markup)
{
    TempDir temp("golden-worksheet-settings");
    const auto path = temp.Path("worksheet-settings.xlsx");
    auto workbook = CreateWorksheetSettingsWorkbook();
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    const auto workbookXml = Package::ReadEntryText(path, "xl/workbook.xml");
    EXPECT_TRUE(Contains(worksheetXml, "dimension ref=\"A1:C4\""));
    EXPECT_TRUE(Contains(worksheetXml, "mergeCell ref=\"A1:B2\""));
    EXPECT_TRUE(Contains(worksheetXml, "hidden=\"1\""));
    EXPECT_TRUE(Contains(workbookXml, "state=\"hidden\""));

    Workbook loaded(path.string());
    AssertWorksheetSettings(loaded);
    AssertWorksheetSettingsScenarioHasVisibleSheet(loaded);
}

TEST(OpenXmlFeatureGoldenTests, worksheet_view_and_tab_color_roundtrip_and_emit_expected_markup)
{
    TempDir temp("golden-worksheet-view");
    const auto path = temp.Path("worksheet-view.xlsx");
    auto workbook = CreateWorksheetSettingsWorkbook();
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    EXPECT_TRUE(Contains(worksheetXml, "<sheetViews>"));
    EXPECT_TRUE(Contains(worksheetXml, "showGridLines=\"0\""));
    EXPECT_TRUE(Contains(worksheetXml, "showRowColHeaders=\"0\""));
    EXPECT_TRUE(Contains(worksheetXml, "showZeros=\"0\""));
    EXPECT_TRUE(Contains(worksheetXml, "rightToLeft=\"1\""));
    EXPECT_TRUE(Contains(worksheetXml, "zoomScale=\"85\""));
    EXPECT_TRUE(Contains(worksheetXml, "tabColor rgb=\"FF224466\""));

    Workbook loaded(path.string());
    AssertWorksheetSettings(loaded);
    AssertWorksheetSettingsScenarioHasVisibleSheet(loaded);
}

TEST(OpenXmlFeatureGoldenTests, worksheet_protection_roundtrip_and_emit_expected_markup)
{
    TempDir temp("golden-worksheet-protection");
    const auto path = temp.Path("worksheet-protection.xlsx");
    auto workbook = CreateWorksheetSettingsWorkbook();
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    EXPECT_TRUE(Contains(worksheetXml, "<sheetProtection"));
    EXPECT_TRUE(Contains(worksheetXml, "sheet=\"1\""));
    EXPECT_TRUE(Contains(worksheetXml, "objects=\"1\""));
    EXPECT_TRUE(Contains(worksheetXml, "scenarios=\"1\""));
    EXPECT_TRUE(Contains(worksheetXml, "formatCells=\"1\""));
    EXPECT_TRUE(Contains(worksheetXml, "insertRows=\"1\""));
    EXPECT_TRUE(Contains(worksheetXml, "autoFilter=\"1\""));
    EXPECT_TRUE(Contains(worksheetXml, "selectLockedCells=\"1\""));
    EXPECT_TRUE(Contains(worksheetXml, "selectUnlockedCells=\"1\""));

    Workbook loaded(path.string());
    AssertWorksheetSettings(loaded);
    AssertWorksheetSettingsScenarioHasVisibleSheet(loaded);
}

TEST(OpenXmlFeatureGoldenTests, autofilter_roundtrip_and_emit_expected_markup)
{
    TempDir temp("golden-autofilter");
    const auto path = temp.Path("autofilter.xlsx");
    auto workbook = CreateAutoFilterWorkbook();
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    const auto workbookXml = Package::ReadEntryText(path, "xl/workbook.xml");
    EXPECT_TRUE(Contains(worksheetXml, "<autoFilter ref=\"A1:E6\""));
    EXPECT_TRUE(Contains(worksheetXml, "<filterColumn colId=\"0\" hiddenButton=\"1\">"));
    EXPECT_TRUE(Contains(worksheetXml, "<filters><filter val=\"Open\" /><filter val=\"Closed\" /></filters>"));
    EXPECT_TRUE(Contains(worksheetXml, "<customFilters and=\"1\">"));
    EXPECT_TRUE(Contains(worksheetXml, "operator=\"greaterThanOrEqual\""));
    EXPECT_TRUE(Contains(worksheetXml, "operator=\"lessThanOrEqual\""));
    EXPECT_TRUE(Contains(worksheetXml, "<colorFilter dxfId=\"3\" cellColor=\"1\" />"));
    EXPECT_TRUE(Contains(worksheetXml, "<dynamicFilter type=\"thisMonth\" val=\"1\" maxVal=\"31\" />"));
    EXPECT_TRUE(Contains(worksheetXml, "<top10 top=\"0\" percent=\"1\" val=\"10\" filterVal=\"2.5\" />"));
    EXPECT_TRUE(Contains(worksheetXml, "<sortState ref=\"A2:E6\" caseSensitive=\"1\" sortMethod=\"pinYin\">"));
    EXPECT_TRUE(Contains(worksheetXml, "<sortCondition ref=\"B2:B6\" descending=\"1\" sortBy=\"value\" customList=\"High,Medium,Low\" />"));
    EXPECT_TRUE(Contains(worksheetXml, "<sortCondition ref=\"C2:C6\" sortBy=\"cellColor\" dxfId=\"4\" />"));
    EXPECT_TRUE(Contains(worksheetXml, "<sortCondition ref=\"E2:E6\" sortBy=\"icon\" iconSet=\"3TrafficLights1\" iconId=\"2\" />"));
    EXPECT_TRUE(Contains(workbookXml, "name=\"_xlnm._FilterDatabase\""));
    EXPECT_TRUE(Contains(workbookXml, "'Filtered'!$A$1:$E$6"));

    Workbook loaded(path.string());
    AssertAutoFilter(loaded);
}

TEST(OpenXmlFeatureGoldenTests, autofilter_omits_invalid_dxf_references)
{
    TempDir temp("golden-autofilter-invalid-dxf");
    const auto path = temp.Path("autofilter-invalid-dxf.xlsx");
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];
    sheet.SetName("InvalidDxf");
    sheet.GetCells()["A1"].PutValue("Status");
    sheet.GetCells()["A2"].PutValue("Open");
    sheet.GetCells()["A3"].PutValue("Closed");
    sheet.GetCells()["B1"].PutValue("Amount");
    sheet.GetCells()["B2"].PutValue(10);
    sheet.GetCells()["B3"].PutValue(20);
    sheet.GetAutoFilter().SetRange("A1:B3");

    auto column = sheet.GetAutoFilter().GetFilterColumns()[sheet.GetAutoFilter().GetFilterColumns().Add(0)];
    column.GetColorFilter().SetEnabled(true);
    column.GetColorFilter().SetDifferentialStyleId(9);
    column.GetColorFilter().SetCellColor(true);

    sheet.GetAutoFilter().GetSortState().SetRef("A2:B3");
    auto sortCondition = sheet.GetAutoFilter().GetSortState().GetSortConditions()[sheet.GetAutoFilter().GetSortState().GetSortConditions().Add("B2:B3")];
    sortCondition.SetSortBy("value");
    sortCondition.SetDifferentialStyleId(8);
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    EXPECT_TRUE(Contains(worksheetXml, "<autoFilter ref=\"A1:B3\""));
    EXPECT_FALSE(Contains(worksheetXml, "<colorFilter"));
    EXPECT_FALSE(Contains(worksheetXml, "dxfId=\"8\""));
    EXPECT_FALSE(Contains(worksheetXml, "dxfId=\"9\""));

    Workbook loaded(path.string());
    EXPECT_EQ("A1:B3", loaded.GetWorksheets()[0].GetAutoFilter().GetRange());
    EXPECT_EQ(0, loaded.GetWorksheets()[0].GetAutoFilter().GetFilterColumns().GetCount());
    ASSERT_EQ(1, loaded.GetWorksheets()[0].GetAutoFilter().GetSortState().GetSortConditions().GetCount());
    EXPECT_FALSE(loaded.GetWorksheets()[0].GetAutoFilter().GetSortState().GetSortConditions()[0].GetDifferentialStyleId().has_value());
}

TEST(OpenXmlFeatureGoldenTests, defined_names_roundtrip_and_emit_expected_markup)
{
    TempDir temp("golden-defined-names");
    const auto path = temp.Path("defined-names.xlsx");
    auto workbook = CreateDefinedNamesWorkbook();
    workbook.Save(path.string());

    const auto workbookXml = Package::ReadEntryText(path, "xl/workbook.xml");
    EXPECT_TRUE(Contains(workbookXml, "<definedNames>"));
    EXPECT_TRUE(Contains(workbookXml, "name=\"GlobalRange\""));
    EXPECT_TRUE(Contains(workbookXml, "hidden=\"1\""));
    EXPECT_TRUE(Contains(workbookXml, "comment=\"Primary range\""));
    EXPECT_TRUE(Contains(workbookXml, "name=\"LocalCell\""));
    EXPECT_TRUE(Contains(workbookXml, "localSheetId=\"1\""));
    EXPECT_TRUE(Contains(workbookXml, "name=\"_xlnm.Print_Area\""));
    EXPECT_TRUE(Contains(workbookXml, "name=\"_xlnm.Print_Titles\""));

    Workbook loaded(path.string());
    AssertDefinedNames(loaded);
}

TEST(OpenXmlFeatureGoldenTests, worksheet_dimension_includes_merge_only_ranges)
{
    TempDir temp("golden-merge-dimension");
    const auto path = temp.Path("merge-only.xlsx");
    Workbook workbook;
    workbook.GetWorksheets()[0].SetName("MergeOnly");
    workbook.GetWorksheets()[0].GetCells().Merge(4, 5, 2, 2);
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    EXPECT_TRUE(Contains(worksheetXml, "dimension ref=\"F5:G6\""));
    EXPECT_TRUE(Contains(worksheetXml, "mergeCell ref=\"F5:G6\""));

    Workbook loaded(path.string());
    const auto merged = loaded.GetWorksheets()[0].GetCells().GetMergedCells();
    ASSERT_EQ(1u, merged.size());
    EXPECT_EQ(4, merged[0].GetFirstRow());
    EXPECT_EQ(5, merged[0].GetFirstColumn());
}

TEST(OpenXmlFeatureGoldenTests, hyperlinks_roundtrip_and_emit_expected_markup)
{
    TempDir temp("golden-hyperlinks");
    const auto path = temp.Path("hyperlinks.xlsx");
    auto workbook = CreateHyperlinkWorkbook();
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    const auto relsXml = Package::ReadEntryText(path, "xl/worksheets/_rels/sheet1.xml.rels");
    EXPECT_TRUE(Contains(worksheetXml, "<hyperlinks>"));
    EXPECT_TRUE(Contains(worksheetXml, "ref=\"A1\""));
    EXPECT_TRUE(Contains(worksheetXml, "location=\"'Target Sheet'!C3\""));
    EXPECT_TRUE(Contains(relsXml, "Target=\"https://example.com/docs?q=1\""));
    EXPECT_TRUE(Contains(relsXml, "Target=\"mailto:test@example.com\""));

    Workbook loaded(path.string());
    AssertHyperlinks(loaded);
}

TEST(OpenXmlFeatureGoldenTests, data_validations_roundtrip_and_emit_expected_markup)
{
    TempDir temp("golden-validations");
    const auto path = temp.Path("validations.xlsx");
    auto workbook = CreateValidationWorkbook();
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    EXPECT_TRUE(Contains(worksheetXml, "<dataValidations"));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"list\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"decimal\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"custom\""));
    EXPECT_TRUE(Contains(worksheetXml, "sqref=\"A1:A3\""));
    EXPECT_TRUE(Contains(worksheetXml, "sqref=\"B2:C3 E2:E3\""));
    EXPECT_TRUE(Contains(worksheetXml, "showDropDown=\"1\""));
    EXPECT_TRUE(Contains(worksheetXml, "<formula1>\"Open,Closed\"</formula1>"));
    EXPECT_TRUE(Contains(worksheetXml, "<formula2>9.5</formula2>"));

    Workbook loaded(path.string());
    AssertValidations(loaded);
}

TEST(OpenXmlFeatureGoldenTests, conditional_formattings_roundtrip_and_emit_expected_markup)
{
    TempDir temp("golden-conditional-formatting");
    const auto path = temp.Path("conditional-formatting.xlsx");
    auto workbook = CreateConditionalFormattingWorkbook();
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    const auto stylesXml = Package::ReadEntryText(path, "xl/styles.xml");
    EXPECT_TRUE(Contains(worksheetXml, "<conditionalFormatting"));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"cellIs\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"expression\""));
    EXPECT_TRUE(Contains(worksheetXml, "operator=\"between\""));
    EXPECT_TRUE(Contains(worksheetXml, "stopIfTrue=\"1\""));
    EXPECT_TRUE(Contains(worksheetXml, "<formula>MOD(A1,2)=0</formula>"));
    EXPECT_TRUE(Contains(stylesXml, "<dxfs"));
    EXPECT_TRUE(Contains(stylesXml, "count=\"3\""));

    Workbook loaded(path.string());
    AssertConditionalFormattings(loaded);
}

TEST(OpenXmlFeatureGoldenTests, advanced_conditional_formattings_roundtrip_and_emit_expected_markup)
{
    TempDir temp("golden-advanced-conditional-formatting");
    const auto path = temp.Path("advanced-conditional-formatting.xlsx");
    auto workbook = CreateAdvancedConditionalFormattingWorkbook();
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    EXPECT_TRUE(Contains(worksheetXml, "type=\"containsText\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"notContainsText\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"beginsWith\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"endsWith\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"timePeriod\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"duplicateValues\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"uniqueValues\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"top10\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"aboveAverage\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"colorScale\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"dataBar\""));
    EXPECT_TRUE(Contains(worksheetXml, "type=\"iconSet\""));
    EXPECT_TRUE(Contains(worksheetXml, "<colorScale>"));
    EXPECT_TRUE(Contains(worksheetXml, "<dataBar>"));
    EXPECT_TRUE(Contains(worksheetXml, "<iconSet iconSet=\"4Arrows\" reverse=\"1\" showValue=\"0\">"));

    Workbook loaded(path.string());
    AssertAdvancedConditionalFormattings(loaded);
}

TEST(OpenXmlFeatureGoldenTests, page_setup_roundtrip_and_emit_expected_markup)
{
    TempDir temp("golden-page-setup");
    const auto path = temp.Path("page-setup.xlsx");
    auto workbook = CreatePageSetupWorkbook();
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    EXPECT_TRUE(Contains(worksheetXml, "pageMargins"));
    EXPECT_TRUE(Contains(worksheetXml, "pageSetup"));
    EXPECT_TRUE(Contains(worksheetXml, "orientation=\"landscape\""));
    EXPECT_TRUE(Contains(worksheetXml, "paperSize=\"9\""));
    EXPECT_TRUE(Contains(worksheetXml, "<rowBreaks"));
    EXPECT_TRUE(Contains(worksheetXml, "<colBreaks"));

    Workbook loaded(path.string());
    AssertPageSetup(loaded);
}
