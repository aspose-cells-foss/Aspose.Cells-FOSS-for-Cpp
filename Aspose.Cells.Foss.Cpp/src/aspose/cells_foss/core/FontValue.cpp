#include "aspose/cells_foss/core/FontValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

FontValue FontValue::Clone() const
{
    FontValue copy;
    copy._name = _name;
    copy._size = _size;
    copy._bold = _bold;
    copy._italic = _italic;
    copy._underline = _underline;
    copy._strikeThrough = _strikeThrough;
    copy._color = _color;
    return copy;
}

}}}  // namespace Aspose::Cells_FOSS::Core
