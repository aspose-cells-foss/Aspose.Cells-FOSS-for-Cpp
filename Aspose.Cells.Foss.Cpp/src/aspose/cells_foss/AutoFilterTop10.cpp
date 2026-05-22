#include "aspose/cells_foss/AutoFilterTop10.h"

namespace Aspose {
namespace Cells_FOSS {

AutoFilterTop10::AutoFilterTop10(Core::AutoFilterTop10Model& model)
    : _model(model)
{
}

bool AutoFilterTop10::GetEnabled() const noexcept
{
    return _model.GetEnabled();
}

void AutoFilterTop10::SetEnabled(bool value)
{
    if (!value)
    {
        _model.Clear();
        return;
    }

    _model.SetEnabled(true);
}

bool AutoFilterTop10::GetTop() const noexcept
{
    return _model.GetTop();
}

void AutoFilterTop10::SetTop(bool value)
{
    _model.SetTop(value);
    _model.SetEnabled(true);
}

bool AutoFilterTop10::GetPercent() const noexcept
{
    return _model.GetPercent();
}

void AutoFilterTop10::SetPercent(bool value)
{
    _model.SetPercent(value);
    _model.SetEnabled(true);
}

std::optional<double> AutoFilterTop10::GetValue() const noexcept
{
    return _model.GetValue();
}

void AutoFilterTop10::SetValue(std::optional<double> value)
{
    _model.SetValue(value);
    if (value.has_value())
    {
        _model.SetEnabled(true);
    }
}

std::optional<double> AutoFilterTop10::GetFilterValue() const noexcept
{
    return _model.GetFilterValue();
}

void AutoFilterTop10::SetFilterValue(std::optional<double> value)
{
    _model.SetFilterValue(value);
    if (value.has_value())
    {
        _model.SetEnabled(true);
    }
}

void AutoFilterTop10::Clear()
{
    _model.Clear();
}

}}  // namespace Aspose::Cells_FOSS
