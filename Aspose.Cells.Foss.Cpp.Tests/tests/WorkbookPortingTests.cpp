#include <gtest/gtest.h>

#include "aspose/cells_foss/Border.h"
#include "aspose/cells_foss/BorderStyleType.h"
#include "aspose/cells_foss/CellArea.h"
#include "aspose/cells_foss/CellValueType.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/Color.h"
#include "aspose/cells_foss/ConditionalFormattingCollection.h"
#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/DocumentProperties.h"
#include "aspose/cells_foss/FillPattern.h"
#include "aspose/cells_foss/FormatCondition.h"
#include "aspose/cells_foss/FormatConditionCollection.h"
#include "aspose/cells_foss/FormatConditionType.h"
#include "aspose/cells_foss/HorizontalAlignmentType.h"
#include "aspose/cells_foss/Hyperlink.h"
#include "aspose/cells_foss/HyperlinkCollection.h"
#include "aspose/cells_foss/OperatorType.h"
#include "aspose/cells_foss/SaveOptions.h"
#include "aspose/cells_foss/Style.h"
#include "aspose/cells_foss/Validation.h"
#include "aspose/cells_foss/ValidationAlertType.h"
#include "aspose/cells_foss/ValidationCollection.h"
#include "aspose/cells_foss/ValidationType.h"
#include "aspose/cells_foss/VisibilityType.h"
#include "aspose/cells_foss/Workbook.h"
#include "aspose/cells_foss/WorkbookSettings.h"
#include "aspose/cells_foss/Worksheet.h"
#include "aspose/cells_foss/WorksheetCollection.h"
#include "aspose/cells_foss/ZipArchive.h"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

namespace {

using namespace Aspose::Cells_FOSS;

class TempDir {
public:
    TempDir()
    {
        const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        _path = TestOutputRoot()
            / (CurrentTestName() + "_" + std::to_string(stamp));
        std::filesystem::create_directories(_path);
    }

    ~TempDir() = default;

    std::filesystem::path Path(std::string_view name) const
    {
        return _path / std::string(name);
    }

private:
    std::filesystem::path _path;

    static std::filesystem::path TestOutputRoot()
    {
#ifdef ASPOSE_CELLS_FOSS_TEST_OUTPUT_DIR
        auto root = std::filesystem::path(ASPOSE_CELLS_FOSS_TEST_OUTPUT_DIR);
#else
        auto root = std::filesystem::path("workspace") / "temp";
#endif
        return root.is_absolute() ? root : std::filesystem::current_path() / root;
    }

    static std::string CurrentTestName()
    {
        const auto* info = ::testing::UnitTest::GetInstance()->current_test_info();
        const std::string suite = info == nullptr ? "unknown_suite" : info->test_suite_name();
        const std::string name = info == nullptr ? "unknown_test" : info->name();
        return Sanitize(suite + "_" + name);
    }

    static std::string Sanitize(std::string value)
    {
        for (char& c : value) {
            const auto ch = static_cast<unsigned char>(c);
            if (!std::isalnum(ch) && c != '_' && c != '-') {
                c = '_';
            }
        }
        return value;
    }
};

std::vector<std::uint8_t> ReadAllBytes(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    EXPECT_TRUE(file.is_open());
    return std::vector<std::uint8_t>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>());
}

std::string BytesToString(const std::vector<std::uint8_t>& bytes)
{
    return std::string(bytes.begin(), bytes.end());
}

std::string ReadZipText(const std::vector<std::uint8_t>& packageBytes, std::string_view entryName)
{
    auto archive = ZipArchive::FromZipBytes(packageBytes);
    auto entry = archive.GetEntry(entryName);
    EXPECT_FALSE(entry.IsNull()) << entryName;
    if (entry.IsNull()) {
        return {};
    }
    return BytesToString(entry.Open());
}

void PopulateBasicWorkbook(Workbook& workbook)
{
    auto& sheet = workbook.GetWorksheets()[0];
    sheet.SetName("Data");
    sheet.GetCells()["A1"].PutValue(std::string_view("alpha"));
    sheet.GetCells()["B1"].PutValue(42);
    sheet.GetCells()["C1"].PutValue(true);
    sheet.GetCells()["D1"].PutValue(DateTime(2024, 5, 6, 7, 8, 9));
    sheet.GetCells()["E1"].PutValue(21);
    sheet.GetCells()["E1"].SetFormula("B1*2");
}

