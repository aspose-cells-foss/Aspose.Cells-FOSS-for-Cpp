#include "aspose/cells_foss/XlsxWorkbookWorksheetViews.h"
#include "aspose/cells_foss/InvalidFileFormatException.h"
#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadIssue.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/core/ColorValue.h"
#include "aspose/cells_foss/core/PageSetupModel.h"
#include "aspose/cells_foss/core/WorksheetModel.h"
#include "aspose/cells_foss/core/WorksheetViewModel.h"

#include <cctype>
#include <cstdio>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

using Core::ColorValue;
using Core::PageSetupModel;
using Core::WorksheetModel;
using Core::WorksheetViewModel;

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
// FormatColor
// ---------------------------------------------------------------------------

std::string XlsxWorkbookWorksheetViews::FormatColor(const ColorValue& color)
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
// TryParseColor
// ---------------------------------------------------------------------------

bool XlsxWorkbookWorksheetViews::TryParseColor(std::string_view value,
                                                ColorValue& color)
{
    if (value.size() != 8) {
        return false;
    }

    auto parseHexByte = [](std::string_view text, std::uint8_t& out) -> bool {
        if (text.size() != 2) {
            return false;
        }
        unsigned parsed = 0;
        for (char ch : text) {
            parsed *= 16;
            if (ch >= '0' && ch <= '9') {
                parsed += static_cast<unsigned>(ch - '0');
            } else if (ch >= 'A' && ch <= 'F') {
                parsed += static_cast<unsigned>(ch - 'A') + 10;
            } else if (ch >= 'a' && ch <= 'f') {
                parsed += static_cast<unsigned>(ch - 'a') + 10;
            } else {
                return false;
            }
        }
        if (parsed > 255) {
            return false;
        }
        out = static_cast<std::uint8_t>(parsed);
        return true;
    };

    std::uint8_t a = 0, r = 0, g = 0, b = 0;
    if (!parseHexByte(value.substr(0, 2), a) ||
        !parseHexByte(value.substr(2, 2), r) ||
        !parseHexByte(value.substr(4, 2), g) ||
        !parseHexByte(value.substr(6, 2), b)) {
        return false;
    }

    color = ColorValue(a, r, g, b);
    return true;
}

// ---------------------------------------------------------------------------
// WorksheetViewIsDefault
// ---------------------------------------------------------------------------

bool XlsxWorkbookWorksheetViews::WorksheetViewIsDefault(
    const WorksheetViewModel& view)
{
    return view.GetShowGridLines()
        && view.GetShowRowColumnHeaders()
        && view.GetShowZeros()
        && !view.GetRightToLeft()
        && view.GetZoomScale() == 100;
}

