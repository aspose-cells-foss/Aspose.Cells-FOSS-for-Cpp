#include "aspose/cells_foss/XlsxWorkbookAutoFilter.h"
#include "aspose/cells_foss/AutoFilterSupport.h"
#include "aspose/cells_foss/InvalidFileFormatException.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdio>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

using Core::AutoFilterColorFilterModel;
using Core::AutoFilterCustomFilterModel;
using Core::AutoFilterDynamicFilterModel;
using Core::AutoFilterSortConditionModel;
using Core::AutoFilterSortStateModel;
using Core::AutoFilterTop10Model;
using Core::FilterColumnModel;
using Core::WorksheetModel;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static const XNamespace& MainNs = XlsxWorkbookSerializerCommon::MainNs;

std::string XlsxWorkbookAutoFilter::IntToString(int value)
{
    return std::to_string(value);
}

std::string XlsxWorkbookAutoFilter::DoubleToString(double value)
{
    char buf[64];
    const auto result = std::to_chars(buf, buf + sizeof(buf) - 1, value);
    if (result.ec == std::errc()) {
        return std::string(buf, result.ptr);
    }
    std::snprintf(buf, sizeof(buf), "%.17g", value);
    return std::string(buf);
}

// ---------------------------------------------------------------------------
// BuildAutoFilterElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookAutoFilter::BuildAutoFilterElement(
    WorksheetModel& worksheet,
    int differentialFormatCount)
{
    const auto& autoFilter = worksheet.GetAutoFilter();
    if (!autoFilter.HasStoredState() || autoFilter.GetRange().empty()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(
        MainNs + "autoFilter",
        std::vector<std::pair<std::string, std::string>>{{"ref", autoFilter.GetRange()}},
        std::vector<XmlElement>{});

    auto orderedColumns = GetOrderedFilterColumns(autoFilter.GetFilterColumns());
    for (std::size_t index = 0; index < orderedColumns.size(); ++index) {
        auto& filterColumn = orderedColumns[index];
        if (!filterColumn.HasStoredState()) {
            continue;
        }

        auto filterColumnElement = BuildFilterColumnElement(filterColumn, differentialFormatCount);
        if (!filterColumnElement.IsNull()) {
            element.AddChild(std::move(filterColumnElement));
        }
    }

    auto sortState = BuildSortStateElement(autoFilter.GetSortState(), differentialFormatCount);
    if (!sortState.IsNull()) {
        element.AddChild(std::move(sortState));
    }

    return element;
}

// ---------------------------------------------------------------------------
// LoadAutoFilter
// ---------------------------------------------------------------------------

void XlsxWorkbookAutoFilter::LoadAutoFilter(
    WorksheetModel& worksheetModel,
    const XmlElement& worksheetRoot,
    const StylesheetLoadContext& stylesheet,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    worksheetModel.GetAutoFilter().Clear();

    auto autoFilterElement = worksheetRoot.GetElement(MainNs + "autoFilter");
    if (autoFilterElement.IsNull()) {
        return;
    }

    const auto referenceAttr = autoFilterElement.GetAttribute("ref");
    const std::string reference = referenceAttr.GetValue();
    std::string normalizedRange;
    if (!AutoFilterSupport::TryNormalizeRange(reference, normalizedRange)) {
        if (options.GetStrictMode()) {
            throw InvalidFileFormatException(
                "The worksheet autoFilter ref '" + reference + "' is invalid.");
        }

        LoadIssue issue("WS-L010", DiagnosticSeverity::LossyRecoverable,
            "Worksheet autoFilter metadata had an invalid range and was dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        issue.SetCellRef(reference);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
        return;
    }

    worksheetModel.GetAutoFilter().SetRange(normalizedRange);

    std::unordered_set<int> seenColumnIndexes;
    const auto filterColumnElements = autoFilterElement.GetElements(MainNs + "filterColumn");
    for (const auto& filterColumnElement : filterColumnElements) {
        auto column = LoadFilterColumn(
            filterColumnElement, stylesheet, diagnostics, options, sheetName, normalizedRange);
        if (!column.has_value()) {
            continue;
        }

        if (!seenColumnIndexes.insert(column->GetColumnIndex()).second) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The worksheet autoFilter contains duplicate filter columns.");
            }

            LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
                "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
                true, true);
            issue.SetSheetName(sheetName);
            issue.SetCellRef(normalizedRange);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            continue;
        }

        worksheetModel.GetAutoFilter().GetFilterColumns().push_back(std::move(*column));
    }

    auto& columns = worksheetModel.GetAutoFilter().GetFilterColumns();
    std::sort(columns.begin(), columns.end(),
        [](const FilterColumnModel& left, const FilterColumnModel& right) {
            return AutoFilterSupport::CompareFilterColumns(left, right) < 0;
        });

    auto sortStateElement = autoFilterElement.GetElement(MainNs + "sortState");
    if (!sortStateElement.IsNull()) {
        LoadSortState(worksheetModel.GetAutoFilter().GetSortState(), sortStateElement,
                      stylesheet, diagnostics, options, sheetName, normalizedRange);
    }
}