void ApplyVisibleStyle(Style& style)
{
    auto font = style.GetFont();
    font.SetName("Arial");
    font.SetBold(true);
    font.SetItalic(true);
    font.SetColor(Color::FromArgb(255, 20, 40, 60));
    style.SetFont(font);

    auto left = style.GetBorders().GetLeft();
    left.SetLineStyle(BorderStyleType::Thin);
    left.SetColor(Color::FromArgb(255, 200, 10, 10));
    auto borders = style.GetBorders();
    borders.SetLeft(left);
    style.SetBorders(borders);

    style.SetPattern(FillPattern::Solid);
    style.SetForegroundColor(Color::FromArgb(255, 250, 230, 100));
    style.SetHorizontalAlignment(HorizontalAlignmentType::Center);
    style.SetNumberFormat("0.00%");
}

}  // namespace

TEST(WorkbookPortingTests, NewWorkbookHasDefaultWorksheetAndBlankCells)
{
    Workbook workbook;
    auto& sheets = workbook.GetWorksheets();

    ASSERT_EQ(1, sheets.GetCount());
    EXPECT_EQ("Sheet1", sheets[0].GetName());

    auto cell = sheets[0].GetCells()["Z99"];
    EXPECT_EQ(CellValueType::Blank, cell.GetType());
    EXPECT_TRUE(cell.GetValue().IsEmpty());
    EXPECT_EQ("", cell.GetStringValue());
}

TEST(WorkbookPortingTests, CellValuesAndFormulaRoundTripThroughFacade)
{
    Workbook workbook;
    PopulateBasicWorkbook(workbook);
    auto& cells = workbook.GetWorksheets()[0].GetCells();

    EXPECT_EQ(CellValueType::String, cells["A1"].GetType());
    EXPECT_EQ("alpha", cells["A1"].GetValue().AsString());
    EXPECT_EQ(CellValueType::Number, cells["B1"].GetType());
    EXPECT_EQ(42, cells["B1"].GetValue().AsInteger());
    EXPECT_EQ(CellValueType::Boolean, cells["C1"].GetType());
    EXPECT_TRUE(cells["C1"].GetValue().AsBool());
    EXPECT_EQ(CellValueType::DateTime, cells["D1"].GetType());
    EXPECT_EQ(DateTime(2024, 5, 6, 7, 8, 9), cells["D1"].GetValue().AsDateTime());
    EXPECT_EQ(CellValueType::Formula, cells["E1"].GetType());
    EXPECT_EQ("=B1*2", cells["E1"].GetFormula());
    EXPECT_EQ(21, cells["E1"].GetValue().AsInteger());
}

TEST(WorkbookPortingTests, StyleMutationsRequireSetStyleAndPersistOnCell)
{
    Workbook workbook;
    auto cell = workbook.GetWorksheets()[0].GetCells()["A1"];
    cell.PutValue(0.125);

    auto style = cell.GetStyle();
    ApplyVisibleStyle(style);

    EXPECT_FALSE(cell.GetStyle().GetFont().GetBold());
    EXPECT_EQ("General", cell.GetStyle().GetNumberFormat());

    cell.SetStyle(style);

    auto applied = cell.GetStyle();
    EXPECT_TRUE(applied.GetFont().GetBold());
    EXPECT_TRUE(applied.GetFont().GetItalic());
    EXPECT_EQ("Arial", applied.GetFont().GetName());
    EXPECT_EQ(Color::FromArgb(255, 20, 40, 60), applied.GetFont().GetColor());
    EXPECT_EQ(FillPattern::Solid, applied.GetPattern());
    EXPECT_EQ(Color::FromArgb(255, 250, 230, 100), applied.GetForegroundColor());
    EXPECT_EQ(HorizontalAlignmentType::Center, applied.GetHorizontalAlignment());
    EXPECT_EQ("0.00%", applied.GetNumberFormat());
    EXPECT_EQ("12.50%", cell.GetDisplayStringValue());

    auto clone = cell.GetStyle();
    auto cloneFont = clone.GetFont();
    cloneFont.SetBold(false);
    clone.SetFont(cloneFont);
    EXPECT_TRUE(cell.GetStyle().GetFont().GetBold());
}

