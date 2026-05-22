#include "aspose/cells_foss/core/AutoFilterTop10Model.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

void AutoFilterTop10Model::Clear() noexcept
{
    _enabled = false;
    _top = true;
    _percent = false;
    _value = std::nullopt;
    _filterValue = std::nullopt;
}

}}}  // namespace Aspose::Cells_FOSS::Core
