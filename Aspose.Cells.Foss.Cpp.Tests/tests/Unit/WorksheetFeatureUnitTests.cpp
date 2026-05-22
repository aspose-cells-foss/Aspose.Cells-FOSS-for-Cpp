#include <gtest/gtest.h>

#include "Support/ScenarioFactories.h"
#include "Support/TestSupport.h"

#include "aspose/cells_foss/CellValueType.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/Color.h"
#include "aspose/cells_foss/FilterOperatorType.h"
#include "aspose/cells_foss/FormatCondition.h"
#include "aspose/cells_foss/FormatConditionCollection.h"
#include "aspose/cells_foss/FormatConditionType.h"
#include "aspose/cells_foss/OperatorType.h"
#include "aspose/cells_foss/PageOrientationType.h"
#include "aspose/cells_foss/PaperSizeType.h"
#include "aspose/cells_foss/ValidationAlertType.h"
#include "aspose/cells_foss/ValidationType.h"
#include "aspose/cells_foss/VisibilityType.h"
#include "aspose/cells_foss/Workbook.h"

using namespace Aspose::Cells_FOSS;
using namespace Aspose::Cells_FOSS::Tests;

TEST(WorksheetFeatureUnitTests, worksheet_row_column_and_merge_apis_mutate_expected_settings)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];

    sheet.SetVisibilityType(VisibilityType::Hidden);
    sheet.GetCells().GetRows()[2].SetHeight(19.75);
    sheet.GetCells().GetRows()[4].SetIsHidden(true);
    sheet.GetCells().GetColumns()[1].SetWidth(25.5);
    sheet.GetCells().GetColumns()[3].SetIsHidden(true);
    sheet.GetCells().Merge(1, 1, 2, 3);

    EXPECT_EQ(VisibilityType::Hidden, sheet.GetVisibilityType());
    EXPECT_DOUBLE_EQ(19.75, sheet.GetCells().GetRows()[2].GetHeight().value_or(0.0));
    EXPECT_TRUE(sheet.GetCells().GetRows()[4].GetIsHidden());
    EXPECT_DOUBLE_EQ(25.5, sheet.GetCells().GetColumns()[1].GetWidth().value_or(0.0));
    EXPECT_TRUE(sheet.GetCells().GetColumns()[3].GetIsHidden());
    auto merged = sheet.GetCells().GetMergedCells();
    ASSERT_EQ(1u, merged.size());
    ExpectArea(merged[0], 1, 1, 2, 3);
    EXPECT_THROW(sheet.GetCells().Merge(2, 2, 2, 2), CellsException);
}

TEST(WorksheetFeatureUnitTests, worksheet_view_apis_mutate_expected_settings)
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

    sheet.SetTabColor(Color::Empty());
    EXPECT_EQ(Color::Empty(), sheet.GetTabColor());
    EXPECT_THROW(sheet.SetZoom(9), CellsException);
    EXPECT_THROW(sheet.SetZoom(401), CellsException);
}

TEST(WorksheetFeatureUnitTests, worksheet_protection_apis_mutate_expected_settings)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];

    sheet.Protect();
    sheet.GetProtection().SetObjects(true);
    sheet.GetProtection().SetFormatCells(true);
    sheet.GetProtection().SetInsertRows(true);
    sheet.GetProtection().SetSelectUnlockedCells(true);

    EXPECT_TRUE(sheet.GetProtection().GetIsProtected());
    EXPECT_TRUE(sheet.GetProtection().GetObjects());
    EXPECT_TRUE(sheet.GetProtection().GetFormatCells());
    EXPECT_TRUE(sheet.GetProtection().GetInsertRows());
    EXPECT_TRUE(sheet.GetProtection().GetSelectUnlockedCells());

    sheet.Unprotect();
    EXPECT_FALSE(sheet.GetProtection().GetIsProtected());
    EXPECT_FALSE(sheet.GetProtection().GetObjects());
    EXPECT_FALSE(sheet.GetProtection().GetFormatCells());
    EXPECT_FALSE(sheet.GetProtection().GetInsertRows());
    EXPECT_FALSE(sheet.GetProtection().GetSelectUnlockedCells());

    sheet.GetProtection().SetAutoFilter(true);
    EXPECT_TRUE(sheet.GetProtection().GetIsProtected());
    EXPECT_TRUE(sheet.GetProtection().GetAutoFilter());
}

