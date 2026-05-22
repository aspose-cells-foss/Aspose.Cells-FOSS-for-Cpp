#include <gtest/gtest.h>

#include "Support/ScenarioFactories.h"
#include "Support/TestSupport.h"

#include "aspose/cells_foss/BorderStyleType.h"
#include "aspose/cells_foss/CellValueType.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/Color.h"
#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/FillPattern.h"
#include "aspose/cells_foss/HorizontalAlignmentType.h"
#include "aspose/cells_foss/Style.h"
#include "aspose/cells_foss/VerticalAlignmentType.h"
#include "aspose/cells_foss/WorkbookSettings.h"
#include "aspose/cells_foss/WorksheetCollection.h"
#include "aspose/cells_foss/core/CellAddress.h"

#include <cmath>

using namespace Aspose::Cells_FOSS;
using namespace Aspose::Cells_FOSS::Tests;

TEST(CellDataUnitTests, a1_indexers_roundtrip)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];
    sheet.GetCells()(2, 27).PutValue("AB3");
    sheet.GetCells()(0, 0).PutValue(42);

    EXPECT_EQ("AB3", sheet.GetCells()["AB3"].GetStringValue());
    EXPECT_EQ("42", sheet.GetCells()(0, 0).GetStringValue());
    EXPECT_EQ(CellValueType::String, sheet.GetCells()["AB3"].GetType());
    EXPECT_EQ(CellValueType::Number, sheet.GetCells()["A1"].GetType());
}

TEST(CellDataUnitTests, put_value_overloads_assign_expected_types)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];
    const auto timestamp = DateTime(2024, 5, 6, 7, 8, 9);

    sheet.GetCells()["A1"].PutValue("alpha");
    sheet.GetCells()["B1"].PutValue(123);
    sheet.GetCells()["C1"].PutValue(12.5);
    sheet.GetCells()["D1"].PutValue(6.02214076E+23);
    sheet.GetCells()["E1"].PutValue(true);
    sheet.GetCells()["F1"].PutValue(timestamp);

    EXPECT_EQ(CellValueType::String, sheet.GetCells()["A1"].GetType());
    EXPECT_EQ(CellValueType::Number, sheet.GetCells()["B1"].GetType());
    EXPECT_EQ(CellValueType::Number, sheet.GetCells()["C1"].GetType());
    EXPECT_EQ(CellValueType::Number, sheet.GetCells()["D1"].GetType());
    EXPECT_EQ(CellValueType::Boolean, sheet.GetCells()["E1"].GetType());
    EXPECT_EQ(CellValueType::DateTime, sheet.GetCells()["F1"].GetType());

    EXPECT_EQ("alpha", sheet.GetCells()["A1"].GetValue().AsString());
    EXPECT_EQ(123, sheet.GetCells()["B1"].GetValue().AsInteger());
    EXPECT_DOUBLE_EQ(12.5, sheet.GetCells()["C1"].GetValue().AsDouble());
    EXPECT_LT(std::abs(sheet.GetCells()["D1"].GetValue().AsDouble() - 6.02214076E+23), 1E+10);
    EXPECT_TRUE(sheet.GetCells()["E1"].GetValue().AsBool());
    EXPECT_EQ(timestamp, sheet.GetCells()["F1"].GetValue().AsDateTime());
}

TEST(CellDataUnitTests, stringvalue_formats_supported_scalar_types)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];
    const auto timestamp = DateTime(2024, 5, 6, 7, 8, 9);

    sheet.GetCells()["A1"].PutValue(true);
    sheet.GetCells()["B1"].PutValue(123);
    sheet.GetCells()["C1"].PutValue(12.5);
    sheet.GetCells()["D1"].PutValue(timestamp);

    EXPECT_EQ("TRUE", sheet.GetCells()["A1"].GetStringValue());
    EXPECT_EQ("TRUE", sheet.GetCells()["A1"].GetDisplayStringValue());
    EXPECT_EQ("123", sheet.GetCells()["B1"].GetStringValue());
    EXPECT_EQ("123", sheet.GetCells()["B1"].GetDisplayStringValue());
    EXPECT_EQ("12.5", sheet.GetCells()["C1"].GetStringValue());
    EXPECT_EQ("12.5", sheet.GetCells()["C1"].GetDisplayStringValue());
    EXPECT_EQ("5/6/2024 7:08", sheet.GetCells()["D1"].GetStringValue());
    EXPECT_EQ("5/6/2024 7:08", sheet.GetCells()["D1"].GetDisplayStringValue());
}

