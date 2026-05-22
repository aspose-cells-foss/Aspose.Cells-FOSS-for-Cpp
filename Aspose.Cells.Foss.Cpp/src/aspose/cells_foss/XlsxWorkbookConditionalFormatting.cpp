#include "aspose/cells_foss/XlsxWorkbookConditionalFormatting.h"
#include "aspose/cells_foss/FormatConditionCollection.h"
#include "aspose/cells_foss/InvalidFileFormatException.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdio>
#include <string>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

using Core::CellAddress;
using Core::ColorValue;
using Core::ConditionalFormattingModel;
using Core::FormatConditionModel;
using Core::StyleValue;
using Core::WorksheetModel;

// ---------------------------------------------------------------------------
// Static namespace alias
// ---------------------------------------------------------------------------

static const XNamespace& MainNs = XlsxWorkbookSerializerCommon::MainNs;

// ---------------------------------------------------------------------------
// BuildConditionalFormattingElements
// ---------------------------------------------------------------------------

std::vector<XmlElement> XlsxWorkbookConditionalFormatting::BuildConditionalFormattingElements(
    WorksheetModel& worksheet,
    StylesheetSaveContext& stylesheet)
{
    auto ordered = GetOrderedConditionalFormattings(worksheet.GetConditionalFormattings());
    std::vector<XmlElement> elements;
    elements.reserve(ordered.size());

    for (std::size_t index = 0; index < ordered.size(); ++index) {
        const auto& formatting = ordered[index];
        if (formatting.GetAreas().empty() || formatting.GetConditions().empty()) {
            continue;
        }

        auto element = XmlElement::MakeElement(
            MainNs + "conditionalFormatting",
            std::vector<std::pair<std::string, std::string>>{
                {"sqref", BuildSqref(formatting.GetAreas())}},
            std::vector<XmlElement>{});

        for (std::size_t conditionIndex = 0; conditionIndex < formatting.GetConditions().size(); ++conditionIndex) {
            auto rule = BuildConditionRule(formatting, formatting.GetConditions()[conditionIndex],
                                           static_cast<int>(conditionIndex), stylesheet);
            element.AddChild(std::move(rule));
        }

        elements.push_back(std::move(element));
    }

    return elements;
}

// ---------------------------------------------------------------------------
// LoadConditionalFormattings
// ---------------------------------------------------------------------------

void XlsxWorkbookConditionalFormatting::LoadConditionalFormattings(
    WorksheetModel& worksheetModel,
    const XmlElement& worksheetRoot,
    StylesheetLoadContext& stylesheet,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    worksheetModel.GetConditionalFormattings().clear();

    const auto formattingElements = worksheetRoot.GetElements(MainNs + "conditionalFormatting");
    for (const auto& formattingElement : formattingElements) {
        ConditionalFormattingModel formatting;
        const auto sqrefAttr = formattingElement.GetAttribute("sqref");
        const std::string sqref = sqrefAttr.GetValue();

        if (!TryLoadAreas(formatting, sqref, diagnostics, options, sheetName)) {
            continue;
        }

        const auto ruleElements = formattingElement.GetElements(MainNs + "cfRule");
        for (const auto& ruleElement : ruleElements) {
            if (!TryLoadCondition(formatting, ruleElement, stylesheet, diagnostics, options, sheetName, sqref)) {
                continue;
            }
        }

        if (formatting.GetConditions().empty()) {
            LoadIssue issue("CF-L003", DiagnosticSeverity::LossyRecoverable,
                "A conditional formatting entry without any supported rules was dropped during load.",
                true, true);
            issue.SetSheetName(sheetName);
            issue.SetCellRef(sqref);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            continue;
        }

        worksheetModel.GetConditionalFormattings().push_back(std::move(formatting));
    }
}