TEST(WorksheetFeatureUnitTests, autofilter_apis_mutate_expected_settings)
{
    auto workbook = CreateAutoFilterWorkbook();
    AssertAutoFilter(workbook);

    auto& sheet = workbook.GetWorksheets()[0];
    EXPECT_THROW(sheet.GetAutoFilter().GetFilterColumns().Add(-1), CellsException);
    EXPECT_THROW(sheet.GetAutoFilter().GetFilterColumns().Add(0), CellsException);
    EXPECT_THROW(sheet.GetAutoFilter().GetSortState().GetSortConditions().Add("1A"), CellsException);

    sheet.GetAutoFilter().GetFilterColumns().RemoveAt(4);
    EXPECT_EQ(4, sheet.GetAutoFilter().GetFilterColumns().GetCount());
    sheet.GetAutoFilter().Clear();
    EXPECT_EQ("", sheet.GetAutoFilter().GetRange());
    EXPECT_EQ(0, sheet.GetAutoFilter().GetFilterColumns().GetCount());
    EXPECT_EQ(0, sheet.GetAutoFilter().GetSortState().GetSortConditions().GetCount());
}

TEST(WorksheetFeatureUnitTests, defined_name_apis_mutate_expected_settings)
{
    Workbook workbook;
    workbook.GetWorksheets()[0].SetName("Data");
    workbook.GetWorksheets().Add("Scoped");

    auto total = workbook.GetDefinedNames()[workbook.GetDefinedNames().Add("Total", "=SUM(Data!$A$1:$A$2)")];
    total.SetHidden(true);
    total.SetComment("Workbook scope");

    auto scoped = workbook.GetDefinedNames()[workbook.GetDefinedNames().Add("Input", "'Scoped'!$B$2", 1)];
    scoped.SetComment("Local scope");

    EXPECT_EQ(2, workbook.GetDefinedNames().GetCount());
    EXPECT_EQ("Total", total.GetName());
    EXPECT_EQ("SUM(Data!$A$1:$A$2)", total.GetFormula());
    EXPECT_FALSE(total.GetLocalSheetIndex().has_value());
    EXPECT_TRUE(total.GetHidden());
    EXPECT_EQ("Workbook scope", total.GetComment());

    EXPECT_EQ("Input", scoped.GetName());
    EXPECT_EQ("'Scoped'!$B$2", scoped.GetFormula());
    EXPECT_EQ(1, scoped.GetLocalSheetIndex().value_or(-1));
    EXPECT_EQ("Local scope", scoped.GetComment());

    EXPECT_THROW(workbook.GetDefinedNames().Add("Total", "1"), CellsException);
    EXPECT_THROW(workbook.GetDefinedNames().Add("_xlnm.Print_Area", "A1"), CellsException);
    EXPECT_THROW(workbook.GetDefinedNames().Add("Broken", "1", 5), CellsException);

    scoped.SetName("Total");
    EXPECT_THROW(scoped.SetLocalSheetIndex(std::nullopt), CellsException);

    workbook.GetDefinedNames().RemoveAt(1);
    EXPECT_EQ(1, workbook.GetDefinedNames().GetCount());
    EXPECT_THROW(workbook.GetDefinedNames().RemoveAt(5), CellsException);
}

