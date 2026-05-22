#include "aspose/cells_foss/XlsxWorkbookProperties.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/InvalidFileFormatException.h"
#include "aspose/cells_foss/WorkbookPropertySupport.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"
#include "aspose/cells_foss/core/CalculationPropertiesModel.h"
#include "aspose/cells_foss/core/DateSystem.h"
#include "aspose/cells_foss/core/WorkbookModel.h"
#include "aspose/cells_foss/core/WorkbookPropertiesModel.h"
#include "aspose/cells_foss/core/WorkbookProtectionModel.h"
#include "aspose/cells_foss/core/WorkbookSettingsModel.h"
#include "aspose/cells_foss/core/WorkbookViewModel.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <utility>

namespace Aspose {
namespace Cells_FOSS {

using Core::CalculationPropertiesModel;
using Core::DateSystem;
using Core::WorkbookModel;
using Core::WorkbookPropertiesModel;
using Core::WorkbookProtectionModel;
using Core::WorkbookSettingsModel;
using Core::WorkbookViewModel;

// ---------------------------------------------------------------------------
// Static namespace alias
// ---------------------------------------------------------------------------

static const XNamespace& MainNs = XlsxWorkbookSerializerCommon::MainNs;

// ---------------------------------------------------------------------------
// String helpers
// ---------------------------------------------------------------------------

static bool EqualsIgnoreCase(std::string_view left, std::string_view right)
{
    if (left.size() != right.size()) {
        return false;
    }
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(left[i])) !=
            std::tolower(static_cast<unsigned char>(right[i]))) {
            return false;
        }
    }
    return true;
}

static bool IsNullOrEmpty(std::string_view value)
{
    return value.empty();
}

// Trim edge whitespace only (matching C# .Trim())
static std::string TrimEdge(std::string_view value)
{
    auto start = value.find_first_not_of(" \t\n\r");
    if (start == std::string_view::npos) {
        return std::string();
    }
    auto end = value.find_last_not_of(" \t\n\r");
    return std::string(value.substr(start, end - start + 1));
}

// ---------------------------------------------------------------------------
// FormatDecimalFraction
// ---------------------------------------------------------------------------

