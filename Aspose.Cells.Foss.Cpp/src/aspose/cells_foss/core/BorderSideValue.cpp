#include "aspose/cells_foss/core/BorderSideValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

BorderSideValue BorderSideValue::Clone() const
{
    BorderSideValue copy;
    copy._style = _style;
    copy._color = _color;
    return copy;
}

}}}  // namespace Aspose::Cells_FOSS::Core
