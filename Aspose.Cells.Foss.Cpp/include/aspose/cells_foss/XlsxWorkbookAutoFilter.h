#pragma once

#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/StylesheetLoadContext.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Provides static methods for building and loading auto-filter XML elements.
class XlsxWorkbookAutoFilter {
public:
    XlsxWorkbookAutoFilter() = delete;

    /// Builds the autoFilter XML element from the worksheet model.
    /// Returns a null XmlElement if the auto-filter has no stored state.
    static XmlElement BuildAutoFilterElement(
        Core::WorksheetModel& worksheet,
        int differentialFormatCount);

    /// Loads auto-filter data from the worksheet XML root into the model.
    static void LoadAutoFilter(
        Core::WorksheetModel& worksheetModel,
        const XmlElement& worksheetRoot,
        const StylesheetLoadContext& stylesheet,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

private:
    static XmlElement BuildFilterColumnElement(
        const Core::FilterColumnModel& model,
        int differentialFormatCount);

    static XmlElement BuildFiltersElement(const std::vector<std::string>& filters);

    static XmlElement BuildCustomFiltersElement(const Core::FilterColumnModel& model);

    static XmlElement BuildColorFilterElement(
        const Core::AutoFilterColorFilterModel& model,
        int differentialFormatCount);

    static XmlElement BuildDynamicFilterElement(const Core::AutoFilterDynamicFilterModel& model);

    static XmlElement BuildTop10Element(const Core::AutoFilterTop10Model& model);

    static XmlElement BuildSortStateElement(
        const Core::AutoFilterSortStateModel& model,
        int differentialFormatCount);

    static std::vector<Core::FilterColumnModel> GetOrderedFilterColumns(
        const std::vector<Core::FilterColumnModel>& columns);

    static std::optional<Core::FilterColumnModel> LoadFilterColumn(
        const XmlElement& filterColumnElement,
        const StylesheetLoadContext& stylesheet,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view autoFilterRange);

    static bool ParseFilterColumnHiddenButton(
        const XmlElement& filterColumnElement,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view autoFilterRange);

    static bool WarnAndReturnFalse(
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view autoFilterRange);

    static void LoadSimpleFilters(
        Core::FilterColumnModel& model,
        const XmlElement& filterColumnElement,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view autoFilterRange);

    static void LoadCustomFilters(
        Core::FilterColumnModel& model,
        const XmlElement& filterColumnElement,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view autoFilterRange);

    static void LoadColorFilter(
        Core::AutoFilterColorFilterModel& model,
        const XmlElement& colorFilterElement,
        const StylesheetLoadContext& stylesheet,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view autoFilterRange);

    static void LoadDynamicFilter(
        Core::AutoFilterDynamicFilterModel& model,
        const XmlElement& dynamicFilterElement,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view autoFilterRange);

    static void LoadTop10(
        Core::AutoFilterTop10Model& model,
        const XmlElement& top10Element,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view autoFilterRange);

    static void LoadSortState(
        Core::AutoFilterSortStateModel& model,
        const XmlElement& sortStateElement,
        const StylesheetLoadContext& stylesheet,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view autoFilterRange);

    static void WarnSortState(
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view autoFilterRange);

    static bool TryParseBooleanAttribute(
        const XmlAttribute& attribute,
        bool defaultValue,
        bool& parsedValue);

    static std::optional<double> ParseOptionalDouble(
        const XmlAttribute& attribute,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view autoFilterRange,
        std::string_view attributeLabel);

    static bool HasValidDifferentialStyleId(
        std::optional<int> differentialStyleId,
        int differentialFormatCount);

    static std::string NormalizeSortBy(std::string_view sortBy);

    static bool AllowsCustomList(std::string_view sortBy);

    static bool AllowsDifferentialStyle(std::string_view sortBy);

    static bool AllowsIconSort(
        const Core::AutoFilterSortConditionModel& condition,
        std::string_view sortBy);

    static bool AllowsIconSort(
        std::string_view sortBy,
        std::string_view iconSet);

    static std::string IntToString(int value);

    static std::string DoubleToString(double value);
};

}}  // namespace Aspose::Cells_FOSS