TEST(WorksheetFeatureUnitTests, hyperlink_apis_mutate_expected_settings)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];

    sheet.GetCells()["A1"].PutValue("Docs");
    auto external = sheet.GetHyperlinks()[sheet.GetHyperlinks().Add("A1", 1, 1, "https://example.com/docs")];
    external.SetTextToDisplay("Docs");
    external.SetScreenTip("External docs");

    auto internalLink = sheet.GetHyperlinks()[sheet.GetHyperlinks().Add("B2", 1, 1, "Sheet1!C3")];
    internalLink.SetTextToDisplay("Jump");

    auto rangeLink = sheet.GetHyperlinks()[sheet.GetHyperlinks().Add("C4", 2, 2, "mailto:test@example.com")];
    rangeLink.SetScreenTip("Send mail");

    EXPECT_EQ(3, sheet.GetHyperlinks().GetCount());
    EXPECT_EQ("A1", external.GetArea());
    EXPECT_EQ("https://example.com/docs", external.GetAddress());
    EXPECT_EQ("External docs", external.GetScreenTip());
    EXPECT_EQ("Docs", external.GetTextToDisplay());
    EXPECT_EQ("B2", internalLink.GetArea());
    EXPECT_EQ("Sheet1!C3", internalLink.GetAddress());
    EXPECT_EQ("Jump", internalLink.GetTextToDisplay());
    EXPECT_EQ("C4:D5", rangeLink.GetArea());
    EXPECT_EQ("mailto:test@example.com", rangeLink.GetAddress());
    EXPECT_EQ("Send mail", rangeLink.GetScreenTip());

    EXPECT_THROW(sheet.GetHyperlinks().Add("A1", 1, 1, "https://overlap.example.com"), CellsException);
    EXPECT_THROW(sheet.GetHyperlinks().Add("Z1", 0, 1, "https://invalid.example.com"), CellsException);
    EXPECT_THROW(sheet.GetHyperlinks().Add("A2", 1, 1, ""), CellsException);
    EXPECT_THROW(static_cast<void>(sheet.GetHyperlinks()[-1]), CellsException);

    sheet.GetHyperlinks().RemoveAt(1);
    EXPECT_EQ(2, sheet.GetHyperlinks().GetCount());
    EXPECT_EQ("C4:D5", sheet.GetHyperlinks()[1].GetArea());
    EXPECT_THROW(sheet.GetHyperlinks().RemoveAt(99), CellsException);
}

TEST(WorksheetFeatureUnitTests, validation_apis_mutate_expected_settings)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];

    auto primary = sheet.GetValidations()[sheet.GetValidations().Add(CellArea::CreateCellArea("A1", "B2"))];
    primary.SetType(ValidationType::WholeNumber);
    primary.SetOperator(OperatorType::Between);
    primary.SetFormula1("=1");
    primary.SetFormula2("=10");
    primary.SetShowError(true);
    primary.SetErrorTitle("Whole Number");
    primary.SetErrorMessage("Enter 1-10");
    primary.AddArea(CellArea::CreateCellArea("D4", "D5"));

    EXPECT_EQ(1, sheet.GetValidations().GetCount());
    ASSERT_EQ(2u, primary.GetAreas().size());
    EXPECT_EQ("1", primary.GetFormula1());
    EXPECT_EQ("10", primary.GetFormula2());
    ASSERT_TRUE(sheet.GetValidations().GetValidationInCell(0, 0).has_value());
    EXPECT_EQ(ValidationType::WholeNumber, sheet.GetValidations().GetValidationInCell(0, 0)->GetType());
    ASSERT_TRUE(sheet.GetValidations().GetValidationInCell(4, 3).has_value());
    EXPECT_EQ(ValidationType::WholeNumber, sheet.GetValidations().GetValidationInCell(4, 3)->GetType());
    EXPECT_THROW(sheet.GetValidations().Add(CellArea::CreateCellArea("B2", "C3")), CellsException);

    auto second = sheet.GetValidations()[sheet.GetValidations().Add(CellArea::CreateCellArea("F1", "F1"))];
    second.SetType(ValidationType::List);
    second.SetFormula1("\"Y,N\"");
    EXPECT_EQ(2, sheet.GetValidations().GetCount());

    sheet.GetValidations().RemoveACell(0, 0);
    EXPECT_FALSE(sheet.GetValidations().GetValidationInCell(0, 0).has_value());
    EXPECT_TRUE(sheet.GetValidations().GetValidationInCell(0, 1).has_value());

    sheet.GetValidations().RemoveArea(CellArea::CreateCellArea("F1", "F1"));
    EXPECT_EQ(1, sheet.GetValidations().GetCount());
    EXPECT_THROW(sheet.GetValidations().RemoveACell(-1, 0), CellsException);
    EXPECT_THROW(static_cast<void>(sheet.GetValidations()[-1]), CellsException);
}