TEST(WorkbookPortingTests, SaveFileCreatesReadableOpenXmlPackage)
{
    TempDir temp;
    const auto path = temp.Path("created.xlsx");

    Workbook workbook;
    PopulateBasicWorkbook(workbook);
    SaveOptions options;
    options.SetUseSharedStrings(true);
    workbook.Save(path.string(), options);

    ASSERT_TRUE(std::filesystem::exists(path));
    auto bytes = ReadAllBytes(path);
    ASSERT_GT(bytes.size(), 100u);

    EXPECT_NE(std::string::npos, ReadZipText(bytes, "[Content_Types].xml").find("spreadsheetml.sheet.main+xml"));
    EXPECT_NE(std::string::npos, ReadZipText(bytes, "xl/workbook.xml").find("Data"));
    EXPECT_NE(std::string::npos, ReadZipText(bytes, "xl/worksheets/sheet1.xml").find("B1"));
    EXPECT_NE(std::string::npos, ReadZipText(bytes, "xl/sharedStrings.xml").find("alpha"));
}

TEST(WorkbookPortingTests, LoadXlsxFilePreservesCellsAndFormula)
{
    TempDir temp;
    const auto path = temp.Path("roundtrip.xlsx");

    Workbook workbook;
    PopulateBasicWorkbook(workbook);
    workbook.Save(path.string());

    Workbook loaded(path.string());
    auto& cells = loaded.GetWorksheets()["Data"].GetCells();

    EXPECT_EQ("alpha", cells["A1"].GetStringValue());
    EXPECT_EQ(42, cells["B1"].GetValue().AsInteger());
    EXPECT_TRUE(cells["C1"].GetValue().AsBool());
    EXPECT_EQ(DateTime(2024, 5, 6, 7, 8, 9), cells["D1"].GetValue().AsDateTime());
    EXPECT_EQ("=B1*2", cells["E1"].GetFormula());
    EXPECT_EQ(21, cells["E1"].GetValue().AsInteger());
}

TEST(WorkbookPortingTests, SaveAndLoadStreamRoundTrip)
{
    Workbook workbook;
    PopulateBasicWorkbook(workbook);
    std::vector<std::uint8_t> output;

    workbook.Save(output, SaveOptions());
    ASSERT_GT(output.size(), 100u);

    Workbook loaded(output);
    EXPECT_EQ("Data", loaded.GetWorksheets()[0].GetName());
    EXPECT_EQ("alpha", loaded.GetWorksheets()[0].GetCells()["A1"].GetStringValue());
    EXPECT_EQ(42, loaded.GetWorksheets()[0].GetCells()["B1"].GetValue().AsInteger());
}

TEST(WorkbookPortingTests, StyleFormattingPersistsThroughXlsxRoundTrip)
{
    TempDir temp;
    const auto path = temp.Path("style.xlsx");

    Workbook workbook;
    auto cell = workbook.GetWorksheets()[0].GetCells()["C4"];
    cell.PutValue(0.125);
    auto style = cell.GetStyle();
    ApplyVisibleStyle(style);
    cell.SetStyle(style);
    workbook.Save(path.string());

    Workbook loaded(path.string());
    auto loadedCell = loaded.GetWorksheets()[0].GetCells()["C4"];
    auto loadedStyle = loadedCell.GetStyle();

    EXPECT_EQ("0.00%", loadedStyle.GetNumberFormat());
    EXPECT_TRUE(loadedStyle.GetFont().GetBold());
    EXPECT_TRUE(loadedStyle.GetFont().GetItalic());
    EXPECT_EQ("Arial", loadedStyle.GetFont().GetName());
    EXPECT_EQ(FillPattern::Solid, loadedStyle.GetPattern());
    EXPECT_EQ("12.50%", loadedCell.GetDisplayStringValue());
}

