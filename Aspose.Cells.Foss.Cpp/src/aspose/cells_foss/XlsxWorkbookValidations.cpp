#include "aspose/cells_foss/XlsxWorkbookValidations.h"
#include "aspose/cells_foss/InvalidFileFormatException.h"
#include "aspose/cells_foss/ValidationCollection.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"
#include "aspose/cells_foss/core/CellAddress.h"
#include "aspose/cells_foss/core/MergeRegion.h"

#include <algorithm>
#include <cctype>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

using Core::CellAddress;
using Core::MergeRegion;
using Core::ValidationModel;
using Core::WorksheetModel;

// ---------------------------------------------------------------------------
// Static namespace aliases
// ---------------------------------------------------------------------------

static const XNamespace& MainNs = XlsxWorkbookSerializerCommon::MainNs;

// ---------------------------------------------------------------------------
// Local helpers
// ---------------------------------------------------------------------------

static bool IsNullOrWhiteSpace(std::string_view s)
{
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (!std::isspace(static_cast<unsigned char>(s[i]))) {
            return false;
        }
    }
    return true;
}

static std::vector<std::string> SplitOnWhitespace(std::string_view text)
{
    std::vector<std::string> tokens;
    std::size_t i = 0;
    while (i < text.size()) {
        while (i < text.size() &&
               std::isspace(static_cast<unsigned char>(text[i]))) {
            ++i;
        }
        if (i >= text.size()) {
            break;
        }
        const std::size_t start = i;
        while (i < text.size() &&
               !std::isspace(static_cast<unsigned char>(text[i]))) {
            ++i;
        }
        tokens.emplace_back(text.substr(start, i - start));
    }
    return tokens;
}

// ---------------------------------------------------------------------------
// BuildDataValidationsElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookValidations::BuildDataValidationsElement(
    const WorksheetModel& worksheet)
{
    auto ordered = GetOrderedValidations(worksheet.GetValidations());
    if (ordered.empty()) {
        return XmlElement();
    }

    auto container = XmlElement::MakeElement(MainNs + "dataValidations");
    int count = 0;

    for (std::size_t index = 0; index < ordered.size(); ++index) {
        const auto& validation = ordered[index];
        if (validation.GetAreas().empty()) {
            continue;
        }

        auto element = XmlElement::MakeElement(
            MainNs + "dataValidation",
            std::vector<std::pair<std::string, std::string>>{
                {"sqref", BuildSqref(validation.GetAreas())}},
            std::vector<XmlElement>{});

        const char* typeName = ToValidationTypeName(validation.GetType());
        if (typeName != nullptr) {
            element.SetAttribute("type", typeName);
        }

        const char* operatorName = ToOperatorName(validation.GetOperator());
        if (operatorName != nullptr) {
            element.SetAttribute("operator", operatorName);
        }

        if (validation.GetAlertStyle() != ValidationAlertType::Stop) {
            element.SetAttribute("errorStyle",
                ToAlertStyleName(validation.GetAlertStyle()));
        }

        if (validation.GetIgnoreBlank()) {
            element.SetAttribute("allowBlank", "1");
        }

        if (!validation.GetInCellDropDown()) {
            element.SetAttribute("showDropDown", "1");
        }

        if (validation.GetShowInput()) {
            element.SetAttribute("showInputMessage", "1");
        }

        if (validation.GetShowError()) {
            element.SetAttribute("showErrorMessage", "1");
        }

        if (!validation.GetErrorTitle().empty()) {
            element.SetAttribute("errorTitle", validation.GetErrorTitle());
        }

        if (!validation.GetErrorMessage().empty()) {
            element.SetAttribute("error", validation.GetErrorMessage());
        }

        if (!validation.GetInputTitle().empty()) {
            element.SetAttribute("promptTitle", validation.GetInputTitle());
        }

        if (!validation.GetInputMessage().empty()) {
            element.SetAttribute("prompt", validation.GetInputMessage());
        }

        if (!validation.GetFormula1().empty()) {
            auto formula1 = XmlElement::MakeElement(MainNs + "formula1");
            auto textNode = std::make_shared<XmlNodeData>();
            textNode->isTextNode = true;
            textNode->text = validation.GetFormula1();
            formula1.AddChild(XmlElement::MakeElement(std::move(textNode)));
            element.AddChild(std::move(formula1));
        }

        if (!validation.GetFormula2().empty()) {
            auto formula2 = XmlElement::MakeElement(MainNs + "formula2");
            auto textNode = std::make_shared<XmlNodeData>();
            textNode->isTextNode = true;
            textNode->text = validation.GetFormula2();
            formula2.AddChild(XmlElement::MakeElement(std::move(textNode)));
            element.AddChild(std::move(formula2));
        }

        container.AddChild(std::move(element));
        ++count;
    }

    if (count == 0) {
        return XmlElement();
    }

    container.SetAttribute("count", std::to_string(count));
    return container;
}

