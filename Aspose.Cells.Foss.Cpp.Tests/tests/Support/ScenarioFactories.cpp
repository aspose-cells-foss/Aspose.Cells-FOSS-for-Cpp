#include "Support/ScenarioFactories.h"

#include "Support/TestSupport.h"

#include "aspose/cells_foss/AutoFilter.h"
#include "aspose/cells_foss/Border.h"
#include "aspose/cells_foss/BorderStyleType.h"
#include "aspose/cells_foss/Borders.h"
#include "aspose/cells_foss/CalculationProperties.h"
#include "aspose/cells_foss/CellValueType.h"
#include "aspose/cells_foss/Color.h"
#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/DefinedName.h"
#include "aspose/cells_foss/DefinedNameCollection.h"
#include "aspose/cells_foss/DocumentProperties.h"
#include "aspose/cells_foss/ExtendedDocumentProperties.h"
#include "aspose/cells_foss/FillPattern.h"
#include "aspose/cells_foss/FilterColumn.h"
#include "aspose/cells_foss/FilterColumnCollection.h"
#include "aspose/cells_foss/FilterOperatorType.h"
#include "aspose/cells_foss/FormatCondition.h"
#include "aspose/cells_foss/FormatConditionType.h"
#include "aspose/cells_foss/Font.h"
#include "aspose/cells_foss/HorizontalAlignmentType.h"
#include "aspose/cells_foss/Hyperlink.h"
#include "aspose/cells_foss/HyperlinkCollection.h"
#include "aspose/cells_foss/OperatorType.h"
#include "aspose/cells_foss/PageOrientationType.h"
#include "aspose/cells_foss/PageSetup.h"
#include "aspose/cells_foss/PaperSizeType.h"
#include "aspose/cells_foss/RowCollection.h"
#include "aspose/cells_foss/ColumnCollection.h"
#include "aspose/cells_foss/VerticalAlignmentType.h"
#include "aspose/cells_foss/VisibilityType.h"
#include "aspose/cells_foss/WorkbookProperties.h"
#include "aspose/cells_foss/WorkbookProtection.h"
#include "aspose/cells_foss/WorkbookSettings.h"
#include "aspose/cells_foss/WorkbookView.h"
#include "aspose/cells_foss/Worksheet.h"
#include "aspose/cells_foss/WorksheetCollection.h"
#include "aspose/cells_foss/WorksheetProtection.h"

#include <gtest/gtest.h>

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