TEST(WorkbookPortingTests, WorksheetsVisibilityAndActiveSheetRoundTrip)
{
    TempDir temp;
    const auto path = temp.Path("sheets.xlsx");

    Workbook workbook;
    workbook.GetWorksheets()[0].SetName("Visible");
    const int hiddenIndex = workbook.GetWorksheets().Add("Hidden");
    workbook.GetWorksheets()[hiddenIndex].SetVisibilityType(VisibilityType::Hidden);
    const int veryHiddenIndex = workbook.GetWorksheets().Add("VeryHidden");
    workbook.GetWorksheets()[veryHiddenIndex].SetVisibilityType(VisibilityType::VeryHidden);
    workbook.GetWorksheets().SetActiveSheetIndex(hiddenIndex);
    workbook.Save(path.string());

    Workbook loaded(path.string());
    ASSERT_EQ(3, loaded.GetWorksheets().GetCount());
    EXPECT_EQ("Visible", loaded.GetWorksheets()[0].GetName());
    EXPECT_EQ("Hidden", loaded.GetWorksheets()[1].GetName());
    EXPECT_EQ("VeryHidden", loaded.GetWorksheets()[2].GetName());
    EXPECT_EQ(VisibilityType::Hidden, loaded.GetWorksheets()[1].GetVisibilityType());
    EXPECT_EQ(VisibilityType::VeryHidden, loaded.GetWorksheets()[2].GetVisibilityType());
    EXPECT_EQ(hiddenIndex, loaded.GetWorksheets().GetActiveSheetIndex());
}

TEST(WorkbookPortingTests, RowsColumnsAndMergesRoundTrip)
{
    TempDir temp;
    const auto path = temp.Path("layout.xlsx");

    Workbook workbook;
    auto& cells = workbook.GetWorksheets()[0].GetCells();
    cells["B2"].PutValue(std::string_view("merged"));
    cells.GetRows()[1].SetHeight(22.5);
    cells.GetRows()[3].SetIsHidden(true);
    cells.GetColumns()[1].SetWidth(18.25);
    cells.GetColumns()[2].SetIsHidden(true);
    cells.Merge(1, 1, 2, 3);
    workbook.Save(path.string());

    Workbook loaded(path.string());
    auto& loadedCells = loaded.GetWorksheets()[0].GetCells();
    EXPECT_EQ("merged", loadedCells["B2"].GetStringValue());
    ASSERT_TRUE(loadedCells.GetRows()[1].GetHeight().has_value());
    EXPECT_DOUBLE_EQ(22.5, *loadedCells.GetRows()[1].GetHeight());
    EXPECT_TRUE(loadedCells.GetRows()[3].GetIsHidden());
    ASSERT_TRUE(loadedCells.GetColumns()[1].GetWidth().has_value());
    EXPECT_DOUBLE_EQ(18.25, *loadedCells.GetColumns()[1].GetWidth());
    EXPECT_TRUE(loadedCells.GetColumns()[2].GetIsHidden());
    auto merged = loadedCells.GetMergedCells();
    ASSERT_EQ(1u, merged.size());
    EXPECT_EQ(1, merged[0].GetFirstRow());
    EXPECT_EQ(1, merged[0].GetFirstColumn());
    EXPECT_EQ(2, merged[0].GetTotalRows());
    EXPECT_EQ(3, merged[0].GetTotalColumns());
}

