#include "aspose/cells_foss/core/AutoFilterColorFilterModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

void AutoFilterColorFilterModel::Clear() noexcept
{
    _enabled = false;
    _differentialStyleId = std::nullopt;
    _cellColor = false;
}

}}}  // namespace Aspose::Cells_FOSS::Core
