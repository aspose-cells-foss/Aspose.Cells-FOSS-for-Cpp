#pragma once

#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/core/ColorValue.h"

#include <optional>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {
class PageSetupModel;
class WorksheetModel;
class WorksheetViewModel;
}  // namespace Core

/// Provides static methods for building and loading worksheet view settings
/// (sheet properties, sheet views) in the XLSX workbook serializer.
class XlsxWorkbookWorksheetViews {
public:
    XlsxWorkbookWorksheetViews() = delete;

    /// Builds the sheetPr XML element for a worksheet, including tab color
    /// and page-setup properties. Returns a null XmlElement when neither
    /// is present.
    static XmlElement BuildWorksheetSheetProperties(
        const Core::WorksheetModel& worksheet);

    /// Builds the sheetViews XML element for a worksheet when the view is
    /// not the default. Returns a null XmlElement when the view is default.
    static XmlElement BuildWorksheetViewsElement(
        const Core::WorksheetModel& worksheet);

    /// Loads worksheet view settings (tab color and sheet view) from the
    /// worksheet XML root into the model.
    static void LoadWorksheetViewSettings(
        Core::WorksheetModel& worksheetModel,
        const XmlElement& worksheetRoot,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

private:
    static XmlElement BuildPageSetupPropertiesElement(
        const Core::PageSetupModel& pageSetup);

    static XmlElement BuildTabColorElement(
        const std::optional<Core::ColorValue>& color);

    static bool WorksheetViewIsDefault(
        const Core::WorksheetViewModel& view);

    static void LoadTabColor(
        Core::WorksheetModel& worksheetModel,
        const XmlElement& sheetPropertiesElement,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

    static void LoadSheetView(
        Core::WorksheetModel& worksheetModel,
        const XmlElement& sheetViewElement,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

    static bool ParseBooleanViewAttribute(
        const XmlAttribute& attribute,
        bool defaultValue,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view attributeName);

    static int ParseZoomScale(
        const XmlAttribute& attribute,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

    static std::string FormatColor(const Core::ColorValue& color);

    static bool TryParseColor(std::string_view value, Core::ColorValue& color);
};

}}  // namespace Aspose::Cells_FOSS