TEST(CellDataUnitTests, displaystringvalue_applies_numeric_formats_and_stringvalue_stays_raw)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];

    auto grouped = sheet.GetCells()["A1"];
    grouped.PutValue(1234.567);
    auto groupedStyle = grouped.GetStyle();
    groupedStyle.SetNumberFormat("#,##0.00");
    grouped.SetStyle(groupedStyle);

    auto percent = sheet.GetCells()["B1"];
    percent.PutValue(0.125);
    auto percentStyle = percent.GetStyle();
    percentStyle.SetNumberFormat("0.00%");
    percent.SetStyle(percentStyle);

    auto scientific = sheet.GetCells()["C1"];
    scientific.PutValue(1234.0);
    auto scientificStyle = scientific.GetStyle();
    scientificStyle.SetNumberFormat("0.00E+00");
    scientific.SetStyle(scientificStyle);

    auto fraction = sheet.GetCells()["D1"];
    fraction.PutValue(1.25);
    auto fractionStyle = fraction.GetStyle();
    fractionStyle.SetNumberFormat("# ?/?");
    fraction.SetStyle(fractionStyle);

    auto negative = sheet.GetCells()["E1"];
    negative.PutValue(-12.3);
    auto negativeStyle = negative.GetStyle();
    negativeStyle.SetNumberFormat("#,##0.00_);(#,##0.00)");
    negative.SetStyle(negativeStyle);

    auto color = sheet.GetCells()["F1"];
    color.PutValue(1.25);
    auto colorStyle = color.GetStyle();
    colorStyle.SetNumberFormat("[Blue]0.000");
    color.SetStyle(colorStyle);

    auto conditionalHigh = sheet.GetCells()["G1"];
    conditionalHigh.PutValue(125.0);
    auto conditionalStyle = conditionalHigh.GetStyle();
    conditionalStyle.SetNumberFormat("[>100]0.0;\"small\"");
    conditionalHigh.SetStyle(conditionalStyle);

    auto conditionalLow = sheet.GetCells()["H1"];
    conditionalLow.PutValue(10.0);
    conditionalLow.SetStyle(conditionalStyle);

    EXPECT_EQ("1234.567", grouped.GetStringValue());
    EXPECT_EQ("1,234.57", grouped.GetDisplayStringValue());
    EXPECT_EQ("0.125", percent.GetStringValue());
    EXPECT_EQ("12.50%", percent.GetDisplayStringValue());
    EXPECT_EQ("1234", scientific.GetStringValue());
    EXPECT_EQ("1.23E+03", scientific.GetDisplayStringValue());
    EXPECT_EQ("1.25", fraction.GetStringValue());
    EXPECT_EQ("1 1/4", fraction.GetDisplayStringValue());
    EXPECT_EQ("-12.3", negative.GetStringValue());
    EXPECT_EQ("(12.30)", negative.GetDisplayStringValue());
    EXPECT_EQ("1.25", color.GetStringValue());
    EXPECT_EQ("1.250", color.GetDisplayStringValue());
    EXPECT_EQ("125", conditionalHigh.GetStringValue());
    EXPECT_EQ("125.0", conditionalHigh.GetDisplayStringValue());
    EXPECT_EQ("10", conditionalLow.GetStringValue());
    EXPECT_EQ("small", conditionalLow.GetDisplayStringValue());
}