// ---------------------------------------------------------------------------
// LoadValidations
// ---------------------------------------------------------------------------

void XlsxWorkbookValidations::LoadValidations(
    WorksheetModel& worksheetModel,
    const XmlElement& worksheetRoot,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    worksheetModel.GetValidations().clear();

    const auto dataValidationsElement =
        worksheetRoot.GetElement(MainNs + "dataValidations");
    if (dataValidationsElement.IsNull()) {
        return;
    }

    const auto validationElements =
        dataValidationsElement.GetElements(MainNs + "dataValidation");
    for (const auto& validationElement : validationElements) {
        ValidationModel validation;

        const std::string sqref =
            validationElement.GetAttribute("sqref").GetValue();
        if (!TryLoadAreas(worksheetModel.GetValidations(), validation, sqref,
                          diagnostics, options, sheetName)) {
            continue;
        }

        validation.SetType(ParseValidationType(
            validationElement.GetAttribute("type").GetValue(),
            diagnostics, options, sheetName, sqref));
        validation.SetOperator(ParseOperatorType(
            validationElement.GetAttribute("operator").GetValue(),
            diagnostics, options, sheetName, sqref));
        validation.SetAlertStyle(ParseAlertStyle(
            validationElement.GetAttribute("errorStyle").GetValue(),
            diagnostics, options, sheetName, sqref));
        validation.SetIgnoreBlank(
            XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
                validationElement.GetAttribute("allowBlank")));
        validation.SetInCellDropDown(
            !XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
                validationElement.GetAttribute("showDropDown")));
        validation.SetShowInput(
            XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
                validationElement.GetAttribute("showInputMessage")));
        validation.SetShowError(
            XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
                validationElement.GetAttribute("showErrorMessage")));
        validation.SetErrorTitle(NormalizeText(
            validationElement.GetAttribute("errorTitle").GetValue()));
        validation.SetErrorMessage(NormalizeText(
            validationElement.GetAttribute("error").GetValue()));
        validation.SetInputTitle(NormalizeText(
            validationElement.GetAttribute("promptTitle").GetValue()));
        validation.SetInputMessage(NormalizeText(
            validationElement.GetAttribute("prompt").GetValue()));

        const auto formula1Element =
            validationElement.GetElement(MainNs + "formula1");
        validation.SetFormula1(
            NormalizeFormulaText(formula1Element.GetValue()));

        const auto formula2Element =
            validationElement.GetElement(MainNs + "formula2");
        validation.SetFormula2(
            NormalizeFormulaText(formula2Element.GetValue()));

        worksheetModel.GetValidations().push_back(std::move(validation));
    }
}

// ---------------------------------------------------------------------------
// TryLoadAreas
// ---------------------------------------------------------------------------