namespace Aspose::Cells_FOSS::Tests {
namespace {

Color ShowcaseColor(int index)
{
    return Color::FromArgb(255,
        static_cast<int32_t>((40 + (index * 29)) % 256),
        static_cast<int32_t>((90 + (index * 37)) % 256),
        static_cast<int32_t>((140 + (index * 43)) % 256));
}

Color ShowcaseAccentColor(int index)
{
    return Color::FromArgb(255,
        static_cast<int32_t>((220 + (index * 17)) % 256),
        static_cast<int32_t>((180 + (index * 19)) % 256),
        static_cast<int32_t>((120 + (index * 23)) % 256));
}

void SetFont(Style& style, const std::function<void(Font&)>& mutate)
{
    auto font = style.GetFont();
    mutate(font);
    style.SetFont(font);
}

void SetBorders(Style& style, const std::function<void(Borders&)>& mutate)
{
    auto borders = style.GetBorders();
    mutate(borders);
    style.SetBorders(borders);
}

void SetBorder(Borders& borders, const std::function<Border(const Borders&)>& getter,
               const std::function<void(Borders&, const Border&)>& setter,
               BorderStyleType lineStyle, const Color& color)
{
    auto border = getter(borders);
    border.SetLineStyle(lineStyle);
    border.SetColor(color);
    setter(borders, border);
}

constexpr int ShowcaseStartColumn = 5;
constexpr int FillPatternRow = 0;
constexpr int BorderStyleRow = 1;
constexpr int HorizontalAlignmentRow = 2;
constexpr int VerticalAlignmentRow = 3;
constexpr int FontRow = 4;
constexpr int NumberFormatRow = 5;
constexpr int BorderSidesRow = 6;
constexpr int RotationRow = 7;
constexpr int ReadingOrderRow = 8;
constexpr int ProtectionRow = 9;
constexpr int FontSizeRow = 10;
constexpr int IndentRow = 11;
constexpr int WrapShrinkRow = 12;

void AddShowcaseLabel(Worksheet& sheet, int rowIndex, const std::string& label)
{
    auto cell = sheet.GetCells()(rowIndex, ShowcaseStartColumn - 1);
    cell.PutValue(label);
    auto style = cell.GetStyle();
    SetFont(style, [](Font& font) { font.SetBold(true); });
    style.SetPattern(FillPattern::Solid);
    style.SetForegroundColor(Color::FromArgb(255, 217, 225, 242));
    SetBorders(style, [](Borders& borders) {
        SetBorder(borders, [](const Borders& b) { return b.GetBottom(); }, [](Borders& b, const Border& v) { b.SetBottom(v); },
                  BorderStyleType::Thin, Color::FromArgb(255, 79, 129, 189));
    });
    cell.SetStyle(style);
}

std::vector<CellArea> AreasOf(FormatConditionCollection& collection)
{
    std::vector<CellArea> areas;
    for (auto index = 0; index < collection.GetRangeCount(); ++index) {
        areas.push_back(collection.GetCellArea(index));
    }
    return areas;
}

void AddDifferentialStyles(Worksheet& sheet)
{
    const std::array<Color, 5> colors = {
        Color::FromArgb(255, 255, 235, 156),
        Color::FromArgb(255, 198, 239, 206),
        Color::FromArgb(255, 221, 235, 247),
        Color::FromArgb(255, 255, 199, 206),
        Color::FromArgb(255, 189, 215, 238),
    };

    for (auto index = 0; index < static_cast<int>(colors.size()); ++index) {
        auto collection = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
        collection.AddArea(CellArea::CreateCellArea(index + 1, 0, index + 1, 0));
        auto condition = collection[collection.AddCondition(FormatConditionType::Expression, OperatorType::None, "TRUE", "")];
        auto style = condition.GetStyle();
        style.SetPattern(FillPattern::Solid);
        style.SetForegroundColor(colors[static_cast<std::size_t>(index)]);
        condition.SetStyle(style);
    }
}

}  // namespace

WorkbookHandle::WorkbookHandle()
    : _workbook(std::make_unique<Workbook>())
{
}

WorkbookHandle::WorkbookHandle(std::unique_ptr<Workbook> workbook)
    : _workbook(std::move(workbook))
{
}

Workbook& WorkbookHandle::Get() noexcept
{
    return *_workbook;
}

const Workbook& WorkbookHandle::Get() const noexcept
{
    return *_workbook;
}

WorkbookHandle::operator Workbook&() noexcept
{
    return *_workbook;
}

WorkbookHandle::operator const Workbook&() const noexcept
{
    return *_workbook;
}

WorksheetCollection& WorkbookHandle::GetWorksheets()
{
    return _workbook->GetWorksheets();
}

const WorksheetCollection& WorkbookHandle::GetWorksheets() const
{
    return _workbook->GetWorksheets();
}

WorkbookSettings& WorkbookHandle::GetSettings()
{
    return _workbook->GetSettings();
}

const WorkbookSettings& WorkbookHandle::GetSettings() const
{
    return _workbook->GetSettings();
}

WorkbookProperties& WorkbookHandle::GetProperties()
{
    return _workbook->GetProperties();
}

const WorkbookProperties& WorkbookHandle::GetProperties() const
{
    return _workbook->GetProperties();
}

DocumentProperties& WorkbookHandle::GetDocumentProperties()
{
    return _workbook->GetDocumentProperties();
}

const DocumentProperties& WorkbookHandle::GetDocumentProperties() const
{
    return _workbook->GetDocumentProperties();
}

DefinedNameCollection& WorkbookHandle::GetDefinedNames()
{
    return _workbook->GetDefinedNames();
}

const DefinedNameCollection& WorkbookHandle::GetDefinedNames() const
{
    return _workbook->GetDefinedNames();
}

void WorkbookHandle::Save(std::string_view fileName) const
{
    _workbook->Save(fileName);
}

void WorkbookHandle::Save(std::string_view fileName, SaveFormat format) const
{
    _workbook->Save(fileName, format);
}

void WorkbookHandle::Save(std::string_view fileName, const SaveOptions& options) const
{
    _workbook->Save(fileName, options);
}

void WorkbookHandle::Save(std::vector<std::uint8_t>& stream, SaveFormat format) const
{
    _workbook->Save(stream, format);
}

void WorkbookHandle::Save(std::vector<std::uint8_t>& stream, const SaveOptions& options) const
{
    _workbook->Save(stream, options);
}

WorkbookHandle CreateMixedCellWorkbook(bool useDate1904)
{
    auto workbook = std::make_unique<Workbook>();
    workbook->GetSettings().SetDate1904(useDate1904);

    auto& sheet = workbook->GetWorksheets()[0];
    sheet.SetName("Data");
    sheet.GetCells()["A1"].PutValue("Hello");
    sheet.GetCells()["B1"].PutValue(123);
    sheet.GetCells()["C1"].PutValue(true);
    sheet.GetCells()["D1"].PutValue(12.5);
    sheet.GetCells()["E1"].PutValue(6.02214076E+23);
    sheet.GetCells()["F1"].PutValue(DateTime(2024, 5, 6, 7, 8, 9));
    sheet.GetCells()["G1"].PutValue(20);
    sheet.GetCells()["G1"].SetFormula("=B1*2");
    return WorkbookHandle(std::move(workbook));
}

WorkbookHandle CreateWorksheetSettingsWorkbook()
{
    auto workbook = std::make_unique<Workbook>();
    auto& sheet = workbook->GetWorksheets()[0];
    sheet.SetName("Layout");
    sheet.SetVisibilityType(VisibilityType::Hidden);
    sheet.SetTabColor(Color::FromArgb(255, 34, 68, 102));
    sheet.SetShowGridlines(false);
    sheet.SetShowRowColumnHeaders(false);
    sheet.SetShowZeros(false);
    sheet.SetRightToLeft(true);
    sheet.SetZoom(85);
    sheet.Protect();
    sheet.GetProtection().SetObjects(true);
    sheet.GetProtection().SetScenarios(true);
    sheet.GetProtection().SetFormatCells(true);
    sheet.GetProtection().SetInsertRows(true);
    sheet.GetProtection().SetAutoFilter(true);
    sheet.GetProtection().SetSelectLockedCells(true);
    sheet.GetProtection().SetSelectUnlockedCells(true);

    sheet.GetCells()["A1"].PutValue("Merged");
    sheet.GetCells()["C4"].PutValue(99);
    sheet.GetCells().GetRows()[1].SetHeight(22.5);
    sheet.GetCells().GetRows()[3].SetIsHidden(true);
    sheet.GetCells().GetColumns()[0].SetWidth(18.25);
    sheet.GetCells().GetColumns()[2].SetIsHidden(true);
    sheet.GetCells().Merge(0, 0, 2, 2);

    const auto visibleSheetIndex = workbook->GetWorksheets().Add();
    auto& visibleSheet = workbook->GetWorksheets()[visibleSheetIndex];
    visibleSheet.SetName("Visible");
    visibleSheet.GetCells()["A1"].PutValue("Visible");
    workbook->GetWorksheets().SetActiveSheetName("Visible");

    return WorkbookHandle(std::move(workbook));
}

void AssertWorksheetSettings(Workbook& workbook)
{
    auto& sheet = workbook.GetWorksheets()["Layout"];
    EXPECT_EQ(VisibilityType::Hidden, sheet.GetVisibilityType());
    EXPECT_EQ(Color::FromArgb(255, 34, 68, 102), sheet.GetTabColor());
    EXPECT_FALSE(sheet.GetShowGridlines());
    EXPECT_FALSE(sheet.GetShowRowColumnHeaders());
    EXPECT_FALSE(sheet.GetShowZeros());
    EXPECT_TRUE(sheet.GetRightToLeft());
    EXPECT_EQ(85, sheet.GetZoom());
    EXPECT_TRUE(sheet.GetProtection().GetIsProtected());
    EXPECT_TRUE(sheet.GetProtection().GetObjects());
    EXPECT_TRUE(sheet.GetProtection().GetScenarios());
    EXPECT_TRUE(sheet.GetProtection().GetFormatCells());
    EXPECT_TRUE(sheet.GetProtection().GetInsertRows());
    EXPECT_TRUE(sheet.GetProtection().GetAutoFilter());
    EXPECT_TRUE(sheet.GetProtection().GetSelectLockedCells());
    EXPECT_TRUE(sheet.GetProtection().GetSelectUnlockedCells());
    EXPECT_EQ("Merged", sheet.GetCells()["A1"].GetStringValue());
    EXPECT_EQ(99, sheet.GetCells()["C4"].GetValue().AsInteger());
    EXPECT_DOUBLE_EQ(22.5, sheet.GetCells().GetRows()[1].GetHeight().value_or(0.0));
    EXPECT_TRUE(sheet.GetCells().GetRows()[3].GetIsHidden());
    EXPECT_DOUBLE_EQ(18.25, sheet.GetCells().GetColumns()[0].GetWidth().value_or(0.0));
    EXPECT_TRUE(sheet.GetCells().GetColumns()[2].GetIsHidden());
    auto merged = sheet.GetCells().GetMergedCells();
    ASSERT_EQ(1u, merged.size());
    ExpectArea(merged[0], 0, 0, 2, 2);
}

void AssertWorksheetSettingsScenarioHasVisibleSheet(Workbook& workbook)
{
    ASSERT_GE(workbook.GetWorksheets().GetCount(), 2);
    auto& visibleSheet = workbook.GetWorksheets()["Visible"];
    EXPECT_EQ(VisibilityType::Visible, visibleSheet.GetVisibilityType());
    EXPECT_EQ("Visible", visibleSheet.GetCells()["A1"].GetStringValue());
    EXPECT_EQ("Visible", workbook.GetWorksheets().GetActiveSheetName());
}

WorkbookHandle CreatePageSetupWorkbook()
{
    auto workbook = std::make_unique<Workbook>();
    auto& sheet = workbook->GetWorksheets()[0];
    sheet.SetName("Print Sheet");
    sheet.GetCells()["A1"].PutValue("Title");
    sheet.GetCells()["C10"].PutValue(42);

    auto& pageSetup = sheet.GetPageSetup();
    pageSetup.SetLeftMarginInch(0.25);
    pageSetup.SetRightMarginInch(0.4);
    pageSetup.SetTopMarginInch(0.5);
    pageSetup.SetBottomMarginInch(0.6);
    pageSetup.SetHeaderMarginInch(0.2);
    pageSetup.SetFooterMarginInch(0.22);
    pageSetup.SetOrientation(PageOrientationType::Landscape);
    pageSetup.SetPaperSize(PaperSizeType::PaperA4);
    pageSetup.SetFirstPageNumber(3);
    pageSetup.SetScale(95);
    pageSetup.SetFitToPagesWide(1);
    pageSetup.SetFitToPagesTall(2);
    pageSetup.SetPrintArea("$A$1:$C$10");
    pageSetup.SetPrintTitleRows("$1:$2");
    pageSetup.SetPrintTitleColumns("$A:$B");
    pageSetup.SetLeftHeader("Left Header");
    pageSetup.SetCenterHeader("Center Header");
    pageSetup.SetRightHeader("Right Header");
    pageSetup.SetLeftFooter("Left Footer");
    pageSetup.SetCenterFooter("Center Footer");
    pageSetup.SetRightFooter("Right Footer");
    pageSetup.SetPrintGridlines(true);
    pageSetup.SetPrintHeadings(true);
    pageSetup.SetCenterHorizontally(true);
    pageSetup.SetCenterVertically(true);
    pageSetup.AddHorizontalPageBreak(4);
    pageSetup.AddHorizontalPageBreak(7);
    pageSetup.AddVerticalPageBreak(2);

    return WorkbookHandle(std::move(workbook));
}

void AssertPageSetup(Workbook& workbook)
{
    auto& pageSetup = workbook.GetWorksheets()[0].GetPageSetup();
    EXPECT_DOUBLE_EQ(0.25, pageSetup.GetLeftMarginInch());
    EXPECT_DOUBLE_EQ(0.4, pageSetup.GetRightMarginInch());
    EXPECT_DOUBLE_EQ(0.5, pageSetup.GetTopMarginInch());
    EXPECT_DOUBLE_EQ(0.6, pageSetup.GetBottomMarginInch());
    EXPECT_DOUBLE_EQ(0.2, pageSetup.GetHeaderMarginInch());
    EXPECT_DOUBLE_EQ(0.22, pageSetup.GetFooterMarginInch());
    EXPECT_EQ(PageOrientationType::Landscape, pageSetup.GetOrientation());
    EXPECT_EQ(PaperSizeType::PaperA4, pageSetup.GetPaperSize());
    EXPECT_EQ(3, pageSetup.GetFirstPageNumber().value_or(0));
    EXPECT_EQ(95, pageSetup.GetScale().value_or(0));
    EXPECT_EQ(1, pageSetup.GetFitToPagesWide().value_or(0));
    EXPECT_EQ(2, pageSetup.GetFitToPagesTall().value_or(0));
    EXPECT_EQ("$A$1:$C$10", pageSetup.GetPrintArea());
    EXPECT_EQ("$1:$2", pageSetup.GetPrintTitleRows());
    EXPECT_EQ("$A:$B", pageSetup.GetPrintTitleColumns());
    EXPECT_EQ("Left Header", pageSetup.GetLeftHeader());
    EXPECT_EQ("Center Header", pageSetup.GetCenterHeader());
    EXPECT_EQ("Right Header", pageSetup.GetRightHeader());
    EXPECT_EQ("Left Footer", pageSetup.GetLeftFooter());
    EXPECT_EQ("Center Footer", pageSetup.GetCenterFooter());
    EXPECT_EQ("Right Footer", pageSetup.GetRightFooter());
    EXPECT_TRUE(pageSetup.GetPrintGridlines());
    EXPECT_TRUE(pageSetup.GetPrintHeadings());
    EXPECT_TRUE(pageSetup.GetCenterHorizontally());
    EXPECT_TRUE(pageSetup.GetCenterVertically());
    auto horizontalBreaks = pageSetup.GetHorizontalPageBreaks();
    ASSERT_EQ(2u, horizontalBreaks.size());
    EXPECT_EQ(4, horizontalBreaks[0]);
    EXPECT_EQ(7, horizontalBreaks[1]);
    auto verticalBreaks = pageSetup.GetVerticalPageBreaks();
    ASSERT_EQ(1u, verticalBreaks.size());
    EXPECT_EQ(2, verticalBreaks[0]);
    EXPECT_EQ("Title", workbook.GetWorksheets()[0].GetCells()["A1"].GetStringValue());
    EXPECT_EQ("42", workbook.GetWorksheets()[0].GetCells()["C10"].GetStringValue());
}

WorkbookHandle CreateHyperlinkWorkbook()
{
    auto workbook = std::make_unique<Workbook>();
    auto& sheet = workbook->GetWorksheets()[0];
    sheet.SetName("Links");
    workbook->GetWorksheets().Add("Target Sheet");
    workbook->GetWorksheets()[1].GetCells()(2, 2).PutValue("Target");

    sheet.GetCells()["A1"].PutValue("Docs");
    auto external = sheet.GetHyperlinks()[sheet.GetHyperlinks().Add("A1", 1, 1, "https://example.com/docs?q=1")];
    external.SetTextToDisplay("Docs");
    external.SetScreenTip("External docs");

    sheet.GetCells()["B2"].PutValue("Jump");
    auto internalLink = sheet.GetHyperlinks()[sheet.GetHyperlinks().Add("B2", 1, 1, "'Target Sheet'!C3")];
    internalLink.SetTextToDisplay("Jump");
    internalLink.SetScreenTip("Jump to target");

    auto rangeLink = sheet.GetHyperlinks()[sheet.GetHyperlinks().Add("C4", 2, 2, "mailto:test@example.com")];
    rangeLink.SetTextToDisplay("Mail");
    rangeLink.SetScreenTip("Send mail");

    return WorkbookHandle(std::move(workbook));
}

void AssertHyperlinks(Workbook& workbook)
{
    auto& sheet = workbook.GetWorksheets()[0];
    ASSERT_EQ(3, sheet.GetHyperlinks().GetCount());

    auto external = sheet.GetHyperlinks()[0];
    EXPECT_EQ("A1", external.GetArea());
    EXPECT_EQ("https://example.com/docs?q=1", external.GetAddress());
    EXPECT_EQ("External docs", external.GetScreenTip());
    EXPECT_EQ("Docs", external.GetTextToDisplay());

    auto internalLink = sheet.GetHyperlinks()[1];
    EXPECT_EQ("B2", internalLink.GetArea());
    EXPECT_EQ("'Target Sheet'!C3", internalLink.GetAddress());
    EXPECT_EQ("Jump to target", internalLink.GetScreenTip());
    EXPECT_EQ("Jump", internalLink.GetTextToDisplay());

    auto rangeLink = sheet.GetHyperlinks()[2];
    EXPECT_EQ("C4:D5", rangeLink.GetArea());
    EXPECT_EQ("mailto:test@example.com", rangeLink.GetAddress());
    EXPECT_EQ("Send mail", rangeLink.GetScreenTip());
    EXPECT_EQ("Mail", rangeLink.GetTextToDisplay());
}

void ApplyPrimaryStyle(Style& style)
{
    SetFont(style, [](Font& font) {
        font.SetName("Arial");
        font.SetSize(14.5);
        font.SetBold(true);
        font.SetItalic(true);
        font.SetUnderline(true);
        font.SetStrikeThrough(true);
        font.SetColor(Color::FromArgb(255, 17, 34, 51));
    });
    style.SetPattern(FillPattern::LightGrid);
    style.SetForegroundColor(Color::FromArgb(255, 210, 220, 30));
    style.SetBackgroundColor(Color::FromArgb(255, 12, 45, 78));
    SetBorders(style, [](Borders& borders) {
        SetBorder(borders, [](const Borders& b) { return b.GetLeft(); }, [](Borders& b, const Border& v) { b.SetLeft(v); },
                  BorderStyleType::Dotted, Color::FromArgb(255, 200, 0, 0));
        SetBorder(borders, [](const Borders& b) { return b.GetRight(); }, [](Borders& b, const Border& v) { b.SetRight(v); },
                  BorderStyleType::MediumDashDot, Color::FromArgb(255, 240, 120, 0));
        SetBorder(borders, [](const Borders& b) { return b.GetTop(); }, [](Borders& b, const Border& v) { b.SetTop(v); },
                  BorderStyleType::Double, Color::FromArgb(255, 0, 0, 255));
        SetBorder(borders, [](const Borders& b) { return b.GetBottom(); }, [](Borders& b, const Border& v) { b.SetBottom(v); },
                  BorderStyleType::DashDotDot, Color::FromArgb(255, 0, 120, 0));
        SetBorder(borders, [](const Borders& b) { return b.GetDiagonal(); }, [](Borders& b, const Border& v) { b.SetDiagonal(v); },
                  BorderStyleType::SlantedDashDot, Color::FromArgb(255, 128, 0, 128));
        borders.SetDiagonalUp(true);
        borders.SetDiagonalDown(true);
    });
    style.SetNumberFormat("#,##0.00");
    style.SetHorizontalAlignment(HorizontalAlignmentType::Distributed);
    style.SetVerticalAlignment(VerticalAlignmentType::Distributed);
    style.SetWrapText(true);
    style.SetIndentLevel(2);
    style.SetTextRotation(45);
    style.SetShrinkToFit(true);
    style.SetReadingOrder(2);
    style.SetRelativeIndent(1);
    style.SetIsLocked(false);
    style.SetIsHidden(true);
}

void ApplyCustomNumberStyle(Style& style)
{
    SetFont(style, [](Font& font) { font.SetBold(true); });
    SetBorders(style, [](Borders& borders) {
        SetBorder(borders, [](const Borders& b) { return b.GetTop(); }, [](Borders& b, const Border& v) { b.SetTop(v); },
                  BorderStyleType::MediumDashed, Color::FromArgb(255, 0, 0, 255));
    });
    style.SetHorizontalAlignment(HorizontalAlignmentType::CenterContinuous);
    style.SetVerticalAlignment(VerticalAlignmentType::Top);
    style.SetNumberFormat("0.0000");
}

void ApplyDateStyle(Style& style)
{
    SetFont(style, [](Font& font) {
        font.SetName("Times New Roman");
        font.SetSize(12);
        font.SetColor(Color::FromArgb(255, 0, 96, 160));
    });
    style.SetPattern(FillPattern::Solid);
    style.SetForegroundColor(Color::FromArgb(255, 221, 235, 247));
    style.SetNumberFormat("m/d/yyyy h:mm");
    style.SetHorizontalAlignment(HorizontalAlignmentType::Center);
    style.SetVerticalAlignment(VerticalAlignmentType::Center);
}

void ApplyVerticalTextStyle(Style& style)
{
    SetFont(style, [](Font& font) {
        font.SetName("Consolas");
        font.SetSize(10);
        font.SetItalic(true);
    });
    style.SetPattern(FillPattern::DarkHorizontal);
    style.SetForegroundColor(Color::FromArgb(255, 217, 217, 217));
    style.SetBackgroundColor(Color::FromArgb(255, 255, 255, 255));
    SetBorders(style, [](Borders& borders) {
        SetBorder(borders, [](const Borders& b) { return b.GetLeft(); }, [](Borders& b, const Border& v) { b.SetLeft(v); },
                  BorderStyleType::Thin, Color::FromArgb(255, 64, 64, 64));
    });
    style.SetHorizontalAlignment(HorizontalAlignmentType::Justify);
    style.SetVerticalAlignment(VerticalAlignmentType::Justify);
    style.SetWrapText(true);
    style.SetTextRotation(255);
    style.SetReadingOrder(1);
    style.SetRelativeIndent(2);
    style.SetIsLocked(true);
    style.SetIsHidden(false);
}

WorkbookHandle CreateStyledWorkbook()
{
    auto workbook = std::make_unique<Workbook>();
    auto& sheet = workbook->GetWorksheets()[0];
    sheet.SetName("Styled");

    auto primaryCell = sheet.GetCells()["A1"];
    primaryCell.PutValue(1234.567);
    auto primaryStyle = primaryCell.GetStyle();
    ApplyPrimaryStyle(primaryStyle);
    primaryCell.SetStyle(primaryStyle);

    auto customCell = sheet.GetCells()["B2"];
    auto customStyle = customCell.GetStyle();
    ApplyCustomNumberStyle(customStyle);
    customCell.SetStyle(customStyle);

    auto dateCell = sheet.GetCells()["C3"];
    dateCell.PutValue(DateTime(2024, 5, 6, 7, 8, 9));
    auto dateStyle = dateCell.GetStyle();
    ApplyDateStyle(dateStyle);
    dateCell.SetStyle(dateStyle);

    auto verticalTextCell = sheet.GetCells()["D4"];
    verticalTextCell.PutValue("Vertical");
    auto verticalTextStyle = verticalTextCell.GetStyle();
    ApplyVerticalTextStyle(verticalTextStyle);
    verticalTextCell.SetStyle(verticalTextStyle);

    AddShowcaseLabel(sheet, FillPatternRow, "Fill Patterns");
    AddShowcaseLabel(sheet, BorderStyleRow, "Border Styles");
    AddShowcaseLabel(sheet, HorizontalAlignmentRow, "Horizontal Align");
    AddShowcaseLabel(sheet, VerticalAlignmentRow, "Vertical Align");
    AddShowcaseLabel(sheet, FontRow, "Font Settings");
    AddShowcaseLabel(sheet, NumberFormatRow, "Number Formats");
    AddShowcaseLabel(sheet, BorderSidesRow, "Border Sides");
    AddShowcaseLabel(sheet, RotationRow, "Text Rotation");
    AddShowcaseLabel(sheet, ReadingOrderRow, "Reading Order");
    AddShowcaseLabel(sheet, ProtectionRow, "Protection");
    AddShowcaseLabel(sheet, FontSizeRow, "Font Sizes");
    AddShowcaseLabel(sheet, IndentRow, "Indent Levels");
    AddShowcaseLabel(sheet, WrapShrinkRow, "Wrap And Shrink");

    // Fill pattern showcase
    {
        const std::array<FillPattern, 17> fillPatterns = {
            FillPattern::Solid, FillPattern::MediumGray, FillPattern::DarkGray, FillPattern::Gray125,
            FillPattern::Gray0625, FillPattern::DarkHorizontal, FillPattern::DarkVertical,
            FillPattern::DarkDown, FillPattern::DarkUp, FillPattern::DarkGrid, FillPattern::DarkTrellis,
            FillPattern::LightHorizontal, FillPattern::LightVertical, FillPattern::LightDown,
            FillPattern::LightUp, FillPattern::LightGrid, FillPattern::LightTrellis,
        };
        for (auto index = 0; index < static_cast<int>(fillPatterns.size()); ++index) {
            auto cell = sheet.GetCells()(FillPatternRow, ShowcaseStartColumn + index);
            cell.PutValue("P" + std::to_string(index));
            auto style = cell.GetStyle();
            style.SetPattern(fillPatterns[static_cast<std::size_t>(index)]);
            style.SetForegroundColor(ShowcaseColor(index));
            style.SetBackgroundColor(ShowcaseAccentColor(index));
            cell.SetStyle(style);
        }
    }

    // Border style showcase
    {
        const std::array<BorderStyleType, 13> borderStyles = {
            BorderStyleType::Thin, BorderStyleType::Medium, BorderStyleType::Thick, BorderStyleType::Dotted,
            BorderStyleType::Dashed, BorderStyleType::Double, BorderStyleType::Hair,
            BorderStyleType::MediumDashed, BorderStyleType::DashDot, BorderStyleType::MediumDashDot,
            BorderStyleType::DashDotDot, BorderStyleType::MediumDashDotDot, BorderStyleType::SlantedDashDot,
        };
        for (auto index = 0; index < static_cast<int>(borderStyles.size()); ++index) {
            auto cell = sheet.GetCells()(BorderStyleRow, ShowcaseStartColumn + index);
            cell.PutValue("B" + std::to_string(index));
            auto style = cell.GetStyle();
            SetBorders(style, [&](Borders& borders) {
                SetBorder(borders, [](const Borders& b) { return b.GetTop(); }, [](Borders& b, const Border& v) { b.SetTop(v); },
                          borderStyles[static_cast<std::size_t>(index)], ShowcaseColor(index));
                SetBorder(borders, [](const Borders& b) { return b.GetBottom(); }, [](Borders& b, const Border& v) { b.SetBottom(v); },
                          BorderStyleType::Thin, ShowcaseAccentColor(index));
            });
            cell.SetStyle(style);
        }
    }

    // Horizontal alignment showcase
    {
        const std::array<HorizontalAlignmentType, 8> alignments = {
            HorizontalAlignmentType::General, HorizontalAlignmentType::Left,
            HorizontalAlignmentType::Center, HorizontalAlignmentType::Right,
            HorizontalAlignmentType::Fill, HorizontalAlignmentType::Justify,
            HorizontalAlignmentType::CenterContinuous, HorizontalAlignmentType::Distributed,
        };
        for (auto index = 0; index < static_cast<int>(alignments.size()); ++index) {
            auto cell = sheet.GetCells()(HorizontalAlignmentRow, ShowcaseStartColumn + index);
            cell.PutValue("Align " + std::to_string(index) + " sample");
            auto style = cell.GetStyle();
            style.SetHorizontalAlignment(alignments[static_cast<std::size_t>(index)]);
            style.SetWrapText(true);
            style.SetPattern(FillPattern::Solid);
            style.SetForegroundColor(ShowcaseAccentColor(index));
            cell.SetStyle(style);
        }
    }

    // Vertical alignment showcase
    {
        const std::array<VerticalAlignmentType, 5> alignments = {
            VerticalAlignmentType::Bottom, VerticalAlignmentType::Center,
            VerticalAlignmentType::Top, VerticalAlignmentType::Justify,
            VerticalAlignmentType::Distributed,
        };
        for (auto index = 0; index < static_cast<int>(alignments.size()); ++index) {
            auto cell = sheet.GetCells()(VerticalAlignmentRow, ShowcaseStartColumn + index);
            cell.PutValue("Vertical " + std::to_string(index));
            auto style = cell.GetStyle();
            style.SetVerticalAlignment(alignments[static_cast<std::size_t>(index)]);
            style.SetShrinkToFit(true);
            style.SetPattern(FillPattern::LightTrellis);
            style.SetForegroundColor(ShowcaseColor(index));
            cell.SetStyle(style);
        }
    }

    // Font showcase
    {
        auto nameCell = sheet.GetCells()(FontRow, ShowcaseStartColumn);
        nameCell.PutValue("Cambria 16");
        auto nameStyle = nameCell.GetStyle();
        SetFont(nameStyle, [](Font& font) {
            font.SetName("Cambria");
            font.SetSize(16.0);
        });
        nameStyle.SetPattern(FillPattern::Solid);
        nameStyle.SetForegroundColor(Color::FromArgb(255, 242, 242, 242));
        nameCell.SetStyle(nameStyle);

        auto boldCell = sheet.GetCells()(FontRow, ShowcaseStartColumn + 1);
        boldCell.PutValue("Bold");
        auto boldStyle = boldCell.GetStyle();
        SetFont(boldStyle, [](Font& font) {
            font.SetBold(true);
            font.SetSize(12.0);
        });
        boldStyle.SetForegroundColor(Color::FromArgb(255, 31, 78, 121));
        boldCell.SetStyle(boldStyle);

        auto italicCell = sheet.GetCells()(FontRow, ShowcaseStartColumn + 2);
        italicCell.PutValue("Italic");
        auto italicStyle = italicCell.GetStyle();
        SetFont(italicStyle, [](Font& font) {
            font.SetItalic(true);
            font.SetSize(12.0);
        });
        italicStyle.SetForegroundColor(Color::FromArgb(255, 128, 100, 162));
        italicCell.SetStyle(italicStyle);

        auto underlineCell = sheet.GetCells()(FontRow, ShowcaseStartColumn + 3);
        underlineCell.PutValue("Underline");
        auto underlineStyle = underlineCell.GetStyle();
        SetFont(underlineStyle, [](Font& font) {
            font.SetUnderline(true);
            font.SetColor(Color::FromArgb(255, 0, 112, 192));
        });
        underlineCell.SetStyle(underlineStyle);

        auto strikeCell = sheet.GetCells()(FontRow, ShowcaseStartColumn + 4);
        strikeCell.PutValue("Strike");
        auto strikeStyle = strikeCell.GetStyle();
        SetFont(strikeStyle, [](Font& font) {
            font.SetStrikeThrough(true);
            font.SetColor(Color::FromArgb(255, 192, 0, 0));
        });
        strikeCell.SetStyle(strikeStyle);

        auto colorCell = sheet.GetCells()(FontRow, ShowcaseStartColumn + 5);
        colorCell.PutValue("Color");
        auto colorStyle = colorCell.GetStyle();
        SetFont(colorStyle, [](Font& font) {
            font.SetBold(true);
            font.SetColor(Color::FromArgb(255, 0, 176, 80));
        });
        colorStyle.SetPattern(FillPattern::Solid);
        colorStyle.SetForegroundColor(Color::FromArgb(255, 226, 239, 218));
        colorCell.SetStyle(colorStyle);
    }

    // Number format showcase
    {
        const std::array<std::string, 7> formats = {
            "0", "0.00", "#,##0.00", "0%", "0.00E+00", "# ?/?", "[$-409]#,##0.00",
        };
        for (auto index = 0; index < static_cast<int>(formats.size()); ++index) {
            auto cell = sheet.GetCells()(NumberFormatRow, ShowcaseStartColumn + index);
            if (index == 3) {
                cell.PutValue(0.375);
            } else if (index == 5) {
                cell.PutValue(12.75);
            } else {
                cell.PutValue(1234.567 + index);
            }
            auto style = cell.GetStyle();
            style.SetNumberFormat(formats[static_cast<std::size_t>(index)]);
            style.SetHorizontalAlignment(HorizontalAlignmentType::Right);
            style.SetPattern(FillPattern::Solid);
            style.SetForegroundColor(ShowcaseAccentColor(index + 10));
            cell.SetStyle(style);
        }
    }

    // Border sides showcase
    {
        auto leftCell = sheet.GetCells()(BorderSidesRow, ShowcaseStartColumn);
        leftCell.PutValue("Left");
        auto leftStyle = leftCell.GetStyle();
        SetBorders(leftStyle, [](Borders& borders) {
            SetBorder(borders, [](const Borders& b) { return b.GetLeft(); }, [](Borders& b, const Border& v) { b.SetLeft(v); },
                      BorderStyleType::Thick, Color::FromArgb(255, 192, 0, 0));
        });
        leftStyle.SetPattern(FillPattern::Solid);
        leftStyle.SetForegroundColor(Color::FromArgb(255, 255, 242, 204));
        leftCell.SetStyle(leftStyle);

        auto rightCell = sheet.GetCells()(BorderSidesRow, ShowcaseStartColumn + 1);
        rightCell.PutValue("Right");
        auto rightStyle = rightCell.GetStyle();
        SetBorders(rightStyle, [](Borders& borders) {
            SetBorder(borders, [](const Borders& b) { return b.GetRight(); }, [](Borders& b, const Border& v) { b.SetRight(v); },
                      BorderStyleType::Double, Color::FromArgb(255, 0, 112, 192));
        });
        rightStyle.SetPattern(FillPattern::Solid);
        rightStyle.SetForegroundColor(Color::FromArgb(255, 221, 235, 247));
        rightCell.SetStyle(rightStyle);

        auto topCell = sheet.GetCells()(BorderSidesRow, ShowcaseStartColumn + 2);
        topCell.PutValue("Top");
        auto topStyle = topCell.GetStyle();
        SetBorders(topStyle, [](Borders& borders) {
            SetBorder(borders, [](const Borders& b) { return b.GetTop(); }, [](Borders& b, const Border& v) { b.SetTop(v); },
                      BorderStyleType::DashDot, Color::FromArgb(255, 112, 48, 160));
        });
        topStyle.SetPattern(FillPattern::Solid);
        topStyle.SetForegroundColor(Color::FromArgb(255, 234, 209, 220));
        topCell.SetStyle(topStyle);

        auto bottomCell = sheet.GetCells()(BorderSidesRow, ShowcaseStartColumn + 3);
        bottomCell.PutValue("Bottom");
        auto bottomStyle = bottomCell.GetStyle();
        SetBorders(bottomStyle, [](Borders& borders) {
            SetBorder(borders, [](const Borders& b) { return b.GetBottom(); }, [](Borders& b, const Border& v) { b.SetBottom(v); },
                      BorderStyleType::MediumDashed, Color::FromArgb(255, 0, 176, 80));
        });
        bottomStyle.SetPattern(FillPattern::Solid);
        bottomStyle.SetForegroundColor(Color::FromArgb(255, 226, 239, 218));
        bottomCell.SetStyle(bottomStyle);

        auto diagUpCell = sheet.GetCells()(BorderSidesRow, ShowcaseStartColumn + 4);
        diagUpCell.PutValue("Diag Up");
        auto diagUpStyle = diagUpCell.GetStyle();
        SetBorders(diagUpStyle, [](Borders& borders) {
            SetBorder(borders, [](const Borders& b) { return b.GetDiagonal(); }, [](Borders& b, const Border& v) { b.SetDiagonal(v); },
                      BorderStyleType::SlantedDashDot, Color::FromArgb(255, 255, 0, 0));
            borders.SetDiagonalUp(true);
        });
        diagUpStyle.SetPattern(FillPattern::Solid);
        diagUpStyle.SetForegroundColor(Color::FromArgb(255, 252, 228, 214));
        diagUpCell.SetStyle(diagUpStyle);

        auto diagDownCell = sheet.GetCells()(BorderSidesRow, ShowcaseStartColumn + 5);
        diagDownCell.PutValue("Diag Down");
        auto diagDownStyle = diagDownCell.GetStyle();
        SetBorders(diagDownStyle, [](Borders& borders) {
            SetBorder(borders, [](const Borders& b) { return b.GetDiagonal(); }, [](Borders& b, const Border& v) { b.SetDiagonal(v); },
                      BorderStyleType::SlantedDashDot, Color::FromArgb(255, 0, 176, 240));
            borders.SetDiagonalDown(true);
        });
        diagDownStyle.SetPattern(FillPattern::Solid);
        diagDownStyle.SetForegroundColor(Color::FromArgb(255, 222, 234, 246));
        diagDownCell.SetStyle(diagDownStyle);
    }

    // Rotation showcase
    {
        const std::array<int, 6> rotations = { 0, 45, 90, 135, 180, 255 };
        for (auto index = 0; index < static_cast<int>(rotations.size()); ++index) {
            auto cell = sheet.GetCells()(RotationRow, ShowcaseStartColumn + index);
            cell.PutValue("Rot " + std::to_string(rotations[static_cast<std::size_t>(index)]));
            auto style = cell.GetStyle();
            style.SetTextRotation(rotations[static_cast<std::size_t>(index)]);
            style.SetHorizontalAlignment(HorizontalAlignmentType::Center);
            style.SetVerticalAlignment(VerticalAlignmentType::Center);
            style.SetWrapText(true);
            style.SetPattern(FillPattern::Solid);
            style.SetForegroundColor(ShowcaseColor(index + 20));
            cell.SetStyle(style);
        }
    }

    // Reading order showcase
    {
        const std::array<int, 3> orders = { 0, 1, 2 };
        for (auto index = 0; index < static_cast<int>(orders.size()); ++index) {
            auto cell = sheet.GetCells()(ReadingOrderRow, ShowcaseStartColumn + index);
            cell.PutValue("Order " + std::to_string(orders[static_cast<std::size_t>(index)]));
            auto style = cell.GetStyle();
            style.SetReadingOrder(orders[static_cast<std::size_t>(index)]);
            style.SetIndentLevel(index);
            if (orders[static_cast<std::size_t>(index)] > 0) {
                style.SetRelativeIndent(index + 1);
            }
            style.SetHorizontalAlignment(HorizontalAlignmentType::Distributed);
            style.SetPattern(FillPattern::Solid);
            style.SetForegroundColor(ShowcaseAccentColor(index + 20));
            cell.SetStyle(style);
        }
    }

    // Protection showcase
    {
        auto lockedCell = sheet.GetCells()(ProtectionRow, ShowcaseStartColumn);
        lockedCell.PutValue("Locked");
        auto lockedStyle = lockedCell.GetStyle();
        lockedStyle.SetIsLocked(true);
        lockedStyle.SetIsHidden(false);
        lockedStyle.SetPattern(FillPattern::Solid);
        lockedStyle.SetForegroundColor(Color::FromArgb(255, 226, 239, 218));
        lockedCell.SetStyle(lockedStyle);

        auto unlockedCell = sheet.GetCells()(ProtectionRow, ShowcaseStartColumn + 1);
        unlockedCell.PutValue("Unlocked");
        auto unlockedStyle = unlockedCell.GetStyle();
        unlockedStyle.SetIsLocked(false);
        unlockedStyle.SetIsHidden(false);
        unlockedStyle.SetPattern(FillPattern::Solid);
        unlockedStyle.SetForegroundColor(Color::FromArgb(255, 255, 242, 204));
        unlockedCell.SetStyle(unlockedStyle);

        auto hiddenCell = sheet.GetCells()(ProtectionRow, ShowcaseStartColumn + 2);
        hiddenCell.PutValue("Hidden");
        auto hiddenStyle = hiddenCell.GetStyle();
        hiddenStyle.SetIsLocked(true);
        hiddenStyle.SetIsHidden(true);
        hiddenStyle.SetPattern(FillPattern::Solid);
        hiddenStyle.SetForegroundColor(Color::FromArgb(255, 242, 220, 219));
        hiddenCell.SetStyle(hiddenStyle);
    }

    // Font size showcase
    {
        const std::array<double, 6> sizes = { 8.0, 10.0, 12.0, 14.0, 18.0, 24.0 };
        for (auto index = 0; index < static_cast<int>(sizes.size()); ++index) {
            auto cell = sheet.GetCells()(FontSizeRow, ShowcaseStartColumn + index);
            cell.PutValue("Size " + std::to_string(static_cast<int>(sizes[static_cast<std::size_t>(index)])));
            auto style = cell.GetStyle();
            SetFont(style, [&sizes, index](Font& font) {
                font.SetName("Calibri");
                font.SetSize(sizes[static_cast<std::size_t>(index)]);
            });
            style.SetPattern(FillPattern::Solid);
            style.SetForegroundColor(ShowcaseAccentColor(index + 30));
            cell.SetStyle(style);
        }
    }

    // Indent showcase
    {
        const std::array<int, 5> indents = { 0, 1, 2, 3, 4 };
        for (auto index = 0; index < static_cast<int>(indents.size()); ++index) {
            auto cell = sheet.GetCells()(IndentRow, ShowcaseStartColumn + index);
            cell.PutValue("Indent " + std::to_string(indents[static_cast<std::size_t>(index)]));
            auto style = cell.GetStyle();
            style.SetHorizontalAlignment(HorizontalAlignmentType::Left);
            style.SetIndentLevel(indents[static_cast<std::size_t>(index)]);
            style.SetRelativeIndent(indents[static_cast<std::size_t>(index)]);
            style.SetPattern(FillPattern::Solid);
            style.SetForegroundColor(ShowcaseColor(index + 30));
            cell.SetStyle(style);
        }
    }

    // Wrap and shrink showcase
    {
        auto wrapCell = sheet.GetCells()(WrapShrinkRow, ShowcaseStartColumn);
        wrapCell.PutValue("Wrap text sample");
        auto wrapStyle = wrapCell.GetStyle();
        wrapStyle.SetWrapText(true);
        wrapStyle.SetHorizontalAlignment(HorizontalAlignmentType::Justify);
        wrapStyle.SetPattern(FillPattern::Solid);
        wrapStyle.SetForegroundColor(Color::FromArgb(255, 252, 228, 214));
        wrapCell.SetStyle(wrapStyle);

        auto shrinkCell = sheet.GetCells()(WrapShrinkRow, ShowcaseStartColumn + 1);
        shrinkCell.PutValue("Shrink to fit sample");
        auto shrinkStyle = shrinkCell.GetStyle();
        shrinkStyle.SetShrinkToFit(true);
        shrinkStyle.SetHorizontalAlignment(HorizontalAlignmentType::Center);
        shrinkStyle.SetPattern(FillPattern::Solid);
        shrinkStyle.SetForegroundColor(Color::FromArgb(255, 221, 235, 247));
        shrinkCell.SetStyle(shrinkStyle);

        auto wrapShrinkCell = sheet.GetCells()(WrapShrinkRow, ShowcaseStartColumn + 2);
        wrapShrinkCell.PutValue("Wrap and shrink");
        auto wrapShrinkStyle = wrapShrinkCell.GetStyle();
        wrapShrinkStyle.SetWrapText(true);
        wrapShrinkStyle.SetShrinkToFit(true);
        wrapShrinkStyle.SetHorizontalAlignment(HorizontalAlignmentType::Distributed);
        wrapShrinkStyle.SetVerticalAlignment(VerticalAlignmentType::Center);
        wrapShrinkStyle.SetPattern(FillPattern::Solid);
        wrapShrinkStyle.SetForegroundColor(Color::FromArgb(255, 226, 239, 218));
        wrapShrinkCell.SetStyle(wrapShrinkStyle);

        auto justifyCell = sheet.GetCells()(WrapShrinkRow, ShowcaseStartColumn + 3);
        justifyCell.PutValue("Distributed indent");
        auto justifyStyle = justifyCell.GetStyle();
        justifyStyle.SetWrapText(true);
        justifyStyle.SetHorizontalAlignment(HorizontalAlignmentType::Distributed);
        justifyStyle.SetIndentLevel(3);
        justifyStyle.SetRelativeIndent(2);
        justifyStyle.SetPattern(FillPattern::Solid);
        justifyStyle.SetForegroundColor(Color::FromArgb(255, 242, 242, 242));
        justifyCell.SetStyle(justifyStyle);
    }

    return WorkbookHandle(std::move(workbook));
}

void AssertPrimaryStyle(const Style& style)
{
    EXPECT_EQ("Arial", style.GetFont().GetName());
    EXPECT_DOUBLE_EQ(14.5, style.GetFont().GetSize());
    EXPECT_TRUE(style.GetFont().GetBold());
    EXPECT_TRUE(style.GetFont().GetItalic());
    EXPECT_TRUE(style.GetFont().GetUnderline());
    EXPECT_TRUE(style.GetFont().GetStrikeThrough());
    EXPECT_EQ(Color::FromArgb(255, 17, 34, 51), style.GetFont().GetColor());
    EXPECT_EQ(FillPattern::LightGrid, style.GetPattern());
    EXPECT_EQ(Color::FromArgb(255, 210, 220, 30), style.GetForegroundColor());
    EXPECT_EQ(Color::FromArgb(255, 12, 45, 78), style.GetBackgroundColor());
    EXPECT_EQ(BorderStyleType::Dotted, style.GetBorders().GetLeft().GetLineStyle());
    EXPECT_EQ(Color::FromArgb(255, 200, 0, 0), style.GetBorders().GetLeft().GetColor());
    EXPECT_EQ(BorderStyleType::MediumDashDot, style.GetBorders().GetRight().GetLineStyle());
    EXPECT_EQ(Color::FromArgb(255, 240, 120, 0), style.GetBorders().GetRight().GetColor());
    EXPECT_EQ(BorderStyleType::Double, style.GetBorders().GetTop().GetLineStyle());
    EXPECT_EQ(Color::FromArgb(255, 0, 0, 255), style.GetBorders().GetTop().GetColor());
    EXPECT_EQ(BorderStyleType::DashDotDot, style.GetBorders().GetBottom().GetLineStyle());
    EXPECT_EQ(Color::FromArgb(255, 0, 120, 0), style.GetBorders().GetBottom().GetColor());
    EXPECT_EQ(BorderStyleType::SlantedDashDot, style.GetBorders().GetDiagonal().GetLineStyle());
    EXPECT_EQ(Color::FromArgb(255, 128, 0, 128), style.GetBorders().GetDiagonal().GetColor());
    EXPECT_TRUE(style.GetBorders().GetDiagonalUp());
    EXPECT_TRUE(style.GetBorders().GetDiagonalDown());
    EXPECT_EQ(4, style.GetNumber());
    EXPECT_EQ("", style.GetCustom());
    EXPECT_EQ("#,##0.00", style.GetNumberFormat());
    EXPECT_EQ(HorizontalAlignmentType::Distributed, style.GetHorizontalAlignment());
    EXPECT_EQ(VerticalAlignmentType::Distributed, style.GetVerticalAlignment());
    EXPECT_TRUE(style.GetWrapText());
    EXPECT_EQ(2, style.GetIndentLevel());
    EXPECT_EQ(45, style.GetTextRotation());
    EXPECT_TRUE(style.GetShrinkToFit());
    EXPECT_EQ(2, style.GetReadingOrder());
    EXPECT_EQ(1, style.GetRelativeIndent());
    EXPECT_FALSE(style.GetIsLocked());
    EXPECT_TRUE(style.GetIsHidden());
}

void AssertCustomNumberStyle(const Style& style)
{
    EXPECT_TRUE(style.GetFont().GetBold());
    EXPECT_EQ(BorderStyleType::MediumDashed, style.GetBorders().GetTop().GetLineStyle());
    EXPECT_EQ(Color::FromArgb(255, 0, 0, 255), style.GetBorders().GetTop().GetColor());
    EXPECT_EQ(HorizontalAlignmentType::CenterContinuous, style.GetHorizontalAlignment());
    EXPECT_EQ(VerticalAlignmentType::Top, style.GetVerticalAlignment());
    EXPECT_EQ("0.0000", style.GetCustom());
    EXPECT_EQ("0.0000", style.GetNumberFormat());
}

void AssertDateStyle(const Style& style)
{
    EXPECT_EQ("Times New Roman", style.GetFont().GetName());
    EXPECT_DOUBLE_EQ(12, style.GetFont().GetSize());
    EXPECT_EQ(Color::FromArgb(255, 0, 96, 160), style.GetFont().GetColor());
    EXPECT_EQ(FillPattern::Solid, style.GetPattern());
    EXPECT_EQ(Color::FromArgb(255, 221, 235, 247), style.GetForegroundColor());
    EXPECT_EQ("m/d/yyyy h:mm", style.GetNumberFormat());
    EXPECT_EQ(HorizontalAlignmentType::Center, style.GetHorizontalAlignment());
    EXPECT_EQ(VerticalAlignmentType::Center, style.GetVerticalAlignment());
}

void AssertVerticalTextStyle(const Style& style)
{
    EXPECT_EQ("Consolas", style.GetFont().GetName());
    EXPECT_DOUBLE_EQ(10, style.GetFont().GetSize());
    EXPECT_TRUE(style.GetFont().GetItalic());
    EXPECT_EQ(FillPattern::DarkHorizontal, style.GetPattern());
    EXPECT_EQ(Color::FromArgb(255, 217, 217, 217), style.GetForegroundColor());
    EXPECT_EQ(Color::FromArgb(255, 255, 255, 255), style.GetBackgroundColor());
    EXPECT_EQ(BorderStyleType::Thin, style.GetBorders().GetLeft().GetLineStyle());
    EXPECT_EQ(Color::FromArgb(255, 64, 64, 64), style.GetBorders().GetLeft().GetColor());
    EXPECT_EQ(HorizontalAlignmentType::Justify, style.GetHorizontalAlignment());
    EXPECT_EQ(VerticalAlignmentType::Justify, style.GetVerticalAlignment());
    EXPECT_TRUE(style.GetWrapText());
    EXPECT_EQ(255, style.GetTextRotation());
    EXPECT_EQ(1, style.GetReadingOrder());
    EXPECT_EQ(2, style.GetRelativeIndent());
    EXPECT_TRUE(style.GetIsLocked());
    EXPECT_FALSE(style.GetIsHidden());
}

void AssertStyleShowcase(Workbook& workbook)
{
    auto& sheet = workbook.GetWorksheets()["Styled"];
    AssertPrimaryStyle(sheet.GetCells()["A1"].GetStyle());
    AssertCustomNumberStyle(sheet.GetCells()["B2"].GetStyle());
    AssertDateStyle(sheet.GetCells()["C3"].GetStyle());
    AssertVerticalTextStyle(sheet.GetCells()["D4"].GetStyle());
}

WorkbookHandle CreateValidationWorkbook()
{
    auto workbook = std::make_unique<Workbook>();
    auto& sheet = workbook->GetWorksheets()[0];
    sheet.SetName("Validation Sheet");

    sheet.GetCells()["A1"].PutValue("Open");
    sheet.GetCells()["B2"].PutValue(5);
    sheet.GetCells()["C3"].PutValue(7);
    sheet.GetCells()["E2"].PutValue(8);
    sheet.GetCells()["G1"].PutValue("ABCDE");

    auto listValidation = sheet.GetValidations()[sheet.GetValidations().Add(CellArea::CreateCellArea("A1", "A3"))];
    listValidation.SetType(ValidationType::List);
    listValidation.SetFormula1("\"Open,Closed\"");
    listValidation.SetIgnoreBlank(true);
    listValidation.SetInCellDropDown(true);
    listValidation.SetShowInput(true);
    listValidation.SetInputTitle("Status");
    listValidation.SetInputMessage("Pick a status");
    listValidation.SetShowError(true);
    listValidation.SetErrorTitle("Invalid");
    listValidation.SetErrorMessage("Choose from the list");

    auto decimalValidation = sheet.GetValidations()[sheet.GetValidations().Add(CellArea::CreateCellArea("B2", "C3"))];
    decimalValidation.SetType(ValidationType::Decimal);
    decimalValidation.SetOperator(OperatorType::Between);
    decimalValidation.SetFormula1("1.5");
    decimalValidation.SetFormula2("9.5");
    decimalValidation.SetIgnoreBlank(false);
    decimalValidation.SetInCellDropDown(false);
    decimalValidation.SetShowError(true);
    decimalValidation.SetErrorTitle("Range");
    decimalValidation.SetErrorMessage("Enter 1.5-9.5");
    decimalValidation.AddArea(CellArea::CreateCellArea("E2", "E3"));

    auto customValidation = sheet.GetValidations()[sheet.GetValidations().Add(CellArea::CreateCellArea("G1", "G1"))];
    customValidation.SetType(ValidationType::Custom);
    customValidation.SetAlertStyle(ValidationAlertType::Warning);
    customValidation.SetFormula1("LEN(G1)<=5");
    customValidation.SetShowInput(true);
    customValidation.SetInputTitle("Code");
    customValidation.SetInputMessage("Up to 5 chars");

    return WorkbookHandle(std::move(workbook));
}

Validation GetValidationByArea(ValidationCollection& validations, const std::string& expectedArea)
{
    for (auto index = 0; index < validations.GetCount(); ++index) {
        auto validation = validations[index];
        if (AreaKey(validation.GetAreas()) == expectedArea) {
            return validation;
        }
    }
    throw std::runtime_error("Validation with area '" + expectedArea + "' was not found.");
}

void AssertValidations(Workbook& workbook)
{
    auto& sheet = workbook.GetWorksheets()["Validation Sheet"];
    ASSERT_EQ(3, sheet.GetValidations().GetCount());

    auto listValidation = GetValidationByArea(sheet.GetValidations(), "A1:A3");
    EXPECT_EQ(ValidationType::List, listValidation.GetType());
    EXPECT_EQ(OperatorType::None, listValidation.GetOperator());
    ASSERT_EQ(1u, listValidation.GetAreas().size());
    ExpectArea(listValidation.GetAreas()[0], 0, 0, 3, 1);
    EXPECT_EQ("\"Open,Closed\"", listValidation.GetFormula1());
    EXPECT_EQ("", listValidation.GetFormula2());
    EXPECT_TRUE(listValidation.GetIgnoreBlank());
    EXPECT_TRUE(listValidation.GetInCellDropDown());
    EXPECT_TRUE(listValidation.GetShowInput());
    EXPECT_TRUE(listValidation.GetShowError());
    EXPECT_EQ("Status", listValidation.GetInputTitle());
    EXPECT_EQ("Pick a status", listValidation.GetInputMessage());
    EXPECT_EQ("Invalid", listValidation.GetErrorTitle());
    EXPECT_EQ("Choose from the list", listValidation.GetErrorMessage());

    auto decimalValidation = GetValidationByArea(sheet.GetValidations(), "B2:C3 E2:E3");
    EXPECT_EQ(ValidationType::Decimal, decimalValidation.GetType());
    EXPECT_EQ(OperatorType::Between, decimalValidation.GetOperator());
    ASSERT_EQ(2u, decimalValidation.GetAreas().size());
    ExpectArea(decimalValidation.GetAreas()[0], 1, 1, 2, 2);
    ExpectArea(decimalValidation.GetAreas()[1], 1, 4, 2, 1);
    EXPECT_EQ("1.5", decimalValidation.GetFormula1());
    EXPECT_EQ("9.5", decimalValidation.GetFormula2());
    EXPECT_FALSE(decimalValidation.GetIgnoreBlank());
    EXPECT_FALSE(decimalValidation.GetInCellDropDown());
    EXPECT_FALSE(decimalValidation.GetShowInput());
    EXPECT_TRUE(decimalValidation.GetShowError());
    EXPECT_EQ("Range", decimalValidation.GetErrorTitle());
    EXPECT_EQ("Enter 1.5-9.5", decimalValidation.GetErrorMessage());

    auto customValidation = GetValidationByArea(sheet.GetValidations(), "G1");
    EXPECT_EQ(ValidationType::Custom, customValidation.GetType());
    EXPECT_EQ(ValidationAlertType::Warning, customValidation.GetAlertStyle());
    EXPECT_EQ(OperatorType::None, customValidation.GetOperator());
    ASSERT_EQ(1u, customValidation.GetAreas().size());
    ExpectArea(customValidation.GetAreas()[0], 0, 6, 1, 1);
    EXPECT_EQ("LEN(G1)<=5", customValidation.GetFormula1());
    EXPECT_EQ("", customValidation.GetFormula2());
    EXPECT_TRUE(customValidation.GetShowInput());
    EXPECT_FALSE(customValidation.GetShowError());
    EXPECT_EQ("Code", customValidation.GetInputTitle());
    EXPECT_EQ("Up to 5 chars", customValidation.GetInputMessage());

    auto a1Validation = sheet.GetValidations().GetValidationInCell(0, 0);
    ASSERT_TRUE(a1Validation.has_value());
    EXPECT_EQ(ValidationType::List, a1Validation->GetType());
    auto c2Validation = sheet.GetValidations().GetValidationInCell(1, 2);
    ASSERT_TRUE(c2Validation.has_value());
    EXPECT_EQ(ValidationType::Decimal, c2Validation->GetType());
    EXPECT_FALSE(sheet.GetValidations().GetValidationInCell(9, 9).has_value());
}

FormatConditionCollection GetCollectionByArea(ConditionalFormattingCollection& collections,
                                             const std::string& expectedArea)
{
    for (auto index = 0; index < collections.GetCount(); ++index) {
        auto collection = collections[index];
        if (AreaKey(AreasOf(collection)) == expectedArea) {
            return collection;
        }
    }
    throw std::runtime_error("Conditional formatting collection with area '" + expectedArea + "' was not found.");
}

WorkbookHandle CreateConditionalFormattingWorkbook()
{
    auto workbook = std::make_unique<Workbook>();
    auto& sheet = workbook->GetWorksheets()[0];
    sheet.SetName("Conditional Formatting");
    sheet.GetCells()["A1"].PutValue(1);
    sheet.GetCells()["A2"].PutValue(5);
    sheet.GetCells()["A3"].PutValue(10);
    sheet.GetCells()["A4"].PutValue(15);
    sheet.GetCells()["A5"].PutValue(20);
    sheet.GetCells()["C1"].PutValue(11);
    sheet.GetCells()["C2"].PutValue(8);
    sheet.GetCells()["C3"].PutValue(15);

    auto primary = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    primary.AddArea(CellArea::CreateCellArea("A1", "A5"));
    auto between = primary[primary.AddCondition(FormatConditionType::CellValue, OperatorType::Between, "5", "15")];
    between.SetStopIfTrue(true);
    between.SetPriority(1);
    auto betweenStyle = between.GetStyle();
    betweenStyle.SetPattern(FillPattern::Solid);
    betweenStyle.SetForegroundColor(Color::FromArgb(255, 255, 199, 206));
    SetFont(betweenStyle, [](Font& font) {
        font.SetBold(true);
        font.SetColor(Color::FromArgb(255, 156, 0, 6));
    });
    between.SetStyle(betweenStyle);

    auto expression = primary[primary.AddCondition(FormatConditionType::Expression, OperatorType::None, "MOD(A1,2)=0", "")];
    expression.SetPriority(2);
    auto expressionStyle = expression.GetStyle();
    SetFont(expressionStyle, [](Font& font) {
        font.SetItalic(true);
        font.SetColor(Color::FromArgb(255, 0, 0, 255));
    });
    SetBorders(expressionStyle, [](Borders& borders) {
        SetBorder(borders, [](const Borders& b) { return b.GetBottom(); }, [](Borders& b, const Border& v) { b.SetBottom(v); },
                  BorderStyleType::Thin, Color::FromArgb(255, 0, 0, 255));
    });
    expression.SetStyle(expressionStyle);

    auto secondary = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    secondary.AddArea(CellArea::CreateCellArea("C1", "C3"));
    auto greaterThan = secondary[secondary.AddCondition(FormatConditionType::CellValue, OperatorType::GreaterThan, "10", "")];
    greaterThan.SetPriority(3);
    auto greaterThanStyle = greaterThan.GetStyle();
    greaterThanStyle.SetPattern(FillPattern::Solid);
    greaterThanStyle.SetForegroundColor(Color::FromArgb(255, 198, 239, 206));
    SetFont(greaterThanStyle, [](Font& font) { font.SetColor(Color::FromArgb(255, 0, 97, 0)); });
    greaterThan.SetStyle(greaterThanStyle);

    return WorkbookHandle(std::move(workbook));
}

void AssertConditionalFormattings(Workbook& workbook)
{
    auto& sheet = workbook.GetWorksheets()["Conditional Formatting"];
    ASSERT_EQ(2, sheet.GetConditionalFormattings().GetCount());

    auto primary = GetCollectionByArea(sheet.GetConditionalFormattings(), "A1:A5");
    EXPECT_EQ(2, primary.GetCount());
    EXPECT_EQ(1, primary.GetRangeCount());
    ExpectArea(primary.GetCellArea(0), 0, 0, 5, 1);
    auto between = primary[0];
    EXPECT_EQ(FormatConditionType::CellValue, between.GetType());
    EXPECT_EQ(OperatorType::Between, between.GetOperator());
    EXPECT_EQ("5", between.GetFormula1());
    EXPECT_EQ("15", between.GetFormula2());
    EXPECT_EQ(1, between.GetPriority());
    EXPECT_TRUE(between.GetStopIfTrue());
    EXPECT_EQ(FillPattern::Solid, between.GetStyle().GetPattern());
    EXPECT_EQ(Color::FromArgb(255, 255, 199, 206), between.GetStyle().GetForegroundColor());
    EXPECT_TRUE(between.GetStyle().GetFont().GetBold());
    EXPECT_EQ(Color::FromArgb(255, 156, 0, 6), between.GetStyle().GetFont().GetColor());

    auto expression = primary[1];
    EXPECT_EQ(FormatConditionType::Expression, expression.GetType());
    EXPECT_EQ(OperatorType::None, expression.GetOperator());
    EXPECT_EQ("MOD(A1,2)=0", expression.GetFormula1());
    EXPECT_EQ("", expression.GetFormula2());
    EXPECT_EQ(2, expression.GetPriority());
    EXPECT_FALSE(expression.GetStopIfTrue());
    EXPECT_TRUE(expression.GetStyle().GetFont().GetItalic());
    EXPECT_EQ(Color::FromArgb(255, 0, 0, 255), expression.GetStyle().GetFont().GetColor());
    EXPECT_EQ(BorderStyleType::Thin, expression.GetStyle().GetBorders().GetBottom().GetLineStyle());

    auto secondary = GetCollectionByArea(sheet.GetConditionalFormattings(), "C1:C3");
    EXPECT_EQ(1, secondary.GetCount());
    EXPECT_EQ(1, secondary.GetRangeCount());
    ExpectArea(secondary.GetCellArea(0), 0, 2, 3, 1);
    auto greaterThan = secondary[0];
    EXPECT_EQ(FormatConditionType::CellValue, greaterThan.GetType());
    EXPECT_EQ(OperatorType::GreaterThan, greaterThan.GetOperator());
    EXPECT_EQ("10", greaterThan.GetFormula1());
    EXPECT_EQ("", greaterThan.GetFormula2());
    EXPECT_EQ(3, greaterThan.GetPriority());
    EXPECT_EQ(FillPattern::Solid, greaterThan.GetStyle().GetPattern());
    EXPECT_EQ(Color::FromArgb(255, 198, 239, 206), greaterThan.GetStyle().GetForegroundColor());
    EXPECT_EQ(Color::FromArgb(255, 0, 97, 0), greaterThan.GetStyle().GetFont().GetColor());
}

WorkbookHandle CreateAdvancedConditionalFormattingWorkbook()
{
    auto workbook = std::make_unique<Workbook>();
    auto& sheet = workbook->GetWorksheets()[0];
    sheet.SetName("Advanced Conditional Formatting");

    sheet.GetCells()["A1"].PutValue("error one");
    sheet.GetCells()["A2"].PutValue("ok");
    sheet.GetCells()["B1"].PutValue("ok");
    sheet.GetCells()["B2"].PutValue("warning");
    sheet.GetCells()["C1"].PutValue("prefix-item");
    sheet.GetCells()["D1"].PutValue("item-suffix");
    sheet.GetCells()["E1"].PutValue(DateTime(2024, 5, 20));
    sheet.GetCells()["F1"].PutValue(1);
    sheet.GetCells()["F2"].PutValue(1);
    sheet.GetCells()["G1"].PutValue(10);
    sheet.GetCells()["G2"].PutValue(20);
    for (auto index = 1; index <= 10; ++index) {
        sheet.GetCells()["H" + std::to_string(index)].PutValue(index * 10);
        sheet.GetCells()["I" + std::to_string(index)].PutValue(index * 10);
        sheet.GetCells()["J" + std::to_string(index)].PutValue(index * 10);
        sheet.GetCells()["K" + std::to_string(index)].PutValue(index * 10);
        sheet.GetCells()["L" + std::to_string(index)].PutValue(index * 10);
        sheet.GetCells()["M" + std::to_string(index)].PutValue(index * 10);
        sheet.GetCells()["N" + std::to_string(index)].PutValue(index * 10);
    }

    auto contains = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    contains.AddArea(CellArea::CreateCellArea("A1", "A10"));
    auto containsRule = contains[contains.AddCondition(FormatConditionType::ContainsText)];
    containsRule.SetFormula1("error");
    auto containsStyle = containsRule.GetStyle();
    containsStyle.SetPattern(FillPattern::Solid);
    containsStyle.SetForegroundColor(Color::FromArgb(255, 255, 235, 156));
    SetFont(containsStyle, [](Font& font) { font.SetColor(Color::FromArgb(255, 156, 0, 6)); });
    containsRule.SetStyle(containsStyle);

    auto notContains = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    notContains.AddArea(CellArea::CreateCellArea("B1", "B10"));
    notContains[notContains.AddCondition(FormatConditionType::NotContainsText)].SetFormula1("warning");

    auto beginsWith = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    beginsWith.AddArea(CellArea::CreateCellArea("C1", "C10"));
    beginsWith[beginsWith.AddCondition(FormatConditionType::BeginsWith)].SetFormula1("prefix");

    auto endsWith = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    endsWith.AddArea(CellArea::CreateCellArea("D1", "D10"));
    endsWith[endsWith.AddCondition(FormatConditionType::EndsWith)].SetFormula1("suffix");

    auto timePeriod = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    timePeriod.AddArea(CellArea::CreateCellArea("E1", "E10"));
    auto timePeriodRule = timePeriod[timePeriod.AddCondition(FormatConditionType::TimePeriod)];
    timePeriodRule.SetTimePeriod("today");

    auto duplicate = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    duplicate.AddArea(CellArea::CreateCellArea("F1", "F10"));
    duplicate[duplicate.AddCondition(FormatConditionType::DuplicateValues)].SetDuplicate(true);

    auto unique = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    unique.AddArea(CellArea::CreateCellArea("G1", "G10"));
    unique[unique.AddCondition(FormatConditionType::UniqueValues)].SetDuplicate(false);

    auto top10 = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    top10.AddArea(CellArea::CreateCellArea("H1", "H10"));
    auto top10Rule = top10[top10.AddCondition(FormatConditionType::Top10)];
    top10Rule.SetTop(true);
    top10Rule.SetPercent(true);
    top10Rule.SetRank(10);

    auto bottom10 = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    bottom10.AddArea(CellArea::CreateCellArea("I1", "I10"));
    auto bottom10Rule = bottom10[bottom10.AddCondition(FormatConditionType::Bottom10)];
    bottom10Rule.SetTop(false);
    bottom10Rule.SetRank(10);

    auto aboveAverage = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    aboveAverage.AddArea(CellArea::CreateCellArea("J1", "J10"));
    aboveAverage[aboveAverage.AddCondition(FormatConditionType::AboveAverage)].SetAbove(true);

    auto belowAverage = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    belowAverage.AddArea(CellArea::CreateCellArea("K1", "K10"));
    auto belowRule = belowAverage[belowAverage.AddCondition(FormatConditionType::BelowAverage)];
    belowRule.SetAbove(false);
    belowRule.SetStandardDeviation(1);

    auto colorScale = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    colorScale.AddArea(CellArea::CreateCellArea("L1", "L10"));
    auto colorRule = colorScale[colorScale.AddCondition(FormatConditionType::ColorScale)];
    colorRule.SetColorScaleCount(3);
    colorRule.SetMinColor(Color::FromArgb(255, 248, 105, 107));
    colorRule.SetMidColor(Color::FromArgb(255, 255, 235, 132));
    colorRule.SetMaxColor(Color::FromArgb(255, 99, 190, 123));

    auto dataBar = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    dataBar.AddArea(CellArea::CreateCellArea("M1", "M10"));
    auto dataBarRule = dataBar[dataBar.AddCondition(FormatConditionType::DataBar)];
    dataBarRule.SetBarColor(Color::FromArgb(255, 99, 142, 198));
    dataBarRule.SetNegativeBarColor(Color::FromArgb(255, 255, 0, 0));
    dataBarRule.SetShowBorder(true);
    dataBarRule.SetDirection("left-to-right");

    auto iconSet = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    iconSet.AddArea(CellArea::CreateCellArea("N1", "N10"));
    auto iconSetRule = iconSet[iconSet.AddCondition(FormatConditionType::IconSet)];
    iconSetRule.SetIconSetType("4Arrows");
    iconSetRule.SetReverseIcons(true);
    iconSetRule.SetShowIconOnly(true);

    return WorkbookHandle(std::move(workbook));
}

void AssertAdvancedConditionalFormattings(Workbook& workbook)
{
    auto& sheet = workbook.GetWorksheets()["Advanced Conditional Formatting"];
    ASSERT_EQ(14, sheet.GetConditionalFormattings().GetCount());
    EXPECT_EQ("error", GetCollectionByArea(sheet.GetConditionalFormattings(), "A1:A10")[0].GetFormula1());
    EXPECT_EQ(FormatConditionType::ContainsText, GetCollectionByArea(sheet.GetConditionalFormattings(), "A1:A10")[0].GetType());
    EXPECT_EQ(FormatConditionType::NotContainsText, GetCollectionByArea(sheet.GetConditionalFormattings(), "B1:B10")[0].GetType());
    EXPECT_EQ("warning", GetCollectionByArea(sheet.GetConditionalFormattings(), "B1:B10")[0].GetFormula1());
    EXPECT_EQ(FormatConditionType::BeginsWith, GetCollectionByArea(sheet.GetConditionalFormattings(), "C1:C10")[0].GetType());
    EXPECT_EQ("prefix", GetCollectionByArea(sheet.GetConditionalFormattings(), "C1:C10")[0].GetFormula1());
    EXPECT_EQ(FormatConditionType::EndsWith, GetCollectionByArea(sheet.GetConditionalFormattings(), "D1:D10")[0].GetType());
    EXPECT_EQ("suffix", GetCollectionByArea(sheet.GetConditionalFormattings(), "D1:D10")[0].GetFormula1());
    EXPECT_EQ("today", GetCollectionByArea(sheet.GetConditionalFormattings(), "E1:E10")[0].GetTimePeriod());
    EXPECT_TRUE(GetCollectionByArea(sheet.GetConditionalFormattings(), "F1:F10")[0].GetDuplicate());
    EXPECT_FALSE(GetCollectionByArea(sheet.GetConditionalFormattings(), "G1:G10")[0].GetDuplicate());
    auto topRule = GetCollectionByArea(sheet.GetConditionalFormattings(), "H1:H10")[0];
    EXPECT_TRUE(topRule.GetTop());
    EXPECT_TRUE(topRule.GetPercent());
    EXPECT_EQ(10, topRule.GetRank());
    auto colorScaleRule = GetCollectionByArea(sheet.GetConditionalFormattings(), "L1:L10")[0];
    EXPECT_EQ(3, colorScaleRule.GetColorScaleCount());
    EXPECT_EQ(Color::FromArgb(255, 248, 105, 107), colorScaleRule.GetMinColor());
    EXPECT_EQ(Color::FromArgb(255, 255, 235, 132), colorScaleRule.GetMidColor());
    EXPECT_EQ(Color::FromArgb(255, 99, 190, 123), colorScaleRule.GetMaxColor());
    auto dataBarRule = GetCollectionByArea(sheet.GetConditionalFormattings(), "M1:M10")[0];
    EXPECT_EQ(Color::FromArgb(255, 99, 142, 198), dataBarRule.GetBarColor());
    auto iconSetRule = GetCollectionByArea(sheet.GetConditionalFormattings(), "N1:N10")[0];
    EXPECT_EQ("4Arrows", iconSetRule.GetIconSetType());
    EXPECT_TRUE(iconSetRule.GetReverseIcons());
    EXPECT_TRUE(iconSetRule.GetShowIconOnly());
}

WorkbookHandle CreateDefinedNamesWorkbook()
{
    auto workbook = CreatePageSetupWorkbook();
    workbook.GetWorksheets().Add("Scoped Sheet");
    workbook.GetWorksheets()[1].GetCells()["B2"].PutValue(5);

    auto global = workbook.GetDefinedNames()[workbook.GetDefinedNames().Add("GlobalRange", "='Print Sheet'!$A$1:$C$10")];
    global.SetHidden(true);
    global.SetComment("Primary range");

    auto local = workbook.GetDefinedNames()[workbook.GetDefinedNames().Add("LocalCell", "'Scoped Sheet'!$B$2", 1)];
    local.SetComment("Scoped name");

    return workbook;
}

void AssertDefinedNames(Workbook& workbook)
{
    AssertPageSetup(workbook);
    ASSERT_EQ(2, workbook.GetDefinedNames().GetCount());
    EXPECT_EQ("5", workbook.GetWorksheets()[1].GetCells()["B2"].GetStringValue());
    auto global = workbook.GetDefinedNames()[0];
    EXPECT_EQ("GlobalRange", global.GetName());
    EXPECT_EQ("'Print Sheet'!$A$1:$C$10", global.GetFormula());
    EXPECT_FALSE(global.GetLocalSheetIndex().has_value());
    EXPECT_TRUE(global.GetHidden());
    EXPECT_EQ("Primary range", global.GetComment());
    auto local = workbook.GetDefinedNames()[1];
    EXPECT_EQ("LocalCell", local.GetName());
    EXPECT_EQ("'Scoped Sheet'!$B$2", local.GetFormula());
    EXPECT_EQ(1, local.GetLocalSheetIndex().value_or(-1));
    EXPECT_FALSE(local.GetHidden());
    EXPECT_EQ("Scoped name", local.GetComment());
}

WorkbookHandle CreateAutoFilterWorkbook()
{
    auto workbook = std::make_unique<Workbook>();
    auto& sheet = workbook->GetWorksheets()[0];
    sheet.SetName("Filtered");

    const std::array<const char*, 5> headers = {"Status", "Amount", "Color", "Date", "Score"};
    for (auto column = 0; column < 5; ++column) {
        sheet.GetCells()(0, column).PutValue(headers[static_cast<std::size_t>(column)]);
    }
    sheet.GetCells()(1, 0).PutValue("Open");
    sheet.GetCells()(1, 1).PutValue(10);
    sheet.GetCells()(1, 2).PutValue("Red");
    sheet.GetCells()(1, 3).PutValue(DateTime(2024, 5, 1));
    sheet.GetCells()(1, 4).PutValue(70);
    sheet.GetCells()(2, 0).PutValue("Closed");
    sheet.GetCells()(2, 1).PutValue(20);
    sheet.GetCells()(2, 2).PutValue("Blue");
    sheet.GetCells()(2, 3).PutValue(DateTime(2024, 5, 2));
    sheet.GetCells()(2, 4).PutValue(80);
    sheet.GetCells()(3, 0).PutValue("Open");
    sheet.GetCells()(3, 1).PutValue(30);
    sheet.GetCells()(3, 2).PutValue("Green");
    sheet.GetCells()(3, 3).PutValue(DateTime(2024, 5, 3));
    sheet.GetCells()(3, 4).PutValue(90);
    sheet.GetCells()(4, 0).PutValue("Pending");
    sheet.GetCells()(4, 1).PutValue(40);
    sheet.GetCells()(4, 2).PutValue("Yellow");
    sheet.GetCells()(4, 3).PutValue(DateTime(2024, 5, 4));
    sheet.GetCells()(4, 4).PutValue(60);
    sheet.GetCells()(5, 0).PutValue("Closed");
    sheet.GetCells()(5, 1).PutValue(50);
    sheet.GetCells()(5, 2).PutValue("Black");
    sheet.GetCells()(5, 3).PutValue(DateTime(2024, 5, 5));
    sheet.GetCells()(5, 4).PutValue(50);
    AddDifferentialStyles(sheet);

    sheet.GetAutoFilter().SetRange("A1:E6");
    auto statusColumn = sheet.GetAutoFilter().GetFilterColumns()[sheet.GetAutoFilter().GetFilterColumns().Add(0)];
    statusColumn.SetHiddenButton(true);
    statusColumn.GetFilters().Add("Open");
    statusColumn.GetFilters().Add("Closed");

    auto amountColumn = sheet.GetAutoFilter().GetFilterColumns()[sheet.GetAutoFilter().GetFilterColumns().Add(1)];
    amountColumn.GetCustomFilters().SetMatchAll(true);
    amountColumn.GetCustomFilters().Add(FilterOperatorType::GreaterOrEqual, "10");
    amountColumn.GetCustomFilters().Add(FilterOperatorType::LessOrEqual, "50");

    auto colorColumn = sheet.GetAutoFilter().GetFilterColumns()[sheet.GetAutoFilter().GetFilterColumns().Add(2)];
    colorColumn.GetColorFilter().SetEnabled(true);
    colorColumn.GetColorFilter().SetDifferentialStyleId(3);
    colorColumn.GetColorFilter().SetCellColor(true);

    auto dateColumn = sheet.GetAutoFilter().GetFilterColumns()[sheet.GetAutoFilter().GetFilterColumns().Add(3)];
    dateColumn.GetDynamicFilter().SetEnabled(true);
    dateColumn.GetDynamicFilter().SetType("thisMonth");
    dateColumn.GetDynamicFilter().SetValue(1.0);
    dateColumn.GetDynamicFilter().SetMaxValue(31.0);

    auto scoreColumn = sheet.GetAutoFilter().GetFilterColumns()[sheet.GetAutoFilter().GetFilterColumns().Add(4)];
    scoreColumn.GetTop10().SetEnabled(true);
    scoreColumn.GetTop10().SetTop(false);
    scoreColumn.GetTop10().SetPercent(true);
    scoreColumn.GetTop10().SetValue(10.0);
    scoreColumn.GetTop10().SetFilterValue(2.5);

    sheet.GetAutoFilter().GetSortState().SetRef("A2:E6");
    sheet.GetAutoFilter().GetSortState().SetCaseSensitive(true);
    sheet.GetAutoFilter().GetSortState().SetSortMethod("pinYin");
    auto valueSort = sheet.GetAutoFilter().GetSortState().GetSortConditions()[sheet.GetAutoFilter().GetSortState().GetSortConditions().Add("B2:B6")];
    valueSort.SetDescending(true);
    valueSort.SetSortBy("value");
    valueSort.SetCustomList("High,Medium,Low");
    auto colorSort = sheet.GetAutoFilter().GetSortState().GetSortConditions()[sheet.GetAutoFilter().GetSortState().GetSortConditions().Add("C2:C6")];
    colorSort.SetSortBy("cellColor");
    colorSort.SetDifferentialStyleId(4);
    auto iconSort = sheet.GetAutoFilter().GetSortState().GetSortConditions()[sheet.GetAutoFilter().GetSortState().GetSortConditions().Add("E2:E6")];
    iconSort.SetSortBy("icon");
    iconSort.SetIconSet("3TrafficLights1");
    iconSort.SetIconId(2);

    return WorkbookHandle(std::move(workbook));
}

void AssertAutoFilter(Workbook& workbook)
{
    auto& sheet = workbook.GetWorksheets()[0];
    EXPECT_EQ("A1:E6", sheet.GetAutoFilter().GetRange());
    ASSERT_EQ(5, sheet.GetAutoFilter().GetFilterColumns().GetCount());
    auto statusColumn = sheet.GetAutoFilter().GetFilterColumns()[0];
    EXPECT_EQ(0, statusColumn.GetColumnIndex());
    EXPECT_TRUE(statusColumn.GetHiddenButton());
    ASSERT_EQ(2, statusColumn.GetFilters().GetCount());
    EXPECT_EQ("Open", statusColumn.GetFilters()[0]);
    EXPECT_EQ("Closed", statusColumn.GetFilters()[1]);
    auto amountColumn = sheet.GetAutoFilter().GetFilterColumns()[1];
    EXPECT_TRUE(amountColumn.GetCustomFilters().GetMatchAll());
    ASSERT_EQ(2, amountColumn.GetCustomFilters().GetCount());
    EXPECT_EQ(FilterOperatorType::GreaterOrEqual, amountColumn.GetCustomFilters()[0].GetOperator());
    EXPECT_EQ("10", amountColumn.GetCustomFilters()[0].GetValue());
    EXPECT_EQ(FilterOperatorType::LessOrEqual, amountColumn.GetCustomFilters()[1].GetOperator());
    EXPECT_EQ("50", amountColumn.GetCustomFilters()[1].GetValue());
    auto colorColumn = sheet.GetAutoFilter().GetFilterColumns()[2];
    EXPECT_TRUE(colorColumn.GetColorFilter().GetEnabled());
    EXPECT_EQ(3, colorColumn.GetColorFilter().GetDifferentialStyleId().value_or(-1));
    EXPECT_TRUE(colorColumn.GetColorFilter().GetCellColor());
    auto dateColumn = sheet.GetAutoFilter().GetFilterColumns()[3];
    EXPECT_TRUE(dateColumn.GetDynamicFilter().GetEnabled());
    EXPECT_EQ("thisMonth", dateColumn.GetDynamicFilter().GetType());
    EXPECT_DOUBLE_EQ(1.0, dateColumn.GetDynamicFilter().GetValue().value_or(0.0));
    EXPECT_DOUBLE_EQ(31.0, dateColumn.GetDynamicFilter().GetMaxValue().value_or(0.0));
    auto scoreColumn = sheet.GetAutoFilter().GetFilterColumns()[4];
    EXPECT_TRUE(scoreColumn.GetTop10().GetEnabled());
    EXPECT_FALSE(scoreColumn.GetTop10().GetTop());
    EXPECT_TRUE(scoreColumn.GetTop10().GetPercent());
    EXPECT_DOUBLE_EQ(10.0, scoreColumn.GetTop10().GetValue().value_or(0.0));
    EXPECT_DOUBLE_EQ(2.5, scoreColumn.GetTop10().GetFilterValue().value_or(0.0));
    EXPECT_EQ("A2:E6", sheet.GetAutoFilter().GetSortState().GetRef());
    EXPECT_TRUE(sheet.GetAutoFilter().GetSortState().GetCaseSensitive());
    EXPECT_EQ("pinYin", sheet.GetAutoFilter().GetSortState().GetSortMethod());
    ASSERT_EQ(3, sheet.GetAutoFilter().GetSortState().GetSortConditions().GetCount());
    auto valueSort = sheet.GetAutoFilter().GetSortState().GetSortConditions()[0];
    EXPECT_EQ("B2:B6", valueSort.GetRef());
    EXPECT_TRUE(valueSort.GetDescending());
    EXPECT_EQ("value", valueSort.GetSortBy());
    EXPECT_EQ("High,Medium,Low", valueSort.GetCustomList());
    EXPECT_FALSE(valueSort.GetDifferentialStyleId().has_value());
    EXPECT_EQ("", valueSort.GetIconSet());
    EXPECT_FALSE(valueSort.GetIconId().has_value());
    auto colorSort = sheet.GetAutoFilter().GetSortState().GetSortConditions()[1];
    EXPECT_EQ("C2:C6", colorSort.GetRef());
    EXPECT_FALSE(colorSort.GetDescending());
    EXPECT_EQ("cellColor", colorSort.GetSortBy());
    EXPECT_EQ("", colorSort.GetCustomList());
    EXPECT_EQ(4, colorSort.GetDifferentialStyleId().value_or(-1));
    auto iconSort = sheet.GetAutoFilter().GetSortState().GetSortConditions()[2];
    EXPECT_EQ("E2:E6", iconSort.GetRef());
    EXPECT_EQ("icon", iconSort.GetSortBy());
    EXPECT_EQ("3TrafficLights1", iconSort.GetIconSet());
    EXPECT_EQ(2, iconSort.GetIconId().value_or(-1));
}

WorkbookHandle CreateWorkbookMetadataWorkbook()
{
    auto workbook = std::make_unique<Workbook>();
    workbook->GetSettings().SetDate1904(true);
    workbook->GetWorksheets()[0].SetName("Summary");
    const auto dataIndex = workbook->GetWorksheets().Add("Data");
    const auto archiveIndex = workbook->GetWorksheets().Add("Archive");
    workbook->GetWorksheets()[archiveIndex].SetVisibilityType(VisibilityType::Hidden);
    workbook->GetWorksheets()[dataIndex].GetCells()(0, 0).PutValue("Ready");

    workbook->GetProperties().SetCodeName("WorkbookCode");
    workbook->GetProperties().SetShowObjects("placeholders");
    workbook->GetProperties().SetFilterPrivacy(true);
    workbook->GetProperties().SetShowBorderUnselectedTables(false);
    workbook->GetProperties().SetShowInkAnnotation(false);
    workbook->GetProperties().SetBackupFile(true);
    workbook->GetProperties().SetSaveExternalLinkValues(false);
    workbook->GetProperties().SetUpdateLinks("never");
    workbook->GetProperties().SetHidePivotFieldList(true);
    workbook->GetProperties().SetDefaultThemeVersion(166925);

    auto& protection = workbook->GetProperties().GetProtection();
    protection.SetLockStructure(true);
    protection.SetLockWindows(true);
    protection.SetWorkbookPassword("ABCD");
    protection.SetRevisionsPassword("EFGH");

    auto& view = workbook->GetProperties().GetView();
    view.SetXWindow(120);
    view.SetYWindow(240);
    view.SetWindowWidth(16000);
    view.SetWindowHeight(9000);
    view.SetActiveTab(1);
    view.SetFirstSheet(1);
    view.SetShowHorizontalScroll(false);
    view.SetShowVerticalScroll(false);
    view.SetShowSheetTabs(false);
    view.SetTabRatio(700);
    view.SetVisibility("visible");
    view.SetMinimized(false);
    view.SetAutoFilterDateGrouping(false);

    auto& calculation = workbook->GetProperties().GetCalculation();
    calculation.SetCalculationId(191029);
    calculation.SetCalculationMode("manual");
    calculation.SetFullCalculationOnLoad(true);
    calculation.SetReferenceMode("R1C1");
    calculation.SetIterate(true);
    calculation.SetIterateCount(9);
    calculation.SetIterateDelta(0.00125);
    calculation.SetFullPrecision(false);
    calculation.SetCalculationCompleted(false);
    calculation.SetCalculationOnSave(false);
    calculation.SetConcurrentCalculation(false);
    calculation.SetForceFullCalculation(true);

    workbook->GetDocumentProperties().SetTitle("Quarterly Summary");
    workbook->GetDocumentProperties().SetSubject("Operations");
    workbook->GetDocumentProperties().SetAuthor("Automation");
    workbook->GetDocumentProperties().SetKeywords("finance;ops");
    workbook->GetDocumentProperties().SetComments("Generated during tests.");
    workbook->GetDocumentProperties().SetCategory("Reports");
    workbook->GetDocumentProperties().SetCompany("Aspose Cells FOSS");
    workbook->GetDocumentProperties().SetManager("Release");
    workbook->GetDocumentProperties().GetCore().SetLastModifiedBy("Verifier");
    workbook->GetDocumentProperties().GetCore().SetRevision("7");
    workbook->GetDocumentProperties().GetCore().SetContentStatus("Draft");
    workbook->GetDocumentProperties().GetCore().SetCreated(DateTime(2024, 1, 2, 3, 4, 5));
    workbook->GetDocumentProperties().GetCore().SetModified(DateTime(2024, 6, 7, 8, 9, 10));
    workbook->GetDocumentProperties().GetExtended().SetApplication("Aspose.Cells_FOSS Tests");
    workbook->GetDocumentProperties().GetExtended().SetAppVersion("0.2");
    workbook->GetDocumentProperties().GetExtended().SetDocSecurity(2);
    workbook->GetDocumentProperties().GetExtended().SetHyperlinkBase("https://example.com/base/");
    workbook->GetDocumentProperties().GetExtended().SetScaleCrop(true);
    workbook->GetDocumentProperties().GetExtended().SetLinksUpToDate(true);
    workbook->GetDocumentProperties().GetExtended().SetSharedDoc(true);

    return WorkbookHandle(std::move(workbook));
}

void AssertWorkbookMetadata(Workbook& workbook)
{
    EXPECT_TRUE(workbook.GetSettings().GetDate1904());
    EXPECT_EQ("WorkbookCode", workbook.GetProperties().GetCodeName());
    EXPECT_EQ("placeholders", workbook.GetProperties().GetShowObjects());
    EXPECT_TRUE(workbook.GetProperties().GetFilterPrivacy());
    EXPECT_FALSE(workbook.GetProperties().GetShowBorderUnselectedTables());
    EXPECT_FALSE(workbook.GetProperties().GetShowInkAnnotation());
    EXPECT_TRUE(workbook.GetProperties().GetBackupFile());
    EXPECT_FALSE(workbook.GetProperties().GetSaveExternalLinkValues());
    EXPECT_EQ("never", workbook.GetProperties().GetUpdateLinks());
    EXPECT_TRUE(workbook.GetProperties().GetHidePivotFieldList());
    EXPECT_EQ(166925, workbook.GetProperties().GetDefaultThemeVersion().value_or(0));
    EXPECT_TRUE(workbook.GetProperties().GetProtection().IsProtected());
    EXPECT_TRUE(workbook.GetProperties().GetProtection().GetLockStructure());
    EXPECT_TRUE(workbook.GetProperties().GetProtection().GetLockWindows());
    EXPECT_EQ("ABCD", workbook.GetProperties().GetProtection().GetWorkbookPassword());
    EXPECT_EQ("EFGH", workbook.GetProperties().GetProtection().GetRevisionsPassword());
    EXPECT_EQ(120, workbook.GetProperties().GetView().GetXWindow());
    EXPECT_EQ(240, workbook.GetProperties().GetView().GetYWindow());
    EXPECT_EQ(16000, workbook.GetProperties().GetView().GetWindowWidth());
    EXPECT_EQ(9000, workbook.GetProperties().GetView().GetWindowHeight());
    EXPECT_EQ(1, workbook.GetProperties().GetView().GetActiveTab());
    EXPECT_EQ(1, workbook.GetProperties().GetView().GetFirstSheet());
    EXPECT_FALSE(workbook.GetProperties().GetView().GetShowHorizontalScroll());
    EXPECT_FALSE(workbook.GetProperties().GetView().GetShowVerticalScroll());
    EXPECT_FALSE(workbook.GetProperties().GetView().GetShowSheetTabs());
    EXPECT_EQ(700, workbook.GetProperties().GetView().GetTabRatio());
    EXPECT_EQ("visible", workbook.GetProperties().GetView().GetVisibility());
    EXPECT_FALSE(workbook.GetProperties().GetView().GetMinimized());
    EXPECT_FALSE(workbook.GetProperties().GetView().GetAutoFilterDateGrouping());
    EXPECT_EQ(191029, workbook.GetProperties().GetCalculation().GetCalculationId().value_or(0));
    EXPECT_EQ("manual", workbook.GetProperties().GetCalculation().GetCalculationMode());
    EXPECT_TRUE(workbook.GetProperties().GetCalculation().GetFullCalculationOnLoad());
    EXPECT_EQ("R1C1", workbook.GetProperties().GetCalculation().GetReferenceMode());
    EXPECT_TRUE(workbook.GetProperties().GetCalculation().GetIterate());
    EXPECT_EQ(9, workbook.GetProperties().GetCalculation().GetIterateCount());
    EXPECT_DOUBLE_EQ(0.00125, workbook.GetProperties().GetCalculation().GetIterateDelta());
    EXPECT_FALSE(workbook.GetProperties().GetCalculation().GetFullPrecision());
    EXPECT_FALSE(workbook.GetProperties().GetCalculation().GetCalculationCompleted());
    EXPECT_FALSE(workbook.GetProperties().GetCalculation().GetCalculationOnSave());
    EXPECT_FALSE(workbook.GetProperties().GetCalculation().GetConcurrentCalculation());
    EXPECT_TRUE(workbook.GetProperties().GetCalculation().GetForceFullCalculation());
    EXPECT_EQ("Quarterly Summary", workbook.GetDocumentProperties().GetTitle());
    EXPECT_EQ("Operations", workbook.GetDocumentProperties().GetSubject());
    EXPECT_EQ("Automation", workbook.GetDocumentProperties().GetAuthor());
    EXPECT_EQ("finance;ops", workbook.GetDocumentProperties().GetKeywords());
    EXPECT_EQ("Generated during tests.", workbook.GetDocumentProperties().GetComments());
    EXPECT_EQ("Reports", workbook.GetDocumentProperties().GetCategory());
    EXPECT_EQ("Aspose Cells FOSS", workbook.GetDocumentProperties().GetCompany());
    EXPECT_EQ("Release", workbook.GetDocumentProperties().GetManager());
    EXPECT_EQ("Verifier", workbook.GetDocumentProperties().GetCore().GetLastModifiedBy());
    EXPECT_EQ("7", workbook.GetDocumentProperties().GetCore().GetRevision());
    EXPECT_EQ("Draft", workbook.GetDocumentProperties().GetCore().GetContentStatus());
    EXPECT_EQ(DateTime(2024, 1, 2, 3, 4, 5), workbook.GetDocumentProperties().GetCore().GetCreated().value_or(DateTime()));
    EXPECT_EQ(DateTime(2024, 6, 7, 8, 9, 10), workbook.GetDocumentProperties().GetCore().GetModified().value_or(DateTime()));
    EXPECT_EQ("Aspose.Cells_FOSS Tests", workbook.GetDocumentProperties().GetExtended().GetApplication());
    EXPECT_EQ("0.2", workbook.GetDocumentProperties().GetExtended().GetAppVersion());
    EXPECT_EQ(2, workbook.GetDocumentProperties().GetExtended().GetDocSecurity());
    EXPECT_EQ("https://example.com/base/", workbook.GetDocumentProperties().GetExtended().GetHyperlinkBase());
    EXPECT_TRUE(workbook.GetDocumentProperties().GetExtended().GetScaleCrop());
    EXPECT_TRUE(workbook.GetDocumentProperties().GetExtended().GetLinksUpToDate());
    EXPECT_TRUE(workbook.GetDocumentProperties().GetExtended().GetSharedDoc());
}

void AssertDocumentPropertiesAreDefault(Workbook& workbook)
{
    EXPECT_EQ("", workbook.GetDocumentProperties().GetTitle());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetSubject());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetAuthor());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetKeywords());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetComments());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetCategory());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetCompany());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetManager());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetCore().GetLastModifiedBy());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetCore().GetRevision());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetCore().GetContentStatus());
    EXPECT_FALSE(workbook.GetDocumentProperties().GetCore().GetCreated().has_value());
    EXPECT_FALSE(workbook.GetDocumentProperties().GetCore().GetModified().has_value());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetExtended().GetApplication());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetExtended().GetAppVersion());
    EXPECT_EQ(0, workbook.GetDocumentProperties().GetExtended().GetDocSecurity());
    EXPECT_EQ("", workbook.GetDocumentProperties().GetExtended().GetHyperlinkBase());
    EXPECT_FALSE(workbook.GetDocumentProperties().GetExtended().GetScaleCrop());
    EXPECT_FALSE(workbook.GetDocumentProperties().GetExtended().GetLinksUpToDate());
    EXPECT_FALSE(workbook.GetDocumentProperties().GetExtended().GetSharedDoc());
}

}  // namespace Aspose::Cells_FOSS::Tests
