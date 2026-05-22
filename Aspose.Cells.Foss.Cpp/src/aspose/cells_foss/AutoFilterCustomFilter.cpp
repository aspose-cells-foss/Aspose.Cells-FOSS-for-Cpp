#include "aspose/cells_foss/AutoFilterCustomFilter.h"
#include "aspose/cells_foss/AutoFilterSupport.h"

namespace Aspose {
namespace Cells_FOSS {

AutoFilterCustomFilter::AutoFilterCustomFilter(
    Core::AutoFilterCustomFilterModel& model)
    : _model(model)
{
}

FilterOperatorType AutoFilterCustomFilter::GetOperator() const
{
    return AutoFilterSupport::ParseOperatorOrDefault(_model.GetOperator());
}

void AutoFilterCustomFilter::SetOperator(FilterOperatorType value)
{
    _model.SetOperator(AutoFilterSupport::ToOperatorName(value));
}

const std::string& AutoFilterCustomFilter::GetValue() const noexcept
{
    return _model.GetValue();
}

void AutoFilterCustomFilter::SetValue(const std::string& value)
{
    _model.SetValue(AutoFilterSupport::NormalizeText(value, "Value"));
}

}}  // namespace Aspose::Cells_FOSS