TEST(CellDataUnitTests, displaystringvalue_applies_date_and_text_formats)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];
    const auto timestamp = DateTime(2024, 5, 6, 7, 8, 9);

    auto builtInDate = sheet.GetCells()["A2"];
    builtInDate.PutValue(timestamp);
    auto builtInDateStyle = builtInDate.GetStyle();
    builtInDateStyle.SetNumber(14);
    builtInDate.SetStyle(builtInDateStyle);

    auto customDate = sheet.GetCells()["B2"];
    customDate.PutValue(timestamp);
    auto customDateStyle = customDate.GetStyle();
    customDateStyle.SetNumberFormat("d-mmm-yy h:mm AM/PM");
    customDate.SetStyle(customDateStyle);

    auto elapsed = sheet.GetCells()["C2"];
    elapsed.PutValue(timestamp);
    auto elapsedStyle = elapsed.GetStyle();
    elapsedStyle.SetNumber(46);
    elapsed.SetStyle(elapsedStyle);

    auto textCell = sheet.GetCells()["D2"];
    textCell.PutValue("ABC");
    auto textStyle = textCell.GetStyle();
    textStyle.SetNumberFormat("0;0;0;\"Item \"@");
    textCell.SetStyle(textStyle);

    EXPECT_EQ("5/6/2024 7:08", builtInDate.GetStringValue());
    EXPECT_EQ("05-06-24", builtInDate.GetDisplayStringValue());
    EXPECT_EQ("5/6/2024 7:08", customDate.GetStringValue());
    EXPECT_EQ("6-May-24 7:08 AM", customDate.GetDisplayStringValue());
    EXPECT_EQ("5/6/2024 7:08", elapsed.GetStringValue());
    EXPECT_EQ("7:08:09", elapsed.GetDisplayStringValue());
    EXPECT_EQ("ABC", textCell.GetStringValue());
    EXPECT_EQ("Item ABC", textCell.GetDisplayStringValue());
}

TEST(CellDataUnitTests, displaystringvalue_uses_workbook_culture_and_locale_directives)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];
    workbook.GetSettings().SetCulture(MakeLocaleStrict({"fr-FR", "fr_FR.UTF-8", "fr_FR"}));

    auto numeric = sheet.GetCells()["A3"];
    numeric.PutValue(1234.5);
    auto numericStyle = numeric.GetStyle();
    numericStyle.SetNumberFormat("#,##0.00");
    numeric.SetStyle(numericStyle);

    auto dateCell = sheet.GetCells()["B3"];
    const auto timestamp = DateTime(2024, 5, 6, 7, 8, 9);
    dateCell.PutValue(timestamp);
    auto dateStyle = dateCell.GetStyle();
    dateStyle.SetNumberFormat("dddd, mmmm d, yyyy");
    dateCell.SetStyle(dateStyle);

    auto englishDate = sheet.GetCells()["C3"];
    englishDate.PutValue(timestamp);
    auto englishDateStyle = englishDate.GetStyle();
    englishDateStyle.SetNumberFormat("[$-409]dddd, mmmm d, yyyy");
    englishDate.SetStyle(englishDateStyle);

    auto yenCell = sheet.GetCells()["D3"];
    yenCell.PutValue(1234.5);
    auto yenStyle = yenCell.GetStyle();
    yenStyle.SetNumberFormat("[$\xC2\xA5-411]#,##0.00");
    yenCell.SetStyle(yenStyle);

    auto longDateCell = sheet.GetCells()["E3"];
    longDateCell.PutValue(timestamp);
    auto longDateStyle = longDateCell.GetStyle();
    longDateStyle.SetNumberFormat("[$-F800]");
    longDateCell.SetStyle(longDateStyle);

    EXPECT_EQ("1 234,50", numeric.GetDisplayStringValue());
    EXPECT_EQ("lundi, mai 6, 2024", dateCell.GetDisplayStringValue());
    EXPECT_EQ("Monday, May 6, 2024", englishDate.GetDisplayStringValue());
    EXPECT_EQ("\xC2\xA5" "1,234.50", yenCell.GetDisplayStringValue());

    workbook.GetSettings().SetCulture(MakeLocaleStrict({"de-DE", "de_DE.UTF-8", "de_DE"}));
    EXPECT_EQ("Montag, 6. Mai 2024", longDateCell.GetDisplayStringValue());
}

