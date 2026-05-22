#include "aspose/cells_foss/AutoFilterSortCondition.h"
#include "aspose/cells_foss/AutoFilterSupport.h"
#include "aspose/cells_foss/CellsException.h"

namespace Aspose {
namespace Cells_FOSS {

AutoFilterSortCondition::AutoFilterSortCondition(
    Core::AutoFilterSortConditionModel& model)
    : _model(model)
{
}

std::string AutoFilterSortCondition::GetRef() const
{
    return _model.GetRef();
}

void AutoFilterSortCondition::SetRef(const std::string& value)
{
    _model.SetRef(AutoFilterSupport::NormalizeRequiredRange(value, "Ref"));
}

bool AutoFilterSortCondition::GetDescending() const noexcept
{
    return _model.GetDescending();
}

void AutoFilterSortCondition::SetDescending(bool value) noexcept
{
    _model.SetDescending(value);
}

std::string AutoFilterSortCondition::GetSortBy() const
{
    return _model.GetSortBy();
}

void AutoFilterSortCondition::SetSortBy(const std::string& value)
{
    _model.SetSortBy(AutoFilterSupport::NormalizeOptionalText(value));
}

std::string AutoFilterSortCondition::GetCustomList() const
{
    return _model.GetCustomList();
}

void AutoFilterSortCondition::SetCustomList(const std::string& value)
{
    _model.SetCustomList(AutoFilterSupport::NormalizeOptionalText(value));
}

std::optional<int> AutoFilterSortCondition::GetDifferentialStyleId() const noexcept
{
    return _model.GetDifferentialStyleId();
}

void AutoFilterSortCondition::SetDifferentialStyleId(std::optional<int> value)
{
    if (value.has_value() && *value < 0)
    {
        throw CellsException("Differential style id must be zero or greater.");
    }

    _model.SetDifferentialStyleId(value);
}

std::string AutoFilterSortCondition::GetIconSet() const
{
    return _model.GetIconSet();
}

void AutoFilterSortCondition::SetIconSet(const std::string& value)
{
    _model.SetIconSet(AutoFilterSupport::NormalizeOptionalText(value));
}

std::optional<int> AutoFilterSortCondition::GetIconId() const noexcept
{
    return _model.GetIconId();
}

void AutoFilterSortCondition::SetIconId(std::optional<int> value)
{
    if (value.has_value() && *value < 0)
    {
        throw CellsException("Icon id must be zero or greater.");
    }

    _model.SetIconId(value);
}

}}  // namespace Aspose::Cells_FOSS
