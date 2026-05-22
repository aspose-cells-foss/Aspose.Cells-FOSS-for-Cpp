#include <gtest/gtest.h>

#include "Support/PackageTestSupport.h"
#include "Support/ScenarioFactories.h"
#include "Support/TestSupport.h"

#include "aspose/cells_foss/CellValueType.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/Color.h"
#include "aspose/cells_foss/FormatConditionType.h"
#include "aspose/cells_foss/InvalidFileFormatException.h"
#include "aspose/cells_foss/OperatorType.h"
#include "aspose/cells_foss/SaveFormat.h"
#include "aspose/cells_foss/TargetModeType.h"
#include "aspose/cells_foss/ValidationType.h"
#include "aspose/cells_foss/VisibilityType.h"
#include "aspose/cells_foss/Workbook.h"
#include "aspose/cells_foss/WorkbookSettings.h"
#include "aspose/cells_foss/WorksheetCollection.h"

using namespace Aspose::Cells_FOSS;
using namespace Aspose::Cells_FOSS::Tests;

TEST(CompatibilityTests, file_and_stream_load_paths_produce_same_values)
{
    TempDir temp("compat-load-paths");
    const auto path = temp.Path("book.xlsx");
    auto workbook = CreateMixedCellWorkbook();
    workbook.Save(path.string());

    auto bytes = ReadAllBytes(path);
    Workbook fromFile(path.string());
    Workbook fromStream(bytes);
    AssertWorkbookDataEquals(fromFile, fromStream);
}

TEST(CompatibilityTests, save_overloads_produce_equivalent_workbooks)
{
    TempDir temp("compat-save-overloads");
    const auto filePath = temp.Path("book-file.xlsx");
    auto workbook = CreateMixedCellWorkbook();
    workbook.Save(filePath.string(), SaveFormat::Xlsx);

    std::vector<std::uint8_t> stream;
    workbook.Save(stream, SaveFormat::Xlsx);

    Workbook fromFile(filePath.string());
    Workbook fromStream(stream);
    AssertWorkbookDataEquals(fromFile, fromStream);
}

TEST(CompatibilityTests, formula_setter_accepts_with_or_without_leading_equal)
{
    Workbook workbook;
    auto cell = workbook.GetWorksheets()[0].GetCells()["A1"];
    cell.PutValue(10);
    cell.SetFormula("B1+C1");
    EXPECT_EQ("=B1+C1", cell.GetFormula());

    cell.SetFormula("=D1+E1");
    EXPECT_EQ("=D1+E1", cell.GetFormula());
}

TEST(CompatibilityTests, exception_mapping_uses_cells_exception_types)
{
    EXPECT_THROW(static_cast<void>(Workbook().GetWorksheets()["missing"]), CellsException);
    EXPECT_THROW(static_cast<void>(Workbook().GetWorksheets()[0].GetCells()["1A"]), CellsException);
    EXPECT_THROW(static_cast<void>(Workbook(std::vector<std::uint8_t>{1, 2, 3, 4})), InvalidFileFormatException);
}

TEST(CompatibilityTests, public_type_mapping_matches_after_roundtrip)
{
    TempDir temp("compat-types");
    const auto path = temp.Path("types.xlsx");
    auto workbook = CreateMixedCellWorkbook();
    workbook.Save(path.string());

    Workbook loaded(path.string());
    EXPECT_EQ(CellValueType::String, loaded.GetWorksheets()[0].GetCells()["A1"].GetType());
    EXPECT_EQ(CellValueType::Number, loaded.GetWorksheets()[0].GetCells()["B1"].GetType());
    EXPECT_EQ(CellValueType::Boolean, loaded.GetWorksheets()[0].GetCells()["C1"].GetType());
    EXPECT_EQ(CellValueType::Number, loaded.GetWorksheets()[0].GetCells()["D1"].GetType());
    EXPECT_EQ(CellValueType::Number, loaded.GetWorksheets()[0].GetCells()["E1"].GetType());
    EXPECT_EQ(CellValueType::DateTime, loaded.GetWorksheets()[0].GetCells()["F1"].GetType());
    EXPECT_EQ(CellValueType::Formula, loaded.GetWorksheets()[0].GetCells()["G1"].GetType());
}

