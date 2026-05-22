#include "aspose/cells_foss/XlsxWorkbookWorksheetProtection.h"
#include "aspose/cells_foss/InvalidFileFormatException.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"

#include <algorithm>
#include <cctype>
#include <optional>
#include <string>

namespace Aspose {
namespace Cells_FOSS {

using Core::WorksheetModel;

static const XNamespace& MainNs = XlsxWorkbookSerializerCommon::MainNs;

// ---------------------------------------------------------------------------
// Local helpers (anonymous namespace)
// ---------------------------------------------------------------------------

namespace {

// Edge-only trim matching C# string.Trim() with default whitespace characters.
std::string TrimEdge(std::string_view sv)
{
    const auto start = sv.find_first_not_of(" \t\n\r");
    if (start == std::string_view::npos) {
        return {};
    }
    const auto end = sv.find_last_not_of(" \t\n\r");
    return std::string(sv.substr(start, end - start + 1));
}

// Case-insensitive equality matching C# StringComparison.OrdinalIgnoreCase.
bool EqualsIgnoreCase(std::string_view a, std::string_view b)
{
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

// Checks whether a trimmed string is empty or consists entirely of whitespace.
// Matches C# string.IsNullOrWhiteSpace semantics after an explicit Trim() call.
bool IsBlankAfterTrim(std::string_view sv) noexcept
{
    return sv.empty() ||
           std::all_of(sv.begin(), sv.end(), [](unsigned char ch) {
               return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'
                   || ch == '\f' || ch == '\v';
           });
}

// Reads an optional attribute value: returns std::nullopt if the attribute is
// absent, or if its trimmed value is empty.
std::optional<std::string> ReadOptionalAttribute(
    const XmlElement& element,
    std::string_view attributeName)
{
    const auto attribute = element.GetAttribute(attributeName);
    if (attribute.IsNull()) {
        return std::nullopt;
    }
    std::string value = TrimEdge(attribute.GetValue());
    if (value.empty()) {
        return std::nullopt;
    }
    return value;
}

// Parses a worksheet protection boolean attribute from an XLSX XML sheet.
// Matches the C# ParseProtectionBooleanAttribute semantics exactly.
bool ParseProtectionBooleanAttribute(
    const XmlAttribute& attribute,
    bool defaultValue,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view attributeName)
{
    if (attribute.IsNull()) {
        return defaultValue;
    }

    const std::string rawValue = TrimEdge(attribute.GetValue());

    if (rawValue == "0" || EqualsIgnoreCase(rawValue, "false")) {
        return false;
    }
    if (rawValue == "1" || EqualsIgnoreCase(rawValue, "true")) {
        return true;
    }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            "Worksheet protection attribute '" +
            std::string(attributeName) + "' is invalid.");
    }

