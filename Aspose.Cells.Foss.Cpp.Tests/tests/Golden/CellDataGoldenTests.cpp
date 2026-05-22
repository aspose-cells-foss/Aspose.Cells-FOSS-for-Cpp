#include <gtest/gtest.h>

#include "Support/PackageTestSupport.h"
#include "Support/ScenarioFactories.h"
#include "Support/TestSupport.h"

#include "aspose/cells_foss/CellValueType.h"
#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/SaveFormat.h"
#include "aspose/cells_foss/SaveOptions.h"
#include "aspose/cells_foss/WorkbookSettings.h"
#include "aspose/cells_foss/WorksheetCollection.h"

using namespace Aspose::Cells_FOSS;
using namespace Aspose::Cells_FOSS::Tests;

TEST(CellDataGoldenTests, xlsx_roundtrip_mixed_scalar_cells_file)
{
    TempDir temp("golden-file");
    const auto path = temp.Path("mixed.xlsx");
    auto workbook = CreateMixedCellWorkbook();
    workbook.Save(path.string());

    Workbook loaded(path.string());
    AssertMixedWorkbook(loaded, false);
}

TEST(CellDataGoldenTests, xlsx_roundtrip_mixed_scalar_cells_stream)
{
    auto workbook = CreateMixedCellWorkbook();
    std::vector<std::uint8_t> stream;
    workbook.Save(stream, SaveFormat::Xlsx);

    Workbook loaded(stream);
    AssertMixedWorkbook(loaded, false);
}

TEST(CellDataGoldenTests, shared_strings_export_uses_sst_when_enabled)
{
    TempDir temp("golden-sst");
    const auto path = temp.Path("shared-strings.xlsx");
    Workbook workbook;
    workbook.GetWorksheets()[0].GetCells()["A1"].PutValue("Hello");
    workbook.GetWorksheets()[0].GetCells()["A2"].PutValue("Hello");
    SaveOptions options;
    options.SetUseSharedStrings(true);
    workbook.Save(path.string(), options);

    EXPECT_TRUE(Package::EntryExists(path, "xl/sharedStrings.xml"));
    const auto sharedStringsXml = Package::ReadEntryText(path, "xl/sharedStrings.xml");
    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");

    EXPECT_TRUE(Contains(sharedStringsXml, "<sst"));
    EXPECT_TRUE(Contains(sharedStringsXml, "uniqueCount=\"1\""));
    EXPECT_TRUE(Contains(worksheetXml, "t=\"s\""));
}

TEST(CellDataGoldenTests, inline_strings_export_when_shared_strings_disabled)
{
    TempDir temp("golden-inline");
    const auto path = temp.Path("inline.xlsx");
    Workbook workbook;
    workbook.GetWorksheets()[0].GetCells()["A1"].PutValue("Inline");
    SaveOptions options;
    options.SetUseSharedStrings(false);
    workbook.Save(path.string(), options);

    EXPECT_FALSE(Package::EntryExists(path, "xl/sharedStrings.xml"));
    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    EXPECT_TRUE(Contains(worksheetXml, "t=\"inlineStr\""));
    EXPECT_TRUE(Contains(worksheetXml, "<t>Inline</t>"));
}

TEST(CellDataGoldenTests, formula_cells_persist_formula_and_cached_value)
{
    TempDir temp("golden-formula");
    const auto path = temp.Path("formula.xlsx");
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];
    sheet.GetCells()["A1"].PutValue(10);
    sheet.GetCells()["B1"].PutValue(20);
    sheet.GetCells()["C1"].PutValue(30);
    sheet.GetCells()["C1"].SetFormula("=A1+B1");
    workbook.Save(path.string());

    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");
    EXPECT_TRUE(Contains(worksheetXml, "<f>A1+B1</f>"));
    EXPECT_TRUE(Contains(worksheetXml, "<v>30</v>"));

    Workbook loaded(path.string());
    EXPECT_EQ("=A1+B1", loaded.GetWorksheets()[0].GetCells()["C1"].GetFormula());
    EXPECT_EQ("30", loaded.GetWorksheets()[0].GetCells()["C1"].GetStringValue());
}

TEST(CellDataGoldenTests, mac1904_datetime_roundtrip_and_markup)
{
    TempDir temp("golden-1904");
    const auto path = temp.Path("date1904.xlsx");
    auto workbook = CreateMixedCellWorkbook(true);
    workbook.Save(path.string());

    const auto workbookXml = Package::ReadEntryText(path, "xl/workbook.xml");
    EXPECT_TRUE(Contains(workbookXml, "date1904=\"1\""));

    Workbook loaded(path.string());
    EXPECT_TRUE(loaded.GetSettings().GetDate1904());
    EXPECT_EQ(DateTime(2024, 5, 6, 7, 8, 9),
              loaded.GetWorksheets()[0].GetCells()["F1"].GetValue().AsDateTime());
}

