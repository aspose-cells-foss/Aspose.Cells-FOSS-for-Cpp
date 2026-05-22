#include "aspose/cells_foss/Borders.h"

namespace Aspose {
namespace Cells_FOSS {

Borders Borders::Clone() const
{
    Borders copy;
    copy._left = _left.Clone();
    copy._right = _right.Clone();
    copy._top = _top.Clone();
    copy._bottom = _bottom.Clone();
    copy._diagonal = _diagonal.Clone();
    copy._diagonalUp = _diagonalUp;
    copy._diagonalDown = _diagonalDown;
    return copy;
}

}}  // namespace Aspose::Cells_FOSS