// ---------------------------------------------------------------------------
// BuildConditionRule
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookConditionalFormatting::BuildConditionRule(
    const ConditionalFormattingModel& formatting,
    const FormatConditionModel& condition,
    int conditionIndex,
    StylesheetSaveContext& stylesheet)
{
    auto rule = XmlElement::MakeElement(
        MainNs + "cfRule",
        std::vector<std::pair<std::string, std::string>>{
            {"type", ToRuleTypeName(condition)},
            {"priority", std::to_string(condition.GetPriority() > 0 ? condition.GetPriority() : conditionIndex + 1)}},
        std::vector<XmlElement>{});

    const auto differentialStyleIndex = stylesheet.GetDifferentialStyleIndex(condition);
    if (differentialStyleIndex.has_value()) {
        rule.SetAttribute("dxfId", std::to_string(differentialStyleIndex.value()));
    }

    if (condition.GetStopIfTrue()) {
        rule.SetAttribute("stopIfTrue", "1");
    }

    switch (condition.GetType()) {
    case FormatConditionType::CellValue: {
        const auto operatorName = ToOperatorName(condition.GetOperator());
        if (!operatorName.empty()) {
            rule.SetAttribute("operator", operatorName);
        }
        AddFormulaElement(rule, condition.GetFormula1());
        AddFormulaElement(rule, condition.GetFormula2());
        break;
    }
    case FormatConditionType::Expression:
        AddFormulaElement(rule, condition.GetFormula1());
        break;
    case FormatConditionType::ContainsText:
    case FormatConditionType::NotContainsText:
    case FormatConditionType::BeginsWith:
    case FormatConditionType::EndsWith:
        if (!condition.GetFormula1().empty()) {
            rule.SetAttribute("text", condition.GetFormula1());
            AddFormulaElement(rule,
                BuildTextRuleFormula(condition.GetType(), condition.GetFormula1(),
                                     GetAnchorCellReference(formatting)));
        }
        break;
    case FormatConditionType::TimePeriod:
        if (!condition.GetTimePeriod().empty()) {
            rule.SetAttribute("timePeriod", condition.GetTimePeriod());
        }
        break;
    case FormatConditionType::Top10:
    case FormatConditionType::Bottom10:
        rule.SetAttribute("bottom",
            (condition.GetType() == FormatConditionType::Bottom10 || !condition.GetTop()) ? "1" : "0");
        if (condition.GetPercent()) {
            rule.SetAttribute("percent", "1");
        }
        if (condition.GetRank() > 0) {
            rule.SetAttribute("rank", std::to_string(condition.GetRank()));
        }
        break;
    case FormatConditionType::AboveAverage:
    case FormatConditionType::BelowAverage:
        if (condition.GetType() == FormatConditionType::BelowAverage || !condition.GetAbove()) {
            rule.SetAttribute("aboveAverage", "0");
        }
        if (condition.GetStandardDeviation() > 0) {
            rule.SetAttribute("stdDev", std::to_string(condition.GetStandardDeviation()));
        }
        break;
    case FormatConditionType::ColorScale:
        rule.AddChild(BuildColorScaleElement(condition));
        break;
    case FormatConditionType::DataBar:
        rule.AddChild(BuildDataBarElement(condition));
        break;
    case FormatConditionType::IconSet:
        rule.AddChild(BuildIconSetElement(condition));
        break;
    default:
        break;
    }

    return rule;
}

// ---------------------------------------------------------------------------
// TryLoadAreas
// ---------------------------------------------------------------------------

