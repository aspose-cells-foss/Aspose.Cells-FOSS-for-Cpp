#include "aspose/cells_foss/AutoFilterColorFilter.h"
#include "aspose/cells_foss/CellsException.h"

namespace Aspose {
namespace Cells_FOSS {

AutoFilterColorFilter::AutoFilterColorFilter(
    Core::AutoFilterColorFilterModel& model)
    : _model(model)
{
}

bool AutoFilterColorFilter::GetEnabled() const noexcept
{
    return _model.GetEnabled();
}

void AutoFilterColorFilter::SetEnabled(bool value)
{
    if (!value)
    {
        _model.Clear();
        return;
    }

    _model.SetEnabled(true);
}

std::optional<int> AutoFilterColorFilter::GetDifferentialStyleId() const noexcept
{
    return _model.GetDifferentialStyleId();
}

void AutoFilterColorFilter::SetDifferentialStyleId(std::optional<int> value)
{
    if (value.has_value() && *value < 0)
    {
        throw CellsException("Differential style id must be zero or greater.");
    }

    _model.SetDifferentialStyleId(value);
    if (value.has_value())
    {
        _model.SetEnabled(true);
    }
}

bool AutoFilterColorFilter::GetCellColor() const noexcept
{
    return _model.GetCellColor();
}

void AutoFilterColorFilter::SetCellColor(bool value)
{
    _model.SetCellColor(value);
    if (value)
    {
        _model.SetEnabled(true);
    }
}

void AutoFilterColorFilter::Clear()
{
    _model.Clear();
}

}}  // namespace Aspose::Cells_FOSS