TEST(CellDataUnitTests, displaystringvalue_applies_extended_date_tokens)
{
    Workbook workbook;
    workbook.GetSettings().SetCulture(MakeLocaleStrict({"en-US", "en_US.UTF-8", "en_US", "C"}));
    auto& sheet = workbook.GetWorksheets()[0];
    const auto timestamp = DateTime(2024, 5, 6, 7, 8, 9, 345);

    auto monthInitial = sheet.GetCells()["A4"];
    monthInitial.PutValue(timestamp);
    auto monthInitialStyle = monthInitial.GetStyle();
    monthInitialStyle.SetNumberFormat("mmmmm d, yyyy");
    monthInitial.SetStyle(monthInitialStyle);

    auto abbreviatedDate = sheet.GetCells()["B4"];
    abbreviatedDate.PutValue(timestamp);
    auto abbreviatedStyle = abbreviatedDate.GetStyle();
    abbreviatedStyle.SetNumberFormat("ddd, mmm d yyyy");
    abbreviatedDate.SetStyle(abbreviatedStyle);

    auto fractionalSeconds = sheet.GetCells()["C4"];
    fractionalSeconds.PutValue(timestamp);
    auto fractionalStyle = fractionalSeconds.GetStyle();
    fractionalStyle.SetNumberFormat("h:mm:ss.000 AM/PM");
    fractionalSeconds.SetStyle(fractionalStyle);

    auto shortFraction = sheet.GetCells()["D4"];
    shortFraction.PutValue(timestamp);
    auto shortFractionStyle = shortFraction.GetStyle();
    shortFractionStyle.SetNumberFormat("hh:mm:ss.00");
    shortFraction.SetStyle(shortFractionStyle);

    auto shortAmPm = sheet.GetCells()["E4"];
    shortAmPm.PutValue(timestamp);
    auto shortAmPmStyle = shortAmPm.GetStyle();
    shortAmPmStyle.SetNumberFormat("h A/P");
    shortAmPm.SetStyle(shortAmPmStyle);

    EXPECT_EQ("M 6, 2024", monthInitial.GetDisplayStringValue());
    EXPECT_EQ("Mon, May 6 2024", abbreviatedDate.GetDisplayStringValue());
    EXPECT_EQ("7:08:09.345 AM", fractionalSeconds.GetDisplayStringValue());
    EXPECT_EQ("07:08:09.34", shortFraction.GetDisplayStringValue());
    EXPECT_EQ("7 A", shortAmPm.GetDisplayStringValue());
}

TEST(CellDataUnitTests, displaystringvalue_applies_long_time_and_elapsed_fraction_with_culture)
{
    Workbook workbook;
    workbook.GetSettings().SetCulture(MakeLocaleStrict({"de-DE", "de_DE.UTF-8", "de_DE"}));
    auto& sheet = workbook.GetWorksheets()[0];
    const auto timestamp = DateTime(2024, 5, 6, 7, 8, 9, 345);

    auto longTimeCell = sheet.GetCells()["F4"];
    longTimeCell.PutValue(timestamp);
    auto longTimeStyle = longTimeCell.GetStyle();
    longTimeStyle.SetNumberFormat("[$-F400]");
    longTimeCell.SetStyle(longTimeStyle);

    auto elapsedCell = sheet.GetCells()["G4"];
    elapsedCell.PutValue(timestamp);
    auto elapsedStyle = elapsedCell.GetStyle();
    elapsedStyle.SetNumberFormat("[h]:mm:ss.000");
    elapsedCell.SetStyle(elapsedStyle);

    EXPECT_EQ("07:08:09", longTimeCell.GetDisplayStringValue());
    EXPECT_EQ("7:08:09,345", elapsedCell.GetDisplayStringValue());
}