TEST(CompatibilityTests, value_property_setter_matches_supported_scalar_behavior)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];
    sheet.GetCells()["A1"].SetValue("alpha");
    sheet.GetCells()["B1"].SetValue(12);
    sheet.GetCells()["C1"].SetValue(true);
    sheet.GetCells()["D1"].SetValue(DateTime(2024, 1, 2, 3, 4, 0));
    sheet.GetCells()["E1"].SetValue(CellValue());

    EXPECT_EQ("alpha", sheet.GetCells()["A1"].GetValue().AsString());
    EXPECT_EQ(12, sheet.GetCells()["B1"].GetValue().AsInteger());
    EXPECT_TRUE(sheet.GetCells()["C1"].GetValue().AsBool());
    EXPECT_EQ(CellValueType::DateTime, sheet.GetCells()["D1"].GetType());
    EXPECT_EQ("", sheet.GetCells()["E1"].GetDisplayStringValue());
}

TEST(CompatibilityTests, compatibility_members_follow_aspose_style)
{
    Workbook workbook;
    workbook.GetSettings().SetDate1904(true);
    EXPECT_TRUE(workbook.GetSettings().GetDate1904());

    const auto index = workbook.GetWorksheets().Add();
    workbook.GetWorksheets()[index].SetName("Report");
    workbook.GetWorksheets().SetActiveSheetName("Report");
    EXPECT_EQ(index, workbook.GetWorksheets().GetActiveSheetIndex());

    auto& sheet = workbook.GetWorksheets()[index];
    sheet.SetVisibilityType(VisibilityType::Hidden);
    EXPECT_EQ(VisibilityType::Hidden, sheet.GetVisibilityType());

    sheet.GetCells()["A1"].SetValue("Docs");
    const auto hyperlinkIndex = sheet.GetHyperlinks().Add(0, 0, 1, 1, "https://example.com/docs");
    auto hyperlink = sheet.GetHyperlinks()[hyperlinkIndex];
    EXPECT_EQ(TargetModeType::External, hyperlink.GetLinkType());
    hyperlink.Delete();
    EXPECT_EQ(0, sheet.GetHyperlinks().GetCount());

    auto validation = sheet.GetValidations()[sheet.GetValidations().Add(CellArea::CreateCellArea("B2", "B2"))];
    validation.SetType(ValidationType::List);
    validation.SetFormula1("\"Yes,No\"");
    ASSERT_TRUE(sheet.GetValidations().GetValidationInCell(1, 1).has_value());
    EXPECT_EQ(ValidationType::List, sheet.GetValidations().GetValidationInCell(1, 1)->GetType());

    auto conditionalFormatting = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    conditionalFormatting.AddArea(CellArea::CreateCellArea("C3", "C3"));
    conditionalFormatting.AddCondition(FormatConditionType::Expression, OperatorType::None, "C3>0", "");
    EXPECT_EQ(1, sheet.GetConditionalFormattings().GetCount());

    auto& pageSetup = sheet.GetPageSetup();
    pageSetup.SetLeftMargin(1.27);
    EXPECT_DOUBLE_EQ(0.5, pageSetup.GetLeftMarginInch());
}

TEST(CompatibilityTests, worksheet_view_members_follow_supported_patterns)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];

    sheet.SetTabColor(Color::FromArgb(255, 34, 68, 102));
    sheet.SetShowGridlines(false);
    sheet.SetShowRowColumnHeaders(false);
    sheet.SetShowZeros(false);
    sheet.SetRightToLeft(true);
    sheet.SetZoom(85);

    EXPECT_EQ(Color::FromArgb(255, 34, 68, 102), sheet.GetTabColor());
    EXPECT_FALSE(sheet.GetShowGridlines());
    EXPECT_FALSE(sheet.GetShowRowColumnHeaders());
    EXPECT_FALSE(sheet.GetShowZeros());
    EXPECT_TRUE(sheet.GetRightToLeft());
    EXPECT_EQ(85, sheet.GetZoom());
}

TEST(CompatibilityTests, worksheet_protection_members_follow_supported_patterns)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];

    sheet.Protect();
    sheet.GetProtection().SetObjects(true);
    sheet.GetProtection().SetScenarios(true);
    sheet.GetProtection().SetAutoFilter(true);
    sheet.GetProtection().SetSelectLockedCells(true);
    sheet.GetProtection().SetSelectUnlockedCells(true);

    EXPECT_TRUE(sheet.GetProtection().GetIsProtected());
    EXPECT_TRUE(sheet.GetProtection().GetObjects());
    EXPECT_TRUE(sheet.GetProtection().GetScenarios());
    EXPECT_TRUE(sheet.GetProtection().GetAutoFilter());
    EXPECT_TRUE(sheet.GetProtection().GetSelectLockedCells());
    EXPECT_TRUE(sheet.GetProtection().GetSelectUnlockedCells());
}