bool XlsxWorkbookConditionalFormatting::TryLoadAreas(
    ConditionalFormattingModel& formatting,
    const std::string& sqref,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    // Check for blank/whitespace-only input (matches C# IsNullOrWhiteSpace).
    bool allWhitespace = true;
    for (char ch : sqref) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            allWhitespace = false;
            break;
        }
    }

    if (allWhitespace) {
        if (options.GetStrictMode()) {
            throw InvalidFileFormatException("A conditional formatting entry is missing sqref.");
        }

        LoadIssue issue("CF-L003", DiagnosticSeverity::LossyRecoverable,
            "A conditional formatting entry without any valid areas was dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
        return false;
    }

    // Split on whitespace characters, removing empty entries.
    std::vector<std::string> tokens;
    {
        std::string current;
        for (char ch : sqref) {
            if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
                if (!current.empty()) {
                    tokens.push_back(std::move(current));
                    current.clear();
                }
            } else {
                current += ch;
            }
        }
        if (!current.empty()) {
            tokens.push_back(std::move(current));
        }
    }

    bool invalidAreaReported = false;
    for (std::size_t index = 0; index < tokens.size(); ++index) {
        const auto& token = tokens[index];
        Core::MergeRegion region;
        if (!XlsxWorkbookSerializerCommon::TryParseMergeReference(token, region)) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The conditional formatting sqref '" + token + "' is invalid.");
            }

            if (!invalidAreaReported) {
                LoadIssue issue("CF-L001", DiagnosticSeverity::LossyRecoverable,
                    "One or more conditional formatting areas were invalid and were dropped during load.",
                    true, true);
                issue.SetSheetName(sheetName);
                issue.SetCellRef(token);
                XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
                invalidAreaReported = true;
            }
            continue;
        }

        formatting.GetAreas().emplace_back(
            region.GetFirstRow(), region.GetFirstColumn(),
            region.GetTotalRows(), region.GetTotalColumns());
    }

    FormatConditionCollection::SortAreas(formatting.GetAreas());
    if (formatting.GetAreas().empty()) {
        LoadIssue issue("CF-L003", DiagnosticSeverity::LossyRecoverable,
            "A conditional formatting entry without any valid areas was dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        issue.SetCellRef(sqref);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// TryLoadCondition
// ---------------------------------------------------------------------------

bool XlsxWorkbookConditionalFormatting::TryLoadCondition(
    ConditionalFormattingModel& formatting,
    const XmlElement& ruleElement,
    StylesheetLoadContext& stylesheet,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    const std::string& cellRef)
{
    FormatConditionType type;
    if (!TryParseRuleType(ruleElement, options, diagnostics, sheetName, cellRef, type)) {
        return false;
    }

    FormatConditionModel condition;
    condition.SetType(type);
    condition.SetOperator(ParseOperatorType(ruleElement.GetAttribute("operator").GetValue()));
    condition.SetStopIfTrue(XlsxWorkbookArchiveHelpers::ParseBoolAttribute(ruleElement.GetAttribute("stopIfTrue")));
    condition.SetPriority(ParsePriority(ruleElement.GetAttribute("priority"), formatting, diagnostics, options, sheetName, cellRef));
    condition.SetStyle(ResolveDifferentialStyle(ruleElement.GetAttribute("dxfId"), stylesheet, diagnostics, options, sheetName, cellRef));
    ApplyDefaults(condition);

    const auto formulaElements = ruleElement.GetElements(MainNs + "formula");
    switch (type) {
    case FormatConditionType::Expression:
        condition.SetFormula1(formulaElements.size() > 0
            ? NormalizeFormulaValue(formulaElements[0].GetValue()) : std::string());
        break;
    case FormatConditionType::CellValue:
        condition.SetFormula1(formulaElements.size() > 0
            ? NormalizeFormulaValue(formulaElements[0].GetValue()) : std::string());
        condition.SetFormula2(formulaElements.size() > 1
            ? NormalizeFormulaValue(formulaElements[1].GetValue()) : std::string());
        break;
    case FormatConditionType::ContainsText:
    case FormatConditionType::NotContainsText:
    case FormatConditionType::BeginsWith:
    case FormatConditionType::EndsWith:
        condition.SetFormula1(NormalizeTextValue(ruleElement.GetAttribute("text").GetValue()));
        break;
    case FormatConditionType::TimePeriod:
        condition.SetTimePeriod(NormalizeTextValue(ruleElement.GetAttribute("timePeriod").GetValue()));
        break;
    case FormatConditionType::Top10:
    case FormatConditionType::Bottom10:
        condition.SetTop(type == FormatConditionType::Top10);
        condition.SetPercent(XlsxWorkbookArchiveHelpers::ParseBoolAttribute(ruleElement.GetAttribute("percent")));
        condition.SetRank(ParseNonNegativeIntAttribute(ruleElement.GetAttribute("rank")));
        break;
    case FormatConditionType::AboveAverage:
    case FormatConditionType::BelowAverage:
        condition.SetAbove(type == FormatConditionType::AboveAverage);
        condition.SetStandardDeviation(ParseNonNegativeIntAttribute(ruleElement.GetAttribute("stdDev")));
        break;
    case FormatConditionType::ColorScale:
        LoadColorScale(condition, ruleElement.GetElement(MainNs + "colorScale"));
        break;
    case FormatConditionType::DataBar:
        LoadDataBar(condition, ruleElement.GetElement(MainNs + "dataBar"));
        break;
    case FormatConditionType::IconSet:
        LoadIconSet(condition, ruleElement.GetElement(MainNs + "iconSet"));
        break;
    case FormatConditionType::DuplicateValues:
        condition.SetDuplicate(true);
        break;
    case FormatConditionType::UniqueValues:
        condition.SetDuplicate(false);
        break;
    }

    formatting.GetConditions().push_back(std::move(condition));
    return true;
}

// ---------------------------------------------------------------------------
// TryParseRuleType
// ---------------------------------------------------------------------------

bool XlsxWorkbookConditionalFormatting::TryParseRuleType(
    const XmlElement& ruleElement,
    const LoadOptions& options,
    LoadDiagnostics& diagnostics,
    std::string_view sheetName,
    const std::string& cellRef,
    FormatConditionType& type)
{
    type = FormatConditionType::CellValue;
    const std::string typeText = NormalizeToken(ruleElement.GetAttribute("type").GetValue());

    if (typeText == "cellis") { type = FormatConditionType::CellValue; return true; }
    if (typeText == "expression") { type = FormatConditionType::Expression; return true; }
    if (typeText == "containstext") { type = FormatConditionType::ContainsText; return true; }
    if (typeText == "notcontainstext") { type = FormatConditionType::NotContainsText; return true; }
    if (typeText == "beginswith") { type = FormatConditionType::BeginsWith; return true; }
    if (typeText == "endswith") { type = FormatConditionType::EndsWith; return true; }
    if (typeText == "timeperiod") { type = FormatConditionType::TimePeriod; return true; }
    if (typeText == "duplicatevalues") { type = FormatConditionType::DuplicateValues; return true; }
    if (typeText == "uniquevalues") { type = FormatConditionType::UniqueValues; return true; }
    if (typeText == "top10") {
        type = XlsxWorkbookArchiveHelpers::ParseBoolAttribute(ruleElement.GetAttribute("bottom"))
            ? FormatConditionType::Bottom10 : FormatConditionType::Top10;
        return true;
    }
    if (typeText == "aboveaverage") {
        auto aboveAverageAttr = ruleElement.GetAttribute("aboveAverage");
        type = !aboveAverageAttr.IsNull() && !XlsxWorkbookArchiveHelpers::ParseBoolAttribute(aboveAverageAttr)
            ? FormatConditionType::BelowAverage : FormatConditionType::AboveAverage;
        return true;
    }
    if (typeText == "colorscale") { type = FormatConditionType::ColorScale; return true; }
    if (typeText == "databar") { type = FormatConditionType::DataBar; return true; }
    if (typeText == "iconset") { type = FormatConditionType::IconSet; return true; }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            "The conditional formatting rule type '" + typeText + "' is not supported.");
    }

    LoadIssue issue("CF-L002", DiagnosticSeverity::LossyRecoverable,
        "An unsupported conditional formatting rule was dropped during load.", true, true);
    issue.SetSheetName(sheetName);
    issue.SetCellRef(cellRef);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return false;
}

