#pragma once
#include <string>
#include <vector>

#include "aspose/cells_foss/core/AutoFilterColorFilterModel.h"
#include "aspose/cells_foss/core/AutoFilterCustomFilterModel.h"
#include "aspose/cells_foss/core/AutoFilterDynamicFilterModel.h"
#include "aspose/cells_foss/core/AutoFilterTop10Model.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents filter column model.
/// </summary>
class FilterColumnModel {
public:
    /// <summary>
    /// Initializes a new instance of the FilterColumnModel class.
    /// </summary>
    FilterColumnModel();

    /// <summary>
    /// Gets or sets the column index.
    /// </summary>
    int GetColumnIndex() const noexcept { return _columnIndex; }
    void SetColumnIndex(int value) noexcept { _columnIndex = value; }

    /// <summary>
    /// Gets or sets a value indicating whether hidden button.
    /// </summary>
    bool GetHiddenButton() const noexcept { return _hiddenButton; }
    void SetHiddenButton(bool value) noexcept { _hiddenButton = value; }

    /// <summary>
    /// Gets the filters.
    /// </summary>
    const std::vector<std::string>& GetFilters() const noexcept { return _filters; }
    std::vector<std::string>& GetFilters() noexcept { return _filters; }

    /// <summary>
    /// Gets the custom filters.
    /// </summary>
    const std::vector<AutoFilterCustomFilterModel>& GetCustomFilters() const noexcept { return _customFilters; }
    std::vector<AutoFilterCustomFilterModel>& GetCustomFilters() noexcept { return _customFilters; }

    /// <summary>
    /// Gets or sets a value indicating whether custom filters and.
    /// </summary>
    bool GetCustomFiltersAnd() const noexcept { return _customFiltersAnd; }
    void SetCustomFiltersAnd(bool value) noexcept { _customFiltersAnd = value; }

    /// <summary>
    /// Gets the color filter.
    /// </summary>
    const AutoFilterColorFilterModel& GetColorFilter() const noexcept { return _colorFilter; }
    AutoFilterColorFilterModel& GetColorFilter() noexcept { return _colorFilter; }

    /// <summary>
    /// Gets the dynamic filter.
    /// </summary>
    const AutoFilterDynamicFilterModel& GetDynamicFilter() const noexcept { return _dynamicFilter; }
    AutoFilterDynamicFilterModel& GetDynamicFilter() noexcept { return _dynamicFilter; }

    /// <summary>
    /// Gets the top10.
    /// </summary>
    const AutoFilterTop10Model& GetTop10() const noexcept { return _top10; }
    AutoFilterTop10Model& GetTop10() noexcept { return _top10; }

    /// <summary>
    /// Performs clear criteria.
    /// </summary>
    void ClearCriteria();

    /// <summary>
    /// Performs has stored state.
    /// </summary>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool HasStoredState() const;

private:
    int _columnIndex = 0;
    bool _hiddenButton = false;
    std::vector<std::string> _filters;
    std::vector<AutoFilterCustomFilterModel> _customFilters;
    bool _customFiltersAnd = false;
    AutoFilterColorFilterModel _colorFilter;
    AutoFilterDynamicFilterModel _dynamicFilter;
    AutoFilterTop10Model _top10;
};

}}}  // namespace Aspose::Cells_FOSS::Core