TEST(CompatibilityTests, autofilter_members_follow_supported_patterns)
{
    auto workbook = CreateAutoFilterWorkbook();
    AssertAutoFilter(workbook);
}

TEST(CompatibilityTests, file_and_stream_roundtrip_preserve_autofilter)
{
    TempDir temp("compat-autofilter");
    const auto filePath = temp.Path("autofilter.xlsx");
    auto workbook = CreateAutoFilterWorkbook();
    workbook.Save(filePath.string());

    std::vector<std::uint8_t> stream;
    workbook.Save(stream, SaveFormat::Xlsx);

    Workbook fromFile(filePath.string());
    Workbook fromStream(stream);
    AssertAutoFilter(fromFile);
    AssertAutoFilter(fromStream);
}

TEST(CompatibilityTests, excel_input_autofilter_roundtrip_preserves_header_only_range)
{
    const auto inputPath = ResolveRepositoryFile("Input", "Autofilter.xlsx");
    Workbook workbook(inputPath.string());
    EXPECT_EQ("A2:C2", workbook.GetWorksheets()[0].GetAutoFilter().GetRange());
    EXPECT_EQ(0, workbook.GetWorksheets()[0].GetAutoFilter().GetFilterColumns().GetCount());
    EXPECT_EQ(0, workbook.GetWorksheets()[0].GetAutoFilter().GetSortState().GetSortConditions().GetCount());

    TempDir temp("compat-input-autofilter");
    const auto roundtripPath = temp.Path("autofilter-roundtrip.xlsx");
    workbook.Save(roundtripPath.string());

    Workbook reloaded(roundtripPath.string());
    EXPECT_EQ("A2:C2", reloaded.GetWorksheets()[0].GetAutoFilter().GetRange());
    EXPECT_EQ(0, reloaded.GetWorksheets()[0].GetAutoFilter().GetFilterColumns().GetCount());
    EXPECT_EQ(0, reloaded.GetWorksheets()[0].GetAutoFilter().GetSortState().GetSortConditions().GetCount());

    const auto workbookXml = Package::ReadEntryText(roundtripPath, "xl/workbook.xml");
    const auto worksheetXml = Package::ReadEntryText(roundtripPath, "xl/worksheets/sheet1.xml");
    EXPECT_TRUE(Contains(worksheetXml, "<autoFilter ref=\"A2:C2\""));
    EXPECT_TRUE(Contains(workbookXml, "name=\"_xlnm._FilterDatabase\""));
    EXPECT_TRUE(Contains(workbookXml, "$A$2:$C$2"));
}

TEST(CompatibilityTests, file_and_stream_roundtrip_preserve_defined_names)
{
    TempDir temp("compat-defined-names");
    const auto filePath = temp.Path("defined-names.xlsx");
    auto workbook = CreateDefinedNamesWorkbook();
    workbook.Save(filePath.string());

    std::vector<std::uint8_t> stream;
    workbook.Save(stream, SaveFormat::Xlsx);

    Workbook fromFile(filePath.string());
    Workbook fromStream(stream);
    AssertDefinedNames(fromFile);
    AssertDefinedNames(fromStream);
}

TEST(CompatibilityTests, file_and_stream_roundtrip_preserve_styles)
{
    TempDir temp("compat-styles");
    const auto filePath = temp.Path("styled-file.xlsx");
    auto workbook = CreateStyledWorkbook();
    workbook.Save(filePath.string());

    std::vector<std::uint8_t> stream;
    workbook.Save(stream, SaveFormat::Xlsx);

    Workbook fromFile(filePath.string());
    Workbook fromStream(stream);
    AssertPrimaryStyle(fromFile.GetWorksheets()[0].GetCells()["A1"].GetStyle());
    AssertPrimaryStyle(fromStream.GetWorksheets()[0].GetCells()["A1"].GetStyle());
    AssertCustomNumberStyle(fromFile.GetWorksheets()[0].GetCells()["B2"].GetStyle());
    AssertCustomNumberStyle(fromStream.GetWorksheets()[0].GetCells()["B2"].GetStyle());
    EXPECT_EQ(CellValueType::Blank, fromFile.GetWorksheets()[0].GetCells()["B2"].GetType());
    EXPECT_EQ(CellValueType::Blank, fromStream.GetWorksheets()[0].GetCells()["B2"].GetType());
}