// ---------------------------------------------------------------------------
// ApplyDefaults
// ---------------------------------------------------------------------------

void XlsxWorkbookConditionalFormatting::ApplyDefaults(FormatConditionModel& condition)
{
    switch (condition.GetType()) {
    case FormatConditionType::DuplicateValues:
        condition.SetDuplicate(true);
        break;
    case FormatConditionType::UniqueValues:
        condition.SetDuplicate(false);
        break;
    case FormatConditionType::Top10:
        condition.SetTop(true);
        if (condition.GetRank() == 0) {
            condition.SetRank(10);
        }
        break;
    case FormatConditionType::Bottom10:
        condition.SetTop(false);
        if (condition.GetRank() == 0) {
            condition.SetRank(10);
        }
        break;
    case FormatConditionType::AboveAverage:
        condition.SetAbove(true);
        break;
    case FormatConditionType::BelowAverage:
        condition.SetAbove(false);
        break;
    case FormatConditionType::ColorScale:
        if (condition.GetColorScaleCount() == 0) {
            condition.SetColorScaleCount(2);
        }
        break;
    case FormatConditionType::DataBar:
        if (IsEmptyColor(condition.GetBarColor())) {
            condition.SetBarColor(ColorValue(255, 99, 142, 198));
        }
        break;
    case FormatConditionType::IconSet:
        if (condition.GetIconSetType().empty()) {
            condition.SetIconSetType("3TrafficLights1");
        }
        break;
    default:
        break;
    }
}

// ---------------------------------------------------------------------------
// ParsePriority
// ---------------------------------------------------------------------------

int XlsxWorkbookConditionalFormatting::ParsePriority(
    const XmlAttribute& attribute,
    const ConditionalFormattingModel& formatting,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    const std::string& cellRef)
{
    const auto parsed = XlsxWorkbookArchiveHelpers::ParseIntAttribute(attribute);
    if (parsed.has_value() && parsed.value() > 0) {
        return parsed.value();
    }

    if (!attribute.IsNull()) {
        LoadIssue issue("CF-R001", DiagnosticSeverity::Recoverable,
            "A conditional formatting priority was invalid and was normalized during load.", true);
        issue.SetSheetName(sheetName);
        issue.SetCellRef(cellRef);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    }

    return static_cast<int>(formatting.GetConditions().size()) + 1;
}

// ---------------------------------------------------------------------------
// ResolveDifferentialStyle
// ---------------------------------------------------------------------------

StyleValue XlsxWorkbookConditionalFormatting::ResolveDifferentialStyle(
    const XmlAttribute& attribute,
    StylesheetLoadContext& stylesheet,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    const std::string& cellRef)
{
    const auto dxfId = XlsxWorkbookArchiveHelpers::ParseIntAttribute(attribute);
    if (!dxfId.has_value()) {
        return StyleValue::Default().Clone();
    }

    if (dxfId.value() >= 0 && dxfId.value() < static_cast<int>(stylesheet.GetDifferentialFormats().size())) {
        return stylesheet.GetDifferentialFormats()[dxfId.value()].Clone();
    }

    if (options.GetStrictMode()) {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%d", dxfId.value());
        throw InvalidFileFormatException(
            std::string("The conditional formatting dxfId '") + buf + "' is invalid.");
    }

    LoadIssue issue("CF-R002", DiagnosticSeverity::Recoverable,
        "A conditional formatting differential style reference was invalid and the rule style was reset to default during load.", true);
    issue.SetSheetName(sheetName);
    issue.SetCellRef(cellRef);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return StyleValue::Default().Clone();
}