TEST(CellDataUnitTests, formula_property_normalizes_and_preserves_cached_value)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];
    auto cell = sheet.GetCells()["C3"];

    cell.PutValue(20);
    cell.SetFormula("A1+B1");

    EXPECT_EQ(CellValueType::Formula, cell.GetType());
    EXPECT_EQ("=A1+B1", cell.GetFormula());
    EXPECT_EQ("20", cell.GetStringValue());
    EXPECT_EQ(20, cell.GetValue().AsInteger());
}

TEST(CellDataUnitTests, blank_cells_are_blank_by_default)
{
    Workbook workbook;
    auto cell = workbook.GetWorksheets()[0].GetCells()["Z99"];

    EXPECT_EQ(CellValueType::Blank, cell.GetType());
    EXPECT_TRUE(cell.GetValue().IsEmpty());
    EXPECT_EQ("", cell.GetStringValue());
}

TEST(CellDataUnitTests, worksheet_name_and_collection_guards)
{
    Workbook workbook;
    workbook.GetWorksheets().Add("Data");

    EXPECT_THROW(workbook.GetWorksheets().Add("data"), CellsException);
    EXPECT_THROW(static_cast<void>(workbook.GetWorksheets()[0].GetCells()["1A"]), CellsException);
    EXPECT_THROW(static_cast<void>(workbook.GetWorksheets()[0].GetCells()(-1, 0)), CellsException);

    auto parsed = Core::CellAddress::Parse("AB3");
    EXPECT_EQ(2, parsed.GetRowIndex());
    EXPECT_EQ(27, parsed.GetColumnIndex());
    EXPECT_EQ("AB3", parsed.ToString());
}

TEST(CellDataUnitTests, style_mutation_requires_setstyle_and_returns_clones)
{
    Workbook workbook;
    auto cell = workbook.GetWorksheets()[0].GetCells()["A1"];

    auto style = cell.GetStyle();
    auto font = style.GetFont();
    font.SetBold(true);
    style.SetFont(font);
    style.SetHorizontalAlignment(HorizontalAlignmentType::Right);

    auto untouched = cell.GetStyle();
    EXPECT_FALSE(untouched.GetFont().GetBold());
    EXPECT_EQ(HorizontalAlignmentType::General, untouched.GetHorizontalAlignment());

    cell.SetStyle(style);
    auto applied = cell.GetStyle();
    EXPECT_TRUE(applied.GetFont().GetBold());
    EXPECT_EQ(HorizontalAlignmentType::Right, applied.GetHorizontalAlignment());

    auto appliedFont = applied.GetFont();
    appliedFont.SetItalic(true);
    applied.SetFont(appliedFont);
    EXPECT_FALSE(cell.GetStyle().GetFont().GetItalic());
}