TEST(WorksheetFeatureUnitTests, conditional_formatting_apis_mutate_expected_settings)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];

    auto collection = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    collection.AddArea(CellArea::CreateCellArea("A1", "A3"));
    auto condition = collection[collection.AddCondition(FormatConditionType::CellValue, OperatorType::Between, "=1", "=9")];
    condition.SetStopIfTrue(true);
    condition.SetPriority(1);
    auto style = condition.GetStyle();
    style.SetPattern(FillPattern::Solid);
    style.SetForegroundColor(Color::FromArgb(255, 255, 0, 0));
    condition.SetStyle(style);

    EXPECT_EQ(1, sheet.GetConditionalFormattings().GetCount());
    EXPECT_EQ(1, collection.GetRangeCount());
    EXPECT_EQ(1, collection.GetCount());
    EXPECT_EQ("1", condition.GetFormula1());
    EXPECT_EQ("9", condition.GetFormula2());
    EXPECT_TRUE(condition.GetStopIfTrue());
    EXPECT_EQ(FillPattern::Solid, condition.GetStyle().GetPattern());

    collection.AddCondition(FormatConditionType::Expression);
    EXPECT_EQ(2, collection.GetCount());
    collection.RemoveCondition(1);
    EXPECT_EQ(1, collection.GetCount());

    collection.AddArea(CellArea::CreateCellArea("C1", "C2"));
    EXPECT_EQ(2, collection.GetRangeCount());
    collection.RemoveArea(0, 0, 1, 1);
    EXPECT_EQ(2, collection.GetRangeCount());
    EXPECT_EQ(0, collection.GetCellArea(0).GetFirstRow());
    EXPECT_EQ(2, collection.GetCellArea(0).GetFirstColumn());

    sheet.GetConditionalFormattings().RemoveArea(0, 2, 2, 1);
    EXPECT_EQ(1, sheet.GetConditionalFormattings().GetCount());
    sheet.GetConditionalFormattings().RemoveAt(0);
    EXPECT_EQ(0, sheet.GetConditionalFormattings().GetCount());
    EXPECT_THROW(static_cast<void>(sheet.GetConditionalFormattings()[-1]), CellsException);
}

TEST(WorksheetFeatureUnitTests, conditional_formatting_advanced_apis_mutate_expected_settings)
{
    Workbook workbook;
    auto& sheet = workbook.GetWorksheets()[0];

    auto contains = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    contains.AddArea(CellArea::CreateCellArea("A1", "A10"));
    auto containsRule = contains[contains.AddCondition(FormatConditionType::ContainsText)];
    containsRule.SetFormula1("error");
    containsRule.SetPriority(2);

    auto timePeriod = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    timePeriod.AddArea(CellArea::CreateCellArea("B1", "B10"));
    auto timeRule = timePeriod[timePeriod.AddCondition(FormatConditionType::TimePeriod)];
    timeRule.SetTimePeriod("today");

    auto top10 = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    top10.AddArea(CellArea::CreateCellArea("C1", "C10"));
    auto topRule = top10[top10.AddCondition(FormatConditionType::Top10)];
    topRule.SetPercent(true);
    topRule.SetRank(10);

    auto colorScale = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    colorScale.AddArea(CellArea::CreateCellArea("D1", "D10"));
    auto colorRule = colorScale[colorScale.AddCondition(FormatConditionType::ColorScale)];
    colorRule.SetColorScaleCount(3);
    colorRule.SetMinColor(Color::FromArgb(255, 248, 105, 107));
    colorRule.SetMidColor(Color::FromArgb(255, 255, 235, 132));
    colorRule.SetMaxColor(Color::FromArgb(255, 99, 190, 123));

    auto dataBar = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    dataBar.AddArea(CellArea::CreateCellArea("E1", "E10"));
    auto dataBarRule = dataBar[dataBar.AddCondition(FormatConditionType::DataBar)];
    dataBarRule.SetBarColor(Color::FromArgb(255, 99, 142, 198));
    dataBarRule.SetShowBorder(true);
    dataBarRule.SetDirection("left-to-right");

    auto iconSet = sheet.GetConditionalFormattings()[sheet.GetConditionalFormattings().Add()];
    iconSet.AddArea(CellArea::CreateCellArea("F1", "F10"));
    auto iconSetRule = iconSet[iconSet.AddCondition(FormatConditionType::IconSet)];
    iconSetRule.SetIconSetType("4Arrows");
    iconSetRule.SetReverseIcons(true);
    iconSetRule.SetShowIconOnly(true);

    EXPECT_EQ(6, sheet.GetConditionalFormattings().GetCount());
    EXPECT_EQ(FormatConditionType::ContainsText, containsRule.GetType());
    EXPECT_EQ("error", containsRule.GetFormula1());
    EXPECT_EQ(2, containsRule.GetPriority());
    EXPECT_EQ("today", timeRule.GetTimePeriod());
    EXPECT_TRUE(topRule.GetPercent());
    EXPECT_EQ(10, topRule.GetRank());
    EXPECT_EQ(3, colorRule.GetColorScaleCount());
    EXPECT_EQ(Color::FromArgb(255, 248, 105, 107), colorRule.GetMinColor());
    EXPECT_EQ(Color::FromArgb(255, 99, 142, 198), dataBarRule.GetBarColor());
    EXPECT_TRUE(dataBarRule.GetShowBorder());
    EXPECT_EQ("4Arrows", iconSetRule.GetIconSetType());
    EXPECT_TRUE(iconSetRule.GetReverseIcons());
    EXPECT_TRUE(iconSetRule.GetShowIconOnly());
}