// ---------------------------------------------------------------------------
// LoadColorScale
// ---------------------------------------------------------------------------

void XlsxWorkbookConditionalFormatting::LoadColorScale(
    FormatConditionModel& condition,
    const XmlElement& colorScaleElement)
{
    if (colorScaleElement.IsNull()) {
        return;
    }

    const auto cfvoElements = colorScaleElement.GetElements(MainNs + "cfvo");
    const int cfvoCount = static_cast<int>(cfvoElements.size());
    condition.SetColorScaleCount(cfvoCount >= 3 ? 3 : 2);

    const auto colorElements = colorScaleElement.GetElements(MainNs + "color");
    if (colorElements.size() > 0) {
        condition.SetMinColor(ReadRgbColor(colorElements[0]));
    }

    if (condition.GetColorScaleCount() == 3 && colorElements.size() > 2) {
        condition.SetMidColor(ReadRgbColor(colorElements[1]));
        condition.SetMaxColor(ReadRgbColor(colorElements[2]));
        return;
    }

    if (colorElements.size() > 1) {
        condition.SetMaxColor(ReadRgbColor(colorElements[1]));
    }
}

// ---------------------------------------------------------------------------
// LoadDataBar
// ---------------------------------------------------------------------------

void XlsxWorkbookConditionalFormatting::LoadDataBar(
    FormatConditionModel& condition,
    const XmlElement& dataBarElement)
{
    if (dataBarElement.IsNull()) {
        return;
    }

    const auto color = dataBarElement.GetElement(MainNs + "color");
    if (!color.IsNull()) {
        condition.SetBarColor(ReadRgbColor(color));
    }
}

// ---------------------------------------------------------------------------
// LoadIconSet
// ---------------------------------------------------------------------------

void XlsxWorkbookConditionalFormatting::LoadIconSet(
    FormatConditionModel& condition,
    const XmlElement& iconSetElement)
{
    if (iconSetElement.IsNull()) {
        return;
    }

    const std::string iconSetType = NormalizeTextValue(iconSetElement.GetAttribute("iconSet").GetValue());
    condition.SetIconSetType(iconSetType.empty() ? "3TrafficLights1" : iconSetType);
    condition.SetReverseIcons(XlsxWorkbookArchiveHelpers::ParseBoolAttribute(iconSetElement.GetAttribute("reverse")));
    const auto showValueAttr = iconSetElement.GetAttribute("showValue");
    condition.SetShowIconOnly(!showValueAttr.IsNull() && !XlsxWorkbookArchiveHelpers::ParseBoolAttribute(showValueAttr));
}

// ---------------------------------------------------------------------------
// BuildColorScaleElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookConditionalFormatting::BuildColorScaleElement(const FormatConditionModel& condition)
{
    std::vector<XmlElement> children;

    children.push_back(XmlElement::MakeElement(
        MainNs + "cfvo",
        std::vector<std::pair<std::string, std::string>>{{"type", "min"}},
        std::vector<XmlElement>{}));

    if (condition.GetColorScaleCount() == 3) {
        children.push_back(XmlElement::MakeElement(
            MainNs + "cfvo",
            std::vector<std::pair<std::string, std::string>>{
                {"type", "percentile"}, {"val", "50"}},
            std::vector<XmlElement>{}));
    }

    children.push_back(XmlElement::MakeElement(
        MainNs + "cfvo",
        std::vector<std::pair<std::string, std::string>>{{"type", "max"}},
        std::vector<XmlElement>{}));

    children.push_back(BuildColorElement(condition.GetMinColor(), ColorValue(255, 248, 105, 107)));

    if (condition.GetColorScaleCount() == 3) {
        children.push_back(BuildColorElement(condition.GetMidColor(), ColorValue(255, 255, 235, 132)));
    }

    children.push_back(BuildColorElement(condition.GetMaxColor(), ColorValue(255, 99, 190, 123)));

    return XmlElement::MakeElement(MainNs + "colorScale", std::vector<std::pair<std::string, std::string>>{}, std::move(children));
}

// ---------------------------------------------------------------------------
// BuildDataBarElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookConditionalFormatting::BuildDataBarElement(const FormatConditionModel& condition)
{
    std::vector<XmlElement> children;
    children.push_back(XmlElement::MakeElement(
        MainNs + "cfvo",
        std::vector<std::pair<std::string, std::string>>{{"type", "min"}},
        std::vector<XmlElement>{}));
    children.push_back(XmlElement::MakeElement(
        MainNs + "cfvo",
        std::vector<std::pair<std::string, std::string>>{{"type", "max"}},
        std::vector<XmlElement>{}));
    children.push_back(BuildColorElement(condition.GetBarColor(), ColorValue(255, 99, 142, 198)));

    return XmlElement::MakeElement(MainNs + "dataBar", std::vector<std::pair<std::string, std::string>>{}, std::move(children));
}

