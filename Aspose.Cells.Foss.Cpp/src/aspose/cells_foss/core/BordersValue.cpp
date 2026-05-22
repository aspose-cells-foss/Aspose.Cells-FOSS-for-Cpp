#include "aspose/cells_foss/core/BordersValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

BordersValue BordersValue::Clone() const
{
    BordersValue result;
    result._left = _left.Clone();
    result._right = _right.Clone();
    result._top = _top.Clone();
    result._bottom = _bottom.Clone();
    result._diagonal = _diagonal.Clone();
    result._diagonalUp = _diagonalUp;
    result._diagonalDown = _diagonalDown;
    return result;
}

}}}  // namespace Aspose::Cells_FOSS::Core
