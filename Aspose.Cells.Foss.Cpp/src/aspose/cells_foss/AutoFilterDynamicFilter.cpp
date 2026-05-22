#include "aspose/cells_foss/AutoFilterDynamicFilter.h"
#include "aspose/cells_foss/AutoFilterSupport.h"

namespace Aspose {
namespace Cells_FOSS {

AutoFilterDynamicFilter::AutoFilterDynamicFilter(
    Core::AutoFilterDynamicFilterModel& model)
    : _model(model)
{
}

bool AutoFilterDynamicFilter::GetEnabled() const noexcept
{
    return _model.GetEnabled();
}

void AutoFilterDynamicFilter::SetEnabled(bool value)
{
    if (!value)
    {
        _model.Clear();
        return;
    }

    _model.SetEnabled(true);
}

std::string AutoFilterDynamicFilter::GetType() const
{
    return _model.GetType();
}

void AutoFilterDynamicFilter::SetType(const std::string& value)
{
    _model.SetType(AutoFilterSupport::NormalizeOptionalText(value));
    if (_model.GetType().size() > 0)
    {
        _model.SetEnabled(true);
    }
}

std::optional<double> AutoFilterDynamicFilter::GetValue() const noexcept
{
    return _model.GetValue();
}

void AutoFilterDynamicFilter::SetValue(std::optional<double> value)
{
    _model.SetValue(value);
    if (value.has_value())
    {
        _model.SetEnabled(true);
    }
}

std::optional<double> AutoFilterDynamicFilter::GetMaxValue() const noexcept
{
    return _model.GetMaxValue();
}

void AutoFilterDynamicFilter::SetMaxValue(std::optional<double> value)
{
    _model.SetMaxValue(value);
    if (value.has_value())
    {
        _model.SetEnabled(true);
    }
}

void AutoFilterDynamicFilter::Clear()
{
    _model.Clear();
}

}}  // namespace Aspose::Cells_FOSS