// ---------------------------------------------------------------------------
// BuildIconSetElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookConditionalFormatting::BuildIconSetElement(const FormatConditionModel& condition)
{
    const std::string iconSetType = condition.GetIconSetType().empty() ? "3TrafficLights1" : condition.GetIconSetType();

    std::vector<XmlElement> children;

    const int iconCount = GetIconCount(iconSetType);
    for (int index = 0; index < iconCount; ++index) {
        const int val = (100 * index) / iconCount;
        children.push_back(XmlElement::MakeElement(
            MainNs + "cfvo",
            std::vector<std::pair<std::string, std::string>>{
                {"type", "percent"}, {"val", std::to_string(val)}},
            std::vector<XmlElement>{}));
    }

    auto element = XmlElement::MakeElement(
        MainNs + "iconSet",
        std::vector<std::pair<std::string, std::string>>{{"iconSet", iconSetType}},
        std::move(children));

    if (condition.GetReverseIcons()) {
        element.SetAttribute("reverse", "1");
    }
    if (condition.GetShowIconOnly()) {
        element.SetAttribute("showValue", "0");
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildColorElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookConditionalFormatting::BuildColorElement(
    const ColorValue& actual,
    const ColorValue& defaultColor)
{
    const ColorValue& color = IsEmptyColor(actual) ? defaultColor : actual;
    return XmlElement::MakeElement(
        MainNs + "color",
        std::vector<std::pair<std::string, std::string>>{{"rgb", ToArgbHex(color)}},
        std::vector<XmlElement>{});
}

// ---------------------------------------------------------------------------
// AddFormulaElement
// ---------------------------------------------------------------------------

void XlsxWorkbookConditionalFormatting::AddFormulaElement(XmlElement& parent, const std::string& formula)
{
    if (!formula.empty()) {
        auto formulaElem = XmlElement::MakeElement(MainNs + "formula");

        // Create a text node child to hold the formula text content.
        auto textNode = std::make_shared<XmlNodeData>();
        textNode->isTextNode = true;
        textNode->text = formula;
        formulaElem.AddChild(XmlElement::MakeElement(std::move(textNode)));

        parent.AddChild(std::move(formulaElem));
    }
}

// ---------------------------------------------------------------------------
// BuildTextRuleFormula
// ---------------------------------------------------------------------------

std::string XlsxWorkbookConditionalFormatting::BuildTextRuleFormula(
    FormatConditionType type,
    const std::string& text,
    const std::string& firstCell)
{
    // Escape double quotes by doubling them (matches C# text.Replace("\"", "\"\"")).
    std::string escapedText;
    escapedText.reserve(text.size());
    for (char ch : text) {
        if (ch == '"') {
            escapedText += "\"\"";
        } else {
            escapedText += ch;
        }
    }

    switch (type) {
    case FormatConditionType::ContainsText:
        return "NOT(ISERROR(SEARCH(\"" + escapedText + "\"," + firstCell + ")))";
    case FormatConditionType::NotContainsText:
        return "ISERROR(SEARCH(\"" + escapedText + "\"," + firstCell + "))";
    case FormatConditionType::BeginsWith:
        return "LEFT(" + firstCell + ",LEN(\"" + escapedText + "\"))=\"" + escapedText + "\"";
    case FormatConditionType::EndsWith:
        return "RIGHT(" + firstCell + ",LEN(\"" + escapedText + "\"))=\"" + escapedText + "\"";
    default:
        return {};
    }
}

// ---------------------------------------------------------------------------
// GetAnchorCellReference
// ---------------------------------------------------------------------------

std::string XlsxWorkbookConditionalFormatting::GetAnchorCellReference(
    const ConditionalFormattingModel& formatting)
{
    if (formatting.GetAreas().empty()) {
        return "A1";
    }

    const auto& area = formatting.GetAreas()[0];
    return CellAddress(area.GetFirstRow(), area.GetFirstColumn()).ToString();
}

// ---------------------------------------------------------------------------
// GetOrderedConditionalFormattings
// ---------------------------------------------------------------------------

std::vector<ConditionalFormattingModel> XlsxWorkbookConditionalFormatting::GetOrderedConditionalFormattings(
    const std::vector<ConditionalFormattingModel>& collections)
{
    std::vector<ConditionalFormattingModel> ordered;
    ordered.reserve(collections.size());

    for (std::size_t index = 0; index < collections.size(); ++index) {
        const auto& collection = collections[index];
        if (collection.GetAreas().empty() || collection.GetConditions().empty()) {
            continue;
        }

        // Need a mutable copy to sort areas.
        ConditionalFormattingModel copy = collection;
        FormatConditionCollection::SortAreas(copy.GetAreas());
        ordered.push_back(std::move(copy));
    }

    std::sort(ordered.begin(), ordered.end(),
        [](const ConditionalFormattingModel& left, const ConditionalFormattingModel& right) {
            return CompareConditionalFormattings(left, right) < 0;
        });

    return ordered;
}

// ---------------------------------------------------------------------------
// CompareConditionalFormattings
// ---------------------------------------------------------------------------

int XlsxWorkbookConditionalFormatting::CompareConditionalFormattings(
    const ConditionalFormattingModel& left,
    const ConditionalFormattingModel& right)
{
    return FormatConditionCollection::CompareAreas(left.GetAreas()[0], right.GetAreas()[0]);
}

// ---------------------------------------------------------------------------
// BuildSqref
// ---------------------------------------------------------------------------

std::string XlsxWorkbookConditionalFormatting::BuildSqref(const std::vector<CellArea>& areas)
{
    std::string result;
    for (std::size_t index = 0; index < areas.size(); ++index) {
        if (index > 0) {
            result += ' ';
        }
        result += ToAreaReference(areas[index]);
    }
    return result;
}

// ---------------------------------------------------------------------------
// ToAreaReference
// ---------------------------------------------------------------------------

std::string XlsxWorkbookConditionalFormatting::ToAreaReference(const CellArea& area)
{
    const std::string first = CellAddress(area.GetFirstRow(), area.GetFirstColumn()).ToString();
    if (area.GetTotalRows() == 1 && area.GetTotalColumns() == 1) {
        return first;
    }

    const std::string last = CellAddress(
        area.GetFirstRow() + area.GetTotalRows() - 1,
        area.GetFirstColumn() + area.GetTotalColumns() - 1).ToString();
    return first + ":" + last;
}

// ---------------------------------------------------------------------------
// ToRuleTypeName
// ---------------------------------------------------------------------------

std::string XlsxWorkbookConditionalFormatting::ToRuleTypeName(const FormatConditionModel& condition)
{
    switch (condition.GetType()) {
    case FormatConditionType::Expression:      return "expression";
    case FormatConditionType::ContainsText:    return "containsText";
    case FormatConditionType::NotContainsText: return "notContainsText";
    case FormatConditionType::BeginsWith:      return "beginsWith";
    case FormatConditionType::EndsWith:        return "endsWith";
    case FormatConditionType::TimePeriod:      return "timePeriod";
    case FormatConditionType::DuplicateValues: return "duplicateValues";
    case FormatConditionType::UniqueValues:    return "uniqueValues";
    case FormatConditionType::Top10:
    case FormatConditionType::Bottom10:        return "top10";
    case FormatConditionType::AboveAverage:
    case FormatConditionType::BelowAverage:    return "aboveAverage";
    case FormatConditionType::ColorScale:      return "colorScale";
    case FormatConditionType::DataBar:         return "dataBar";
    case FormatConditionType::IconSet:         return "iconSet";
    default:                                   return "cellIs";
    }
}

// ---------------------------------------------------------------------------
// ToOperatorName
// ---------------------------------------------------------------------------

std::string XlsxWorkbookConditionalFormatting::ToOperatorName(OperatorType type)
{
    switch (type) {
    case OperatorType::Between:        return "between";
    case OperatorType::Equal:          return "equal";
    case OperatorType::GreaterThan:    return "greaterThan";
    case OperatorType::GreaterOrEqual: return "greaterThanOrEqual";
    case OperatorType::LessThan:       return "lessThan";
    case OperatorType::LessOrEqual:    return "lessThanOrEqual";
    case OperatorType::NotBetween:     return "notBetween";
    case OperatorType::NotEqual:       return "notEqual";
    default:                           return {};
    }
}

// ---------------------------------------------------------------------------
// ParseOperatorType
// ---------------------------------------------------------------------------

OperatorType XlsxWorkbookConditionalFormatting::ParseOperatorType(std::string_view text)
{
    const std::string normalized = NormalizeToken(text);

    if (normalized == "between")                return OperatorType::Between;
    if (normalized == "equal")                  return OperatorType::Equal;
    if (normalized == "greaterthan")            return OperatorType::GreaterThan;
    if (normalized == "greaterthanorequal")     return OperatorType::GreaterOrEqual;
    if (normalized == "lessthan")               return OperatorType::LessThan;
    if (normalized == "lessthanorequal")        return OperatorType::LessOrEqual;
    if (normalized == "notbetween")             return OperatorType::NotBetween;
    if (normalized == "notequal")               return OperatorType::NotEqual;

    return OperatorType::None;
}

// ---------------------------------------------------------------------------
// ParseNonNegativeIntAttribute
// ---------------------------------------------------------------------------

int XlsxWorkbookConditionalFormatting::ParseNonNegativeIntAttribute(const XmlAttribute& attribute)
{
    const auto parsed = XlsxWorkbookArchiveHelpers::ParseIntAttribute(attribute);
    if (parsed.has_value() && parsed.value() >= 0) {
        return parsed.value();
    }
    return 0;
}

// ---------------------------------------------------------------------------
// GetIconCount
// ---------------------------------------------------------------------------

int XlsxWorkbookConditionalFormatting::GetIconCount(std::string_view iconSetType)
{
    if (iconSetType.empty()) {
        return 3;
    }
    if (iconSetType[0] == '4') {
        return 4;
    }
    if (iconSetType[0] == '5') {
        return 5;
    }
    return 3;
}

// ---------------------------------------------------------------------------
// NormalizeToken
// ---------------------------------------------------------------------------

std::string XlsxWorkbookConditionalFormatting::NormalizeToken(std::string_view value)
{
    // Trim leading and trailing whitespace (matches C# string.Trim()).
    const auto start = std::find_if_not(value.begin(), value.end(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); });
    const auto end = std::find_if_not(value.rbegin(), value.rend(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); }).base();

    if (start >= end) {
        return {};
    }

    std::string trimmed(start, end);
    std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(),
        [](char c) { return static_cast<char>(std::tolower(static_cast<unsigned char>(c))); });
    return trimmed;
}