bool XlsxWorkbookValidations::TryLoadAreas(
    const std::vector<ValidationModel>& existingValidations,
    ValidationModel& candidate,
    std::string_view sqref,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    if (IsNullOrWhiteSpace(sqref)) {
        if (options.GetStrictMode()) {
            throw InvalidFileFormatException(
                "A data validation is missing sqref.");
        }

        LoadIssue issue("DV-L002", DiagnosticSeverity::LossyRecoverable,
            "A data validation without any valid areas was dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
        return false;
    }

    const auto tokens = SplitOnWhitespace(sqref);
    bool invalidAreaReported = false;
    bool overlapReported = false;

    for (std::size_t index = 0; index < tokens.size(); ++index) {
        const auto& token = tokens[index];
        MergeRegion region;
        if (!XlsxWorkbookSerializerCommon::TryParseMergeReference(
                token, region)) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The data validation sqref '" + token + "' is invalid.");
            }

            if (!invalidAreaReported) {
                LoadIssue issue("DV-L001",
                    DiagnosticSeverity::LossyRecoverable,
                    "One or more data validation areas were invalid and were "
                    "dropped during load.",
                    true, true);
                issue.SetSheetName(sheetName);
                issue.SetCellRef(token);
                XlsxWorkbookArchiveHelpers::AddIssue(
                    diagnostics, options, issue);
                invalidAreaReported = true;
            }
            continue;
        }

        const CellArea area(region.GetFirstRow(), region.GetFirstColumn(),
                            region.GetTotalRows(), region.GetTotalColumns());
        if (ConflictsWithExisting(existingValidations, candidate, area)) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The data validation sqref '" + token +
                    "' overlaps an existing validation.");
            }

            if (!overlapReported) {
                LoadIssue issue("DV-L003",
                    DiagnosticSeverity::LossyRecoverable,
                    "Overlapping data validation areas were normalized "
                    "during load.",
                    true, true);
                issue.SetSheetName(sheetName);
                issue.SetCellRef(token);
                XlsxWorkbookArchiveHelpers::AddIssue(
                    diagnostics, options, issue);
                overlapReported = true;
            }
            continue;
        }

        candidate.GetAreas().push_back(area);
    }

    ValidationCollection::SortAreas(candidate.GetAreas());
    if (candidate.GetAreas().empty()) {
        LoadIssue issue("DV-L002", DiagnosticSeverity::LossyRecoverable,
            "A data validation without any valid areas was dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        issue.SetCellRef(std::string(sqref));
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// ConflictsWithExisting
// ---------------------------------------------------------------------------

bool XlsxWorkbookValidations::ConflictsWithExisting(
    const std::vector<ValidationModel>& existingValidations,
    const ValidationModel& candidate,
    const CellArea& area)
{
    for (std::size_t validationIndex = 0;
         validationIndex < existingValidations.size(); ++validationIndex) {
        const auto& validation = existingValidations[validationIndex];
        const auto& areas = validation.GetAreas();
        for (std::size_t areaIndex = 0; areaIndex < areas.size();
             ++areaIndex) {
            if (ValidationCollection::AreasOverlap(areas[areaIndex], area)) {
                return true;
            }
        }
    }

    const auto& candidateAreas = candidate.GetAreas();
    for (std::size_t areaIndex = 0; areaIndex < candidateAreas.size();
         ++areaIndex) {
        if (ValidationCollection::AreasOverlap(candidateAreas[areaIndex],
                                               area)) {
            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
// GetOrderedValidations
// ---------------------------------------------------------------------------

std::vector<ValidationModel> XlsxWorkbookValidations::GetOrderedValidations(
    const std::vector<ValidationModel>& validations)
{
    std::vector<ValidationModel> ordered;
    ordered.reserve(validations.size());
    for (std::size_t index = 0; index < validations.size(); ++index) {
        const auto& validation = validations[index];
        if (validation.GetAreas().empty()) {
            continue;
        }

        ValidationModel copy = validation;
        ValidationCollection::SortAreas(copy.GetAreas());
        ordered.push_back(std::move(copy));
    }

    std::sort(ordered.begin(), ordered.end(),
              [](const ValidationModel& left, const ValidationModel& right) {
                  return CompareValidations(left, right) < 0;
              });

    return ordered;
}

// ---------------------------------------------------------------------------
// CompareValidations
// ---------------------------------------------------------------------------

int XlsxWorkbookValidations::CompareValidations(
    const ValidationModel& left,
    const ValidationModel& right)
{
    return ValidationCollection::CompareAreas(left.GetAreas()[0],
                                              right.GetAreas()[0]);
}

// ---------------------------------------------------------------------------
// BuildSqref
// ---------------------------------------------------------------------------

std::string XlsxWorkbookValidations::BuildSqref(
    const std::vector<CellArea>& areas)
{
    std::string result;
    for (std::size_t index = 0; index < areas.size(); ++index) {
        if (index > 0) {
            result.push_back(' ');
        }
        result.append(ToAreaReference(areas[index]));
    }
    return result;
}

// ---------------------------------------------------------------------------
// ToAreaReference
// ---------------------------------------------------------------------------

std::string XlsxWorkbookValidations::ToAreaReference(const CellArea& area)
{
    const std::string first =
        CellAddress(area.GetFirstRow(), area.GetFirstColumn()).ToString();
    if (area.GetTotalRows() == 1 && area.GetTotalColumns() == 1) {
        return first;
    }

    const std::string last =
        CellAddress(area.GetFirstRow() + area.GetTotalRows() - 1,
                    area.GetFirstColumn() + area.GetTotalColumns() - 1)
            .ToString();
    return first + ":" + last;
}

// ---------------------------------------------------------------------------
// ParseValidationType
// ---------------------------------------------------------------------------

ValidationType XlsxWorkbookValidations::ParseValidationType(
    std::string_view text,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view cellRef)
{
    const std::string token = NormalizeToken(text);
    if (token.empty()) {
        return ValidationType::AnyValue;
    }
    if (token == "whole") {
        return ValidationType::WholeNumber;
    }
    if (token == "decimal") {
        return ValidationType::Decimal;
    }
    if (token == "list") {
        return ValidationType::List;
    }
    if (token == "date") {
        return ValidationType::Date;
    }
    if (token == "time") {
        return ValidationType::Time;
    }
    if (token == "textlength") {
        return ValidationType::TextLength;
    }
    if (token == "custom") {
        return ValidationType::Custom;
    }
    return WarnAndReturnAnyValue("type", text, diagnostics, options,
                                 sheetName, cellRef);
}

// ---------------------------------------------------------------------------
// ParseOperatorType
// ---------------------------------------------------------------------------

OperatorType XlsxWorkbookValidations::ParseOperatorType(
    std::string_view text,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view cellRef)
{
    const std::string token = NormalizeToken(text);
    if (token.empty()) {
        return OperatorType::None;
    }
    if (token == "between") {
        return OperatorType::Between;
    }
    if (token == "equal") {
        return OperatorType::Equal;
    }
    if (token == "greaterthan") {
        return OperatorType::GreaterThan;
    }
    if (token == "greaterthanorequal") {
        return OperatorType::GreaterOrEqual;
    }
    if (token == "lessthan") {
        return OperatorType::LessThan;
    }
    if (token == "lessthanorequal") {
        return OperatorType::LessOrEqual;
    }
    if (token == "notbetween") {
        return OperatorType::NotBetween;
    }
    if (token == "notequal") {
        return OperatorType::NotEqual;
    }
    return WarnAndReturnNone("operator", text, diagnostics, options,
                             sheetName, cellRef);
}

// ---------------------------------------------------------------------------
// ParseAlertStyle
// ---------------------------------------------------------------------------

ValidationAlertType XlsxWorkbookValidations::ParseAlertStyle(
    std::string_view text,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view cellRef)
{
    const std::string token = NormalizeToken(text);
    if (token.empty() || token == "stop") {
        return ValidationAlertType::Stop;
    }
    if (token == "warning") {
        return ValidationAlertType::Warning;
    }
    if (token == "information") {
        return ValidationAlertType::Information;
    }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            "The data validation errorStyle '" + std::string(text) +
            "' is invalid.");
    }

    LoadIssue issue("DV-R001", DiagnosticSeverity::Recoverable,
        "An unknown data validation errorStyle was normalized during load.",
        true);
    issue.SetSheetName(sheetName);
    issue.SetCellRef(cellRef);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return ValidationAlertType::Stop;
}

// ---------------------------------------------------------------------------
// WarnAndReturnAnyValue
// ---------------------------------------------------------------------------

ValidationType XlsxWorkbookValidations::WarnAndReturnAnyValue(
    std::string_view attributeName,
    std::string_view text,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view cellRef)
{
    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            "The data validation " + std::string(attributeName) + " '" +
            std::string(text) + "' is invalid.");
    }

    LoadIssue issue("DV-R001", DiagnosticSeverity::Recoverable,
        "An unknown data validation " + std::string(attributeName) +
        " was normalized during load.",
        true);
    issue.SetSheetName(sheetName);
    issue.SetCellRef(cellRef);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return ValidationType::AnyValue;
}

// ---------------------------------------------------------------------------
// WarnAndReturnNone
// ---------------------------------------------------------------------------

OperatorType XlsxWorkbookValidations::WarnAndReturnNone(
    std::string_view attributeName,
    std::string_view text,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view cellRef)
{
    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            "The data validation " + std::string(attributeName) + " '" +
            std::string(text) + "' is invalid.");
    }

    LoadIssue issue("DV-R001", DiagnosticSeverity::Recoverable,
        "An unknown data validation " + std::string(attributeName) +
        " was normalized during load.",
        true);
    issue.SetSheetName(sheetName);
    issue.SetCellRef(cellRef);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return OperatorType::None;
}

