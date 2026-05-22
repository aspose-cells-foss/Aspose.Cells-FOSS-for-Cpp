#include "aspose/cells_foss/FilterColumn.h"

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

FilterColumn::FilterColumn(Core::FilterColumnModel& model)
    : _model(model)
    , _filters(model.GetFilters())
    , _customFilters(model.GetCustomFilters(), model)
    , _colorFilter(model.GetColorFilter())
    , _dynamicFilter(model.GetDynamicFilter())
    , _top10(model.GetTop10())
{
}

// ---------------------------------------------------------------------------
// ColumnIndex
// ---------------------------------------------------------------------------

int FilterColumn::GetColumnIndex() const noexcept
{
    return _model.GetColumnIndex();
}

// ---------------------------------------------------------------------------
// HiddenButton
// ---------------------------------------------------------------------------

bool FilterColumn::GetHiddenButton() const noexcept
{
    return _model.GetHiddenButton();
}

void FilterColumn::SetHiddenButton(bool value)
{
    _model.SetHiddenButton(value);
}

// ---------------------------------------------------------------------------
// Filters
// ---------------------------------------------------------------------------

const FilterValueCollection& FilterColumn::GetFilters() const noexcept
{
    return _filters;
}

FilterValueCollection& FilterColumn::GetFilters() noexcept
{
    return _filters;
}

// ---------------------------------------------------------------------------
// CustomFilters
// ---------------------------------------------------------------------------

const AutoFilterCustomFilterCollection& FilterColumn::GetCustomFilters() const noexcept
{
    return _customFilters;
}

AutoFilterCustomFilterCollection& FilterColumn::GetCustomFilters() noexcept
{
    return _customFilters;
}

// ---------------------------------------------------------------------------
// ColorFilter
// ---------------------------------------------------------------------------

const AutoFilterColorFilter& FilterColumn::GetColorFilter() const noexcept
{
    return _colorFilter;
}

AutoFilterColorFilter& FilterColumn::GetColorFilter() noexcept
{
    return _colorFilter;
}

// ---------------------------------------------------------------------------
// DynamicFilter
// ---------------------------------------------------------------------------

const AutoFilterDynamicFilter& FilterColumn::GetDynamicFilter() const noexcept
{
    return _dynamicFilter;
}

AutoFilterDynamicFilter& FilterColumn::GetDynamicFilter() noexcept
{
    return _dynamicFilter;
}

// ---------------------------------------------------------------------------
// Top10
// ---------------------------------------------------------------------------

const AutoFilterTop10& FilterColumn::GetTop10() const noexcept
{
    return _top10;
}

AutoFilterTop10& FilterColumn::GetTop10() noexcept
{
    return _top10;
}

// ---------------------------------------------------------------------------
// Clear
// ---------------------------------------------------------------------------

void FilterColumn::Clear()
{
    _model.ClearCriteria();
}

}}  // namespace Aspose::Cells_FOSS