TEST(CompatibilityTests, file_and_stream_roundtrip_preserve_worksheet_settings)
{
    TempDir temp("compat-worksheet-settings");
    const auto filePath = temp.Path("worksheet-settings.xlsx");
    auto workbook = CreateWorksheetSettingsWorkbook();
    workbook.Save(filePath.string());

    std::vector<std::uint8_t> stream;
    workbook.Save(stream, SaveFormat::Xlsx);

    Workbook fromFile(filePath.string());
    Workbook fromStream(stream);
    AssertWorksheetSettings(fromFile);
    AssertWorksheetSettingsScenarioHasVisibleSheet(fromFile);
    AssertWorksheetSettings(fromStream);
    AssertWorksheetSettingsScenarioHasVisibleSheet(fromStream);
}

TEST(CompatibilityTests, file_and_stream_roundtrip_preserve_data_validations)
{
    TempDir temp("compat-validations");
    const auto filePath = temp.Path("validations.xlsx");
    auto workbook = CreateValidationWorkbook();
    workbook.Save(filePath.string());

    std::vector<std::uint8_t> stream;
    workbook.Save(stream, SaveFormat::Xlsx);

    Workbook fromFile(filePath.string());
    Workbook fromStream(stream);
    AssertValidations(fromFile);
    AssertValidations(fromStream);
}

TEST(CompatibilityTests, file_and_stream_roundtrip_preserve_conditional_formattings)
{
    TempDir temp("compat-conditional-formatting");
    const auto filePath = temp.Path("conditional-formatting.xlsx");
    auto workbook = CreateConditionalFormattingWorkbook();
    workbook.Save(filePath.string());

    std::vector<std::uint8_t> stream;
    workbook.Save(stream, SaveFormat::Xlsx);

    Workbook fromFile(filePath.string());
    Workbook fromStream(stream);
    AssertConditionalFormattings(fromFile);
    AssertConditionalFormattings(fromStream);
}

TEST(CompatibilityTests, file_and_stream_roundtrip_preserve_advanced_conditional_formattings)
{
    TempDir temp("compat-advanced-conditional-formatting");
    const auto filePath = temp.Path("advanced-conditional-formatting.xlsx");
    auto workbook = CreateAdvancedConditionalFormattingWorkbook();
    workbook.Save(filePath.string());

    std::vector<std::uint8_t> stream;
    workbook.Save(stream, SaveFormat::Xlsx);

    Workbook fromFile(filePath.string());
    Workbook fromStream(stream);
    AssertAdvancedConditionalFormattings(fromFile);
    AssertAdvancedConditionalFormattings(fromStream);
}

TEST(CompatibilityTests, file_and_stream_roundtrip_preserve_page_setup)
{
    TempDir temp("compat-page-setup");
    const auto filePath = temp.Path("page-setup.xlsx");
    auto workbook = CreatePageSetupWorkbook();
    workbook.Save(filePath.string());

    std::vector<std::uint8_t> stream;
    workbook.Save(stream, SaveFormat::Xlsx);

    Workbook fromFile(filePath.string());
    Workbook fromStream(stream);
    AssertPageSetup(fromFile);
    AssertPageSetup(fromStream);
}

TEST(CompatibilityTests, workbook_metadata_members_follow_supported_patterns)
{
    auto workbook = CreateWorkbookMetadataWorkbook();
    AssertWorkbookMetadata(workbook);
    EXPECT_EQ("Data", workbook.GetWorksheets()[workbook.GetProperties().GetView().GetActiveTab()].GetName());
}

TEST(CompatibilityTests, file_and_stream_roundtrip_preserve_workbook_metadata)
{
    TempDir temp("compat-workbook-metadata");
    const auto filePath = temp.Path("workbook-metadata.xlsx");
    auto workbook = CreateWorkbookMetadataWorkbook();
    workbook.Save(filePath.string());

    std::vector<std::uint8_t> stream;
    workbook.Save(stream, SaveFormat::Xlsx);

    Workbook fromFile(filePath.string());
    Workbook fromStream(stream);
    AssertWorkbookMetadata(fromFile);
    AssertWorkbookMetadata(fromStream);
}
