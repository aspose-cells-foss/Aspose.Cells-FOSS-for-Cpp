#include "aspose/cells_foss/Font.h"

namespace Aspose {
namespace Cells_FOSS {

Font Font::Clone() const
{
    Font copy;
    copy._name = _name;
    copy._size = _size;
    copy._bold = _bold;
    copy._italic = _italic;
    copy._underline = _underline;
    copy._strikeThrough = _strikeThrough;
    copy._color = _color;
    return copy;
}

}}  // namespace Aspose::Cells_FOSS