std::string XlsxWorkbookProperties::FormatDecimalFraction(double value)
{
    char buf[128];
    const int len = std::snprintf(buf, sizeof(buf), "%.16f", value);
    if (len < 0 || static_cast<std::size_t>(len) >= sizeof(buf)) {
        return std::to_string(value);
    }

    std::string result(buf, static_cast<std::size_t>(len));
    const auto dotPos = result.find('.');
    if (dotPos != std::string::npos) {
        const auto lastNonZero = result.find_last_not_of('0');
        if (lastNonZero == dotPos) {
            result.erase(dotPos);
        } else {
            result.erase(lastNonZero + 1);
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
// BuildWorkbookPropertiesElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookProperties::BuildWorkbookPropertiesElement(
    const WorkbookModel& model)
{
    auto element = XmlElement::MakeElement(MainNs + "workbookPr");
    bool hasState = false;

    if (model.GetSettings().GetDateSystem() == DateSystem::Mac1904) {
        element.SetAttribute("date1904", "1");
        hasState = true;
    }

    const auto& properties = model.GetProperties();

    if (!IsNullOrEmpty(properties.GetCodeName())) {
        element.SetAttribute("codeName", properties.GetCodeName());
        hasState = true;
    }

    if (!IsNullOrEmpty(properties.GetShowObjects()) &&
        !EqualsIgnoreCase(properties.GetShowObjects(), "all")) {
        element.SetAttribute("showObjects", properties.GetShowObjects());
        hasState = true;
    }

    if (properties.GetFilterPrivacy()) {
        element.SetAttribute("filterPrivacy", "1");
        hasState = true;
    }

    if (!properties.GetShowBorderUnselectedTables()) {
        element.SetAttribute("showBorderUnselectedTables", "0");
        hasState = true;
    }

    if (!properties.GetShowInkAnnotation()) {
        element.SetAttribute("showInkAnnotation", "0");
        hasState = true;
    }

    if (properties.GetBackupFile()) {
        element.SetAttribute("backupFile", "1");
        hasState = true;
    }

    if (!properties.GetSaveExternalLinkValues()) {
        element.SetAttribute("saveExternalLinkValues", "0");
        hasState = true;
    }

    if (!IsNullOrEmpty(properties.GetUpdateLinks()) &&
        !EqualsIgnoreCase(properties.GetUpdateLinks(), "userSet")) {
        element.SetAttribute("updateLinks", properties.GetUpdateLinks());
        hasState = true;
    }

    if (properties.GetHidePivotFieldList()) {
        element.SetAttribute("hidePivotFieldList", "1");
        hasState = true;
    }

    if (properties.GetDefaultThemeVersion().has_value()) {
        element.SetAttribute("defaultThemeVersion",
            std::to_string(properties.GetDefaultThemeVersion().value()));
        hasState = true;
    }

    return hasState ? element : XmlElement();
}

// ---------------------------------------------------------------------------
// BuildWorkbookProtectionElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookProperties::BuildWorkbookProtectionElement(
    const WorkbookModel& model)
{
    const auto& protection = model.GetProperties().GetProtection();
    if (!protection.HasStoredState()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(MainNs + "workbookProtection");

    if (protection.GetLockStructure()) {
        element.SetAttribute("lockStructure", "1");
    }

    if (protection.GetLockWindows()) {
        element.SetAttribute("lockWindows", "1");
    }

    if (protection.GetLockRevision()) {
        element.SetAttribute("lockRevision", "1");
    }

    if (!IsNullOrEmpty(protection.GetWorkbookPassword())) {
        element.SetAttribute("workbookPassword", protection.GetWorkbookPassword());
    }

    if (!IsNullOrEmpty(protection.GetRevisionsPassword())) {
        element.SetAttribute("revisionsPassword", protection.GetRevisionsPassword());
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildBookViewsElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookProperties::BuildBookViewsElement(
    const WorkbookModel& model)
{
    const auto& view = model.GetProperties().GetView();
    if (!view.HasStoredState(model.GetActiveSheetIndex())) {
        return XmlElement();
    }

    auto workbookView = XmlElement::MakeElement(MainNs + "workbookView");

    if (view.GetXWindow().has_value()) {
        workbookView.SetAttribute("xWindow",
            std::to_string(view.GetXWindow().value()));
    }

    if (view.GetYWindow().has_value()) {
        workbookView.SetAttribute("yWindow",
            std::to_string(view.GetYWindow().value()));
    }

    if (view.GetWindowWidth().has_value()) {
        workbookView.SetAttribute("windowWidth",
            std::to_string(view.GetWindowWidth().value()));
    }

    if (view.GetWindowHeight().has_value()) {
        workbookView.SetAttribute("windowHeight",
            std::to_string(view.GetWindowHeight().value()));
    }

    const auto& worksheets = model.GetWorksheets();
    if (model.GetActiveSheetIndex() > 0 &&
        model.GetActiveSheetIndex() < static_cast<int>(worksheets.size())) {
        workbookView.SetAttribute("activeTab",
            std::to_string(model.GetActiveSheetIndex()));
    }

    if (view.GetFirstSheet().has_value()) {
        int firstSheet = view.GetFirstSheet().value();
        if (!worksheets.empty() &&
            firstSheet >= static_cast<int>(worksheets.size())) {
            firstSheet = static_cast<int>(worksheets.size()) - 1;
        }
        if (firstSheet < 0) {
            firstSheet = 0;
        }
        workbookView.SetAttribute("firstSheet", std::to_string(firstSheet));
    }

    if (view.GetShowHorizontalScroll().has_value()) {
        workbookView.SetAttribute("showHorizontalScroll",
            view.GetShowHorizontalScroll().value() ? "1" : "0");
    }

    if (view.GetShowVerticalScroll().has_value()) {
        workbookView.SetAttribute("showVerticalScroll",
            view.GetShowVerticalScroll().value() ? "1" : "0");
    }

    if (view.GetShowSheetTabs().has_value()) {
        workbookView.SetAttribute("showSheetTabs",
            view.GetShowSheetTabs().value() ? "1" : "0");
    }

    if (view.GetTabRatio().has_value()) {
        workbookView.SetAttribute("tabRatio",
            std::to_string(view.GetTabRatio().value()));
    }

    if (!IsNullOrEmpty(view.GetVisibility()) &&
        !EqualsIgnoreCase(view.GetVisibility(), "visible")) {
        workbookView.SetAttribute("visibility", view.GetVisibility());
    }

    if (view.GetMinimized()) {
        workbookView.SetAttribute("minimized", "1");
    }

    if (!view.GetAutoFilterDateGrouping()) {
        workbookView.SetAttribute("autoFilterDateGrouping", "0");
    }

    return XmlElement::MakeElement(
        MainNs + "bookViews",
        std::vector<std::pair<std::string, std::string>>{},
        std::vector<XmlElement>{std::move(workbookView)});
}

// ---------------------------------------------------------------------------
// BuildCalculationPropertiesElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookProperties::BuildCalculationPropertiesElement(
    const WorkbookModel& model)
{
    const auto& calculation = model.GetProperties().GetCalculation();
    if (!calculation.HasStoredState()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(MainNs + "calcPr");

    if (calculation.GetCalculationId().has_value()) {
        element.SetAttribute("calcId",
            std::to_string(calculation.GetCalculationId().value()));
    }

    if (!IsNullOrEmpty(calculation.GetCalculationMode()) &&
        !EqualsIgnoreCase(calculation.GetCalculationMode(), "auto")) {
        element.SetAttribute("calcMode", calculation.GetCalculationMode());
    }

    if (calculation.GetFullCalculationOnLoad()) {
        element.SetAttribute("fullCalcOnLoad", "1");
    }

    if (!IsNullOrEmpty(calculation.GetReferenceMode()) &&
        !EqualsIgnoreCase(calculation.GetReferenceMode(), "A1")) {
        element.SetAttribute("refMode", calculation.GetReferenceMode());
    }

    if (calculation.GetIterate()) {
        element.SetAttribute("iterate", "1");
    }

    if (calculation.GetIterateCount().has_value()) {
        element.SetAttribute("iterateCount",
            std::to_string(calculation.GetIterateCount().value()));
    }

    if (calculation.GetIterateDelta().has_value()) {
        element.SetAttribute("iterateDelta",
            FormatDecimalFraction(calculation.GetIterateDelta().value()));
    }

    if (calculation.GetFullPrecision().has_value()) {
        element.SetAttribute("fullPrecision",
            calculation.GetFullPrecision().value() ? "1" : "0");
    }

    if (calculation.GetCalculationCompleted().has_value()) {
        element.SetAttribute("calcCompleted",
            calculation.GetCalculationCompleted().value() ? "1" : "0");
    }

    if (calculation.GetCalculationOnSave().has_value()) {
        element.SetAttribute("calcOnSave",
            calculation.GetCalculationOnSave().value() ? "1" : "0");
    }

    if (calculation.GetConcurrentCalculation().has_value()) {
        element.SetAttribute("concurrentCalc",
            calculation.GetConcurrentCalculation().value() ? "1" : "0");
    }

    if (calculation.GetForceFullCalculation()) {
        element.SetAttribute("forceFullCalc", "1");
    }

    return element;
}

// ---------------------------------------------------------------------------
// LoadWorkbookMetadata
// ---------------------------------------------------------------------------

void XlsxWorkbookProperties::LoadWorkbookMetadata(
    const XmlElement& workbookRoot,
    WorkbookModel& workbookModel,
    int sheetCount,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options)
{
    auto& workbookProperties = workbookModel.GetProperties();
    const auto workbookPr = workbookRoot.GetElement(MainNs + "workbookPr");

    if (workbookPr.IsNull()) {
        workbookModel.GetSettings().SetDateSystem(DateSystem::Windows1900);
    } else {
        workbookModel.GetSettings().SetDateSystem(
            ReadBoolAttribute(workbookPr, "date1904", diagnostics, options,
                              false, "/xl/workbook.xml")
                ? DateSystem::Mac1904
                : DateSystem::Windows1900);

        workbookProperties.SetCodeName(
            ReadStringAttribute(workbookPr, "codeName"));

        workbookProperties.SetShowObjects(ReadChoiceAttribute(
            workbookPr, "showObjects", diagnostics, options,
            "/xl/workbook.xml", WorkbookPropertySupport::NormalizeShowObjects));

        workbookProperties.SetFilterPrivacy(ReadBoolAttribute(
            workbookPr, "filterPrivacy", diagnostics, options,
            false, "/xl/workbook.xml"));

        workbookProperties.SetShowBorderUnselectedTables(ReadBoolAttribute(
            workbookPr, "showBorderUnselectedTables", diagnostics, options,
            true, "/xl/workbook.xml"));

        workbookProperties.SetShowInkAnnotation(ReadBoolAttribute(
            workbookPr, "showInkAnnotation", diagnostics, options,
            true, "/xl/workbook.xml"));

        workbookProperties.SetBackupFile(ReadBoolAttribute(
            workbookPr, "backupFile", diagnostics, options,
            false, "/xl/workbook.xml"));

        workbookProperties.SetSaveExternalLinkValues(ReadBoolAttribute(
            workbookPr, "saveExternalLinkValues", diagnostics, options,
            true, "/xl/workbook.xml"));

        workbookProperties.SetUpdateLinks(ReadChoiceAttribute(
            workbookPr, "updateLinks", diagnostics, options,
            "/xl/workbook.xml", WorkbookPropertySupport::NormalizeUpdateLinks));

        workbookProperties.SetHidePivotFieldList(ReadBoolAttribute(
            workbookPr, "hidePivotFieldList", diagnostics, options,
            false, "/xl/workbook.xml"));

        workbookProperties.SetDefaultThemeVersion(ReadNonNegativeIntAttribute(
            workbookPr, "defaultThemeVersion", diagnostics, options,
            "/xl/workbook.xml"));
    }

    const auto protection =
        workbookRoot.GetElement(MainNs + "workbookProtection");
    if (!protection.IsNull()) {
        auto& prot = workbookProperties.GetProtection();
        prot.SetLockStructure(ReadBoolAttribute(
            protection, "lockStructure", diagnostics, options,
            false, "/xl/workbook.xml"));
        prot.SetLockWindows(ReadBoolAttribute(
            protection, "lockWindows", diagnostics, options,
            false, "/xl/workbook.xml"));
        prot.SetLockRevision(ReadBoolAttribute(
            protection, "lockRevision", diagnostics, options,
            false, "/xl/workbook.xml"));
        prot.SetWorkbookPassword(
            ReadStringAttribute(protection, "workbookPassword"));
        prot.SetRevisionsPassword(
            ReadStringAttribute(protection, "revisionsPassword"));
    }

    const auto bookViews =
        workbookRoot.GetElement(MainNs + "bookViews");
    const auto workbookView =
        bookViews.IsNull()
            ? XmlElement()
            : bookViews.GetElement(MainNs + "workbookView");

    if (!workbookView.IsNull()) {
        auto& view = workbookProperties.GetView();

        view.SetXWindow(ReadNonNegativeIntAttribute(
            workbookView, "xWindow", diagnostics, options, "/xl/workbook.xml"));
        view.SetYWindow(ReadNonNegativeIntAttribute(
            workbookView, "yWindow", diagnostics, options, "/xl/workbook.xml"));
        view.SetWindowWidth(ReadNonNegativeIntAttribute(
            workbookView, "windowWidth", diagnostics, options,
            "/xl/workbook.xml"));
        view.SetWindowHeight(ReadNonNegativeIntAttribute(
            workbookView, "windowHeight", diagnostics, options,
            "/xl/workbook.xml"));

        auto firstSheet = ReadNonNegativeIntAttribute(
            workbookView, "firstSheet", diagnostics, options,
            "/xl/workbook.xml");
        if (firstSheet.has_value()) {
            if (firstSheet.value() >= sheetCount) {
                AddWorkbookMetadataIssue(
                    diagnostics, options, "/xl/workbook.xml",
                    "Workbook firstSheet exceeded the worksheet count and was "
                    "clamped.");
                firstSheet = sheetCount > 0 ? sheetCount - 1 : 0;
            }
            view.SetFirstSheet(firstSheet);
        }

        view.SetShowHorizontalScroll(ReadNullableBoolAttribute(
            workbookView, "showHorizontalScroll", diagnostics, options,
            "/xl/workbook.xml"));
        view.SetShowVerticalScroll(ReadNullableBoolAttribute(
            workbookView, "showVerticalScroll", diagnostics, options,
            "/xl/workbook.xml"));
        view.SetShowSheetTabs(ReadNullableBoolAttribute(
            workbookView, "showSheetTabs", diagnostics, options,
            "/xl/workbook.xml"));

        auto tabRatio = ReadNonNegativeIntAttribute(
            workbookView, "tabRatio", diagnostics, options,
            "/xl/workbook.xml");
        if (tabRatio.has_value()) {
            if (tabRatio.value() > 1000) {
                AddWorkbookMetadataIssue(
                    diagnostics, options, "/xl/workbook.xml",
                    "Workbook tabRatio was out of range and was ignored.");
            } else {
                view.SetTabRatio(tabRatio);
            }
        }

        view.SetVisibility(ReadChoiceAttribute(
            workbookView, "visibility", diagnostics, options,
            "/xl/workbook.xml", WorkbookPropertySupport::NormalizeVisibility));

        view.SetMinimized(ReadBoolAttribute(
            workbookView, "minimized", diagnostics, options,
            false, "/xl/workbook.xml"));

        view.SetAutoFilterDateGrouping(ReadBoolAttribute(
            workbookView, "autoFilterDateGrouping", diagnostics, options,
            true, "/xl/workbook.xml"));

        auto activeTab = ReadNonNegativeIntAttribute(
            workbookView, "activeTab", diagnostics, options,
            "/xl/workbook.xml");
        if (activeTab.has_value()) {
            if (activeTab.value() >= sheetCount) {
                AddWorkbookMetadataIssue(
                    diagnostics, options, "/xl/workbook.xml",
                    "Workbook activeTab exceeded the worksheet count and was "
                    "ignored.");
            } else {
                workbookModel.SetActiveSheetIndex(activeTab.value());
            }
        }
    }

    const auto calcPr = workbookRoot.GetElement(MainNs + "calcPr");
    if (!calcPr.IsNull()) {
        auto& calculation = workbookProperties.GetCalculation();

        calculation.SetCalculationId(ReadNonNegativeIntAttribute(
            calcPr, "calcId", diagnostics, options, "/xl/workbook.xml"));

        calculation.SetCalculationMode(ReadChoiceAttribute(
            calcPr, "calcMode", diagnostics, options, "/xl/workbook.xml",
            WorkbookPropertySupport::NormalizeCalculationMode));

        calculation.SetFullCalculationOnLoad(ReadBoolAttribute(
            calcPr, "fullCalcOnLoad", diagnostics, options,
            false, "/xl/workbook.xml"));

        calculation.SetReferenceMode(ReadChoiceAttribute(
            calcPr, "refMode", diagnostics, options, "/xl/workbook.xml",
            WorkbookPropertySupport::NormalizeReferenceMode));

        calculation.SetIterate(ReadBoolAttribute(
            calcPr, "iterate", diagnostics, options,
            false, "/xl/workbook.xml"));

        calculation.SetIterateCount(ReadNonNegativeIntAttribute(
            calcPr, "iterateCount", diagnostics, options,
            "/xl/workbook.xml"));

        calculation.SetIterateDelta(ReadNonNegativeDoubleAttribute(
            calcPr, "iterateDelta", diagnostics, options,
            "/xl/workbook.xml"));

        calculation.SetFullPrecision(ReadNullableBoolAttribute(
            calcPr, "fullPrecision", diagnostics, options,
            "/xl/workbook.xml"));

        calculation.SetCalculationCompleted(ReadNullableBoolAttribute(
            calcPr, "calcCompleted", diagnostics, options,
            "/xl/workbook.xml"));

        calculation.SetCalculationOnSave(ReadNullableBoolAttribute(
            calcPr, "calcOnSave", diagnostics, options,
            "/xl/workbook.xml"));

        calculation.SetConcurrentCalculation(ReadNullableBoolAttribute(
            calcPr, "concurrentCalc", diagnostics, options,
            "/xl/workbook.xml"));

        calculation.SetForceFullCalculation(ReadBoolAttribute(
            calcPr, "forceFullCalc", diagnostics, options,
            false, "/xl/workbook.xml"));
    }
}

// ---------------------------------------------------------------------------
// Private: ReadStringAttribute
// ---------------------------------------------------------------------------

std::string XlsxWorkbookProperties::ReadStringAttribute(
    const XmlElement& element,
    std::string_view name)
{
    auto attr = element.GetAttribute(name);
    if (attr.IsNull()) {
        return std::string();
    }
    return TrimEdge(attr.GetValue());
}

// ---------------------------------------------------------------------------
// Private: ReadChoiceAttribute
// ---------------------------------------------------------------------------

std::string XlsxWorkbookProperties::ReadChoiceAttribute(
    const XmlElement& element,
    std::string_view attributeName,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view partUri,
    std::string (*normalizer)(std::string_view))
{
    auto attribute = element.GetAttribute(attributeName);
    if (attribute.IsNull()) {
        return std::string();
    }

    try {
        return normalizer(attribute.GetValue());
    } catch (const CellsException&) {
        AddWorkbookMetadataIssue(
            diagnostics, options, partUri,
            "Workbook metadata attribute '" + std::string(attributeName) +
                "' had an invalid value and was ignored.");
        return std::string();
    }
}

// ---------------------------------------------------------------------------
// Private: ReadBoolAttribute
// ---------------------------------------------------------------------------

bool XlsxWorkbookProperties::ReadBoolAttribute(
    const XmlElement& element,
    std::string_view attributeName,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    bool defaultValue,
    std::string_view partUri)
{
    auto value =
        ReadNullableBoolAttribute(element, attributeName, diagnostics, options,
                                  partUri);
    return value.value_or(defaultValue);
}

// ---------------------------------------------------------------------------
// Private: ReadNullableBoolAttribute
// ---------------------------------------------------------------------------

std::optional<bool> XlsxWorkbookProperties::ReadNullableBoolAttribute(
    const XmlElement& element,
    std::string_view attributeName,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view partUri)
{
    auto attribute = element.GetAttribute(attributeName);
    if (attribute.IsNull()) {
        return std::nullopt;
    }

    bool value = false;
    if (TryReadBoolean(attribute.GetValue(), value)) {
        return value;
    }

    AddWorkbookMetadataIssue(
        diagnostics, options, partUri,
        "Workbook metadata attribute '" + std::string(attributeName) +
            "' had an invalid Boolean value and was ignored.");
    return std::nullopt;
}

// ---------------------------------------------------------------------------
// Private: ReadNonNegativeIntAttribute
// ---------------------------------------------------------------------------

std::optional<int> XlsxWorkbookProperties::ReadNonNegativeIntAttribute(
    const XmlElement& element,
    std::string_view attributeName,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view partUri)
{
    auto attribute = element.GetAttribute(attributeName);
    if (attribute.IsNull()) {
        return std::nullopt;
    }

    auto parsed = XlsxWorkbookArchiveHelpers::ParseIntAttribute(attribute);
    if (parsed.has_value() && parsed.value() >= 0) {
        return parsed;
    }

    AddWorkbookMetadataIssue(
        diagnostics, options, partUri,
        "Workbook metadata attribute '" + std::string(attributeName) +
            "' had an invalid integer value and was ignored.");
    return std::nullopt;
}

// ---------------------------------------------------------------------------
// Private: ReadNonNegativeDoubleAttribute
// ---------------------------------------------------------------------------

std::optional<double> XlsxWorkbookProperties::ReadNonNegativeDoubleAttribute(
    const XmlElement& element,
    std::string_view attributeName,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view partUri)
{
    auto attribute = element.GetAttribute(attributeName);
    if (attribute.IsNull()) {
        return std::nullopt;
    }

    auto parsed = XlsxWorkbookArchiveHelpers::ParseDoubleAttribute(attribute);
    if (parsed.has_value() && parsed.value() >= 0.0) {
        return parsed;
    }

    AddWorkbookMetadataIssue(
        diagnostics, options, partUri,
        "Workbook metadata attribute '" + std::string(attributeName) +
            "' had an invalid numeric value and was ignored.");
    return std::nullopt;
}

// ---------------------------------------------------------------------------
// Private: TryReadBoolean
// ---------------------------------------------------------------------------

bool XlsxWorkbookProperties::TryReadBoolean(
    std::string_view rawValue,
    bool& value)
{
    if (rawValue == "1" || EqualsIgnoreCase(rawValue, "true")) {
        value = true;
        return true;
    }

    if (rawValue == "0" || EqualsIgnoreCase(rawValue, "false")) {
        value = false;
        return true;
    }

    value = false;
    return false;
}

// ---------------------------------------------------------------------------
// Private: AddWorkbookMetadataIssue
// ---------------------------------------------------------------------------

void XlsxWorkbookProperties::AddWorkbookMetadataIssue(
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view partUri,
    std::string_view message)
{
    LoadIssue issue("WB-L003", DiagnosticSeverity::Warning, message);
    issue.SetPartUri(partUri);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
}

}}  // namespace Aspose::Cells_FOSS