TEST(WorkbookPortingTests, HyperlinksValidationsAndConditionalFormattingRoundTrip)
{
    TempDir temp;
    const auto path = temp.Path("features.xlsx");

    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];
    sheet.GetCells()["A1"].PutValue(std::string_view("Docs"));
    const int linkIndex = sheet.GetHyperlinks().Add("A1", 1, 1, "https://example.com/docs");
    auto link = sheet.GetHyperlinks()[linkIndex];
    link.SetTextToDisplay("Docs");
    link.SetScreenTip("External docs");

    const int validationIndex = sheet.GetValidations().Add(CellArea::CreateCellArea("B1", "B3"));
    auto validation = sheet.GetValidations()[validationIndex];
    validation.SetType(ValidationType::List);
    validation.SetFormula1("\"Open,Closed\"");
    validation.SetShowError(true);
    validation.SetErrorTitle("Invalid");

    const int cfIndex = sheet.GetConditionalFormattings().Add();
    auto formatCollection = sheet.GetConditionalFormattings()[cfIndex];
    formatCollection.AddArea(CellArea::CreateCellArea("C1", "C3"));
    const int conditionIndex = formatCollection.AddCondition(
        FormatConditionType::CellValue,
        OperatorType::Between,
        "1",
        "9");
    auto condition = formatCollection[conditionIndex];
    condition.SetStopIfTrue(true);
    auto conditionStyle = condition.GetStyle();
    conditionStyle.SetPattern(FillPattern::Solid);
    conditionStyle.SetForegroundColor(Color::FromArgb(255, 255, 199, 206));
    condition.SetStyle(conditionStyle);

    workbook.Save(path.string());
    Workbook loaded(path.string());
    auto& loadedSheet = loaded.GetWorksheets()[0];

    ASSERT_EQ(1, loadedSheet.GetHyperlinks().GetCount());
    EXPECT_EQ("A1", loadedSheet.GetHyperlinks()[0].GetArea());
    EXPECT_EQ("https://example.com/docs", loadedSheet.GetHyperlinks()[0].GetAddress());
    EXPECT_EQ("Docs", loadedSheet.GetHyperlinks()[0].GetTextToDisplay());
    EXPECT_EQ("External docs", loadedSheet.GetHyperlinks()[0].GetScreenTip());

    ASSERT_EQ(1, loadedSheet.GetValidations().GetCount());
    auto loadedValidation = loadedSheet.GetValidations()[0];
    EXPECT_EQ(ValidationType::List, loadedValidation.GetType());
    EXPECT_EQ("\"Open,Closed\"", loadedValidation.GetFormula1());
    EXPECT_TRUE(loadedValidation.GetShowError());
    EXPECT_EQ("Invalid", loadedValidation.GetErrorTitle());

    ASSERT_EQ(1, loadedSheet.GetConditionalFormattings().GetCount());
    auto loadedFormatting = loadedSheet.GetConditionalFormattings()[0];
    ASSERT_EQ(1, loadedFormatting.GetCount());
    auto loadedCondition = loadedFormatting[0];
    EXPECT_EQ(FormatConditionType::CellValue, loadedCondition.GetType());
    EXPECT_EQ(OperatorType::Between, loadedCondition.GetOperator());
    EXPECT_EQ("1", loadedCondition.GetFormula1());
    EXPECT_EQ("9", loadedCondition.GetFormula2());
    EXPECT_TRUE(loadedCondition.GetStopIfTrue());
    EXPECT_EQ(FillPattern::Solid, loadedCondition.GetStyle().GetPattern());
}

TEST(WorkbookPortingTests, WorkbookSettingsAndDocumentPropertiesRoundTrip)
{
    TempDir temp;
    const auto path = temp.Path("metadata.xlsx");

    Workbook workbook;
    workbook.GetSettings().SetDate1904(true);
    workbook.GetDocumentProperties().SetTitle("Quarterly Summary");
    workbook.GetDocumentProperties().SetAuthor("Automation");
    workbook.GetDocumentProperties().SetCompany("Aspose.Cells.FOSS Tests");
    workbook.Save(path.string());

    auto bytes = ReadAllBytes(path);
    EXPECT_NE(std::string::npos, ReadZipText(bytes, "xl/workbook.xml").find("date1904=\"1\""));
    EXPECT_NE(std::string::npos, ReadZipText(bytes, "docProps/core.xml").find("Quarterly Summary"));
    EXPECT_NE(std::string::npos, ReadZipText(bytes, "docProps/app.xml").find("Aspose.Cells.FOSS Tests"));

    Workbook loaded(path.string());
    EXPECT_TRUE(loaded.GetSettings().GetDate1904());
    EXPECT_EQ("Quarterly Summary", loaded.GetDocumentProperties().GetTitle());
    EXPECT_EQ("Automation", loaded.GetDocumentProperties().GetAuthor());
    EXPECT_EQ("Aspose.Cells.FOSS Tests", loaded.GetDocumentProperties().GetCompany());
}