TEST(CellDataGoldenTests, display_runtime_culture_after_roundtrip)
{
    TempDir temp("golden-display-culture");
    const auto path = temp.Path("display-culture.xlsx");
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];
    const auto timestamp = DateTime(2024, 5, 6, 7, 8, 9);

    sheet.GetCells()["A1"].PutValue(1234.5);
    auto numericStyle = sheet.GetCells()["A1"].GetStyle();
    numericStyle.SetNumberFormat("#,##0.00");
    sheet.GetCells()["A1"].SetStyle(numericStyle);

    sheet.GetCells()["B1"].PutValue(timestamp);
    auto dateStyle = sheet.GetCells()["B1"].GetStyle();
    dateStyle.SetNumberFormat("[$-409]dddd, mmmm d, yyyy");
    sheet.GetCells()["B1"].SetStyle(dateStyle);
    workbook.Save(path.string());

    Workbook loaded(path.string());
    loaded.GetSettings().SetCulture(MakeLocaleStrict({"fr-FR", "fr_FR.UTF-8", "fr_FR"}));
    EXPECT_EQ("1 234,50", loaded.GetWorksheets()[0].GetCells()["A1"].GetDisplayStringValue());
    EXPECT_EQ("Monday, May 6, 2024", loaded.GetWorksheets()[0].GetCells()["B1"].GetDisplayStringValue());
}

TEST(CellDataGoldenTests, styled_cells_roundtrip_and_emit_stylesheet)
{
    TempDir temp("golden-styles");
    const auto path = temp.Path("styles.xlsx");
    auto workbook = CreateStyledWorkbook();
    workbook.Save(path.string());

    ASSERT_TRUE(Package::EntryExists(path, "xl/styles.xml"));
    const auto stylesheetXml = Package::ReadEntryText(path, "xl/styles.xml");
    const auto worksheetXml = Package::ReadEntryText(path, "xl/worksheets/sheet1.xml");

    EXPECT_TRUE(Contains(stylesheetXml, "Arial"));
    EXPECT_TRUE(Contains(stylesheetXml, "0.0000"));
    EXPECT_TRUE(Contains(stylesheetXml, "<strike"));
    EXPECT_TRUE(Contains(stylesheetXml, "patternType=\"lightGrid\""));
    EXPECT_TRUE(Contains(stylesheetXml, "fgColor rgb=\"FFD2DC1E\""));
    EXPECT_TRUE(Contains(stylesheetXml, "bgColor rgb=\"FF0C2D4E\""));
    EXPECT_TRUE(Contains(stylesheetXml, "style=\"dotted\""));
    EXPECT_TRUE(Contains(stylesheetXml, "style=\"mediumDashDot\""));
    EXPECT_TRUE(Contains(stylesheetXml, "style=\"double\""));
    EXPECT_TRUE(Contains(stylesheetXml, "style=\"dashDotDot\""));
    EXPECT_TRUE(Contains(stylesheetXml, "style=\"slantDashDot\""));
    EXPECT_TRUE(Contains(stylesheetXml, "diagonalUp=\"1\""));
    EXPECT_TRUE(Contains(stylesheetXml, "diagonalDown=\"1\""));
    EXPECT_TRUE(Contains(stylesheetXml, "horizontal=\"distributed\""));
    EXPECT_TRUE(Contains(stylesheetXml, "vertical=\"distributed\""));
    EXPECT_TRUE(Contains(stylesheetXml, "indent=\"2\""));
    EXPECT_TRUE(Contains(stylesheetXml, "textRotation=\"45\""));
    EXPECT_TRUE(Contains(stylesheetXml, "shrinkToFit=\"1\""));
    EXPECT_TRUE(Contains(stylesheetXml, "readingOrder=\"2\""));
    EXPECT_TRUE(Contains(stylesheetXml, "relativeIndent=\"1\""));
    EXPECT_TRUE(Contains(stylesheetXml, "wrapText=\"1\""));
    EXPECT_TRUE(Contains(stylesheetXml, "locked=\"0\""));
    EXPECT_TRUE(Contains(stylesheetXml, "hidden=\"1\""));
    EXPECT_TRUE(Contains(stylesheetXml, "numFmtId=\"4\""));
    EXPECT_TRUE(Contains(worksheetXml, "s=\""));

    Workbook loaded(path.string());
    AssertPrimaryStyle(loaded.GetWorksheets()[0].GetCells()["A1"].GetStyle());
    AssertCustomNumberStyle(loaded.GetWorksheets()[0].GetCells()["B2"].GetStyle());
    EXPECT_EQ(CellValueType::Blank, loaded.GetWorksheets()[0].GetCells()["B2"].GetType());
    EXPECT_EQ(CellValueType::DateTime, loaded.GetWorksheets()[0].GetCells()["C3"].GetType());
}
