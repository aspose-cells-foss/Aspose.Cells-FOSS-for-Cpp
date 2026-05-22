#include "aspose/cells_foss/AutoFilterSortState.h"
#include "aspose/cells_foss/AutoFilterSupport.h"

namespace Aspose {
namespace Cells_FOSS {

AutoFilterSortState::AutoFilterSortState(Core::AutoFilterSortStateModel& model)
    : _model(model)
    , _conditions(_model.GetConditions())
{
}

bool AutoFilterSortState::GetColumnSort() const noexcept
{
    return _model.GetColumnSort();
}

void AutoFilterSortState::SetColumnSort(bool value) noexcept
{
    _model.SetColumnSort(value);
}

bool AutoFilterSortState::GetCaseSensitive() const noexcept
{
    return _model.GetCaseSensitive();
}

void AutoFilterSortState::SetCaseSensitive(bool value) noexcept
{
    _model.SetCaseSensitive(value);
}

std::string AutoFilterSortState::GetSortMethod() const
{
    return _model.GetSortMethod();
}

void AutoFilterSortState::SetSortMethod(const std::string& value)
{
    _model.SetSortMethod(AutoFilterSupport::NormalizeOptionalText(value));
}

std::string AutoFilterSortState::GetRef() const
{
    return _model.GetRef();
}

void AutoFilterSortState::SetRef(const std::string& value)
{
    _model.SetRef(AutoFilterSupport::NormalizeOptionalRange(value, "Ref"));
}

AutoFilterSortConditionCollection& AutoFilterSortState::GetSortConditions() noexcept
{
    return _conditions;
}

void AutoFilterSortState::Clear()
{
    _model.Clear();
}

}}  // namespace Aspose::Cells_FOSS