// ---------------------------------------------------------------------------
// BuildPageSetupPropertiesElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookWorksheetViews::BuildPageSetupPropertiesElement(
    const PageSetupModel& pageSetup)
{
    if (!pageSetup.GetFitToWidth().has_value() &&
        !pageSetup.GetFitToHeight().has_value()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(MainNs + "pageSetUpPr");
    element.SetAttribute("fitToPage", "1");
    return element;
}

// ---------------------------------------------------------------------------
// BuildTabColorElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookWorksheetViews::BuildTabColorElement(
    const std::optional<ColorValue>& color)
{
    if (!color.has_value()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(MainNs + "tabColor");
    element.SetAttribute("rgb", FormatColor(color.value()));
    return element;
}

// ---------------------------------------------------------------------------
// BuildWorksheetSheetProperties
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookWorksheetViews::BuildWorksheetSheetProperties(
    const WorksheetModel& worksheet)
{
    auto pageSetUpProperties = BuildPageSetupPropertiesElement(
        worksheet.GetPageSetup());
    auto tabColorElement = BuildTabColorElement(worksheet.GetTabColor());

    if (pageSetUpProperties.IsNull() && tabColorElement.IsNull()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(MainNs + "sheetPr");
    if (!tabColorElement.IsNull()) {
        element.AddChild(std::move(tabColorElement));
    }

    if (!pageSetUpProperties.IsNull()) {
        element.AddChild(std::move(pageSetUpProperties));
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildWorksheetViewsElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookWorksheetViews::BuildWorksheetViewsElement(
    const WorksheetModel& worksheet)
{
    if (WorksheetViewIsDefault(worksheet.GetView())) {
        return XmlElement();
    }

    auto sheetView = XmlElement::MakeElement(MainNs + "sheetView");
    sheetView.SetAttribute("workbookViewId", "0");

    if (!worksheet.GetView().GetShowGridLines()) {
        sheetView.SetAttribute("showGridLines", "0");
    }

    if (!worksheet.GetView().GetShowRowColumnHeaders()) {
        sheetView.SetAttribute("showRowColHeaders", "0");
    }

    if (!worksheet.GetView().GetShowZeros()) {
        sheetView.SetAttribute("showZeros", "0");
    }

    if (worksheet.GetView().GetRightToLeft()) {
        sheetView.SetAttribute("rightToLeft", "1");
    }

    if (worksheet.GetView().GetZoomScale() != 100) {
        sheetView.SetAttribute("zoomScale",
            std::to_string(worksheet.GetView().GetZoomScale()));
    }

    auto sheetViews = XmlElement::MakeElement(MainNs + "sheetViews");
    sheetViews.AddChild(std::move(sheetView));
    return sheetViews;
}

// ---------------------------------------------------------------------------
// LoadTabColor
// ---------------------------------------------------------------------------

void XlsxWorkbookWorksheetViews::LoadTabColor(
    WorksheetModel& worksheetModel,
    const XmlElement& sheetPropertiesElement,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    worksheetModel.SetTabColor(std::nullopt);
    if (sheetPropertiesElement.IsNull()) {
        return;
    }

    auto tabColor = sheetPropertiesElement.GetElement(MainNs + "tabColor");
    if (tabColor.IsNull()) {
        return;
    }

    auto rgbAttr = tabColor.GetAttribute("rgb");
    std::string rgb = rgbAttr.IsNull() ? std::string() : TrimEdge(rgbAttr.GetValue());
    if (rgb.empty()) {
        return;
    }

    ColorValue color;
    if (TryParseColor(rgb, color)) {
        worksheetModel.SetTabColor(color);
        return;
    }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException("Worksheet tab color is invalid.");
    }

    LoadIssue issue("WS-L007", DiagnosticSeverity::Warning,
                    "Worksheet tab color is invalid and was ignored.",
                    false, true);
    issue.SetSheetName(sheetName);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
}

// ---------------------------------------------------------------------------
// LoadSheetView
// ---------------------------------------------------------------------------

void XlsxWorkbookWorksheetViews::LoadSheetView(
    WorksheetModel& worksheetModel,
    const XmlElement& sheetViewElement,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    worksheetModel.GetView().SetShowGridLines(true);
    worksheetModel.GetView().SetShowRowColumnHeaders(true);
    worksheetModel.GetView().SetShowZeros(true);
    worksheetModel.GetView().SetRightToLeft(false);
    worksheetModel.GetView().SetZoomScale(100);

    if (sheetViewElement.IsNull()) {
        return;
    }

    worksheetModel.GetView().SetShowGridLines(
        ParseBooleanViewAttribute(
            sheetViewElement.GetAttribute("showGridLines"),
            true, diagnostics, options, sheetName, "showGridLines"));

    worksheetModel.GetView().SetShowRowColumnHeaders(
        ParseBooleanViewAttribute(
            sheetViewElement.GetAttribute("showRowColHeaders"),
            true, diagnostics, options, sheetName, "showRowColHeaders"));

    worksheetModel.GetView().SetShowZeros(
        ParseBooleanViewAttribute(
            sheetViewElement.GetAttribute("showZeros"),
            true, diagnostics, options, sheetName, "showZeros"));

    worksheetModel.GetView().SetRightToLeft(
        ParseBooleanViewAttribute(
            sheetViewElement.GetAttribute("rightToLeft"),
            false, diagnostics, options, sheetName, "rightToLeft"));

    worksheetModel.GetView().SetZoomScale(
        ParseZoomScale(
            sheetViewElement.GetAttribute("zoomScale"),
            diagnostics, options, sheetName));
}

// ---------------------------------------------------------------------------
// LoadWorksheetViewSettings
// ---------------------------------------------------------------------------

void XlsxWorkbookWorksheetViews::LoadWorksheetViewSettings(
    WorksheetModel& worksheetModel,
    const XmlElement& worksheetRoot,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    LoadTabColor(worksheetModel,
        worksheetRoot.GetElement(MainNs + "sheetPr"),
        diagnostics, options, sheetName);

    auto sheetViews = worksheetRoot.GetElement(MainNs + "sheetViews");
    auto sheetView = sheetViews.IsNull()
        ? XmlElement()
        : sheetViews.GetElement(MainNs + "sheetView");
    LoadSheetView(worksheetModel, sheetView, diagnostics, options, sheetName);
}

// ---------------------------------------------------------------------------
// ParseBooleanViewAttribute
// ---------------------------------------------------------------------------

bool XlsxWorkbookWorksheetViews::ParseBooleanViewAttribute(
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

    std::string rawValue = TrimEdge(attribute.GetValue());
    if (rawValue == "0" || EqualsIgnoreCase(rawValue, "false")) {
        return false;
    }

    if (rawValue == "1" || EqualsIgnoreCase(rawValue, "true")) {
        return true;
    }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            "Worksheet view attribute '" + std::string(attributeName) +
            "' is invalid.");
    }

    LoadIssue issue("WS-L008", DiagnosticSeverity::Warning,
        "Worksheet view attribute '" + std::string(attributeName) +
        "' is invalid and the default value was used.",
        false, true);
    issue.SetSheetName(sheetName);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return defaultValue;
}

// ---------------------------------------------------------------------------
// ParseZoomScale
// ---------------------------------------------------------------------------

int XlsxWorkbookWorksheetViews::ParseZoomScale(
    const XmlAttribute& attribute,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    if (attribute.IsNull()) {
        return 100;
    }

    auto value = XlsxWorkbookArchiveHelpers::ParseIntAttribute(attribute);
    if (value.has_value() && value.value() >= 10 && value.value() <= 400) {
        return value.value();
    }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException("Worksheet zoomScale is invalid.");
    }

    LoadIssue issue("WS-L008", DiagnosticSeverity::Warning,
        "Worksheet zoomScale is invalid and the default value was used.",
        false, true);
    issue.SetSheetName(sheetName);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return 100;
}

}}  // namespace Aspose::Cells_FOSS