// ---------------------------------------------------------------------------
// NormalizeToken
// ---------------------------------------------------------------------------

std::string XlsxWorkbookValidations::NormalizeToken(std::string_view value)
{
    std::size_t start = 0;
    while (start < value.size() &&
           std::isspace(static_cast<unsigned char>(value[start]))) {
        ++start;
    }
    std::size_t end = value.size();
    while (end > start &&
           std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }

    if (start >= end) {
        return {};
    }

    std::string result(value.substr(start, end - start));
    for (char& c : result) {
        c = static_cast<char>(
            std::tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

// ---------------------------------------------------------------------------
// NormalizeFormulaText
// ---------------------------------------------------------------------------

std::string XlsxWorkbookValidations::NormalizeFormulaText(
    std::string_view value)
{
    std::size_t start = 0;
    while (start < value.size() &&
           std::isspace(static_cast<unsigned char>(value[start]))) {
        ++start;
    }
    std::size_t end = value.size();
    while (end > start &&
           std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }

    if (start >= end) {
        return {};
    }

    std::string_view trimmed = value.substr(start, end - start);
    if (!trimmed.empty() && trimmed[0] == '=') {
        trimmed.remove_prefix(1);
    }
    return std::string(trimmed);
}

// ---------------------------------------------------------------------------
// NormalizeText
// ---------------------------------------------------------------------------

std::string XlsxWorkbookValidations::NormalizeText(std::string_view value)
{
    return value.empty() ? std::string() : std::string(value);
}

// ---------------------------------------------------------------------------
// ToValidationTypeName
// ---------------------------------------------------------------------------

const char* XlsxWorkbookValidations::ToValidationTypeName(ValidationType type)
{
    switch (type) {
    case ValidationType::WholeNumber:
        return "whole";
    case ValidationType::Decimal:
        return "decimal";
    case ValidationType::List:
        return "list";
    case ValidationType::Date:
        return "date";
    case ValidationType::Time:
        return "time";
    case ValidationType::TextLength:
        return "textLength";
    case ValidationType::Custom:
        return "custom";
    default:
        return nullptr;
    }
}

// ---------------------------------------------------------------------------
// ToOperatorName
// ---------------------------------------------------------------------------

const char* XlsxWorkbookValidations::ToOperatorName(OperatorType type)
{
    switch (type) {
    case OperatorType::Between:
        return "between";
    case OperatorType::Equal:
        return "equal";
    case OperatorType::GreaterThan:
        return "greaterThan";
    case OperatorType::GreaterOrEqual:
        return "greaterThanOrEqual";
    case OperatorType::LessThan:
        return "lessThan";
    case OperatorType::LessOrEqual:
        return "lessThanOrEqual";
    case OperatorType::NotBetween:
        return "notBetween";
    case OperatorType::NotEqual:
        return "notEqual";
    default:
        return nullptr;
    }
}

// ---------------------------------------------------------------------------
// ToAlertStyleName
// ---------------------------------------------------------------------------

const char* XlsxWorkbookValidations::ToAlertStyleName(
    ValidationAlertType type)
{
    switch (type) {
    case ValidationAlertType::Warning:
        return "warning";
    case ValidationAlertType::Information:
        return "information";
    default:
        return "stop";
    }
}

}}  // namespace Aspose::Cells_FOSS