// ---------------------------------------------------------------------------
// BuildFilterColumnElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookAutoFilter::BuildFilterColumnElement(
    const FilterColumnModel& model,
    int differentialFormatCount)
{
    auto customFilters = BuildCustomFiltersElement(model);
    auto filters = BuildFiltersElement(model.GetFilters());
    auto colorFilter = BuildColorFilterElement(model.GetColorFilter(), differentialFormatCount);
    auto dynamicFilter = BuildDynamicFilterElement(model.GetDynamicFilter());
    auto top10 = BuildTop10Element(model.GetTop10());

    if (!model.GetHiddenButton()
        && filters.IsNull()
        && customFilters.IsNull()
        && colorFilter.IsNull()
        && dynamicFilter.IsNull()
        && top10.IsNull()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(
        MainNs + "filterColumn",
        std::vector<std::pair<std::string, std::string>>{
            {"colId", IntToString(model.GetColumnIndex())}},
        std::vector<XmlElement>{});

    if (model.GetHiddenButton()) {
        element.SetAttribute("hiddenButton", "1");
    }

    if (!colorFilter.IsNull()) {
        element.AddChild(std::move(colorFilter));
    }

    if (!customFilters.IsNull()) {
        element.AddChild(std::move(customFilters));
    }

    if (!dynamicFilter.IsNull()) {
        element.AddChild(std::move(dynamicFilter));
    }

    if (!filters.IsNull()) {
        element.AddChild(std::move(filters));
    }

    if (!top10.IsNull()) {
        element.AddChild(std::move(top10));
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildFiltersElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookAutoFilter::BuildFiltersElement(const std::vector<std::string>& filters)
{
    if (filters.empty()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(MainNs + "filters");
    for (std::size_t index = 0; index < filters.size(); ++index) {
        auto filterElem = XmlElement::MakeElement(
            MainNs + "filter",
            std::vector<std::pair<std::string, std::string>>{{"val", filters[index]}},
            std::vector<XmlElement>{});
        element.AddChild(std::move(filterElem));
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildCustomFiltersElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookAutoFilter::BuildCustomFiltersElement(const FilterColumnModel& model)
{
    if (model.GetCustomFilters().empty()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(MainNs + "customFilters");
    if (model.GetCustomFiltersAnd()) {
        element.SetAttribute("and", "1");
    }

    for (std::size_t index = 0; index < model.GetCustomFilters().size(); ++index) {
        const auto& filter = model.GetCustomFilters()[index];
        auto customFilter = XmlElement::MakeElement(
            MainNs + "customFilter",
            std::vector<std::pair<std::string, std::string>>{{"val", filter.GetValue()}},
            std::vector<XmlElement>{});
        if (!filter.GetOperator().empty()) {
            customFilter.SetAttribute("operator", filter.GetOperator());
        }

        element.AddChild(std::move(customFilter));
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildColorFilterElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookAutoFilter::BuildColorFilterElement(
    const AutoFilterColorFilterModel& model,
    int differentialFormatCount)
{
    if (!model.GetEnabled() || !HasValidDifferentialStyleId(model.GetDifferentialStyleId(), differentialFormatCount)) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(MainNs + "colorFilter");
    element.SetAttribute("dxfId", IntToString(model.GetDifferentialStyleId().value()));

    if (model.GetCellColor()) {
        element.SetAttribute("cellColor", "1");
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildDynamicFilterElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookAutoFilter::BuildDynamicFilterElement(const AutoFilterDynamicFilterModel& model)
{
    if (!model.GetEnabled()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(MainNs + "dynamicFilter");
    if (!model.GetType().empty()) {
        element.SetAttribute("type", model.GetType());
    }

    if (model.GetValue().has_value()) {
        element.SetAttribute("val", DoubleToString(model.GetValue().value()));
    }

    if (model.GetMaxValue().has_value()) {
        element.SetAttribute("maxVal", DoubleToString(model.GetMaxValue().value()));
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildTop10Element
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookAutoFilter::BuildTop10Element(const AutoFilterTop10Model& model)
{
    if (!model.GetEnabled()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(MainNs + "top10");
    if (!model.GetTop()) {
        element.SetAttribute("top", "0");
    }

    if (model.GetPercent()) {
        element.SetAttribute("percent", "1");
    }

    if (model.GetValue().has_value()) {
        element.SetAttribute("val", DoubleToString(model.GetValue().value()));
    }

    if (model.GetFilterValue().has_value()) {
        element.SetAttribute("filterVal", DoubleToString(model.GetFilterValue().value()));
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildSortStateElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookAutoFilter::BuildSortStateElement(
    const AutoFilterSortStateModel& model,
    int differentialFormatCount)
{
    if (!model.HasStoredState() || model.GetRef().empty()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(
        MainNs + "sortState",
        std::vector<std::pair<std::string, std::string>>{{"ref", model.GetRef()}},
        std::vector<XmlElement>{});

    if (model.GetColumnSort()) {
        element.SetAttribute("columnSort", "1");
    }

    if (model.GetCaseSensitive()) {
        element.SetAttribute("caseSensitive", "1");
    }

    if (!model.GetSortMethod().empty()) {
        element.SetAttribute("sortMethod", model.GetSortMethod());
    }

    for (std::size_t index = 0; index < model.GetConditions().size(); ++index) {
        const auto& condition = model.GetConditions()[index];
        if (condition.GetRef().empty()) {
            continue;
        }

        auto conditionElement = XmlElement::MakeElement(
            MainNs + "sortCondition",
            std::vector<std::pair<std::string, std::string>>{{"ref", condition.GetRef()}},
            std::vector<XmlElement>{});

        const auto sortBy = NormalizeSortBy(condition.GetSortBy());
        if (condition.GetDescending()) {
            conditionElement.SetAttribute("descending", "1");
        }

        if (!sortBy.empty()) {
            conditionElement.SetAttribute("sortBy", sortBy);
        }

        if (AllowsCustomList(sortBy) && !condition.GetCustomList().empty()) {
            conditionElement.SetAttribute("customList", condition.GetCustomList());
        }

        if (AllowsDifferentialStyle(sortBy) &&
            HasValidDifferentialStyleId(condition.GetDifferentialStyleId(), differentialFormatCount)) {
            conditionElement.SetAttribute("dxfId", IntToString(condition.GetDifferentialStyleId().value()));
        }

        if (AllowsIconSort(condition, sortBy)) {
            conditionElement.SetAttribute("iconSet", condition.GetIconSet());
            conditionElement.SetAttribute("iconId", IntToString(condition.GetIconId().value()));
        }

        element.AddChild(std::move(conditionElement));
    }

    return element;
}

// ---------------------------------------------------------------------------
// GetOrderedFilterColumns
// ---------------------------------------------------------------------------

std::vector<FilterColumnModel> XlsxWorkbookAutoFilter::GetOrderedFilterColumns(
    const std::vector<FilterColumnModel>& columns)
{
    std::vector<FilterColumnModel> ordered(columns.begin(), columns.end());
    std::sort(ordered.begin(), ordered.end(),
        [](const FilterColumnModel& left, const FilterColumnModel& right) {
            return AutoFilterSupport::CompareFilterColumns(left, right) < 0;
        });
    return ordered;
}

// ---------------------------------------------------------------------------
// LoadFilterColumn
// ---------------------------------------------------------------------------

std::optional<FilterColumnModel> XlsxWorkbookAutoFilter::LoadFilterColumn(
    const XmlElement& filterColumnElement,
    const StylesheetLoadContext& stylesheet,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view autoFilterRange)
{
    const auto columnIndexAttr = filterColumnElement.GetAttribute("colId");
    const auto columnIndex = XlsxWorkbookArchiveHelpers::ParseIntAttribute(columnIndexAttr);
    if (!columnIndex.has_value() || columnIndex.value() < 0) {
        if (options.GetStrictMode()) {
            throw InvalidFileFormatException(
                "The worksheet autoFilter contains an invalid filter column id.");
        }

        LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
            "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        issue.SetCellRef(autoFilterRange);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
        return std::nullopt;
    }

    FilterColumnModel outModel;
    outModel.SetColumnIndex(columnIndex.value());
    outModel.SetHiddenButton(ParseFilterColumnHiddenButton(
        filterColumnElement, diagnostics, options, sheetName, autoFilterRange));
    LoadSimpleFilters(outModel, filterColumnElement, diagnostics, options, sheetName, autoFilterRange);
    LoadCustomFilters(outModel, filterColumnElement, diagnostics, options, sheetName, autoFilterRange);
    LoadColorFilter(outModel.GetColorFilter(),
                    filterColumnElement.GetElement(MainNs + "colorFilter"),
                    stylesheet, diagnostics, options, sheetName, autoFilterRange);
    LoadDynamicFilter(outModel.GetDynamicFilter(),
                      filterColumnElement.GetElement(MainNs + "dynamicFilter"),
                      diagnostics, options, sheetName, autoFilterRange);
    LoadTop10(outModel.GetTop10(),
              filterColumnElement.GetElement(MainNs + "top10"),
              diagnostics, options, sheetName, autoFilterRange);
    return outModel;
}

// ---------------------------------------------------------------------------
// ParseFilterColumnHiddenButton
// ---------------------------------------------------------------------------

bool XlsxWorkbookAutoFilter::ParseFilterColumnHiddenButton(
    const XmlElement& filterColumnElement,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view autoFilterRange)
{
    const auto hiddenButtonAttr = filterColumnElement.GetAttribute("hiddenButton");
    bool hiddenButton;
    if (TryParseBooleanAttribute(hiddenButtonAttr, false, hiddenButton)) {
        return hiddenButton;
    }

    if (!hiddenButtonAttr.IsNull()) {
        return WarnAndReturnFalse(diagnostics, options, sheetName, autoFilterRange);
    }

    const auto showButtonAttr = filterColumnElement.GetAttribute("showButton");
    bool showButton;
    if (TryParseBooleanAttribute(showButtonAttr, true, showButton)) {
        return !showButton;
    }

    if (!showButtonAttr.IsNull()) {
        return WarnAndReturnFalse(diagnostics, options, sheetName, autoFilterRange);
    }

    return false;
}

// ---------------------------------------------------------------------------
// WarnAndReturnFalse
// ---------------------------------------------------------------------------

bool XlsxWorkbookAutoFilter::WarnAndReturnFalse(
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view autoFilterRange)
{
    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            "The worksheet autoFilter contains an invalid filter column attribute.");
    }

    LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
        "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
        true, true);
    issue.SetSheetName(sheetName);
    issue.SetCellRef(autoFilterRange);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return false;
}

// ---------------------------------------------------------------------------
// LoadSimpleFilters
// ---------------------------------------------------------------------------

void XlsxWorkbookAutoFilter::LoadSimpleFilters(
    FilterColumnModel& model,
    const XmlElement& filterColumnElement,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view autoFilterRange)
{
    auto filtersElement = filterColumnElement.GetElement(MainNs + "filters");
    if (filtersElement.IsNull()) {
        return;
    }

    if (!filtersElement.GetElements(MainNs + "dateGroupItem").empty()) {
        if (options.GetStrictMode()) {
            throw InvalidFileFormatException(
                "The worksheet autoFilter contains unsupported date group filters.");
        }

        LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
            "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        issue.SetCellRef(autoFilterRange);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    }

    const auto filterElements = filtersElement.GetElements(MainNs + "filter");
    for (const auto& filterElement : filterElements) {
        const auto valueAttr = filterElement.GetAttribute("val");
        if (valueAttr.IsNull()) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The worksheet autoFilter contains a filter without a value.");
            }

            LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
                "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
                true, true);
            issue.SetSheetName(sheetName);
            issue.SetCellRef(autoFilterRange);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            continue;
        }

        model.GetFilters().push_back(valueAttr.GetValue());
    }
}

// ---------------------------------------------------------------------------
// LoadCustomFilters
// ---------------------------------------------------------------------------

void XlsxWorkbookAutoFilter::LoadCustomFilters(
    FilterColumnModel& model,
    const XmlElement& filterColumnElement,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view autoFilterRange)
{
    auto customFiltersElement = filterColumnElement.GetElement(MainNs + "customFilters");
    if (customFiltersElement.IsNull()) {
        return;
    }

    const auto andAttr = customFiltersElement.GetAttribute("and");
    bool andValue;
    if (TryParseBooleanAttribute(andAttr, false, andValue)) {
        model.SetCustomFiltersAnd(andValue);
    } else if (!andAttr.IsNull()) {
        if (options.GetStrictMode()) {
            throw InvalidFileFormatException(
                "The worksheet autoFilter contains an invalid custom filter 'and' attribute.");
        }

        LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
            "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        issue.SetCellRef(autoFilterRange);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    }

    const auto customFilterElements = customFiltersElement.GetElements(MainNs + "customFilter");
    for (const auto& customFilterElement : customFilterElements) {
        const auto valueAttr = customFilterElement.GetAttribute("val");
        if (valueAttr.IsNull()) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The worksheet autoFilter contains a custom filter without a value.");
            }

            LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
                "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
                true, true);
            issue.SetSheetName(sheetName);
            issue.SetCellRef(autoFilterRange);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            continue;
        }

        FilterOperatorType operatorType;
        const auto operatorAttr = customFilterElement.GetAttribute("operator");
        if (!AutoFilterSupport::TryParseOperator(operatorAttr.GetValue(), operatorType)) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The worksheet autoFilter contains an invalid custom filter operator.");
            }

            LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
                "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
                true, true);
            issue.SetSheetName(sheetName);
            issue.SetCellRef(autoFilterRange);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            continue;
        }

        AutoFilterCustomFilterModel filter;
        const auto opName = AutoFilterSupport::ToOperatorName(operatorType);
        filter.SetOperator(opName.empty() ? std::string() : opName);
        filter.SetValue(valueAttr.GetValue());
        model.GetCustomFilters().push_back(std::move(filter));
    }
}

// ---------------------------------------------------------------------------
// LoadColorFilter
// ---------------------------------------------------------------------------

void XlsxWorkbookAutoFilter::LoadColorFilter(
    AutoFilterColorFilterModel& model,
    const XmlElement& colorFilterElement,
    const StylesheetLoadContext& stylesheet,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view autoFilterRange)
{
    model.Clear();
    if (colorFilterElement.IsNull()) {
        return;
    }

    model.SetEnabled(true);
    const auto dxfIdAttr = colorFilterElement.GetAttribute("dxfId");
    const auto dxfId = XlsxWorkbookArchiveHelpers::ParseIntAttribute(dxfIdAttr);
    if (!dxfIdAttr.IsNull()) {
        if (HasValidDifferentialStyleId(dxfId, static_cast<int>(stylesheet.GetDifferentialFormats().size()))) {
            model.SetDifferentialStyleId(dxfId.value());
        } else {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The worksheet autoFilter contains an invalid color filter dxfId.");
            }

            LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
                "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
                true, true);
            issue.SetSheetName(sheetName);
            issue.SetCellRef(autoFilterRange);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            model.Clear();
            return;
        }
    }

    const auto cellColorAttr = colorFilterElement.GetAttribute("cellColor");
    bool cellColor;
    if (TryParseBooleanAttribute(cellColorAttr, false, cellColor)) {
        model.SetCellColor(cellColor);
    } else if (!cellColorAttr.IsNull()) {
        if (options.GetStrictMode()) {
            throw InvalidFileFormatException(
                "The worksheet autoFilter contains an invalid color filter cellColor attribute.");
        }

        LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
            "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        issue.SetCellRef(autoFilterRange);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    }
}

// ---------------------------------------------------------------------------
// LoadDynamicFilter
// ---------------------------------------------------------------------------

void XlsxWorkbookAutoFilter::LoadDynamicFilter(
    AutoFilterDynamicFilterModel& model,
    const XmlElement& dynamicFilterElement,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view autoFilterRange)
{
    model.Clear();
    if (dynamicFilterElement.IsNull()) {
        return;
    }

    model.SetEnabled(true);
    model.SetType(AutoFilterSupport::NormalizeOptionalText(
        dynamicFilterElement.GetAttribute("type").GetValue()));
    model.SetValue(ParseOptionalDouble(
        dynamicFilterElement.GetAttribute("val"),
        diagnostics, options, sheetName, autoFilterRange, "dynamic filter value"));
    model.SetMaxValue(ParseOptionalDouble(
        dynamicFilterElement.GetAttribute("maxVal"),
        diagnostics, options, sheetName, autoFilterRange, "dynamic filter max value"));
}

// ---------------------------------------------------------------------------
// LoadTop10
// ---------------------------------------------------------------------------

void XlsxWorkbookAutoFilter::LoadTop10(
    AutoFilterTop10Model& model,
    const XmlElement& top10Element,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view autoFilterRange)
{
    model.Clear();
    if (top10Element.IsNull()) {
        return;
    }

    model.SetEnabled(true);

    const auto topAttr = top10Element.GetAttribute("top");
    bool topValue;
    if (TryParseBooleanAttribute(topAttr, true, topValue)) {
        model.SetTop(topValue);
    } else if (!topAttr.IsNull()) {
        if (options.GetStrictMode()) {
            throw InvalidFileFormatException(
                "The worksheet autoFilter contains an invalid top10 top attribute.");
        }

        LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
            "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        issue.SetCellRef(autoFilterRange);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    }

    const auto percentAttr = top10Element.GetAttribute("percent");
    bool percentValue;
    if (TryParseBooleanAttribute(percentAttr, false, percentValue)) {
        model.SetPercent(percentValue);
    } else if (!percentAttr.IsNull()) {
        if (options.GetStrictMode()) {
            throw InvalidFileFormatException(
                "The worksheet autoFilter contains an invalid top10 percent attribute.");
        }

        LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
            "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        issue.SetCellRef(autoFilterRange);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    }

    model.SetValue(ParseOptionalDouble(
        top10Element.GetAttribute("val"),
        diagnostics, options, sheetName, autoFilterRange, "top10 value"));
    model.SetFilterValue(ParseOptionalDouble(
        top10Element.GetAttribute("filterVal"),
        diagnostics, options, sheetName, autoFilterRange, "top10 filter value"));
}

// ---------------------------------------------------------------------------
// LoadSortState
// ---------------------------------------------------------------------------

void XlsxWorkbookAutoFilter::LoadSortState(
    AutoFilterSortStateModel& model,
    const XmlElement& sortStateElement,
    const StylesheetLoadContext& stylesheet,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view autoFilterRange)
{
    model.Clear();

    const auto referenceAttr = sortStateElement.GetAttribute("ref");
    const std::string reference = referenceAttr.GetValue();
    std::string normalizedRange;
    if (!AutoFilterSupport::TryNormalizeRange(reference, normalizedRange)) {
        if (options.GetStrictMode()) {
            throw InvalidFileFormatException(
                "The worksheet autoFilter sortState ref is invalid.");
        }

        LoadIssue issue("WS-L012", DiagnosticSeverity::LossyRecoverable,
            "Worksheet autoFilter sort state was invalid and was dropped during load.",
            true, true);
        issue.SetSheetName(sheetName);
        issue.SetCellRef(autoFilterRange);
        XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
        return;
    }

    model.SetRef(normalizedRange);

    const auto columnSortAttr = sortStateElement.GetAttribute("columnSort");
    bool columnSort;
    if (TryParseBooleanAttribute(columnSortAttr, false, columnSort)) {
        model.SetColumnSort(columnSort);
    } else if (!columnSortAttr.IsNull()) {
        WarnSortState(diagnostics, options, sheetName, autoFilterRange);
    }

    const auto caseSensitiveAttr = sortStateElement.GetAttribute("caseSensitive");
    bool caseSensitive;
    if (TryParseBooleanAttribute(caseSensitiveAttr, false, caseSensitive)) {
        model.SetCaseSensitive(caseSensitive);
    } else if (!caseSensitiveAttr.IsNull()) {
        WarnSortState(diagnostics, options, sheetName, autoFilterRange);
    }

    model.SetSortMethod(AutoFilterSupport::NormalizeOptionalText(
        sortStateElement.GetAttribute("sortMethod").GetValue()));

    const auto sortConditionElements = sortStateElement.GetElements(MainNs + "sortCondition");
    for (const auto& sortConditionElement : sortConditionElements) {
        const auto conditionRefAttr = sortConditionElement.GetAttribute("ref");
        const std::string conditionReference = conditionRefAttr.GetValue();
        std::string normalizedConditionRef;
        if (!AutoFilterSupport::TryNormalizeRange(conditionReference, normalizedConditionRef)) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The worksheet autoFilter sortCondition ref is invalid.");
            }

            WarnSortState(diagnostics, options, sheetName, autoFilterRange);
            continue;
        }

        AutoFilterSortConditionModel condition;
        condition.SetRef(normalizedConditionRef);

        const auto descendingAttr = sortConditionElement.GetAttribute("descending");
        bool descending;
        if (TryParseBooleanAttribute(descendingAttr, false, descending)) {
            condition.SetDescending(descending);
        } else if (!descendingAttr.IsNull()) {
            WarnSortState(diagnostics, options, sheetName, autoFilterRange);
        }

        const auto sortByAttr = sortConditionElement.GetAttribute("sortBy");
        const auto sortBy = NormalizeSortBy(sortByAttr.GetValue());
        if (!sortByAttr.IsNull() && sortBy.empty()) {
            WarnSortState(diagnostics, options, sheetName, autoFilterRange);
        }

        condition.SetSortBy(sortBy);

        const auto customList = AutoFilterSupport::NormalizeOptionalText(
            sortConditionElement.GetAttribute("customList").GetValue());
        if (AllowsCustomList(sortBy)) {
            condition.SetCustomList(customList.empty() ? std::string() : customList);
        } else if (!customList.empty()) {
            WarnSortState(diagnostics, options, sheetName, autoFilterRange);
        }

        const auto iconSet = AutoFilterSupport::NormalizeOptionalText(
            sortConditionElement.GetAttribute("iconSet").GetValue());
        if (AllowsIconSort(sortBy, iconSet)) {
            condition.SetIconSet(iconSet.empty() ? std::string() : iconSet);
        } else if (!iconSet.empty()) {
            WarnSortState(diagnostics, options, sheetName, autoFilterRange);
        }

        const auto dxfIdAttr = sortConditionElement.GetAttribute("dxfId");
        const auto dxfId = XlsxWorkbookArchiveHelpers::ParseIntAttribute(dxfIdAttr);
        if (!dxfIdAttr.IsNull()) {
            if (AllowsDifferentialStyle(sortBy) &&
                HasValidDifferentialStyleId(dxfId, static_cast<int>(stylesheet.GetDifferentialFormats().size()))) {
                condition.SetDifferentialStyleId(dxfId.value());
            } else {
                if (options.GetStrictMode()) {
                    throw InvalidFileFormatException(
                        "The worksheet autoFilter sortCondition dxfId is invalid.");
                }

                WarnSortState(diagnostics, options, sheetName, autoFilterRange);
            }
        }

        const auto iconIdAttr = sortConditionElement.GetAttribute("iconId");
        const auto iconId = XlsxWorkbookArchiveHelpers::ParseIntAttribute(iconIdAttr);
        if (!iconIdAttr.IsNull()) {
            if (AllowsIconSort(sortBy, iconSet) && iconId.has_value() && iconId.value() >= 0) {
                condition.SetIconId(iconId.value());
            } else {
                if (options.GetStrictMode()) {
                    throw InvalidFileFormatException(
                        "The worksheet autoFilter sortCondition iconId is invalid.");
                }

                WarnSortState(diagnostics, options, sheetName, autoFilterRange);
            }
        }

        model.GetConditions().push_back(std::move(condition));
    }
}

// ---------------------------------------------------------------------------
// WarnSortState
// ---------------------------------------------------------------------------

void XlsxWorkbookAutoFilter::WarnSortState(
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view autoFilterRange)
{
    LoadIssue issue("WS-L012", DiagnosticSeverity::LossyRecoverable,
        "Worksheet autoFilter sort state was invalid and was dropped during load.",
        true, true);
    issue.SetSheetName(sheetName);
    issue.SetCellRef(autoFilterRange);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
}

// ---------------------------------------------------------------------------
// TryParseBooleanAttribute
// ---------------------------------------------------------------------------

bool XlsxWorkbookAutoFilter::TryParseBooleanAttribute(
    const XmlAttribute& attribute,
    bool defaultValue,
    bool& parsedValue)
{
    parsedValue = defaultValue;
    if (attribute.IsNull()) {
        return true;
    }

    // Trim leading and trailing whitespace only (matches C# Trim()).
    const auto& raw = attribute.GetValue();
    const auto start = std::find_if_not(raw.begin(), raw.end(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); });
    const auto end = std::find_if_not(raw.rbegin(), raw.rend(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); }).base();
    const std::string trimmed(start, end);

    if (trimmed == "1") {
        parsedValue = true;
        return true;
    }
    if (trimmed.size() == 4 &&
        (trimmed[0] == 't' || trimmed[0] == 'T') &&
        (trimmed[1] == 'r' || trimmed[1] == 'R') &&
        (trimmed[2] == 'u' || trimmed[2] == 'U') &&
        (trimmed[3] == 'e' || trimmed[3] == 'E')) {
        parsedValue = true;
        return true;
    }
    if (trimmed == "0") {
        parsedValue = false;
        return true;
    }
    if (trimmed.size() == 5 &&
        (trimmed[0] == 'f' || trimmed[0] == 'F') &&
        (trimmed[1] == 'a' || trimmed[1] == 'A') &&
        (trimmed[2] == 'l' || trimmed[2] == 'L') &&
        (trimmed[3] == 's' || trimmed[3] == 'S') &&
        (trimmed[4] == 'e' || trimmed[4] == 'E')) {
        parsedValue = false;
        return true;
    }

    return false;
}

// ---------------------------------------------------------------------------
// ParseOptionalDouble
// ---------------------------------------------------------------------------

std::optional<double> XlsxWorkbookAutoFilter::ParseOptionalDouble(
    const XmlAttribute& attribute,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view autoFilterRange,
    std::string_view attributeLabel)
{
    if (attribute.IsNull()) {
        return std::nullopt;
    }

    const auto parsed = XlsxWorkbookArchiveHelpers::ParseDoubleAttribute(attribute);
    if (parsed.has_value()) {
        return parsed;
    }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            std::string("The worksheet autoFilter contains an invalid ") +
            std::string(attributeLabel) + ".");
    }

    LoadIssue issue("WS-L011", DiagnosticSeverity::LossyRecoverable,
        "Worksheet autoFilter column metadata was invalid or conflicting and some entries were dropped during load.",
        true, true);
    issue.SetSheetName(sheetName);
    issue.SetCellRef(autoFilterRange);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return std::nullopt;
}

// ---------------------------------------------------------------------------
// HasValidDifferentialStyleId
// ---------------------------------------------------------------------------

bool XlsxWorkbookAutoFilter::HasValidDifferentialStyleId(
    std::optional<int> differentialStyleId,
    int differentialFormatCount)
{
    return differentialStyleId.has_value()
        && differentialStyleId.value() >= 0
        && differentialStyleId.value() < differentialFormatCount;
}

// ---------------------------------------------------------------------------
// NormalizeSortBy
// ---------------------------------------------------------------------------

std::string XlsxWorkbookAutoFilter::NormalizeSortBy(std::string_view sortBy)
{
    // Check for blank/whitespace-only input (matches C# IsNullOrWhiteSpace).
    bool allWhitespace = true;
    for (char ch : sortBy) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            allWhitespace = false;
            break;
        }
    }
    if (allWhitespace) {
        return {};
    }

    // Trim leading and trailing whitespace only (matches C# Trim()).
    const auto start = std::find_if_not(sortBy.begin(), sortBy.end(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); });
    const auto end = std::find_if_not(sortBy.rbegin(), sortBy.rend(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); }).base();
    const std::string trimmed(start, end);

    // Case-insensitive comparison (matches C# StringComparison.OrdinalIgnoreCase).
    auto toLower = [](const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(),
            [](char c) { return static_cast<char>(std::tolower(static_cast<unsigned char>(c))); });
        return result;
    };

    const auto lower = toLower(trimmed);
    if (lower == "value") return "value";
    if (lower == "cellcolor") return "cellColor";
    if (lower == "fontcolor") return "fontColor";
    if (lower == "icon") return "icon";

    return {};
}

