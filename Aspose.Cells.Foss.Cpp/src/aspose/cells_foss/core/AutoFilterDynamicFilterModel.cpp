#include "aspose/cells_foss/core/AutoFilterDynamicFilterModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

void AutoFilterDynamicFilterModel::Clear() noexcept
{
    _enabled = false;
    _type.clear();
    _value = std::nullopt;
    _maxValue = std::nullopt;
}

}}}  // namespace Aspose::Cells_FOSS::Core
