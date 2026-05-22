#include "aspose/cells_foss/Border.h"

namespace Aspose {
namespace Cells_FOSS {

Border Border::Clone() const
{
    Border copy;
    copy._lineStyle = _lineStyle;
    copy._color = _color;
    return copy;
}

}}  // namespace Aspose::Cells_FOSS