// ---------------------------------------------------------------------------
// AllowsCustomList
// ---------------------------------------------------------------------------

bool XlsxWorkbookAutoFilter::AllowsCustomList(std::string_view sortBy)
{
    return sortBy.empty() || sortBy == "value";
}

// ---------------------------------------------------------------------------
// AllowsDifferentialStyle
// ---------------------------------------------------------------------------

bool XlsxWorkbookAutoFilter::AllowsDifferentialStyle(std::string_view sortBy)
{
    return sortBy == "cellColor" || sortBy == "fontColor";
}

// ---------------------------------------------------------------------------
// AllowsIconSort (condition overload)
// ---------------------------------------------------------------------------

bool XlsxWorkbookAutoFilter::AllowsIconSort(
    const AutoFilterSortConditionModel& condition,
    std::string_view sortBy)
{
    return sortBy == "icon"
        && !condition.GetIconSet().empty()
        && condition.GetIconId().has_value()
        && condition.GetIconId().value() >= 0;
}

// ---------------------------------------------------------------------------
// AllowsIconSort (string overload)
// ---------------------------------------------------------------------------

bool XlsxWorkbookAutoFilter::AllowsIconSort(
    std::string_view sortBy,
    std::string_view iconSet)
{
    return sortBy == "icon" && !iconSet.empty();
}

}}  // namespace Aspose::Cells_FOSS
