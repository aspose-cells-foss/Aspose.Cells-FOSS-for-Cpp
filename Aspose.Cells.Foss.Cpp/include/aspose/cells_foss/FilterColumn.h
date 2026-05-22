#pragma once

#include "aspose/cells_foss/AutoFilterColorFilter.h"
#include "aspose/cells_foss/AutoFilterCustomFilterCollection.h"
#include "aspose/cells_foss/AutoFilterDynamicFilter.h"
#include "aspose/cells_foss/AutoFilterTop10.h"
#include "aspose/cells_foss/FilterValueCollection.h"
#include "aspose/cells_foss/core/FilterColumnModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents filter column.
/// </summary>
class FilterColumn final {
public:
    /// <summary>
    /// Initializes a new instance of the FilterColumn class.
    /// </summary>
    /// <param name="model">The filter column model.</param>
    explicit FilterColumn(Core::FilterColumnModel& model);

    /// <summary>
    /// Gets the column index.
    /// </summary>
    int GetColumnIndex() const noexcept;

    /// <summary>
    /// Gets or sets a value indicating whether hidden button.
    /// </summary>
    bool GetHiddenButton() const noexcept;
    void SetHiddenButton(bool value);

    /// <summary>
    /// Gets the filters.
    /// </summary>
    const FilterValueCollection& GetFilters() const noexcept;
    FilterValueCollection& GetFilters() noexcept;

    /// <summary>
    /// Gets the custom filters.
    /// </summary>
    const AutoFilterCustomFilterCollection& GetCustomFilters() const noexcept;
    AutoFilterCustomFilterCollection& GetCustomFilters() noexcept;

    /// <summary>
    /// Gets the color filter.
    /// </summary>
    const AutoFilterColorFilter& GetColorFilter() const noexcept;
    AutoFilterColorFilter& GetColorFilter() noexcept;

    /// <summary>
    /// Gets the dynamic filter.
    /// </summary>
    const AutoFilterDynamicFilter& GetDynamicFilter() const noexcept;
    AutoFilterDynamicFilter& GetDynamicFilter() noexcept;

    /// <summary>
    /// Gets the top10.
    /// </summary>
    const AutoFilterTop10& GetTop10() const noexcept;
    AutoFilterTop10& GetTop10() noexcept;

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear();

private:
    Core::FilterColumnModel& _model;
    FilterValueCollection _filters;
    AutoFilterCustomFilterCollection _customFilters;
    AutoFilterColorFilter _colorFilter;
    AutoFilterDynamicFilter _dynamicFilter;
    AutoFilterTop10 _top10;
};

}}  // namespace Aspose::Cells_FOSS