    LoadIssue issue(
        "WS-L009",
        DiagnosticSeverity::Warning,
        "Worksheet protection attribute '" +
            std::string(attributeName) +
            "' is invalid and the default value was used.",
        false, true);
    issue.SetSheetName(sheetName);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return defaultValue;
}

// Sets an integer-valued attribute when the boolean value is true.
// Matches C# SetBoolAttributeWhenTrue.
void SetBoolAttributeWhenTrue(
    XmlElement& element,
    std::string_view attributeName,
    bool value)
{
    if (value) {
        element.SetAttribute(attributeName, "1");
    }
}

// Sets a string attribute when the trimmed value is non-blank.
// Matches C# SetStringAttributeWhenPresent (IsNullOrWhiteSpace check).
void SetStringAttributeWhenPresent(
    XmlElement& element,
    std::string_view attributeName,
    const std::string& value)
{
    if (!IsBlankAfterTrim(value)) {
        element.SetAttribute(attributeName, std::string(value));
    }
}

}  // anonymous namespace

// ---------------------------------------------------------------------------
// BuildSheetProtectionElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookWorksheetProtection::BuildSheetProtectionElement(
    WorksheetModel& worksheet)
{
    auto& protection = worksheet.GetProtection();
    if (!protection.HasStoredState()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(MainNs + "sheetProtection");
    element.SetAttribute("sheet", "1");
    SetBoolAttributeWhenTrue(element, "objects", protection.GetObjects());
    SetBoolAttributeWhenTrue(element, "scenarios", protection.GetScenarios());
    SetBoolAttributeWhenTrue(element, "formatCells", protection.GetFormatCells());
    SetBoolAttributeWhenTrue(element, "formatColumns", protection.GetFormatColumns());
    SetBoolAttributeWhenTrue(element, "formatRows", protection.GetFormatRows());
    SetBoolAttributeWhenTrue(element, "insertColumns", protection.GetInsertColumns());
    SetBoolAttributeWhenTrue(element, "insertRows", protection.GetInsertRows());
    SetBoolAttributeWhenTrue(element, "insertHyperlinks", protection.GetInsertHyperlinks());
    SetBoolAttributeWhenTrue(element, "deleteColumns", protection.GetDeleteColumns());
    SetBoolAttributeWhenTrue(element, "deleteRows", protection.GetDeleteRows());
    SetBoolAttributeWhenTrue(element, "selectLockedCells", protection.GetSelectLockedCells());
    SetBoolAttributeWhenTrue(element, "sort", protection.GetSort());
    SetBoolAttributeWhenTrue(element, "autoFilter", protection.GetAutoFilter());
    SetBoolAttributeWhenTrue(element, "pivotTables", protection.GetPivotTables());
    SetBoolAttributeWhenTrue(element, "selectUnlockedCells", protection.GetSelectUnlockedCells());
    SetStringAttributeWhenPresent(element, "password", protection.GetPasswordHash());
    SetStringAttributeWhenPresent(element, "algorithmName", protection.GetAlgorithmName());
    SetStringAttributeWhenPresent(element, "hashValue", protection.GetHashValue());
    SetStringAttributeWhenPresent(element, "saltValue", protection.GetSaltValue());
    SetStringAttributeWhenPresent(element, "spinCount", protection.GetSpinCount());
    return element;
}

// ---------------------------------------------------------------------------
// LoadWorksheetProtection
// ---------------------------------------------------------------------------

void XlsxWorkbookWorksheetProtection::LoadWorksheetProtection(
    WorksheetModel& worksheetModel,
    const XmlElement& worksheetRoot,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    worksheetModel.GetProtection().Clear();

    auto protectionElement = worksheetRoot.GetElement(MainNs + "sheetProtection");
    if (protectionElement.IsNull()) {
        return;
    }

    auto& protection = worksheetModel.GetProtection();

    protection.SetIsProtected(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("sheet"), true,
        diagnostics, options, sheetName, "sheet"));
    protection.SetObjects(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("objects"), false,
        diagnostics, options, sheetName, "objects"));
    protection.SetScenarios(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("scenarios"), false,
        diagnostics, options, sheetName, "scenarios"));
    protection.SetFormatCells(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("formatCells"), false,
        diagnostics, options, sheetName, "formatCells"));
    protection.SetFormatColumns(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("formatColumns"), false,
        diagnostics, options, sheetName, "formatColumns"));
    protection.SetFormatRows(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("formatRows"), false,
        diagnostics, options, sheetName, "formatRows"));
    protection.SetInsertColumns(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("insertColumns"), false,
        diagnostics, options, sheetName, "insertColumns"));
    protection.SetInsertRows(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("insertRows"), false,
        diagnostics, options, sheetName, "insertRows"));
    protection.SetInsertHyperlinks(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("insertHyperlinks"), false,
        diagnostics, options, sheetName, "insertHyperlinks"));
    protection.SetDeleteColumns(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("deleteColumns"), false,
        diagnostics, options, sheetName, "deleteColumns"));
    protection.SetDeleteRows(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("deleteRows"), false,
        diagnostics, options, sheetName, "deleteRows"));
    protection.SetSelectLockedCells(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("selectLockedCells"), false,
        diagnostics, options, sheetName, "selectLockedCells"));
    protection.SetSort(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("sort"), false,
        diagnostics, options, sheetName, "sort"));
    protection.SetAutoFilter(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("autoFilter"), false,
        diagnostics, options, sheetName, "autoFilter"));
    protection.SetPivotTables(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("pivotTables"), false,
        diagnostics, options, sheetName, "pivotTables"));
    protection.SetSelectUnlockedCells(ParseProtectionBooleanAttribute(
        protectionElement.GetAttribute("selectUnlockedCells"), false,
        diagnostics, options, sheetName, "selectUnlockedCells"));

    auto passwordHash = ReadOptionalAttribute(protectionElement, "password");
    if (passwordHash.has_value()) {
        protection.SetPasswordHash(std::move(passwordHash.value()));
    }
    auto algorithmName = ReadOptionalAttribute(protectionElement, "algorithmName");
    if (algorithmName.has_value()) {
        protection.SetAlgorithmName(std::move(algorithmName.value()));
    }
    auto hashValue = ReadOptionalAttribute(protectionElement, "hashValue");
    if (hashValue.has_value()) {
        protection.SetHashValue(std::move(hashValue.value()));
    }
    auto saltValue = ReadOptionalAttribute(protectionElement, "saltValue");
    if (saltValue.has_value()) {
        protection.SetSaltValue(std::move(saltValue.value()));
    }
    auto spinCount = ReadOptionalAttribute(protectionElement, "spinCount");
    if (spinCount.has_value()) {
        protection.SetSpinCount(std::move(spinCount.value()));
    }
}

}}  // namespace Aspose::Cells_FOSS
