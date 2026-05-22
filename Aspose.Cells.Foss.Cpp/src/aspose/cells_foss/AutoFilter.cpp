#include "aspose/cells_foss/AutoFilter.h"
#include "aspose/cells_foss/AutoFilterSupport.h"

namespace Aspose {
namespace Cells_FOSS {

AutoFilter::AutoFilter(Core::AutoFilterModel& model)
    : _model(model)
    , _filterColumns(_model.GetFilterColumns())
    , _sortState(_model.GetSortState())
{
}

std::string AutoFilter::GetRange() const
{
    return _model.GetRange();
}

void AutoFilter::SetRange(const std::string& value)
{
    _model.SetRange(AutoFilterSupport::NormalizeOptionalRange(value, "Range"));
}

FilterColumnCollection& AutoFilter::GetFilterColumns() noexcept
{
    return _filterColumns;
}

AutoFilterSortState& AutoFilter::GetSortState() noexcept
{
    return _sortState;
}

void AutoFilter::Clear()
{
    _model.Clear();
}

}}  // namespace Aspose::Cells_FOSS