TEST(CellDataUnitTests, style_api_covers_all_public_settings)
{
    Workbook workbook;
    auto primaryCell = workbook.GetWorksheets()[0].GetCells()["A1"];
    primaryCell.PutValue(1);

    auto primaryStyle = primaryCell.GetStyle();
    ApplyPrimaryStyle(primaryStyle);

    auto untouched = primaryCell.GetStyle();
    EXPECT_EQ("Calibri", untouched.GetFont().GetName());
    EXPECT_EQ(FillPattern::None, untouched.GetPattern());
    EXPECT_TRUE(untouched.GetIsLocked());
    EXPECT_FALSE(untouched.GetIsHidden());
    EXPECT_EQ(0, untouched.GetIndentLevel());
    EXPECT_EQ(0, untouched.GetTextRotation());
    EXPECT_EQ(0, untouched.GetReadingOrder());
    EXPECT_FALSE(untouched.GetShrinkToFit());
    EXPECT_FALSE(untouched.GetFont().GetStrikeThrough());
    EXPECT_FALSE(untouched.GetBorders().GetDiagonalUp());
    EXPECT_FALSE(untouched.GetBorders().GetDiagonalDown());
    EXPECT_EQ("General", untouched.GetNumberFormat());

    primaryCell.SetStyle(primaryStyle);
    AssertPrimaryStyle(primaryCell.GetStyle());

    auto mutatedClone = primaryCell.GetStyle();
    auto mutatedFont = mutatedClone.GetFont();
    mutatedFont.SetName("Mutated");
    mutatedFont.SetStrikeThrough(false);
    mutatedClone.SetFont(mutatedFont);
    mutatedClone.SetPattern(FillPattern::None);
    mutatedClone.SetForegroundColor(Color::Empty());
    mutatedClone.SetBackgroundColor(Color::Empty());
    mutatedClone.SetHorizontalAlignment(HorizontalAlignmentType::Left);
    mutatedClone.SetVerticalAlignment(VerticalAlignmentType::Top);
    mutatedClone.SetWrapText(false);
    mutatedClone.SetIndentLevel(0);
    mutatedClone.SetTextRotation(0);
    mutatedClone.SetShrinkToFit(false);
    mutatedClone.SetReadingOrder(0);
    mutatedClone.SetRelativeIndent(0);
    mutatedClone.SetIsLocked(true);
    mutatedClone.SetIsHidden(false);

    auto persistedPrimary = primaryCell.GetStyle();
    EXPECT_EQ("Arial", persistedPrimary.GetFont().GetName());
    EXPECT_TRUE(persistedPrimary.GetFont().GetStrikeThrough());
    EXPECT_EQ(FillPattern::LightGrid, persistedPrimary.GetPattern());
    EXPECT_EQ(BorderStyleType::MediumDashDot, persistedPrimary.GetBorders().GetRight().GetLineStyle());
    EXPECT_EQ(BorderStyleType::SlantedDashDot, persistedPrimary.GetBorders().GetDiagonal().GetLineStyle());
    EXPECT_TRUE(persistedPrimary.GetBorders().GetDiagonalUp());
    EXPECT_TRUE(persistedPrimary.GetBorders().GetDiagonalDown());
    EXPECT_EQ(4, persistedPrimary.GetNumber());
    EXPECT_EQ("#,##0.00", persistedPrimary.GetNumberFormat());
    EXPECT_EQ(HorizontalAlignmentType::Distributed, persistedPrimary.GetHorizontalAlignment());
    EXPECT_EQ(VerticalAlignmentType::Distributed, persistedPrimary.GetVerticalAlignment());
    EXPECT_TRUE(persistedPrimary.GetWrapText());
    EXPECT_EQ(2, persistedPrimary.GetIndentLevel());
    EXPECT_EQ(45, persistedPrimary.GetTextRotation());
    EXPECT_TRUE(persistedPrimary.GetShrinkToFit());
    EXPECT_EQ(2, persistedPrimary.GetReadingOrder());
    EXPECT_EQ(1, persistedPrimary.GetRelativeIndent());
    EXPECT_FALSE(persistedPrimary.GetIsLocked());
    EXPECT_TRUE(persistedPrimary.GetIsHidden());

    auto customCell = workbook.GetWorksheets()[0].GetCells()["B2"];
    auto customStyle = customCell.GetStyle();
    ApplyCustomNumberStyle(customStyle);
    customCell.SetStyle(customStyle);

    AssertCustomNumberStyle(customCell.GetStyle());
    EXPECT_EQ(CellValueType::Blank, customCell.GetType());

    Style numberFormatStyle;
    numberFormatStyle.SetNumberFormat("0.00%");
    EXPECT_EQ(10, numberFormatStyle.GetNumber());
    EXPECT_EQ("", numberFormatStyle.GetCustom());
    numberFormatStyle.SetNumberFormat("[Blue]0.000");
    EXPECT_EQ(0, numberFormatStyle.GetNumber());
    EXPECT_EQ("[Blue]0.000", numberFormatStyle.GetCustom());

    EXPECT_THROW(numberFormatStyle.SetIndentLevel(-1), CellsException);
    EXPECT_THROW(numberFormatStyle.SetTextRotation(181), CellsException);
    EXPECT_THROW(numberFormatStyle.SetReadingOrder(3), CellsException);
}