TEST(WorksheetFeatureUnitTests, page_setup_apis_mutate_expected_settings)
{
    Workbook workbook;
    auto& pageSetup = workbook.GetWorksheets()[0].GetPageSetup();

    pageSetup.SetLeftMargin(0.508);
    pageSetup.SetRightMargin(0.635);
    pageSetup.SetOrientation(PageOrientationType::Landscape);
    pageSetup.SetPaperSize(PaperSizeType::PaperA4);
    pageSetup.SetFirstPageNumber(2);
    pageSetup.SetScale(90);
    pageSetup.SetFitToPagesWide(1);
    pageSetup.SetFitToPagesTall(3);
    pageSetup.SetPrintArea("$A$1:$D$20");
    pageSetup.SetPrintTitleRows("$1:$2");
    pageSetup.SetPrintTitleColumns("$A:$B");
    pageSetup.SetLeftHeader("LH");
    pageSetup.SetCenterFooter("CF");
    pageSetup.SetPrintGridlines(true);
    pageSetup.SetCenterHorizontally(true);
    pageSetup.AddHorizontalPageBreak(5);
    pageSetup.AddVerticalPageBreak(2);

    EXPECT_DOUBLE_EQ(0.508, pageSetup.GetLeftMargin());
    EXPECT_DOUBLE_EQ(0.635, pageSetup.GetRightMargin());
    EXPECT_EQ(PageOrientationType::Landscape, pageSetup.GetOrientation());
    EXPECT_EQ(PaperSizeType::PaperA4, pageSetup.GetPaperSize());
    EXPECT_EQ(2, pageSetup.GetFirstPageNumber().value_or(0));
    EXPECT_EQ(90, pageSetup.GetScale().value_or(0));
    EXPECT_EQ(1, pageSetup.GetFitToPagesWide().value_or(0));
    EXPECT_EQ(3, pageSetup.GetFitToPagesTall().value_or(0));
    EXPECT_EQ("$A$1:$D$20", pageSetup.GetPrintArea());
    EXPECT_EQ("$1:$2", pageSetup.GetPrintTitleRows());
    EXPECT_EQ("$A:$B", pageSetup.GetPrintTitleColumns());
    EXPECT_EQ("LH", pageSetup.GetLeftHeader());
    EXPECT_EQ("CF", pageSetup.GetCenterFooter());
    EXPECT_TRUE(pageSetup.GetPrintGridlines());
    EXPECT_TRUE(pageSetup.GetCenterHorizontally());
    ASSERT_EQ(1u, pageSetup.GetHorizontalPageBreaks().size());
    EXPECT_EQ(5, pageSetup.GetHorizontalPageBreaks()[0]);
    ASSERT_EQ(1u, pageSetup.GetVerticalPageBreaks().size());
    EXPECT_EQ(2, pageSetup.GetVerticalPageBreaks()[0]);
    EXPECT_THROW(pageSetup.SetScale(5), CellsException);
    EXPECT_THROW(pageSetup.SetLeftMargin(-0.1), CellsException);
}