// ---------------------------------------------------------------------------
// NormalizeFormulaValue
// ---------------------------------------------------------------------------

std::string XlsxWorkbookConditionalFormatting::NormalizeFormulaValue(std::string_view value)
{
    // Trim leading and trailing whitespace (matches C# string.Trim()).
    const auto start = std::find_if_not(value.begin(), value.end(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); });
    const auto end = std::find_if_not(value.rbegin(), value.rend(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); }).base();

    if (start >= end) {
        return {};
    }

    std::string trimmed(start, end);
    if (trimmed[0] == '=') {
        return trimmed.substr(1);
    }
    return trimmed;
}

// ---------------------------------------------------------------------------
// NormalizeTextValue
// ---------------------------------------------------------------------------

std::string XlsxWorkbookConditionalFormatting::NormalizeTextValue(std::string_view value)
{
    // Trim leading and trailing whitespace (matches C# string.Trim()).
    const auto start = std::find_if_not(value.begin(), value.end(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); });
    const auto end = std::find_if_not(value.rbegin(), value.rend(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); }).base();

    if (start >= end) {
        return {};
    }

    return std::string(start, end);
}

// ---------------------------------------------------------------------------
// ReadRgbColor
// ---------------------------------------------------------------------------

ColorValue XlsxWorkbookConditionalFormatting::ReadRgbColor(const XmlElement& element)
{
    const std::string rgb = NormalizeTextValue(element.GetAttribute("rgb").GetValue());
    if (rgb.empty() || rgb.size() != 8) {
        return ColorValue();
    }

    // Parse 2-char hex components using std::from_chars.
    auto parseHex = [](std::string_view hex) -> std::uint8_t {
        unsigned int result = 0;
        auto [ptr, ec] = std::from_chars(hex.data(), hex.data() + hex.size(), result, 16);
        if (ec != std::errc()) {
            return 0;
        }
        return static_cast<std::uint8_t>(result);
    };

    return ColorValue(
        parseHex(std::string_view(rgb.data(), 2)),
        parseHex(std::string_view(rgb.data() + 2, 2)),
        parseHex(std::string_view(rgb.data() + 4, 2)),
        parseHex(std::string_view(rgb.data() + 6, 2)));
}

// ---------------------------------------------------------------------------
// ToArgbHex
// ---------------------------------------------------------------------------

std::string XlsxWorkbookConditionalFormatting::ToArgbHex(const ColorValue& color)
{
    char buf[9];
    std::snprintf(buf, sizeof(buf), "%02X%02X%02X%02X",
        static_cast<unsigned>(color.GetA()),
        static_cast<unsigned>(color.GetR()),
        static_cast<unsigned>(color.GetG()),
        static_cast<unsigned>(color.GetB()));
    return std::string(buf, 8);
}

// ---------------------------------------------------------------------------
// IsEmptyColor
// ---------------------------------------------------------------------------

bool XlsxWorkbookConditionalFormatting::IsEmptyColor(const ColorValue& color)
{
    return color.GetA() == 0 && color.GetR() == 0 && color.GetG() == 0 && color.GetB() == 0;
}

}}  // namespace Aspose::Cells_FOSS
