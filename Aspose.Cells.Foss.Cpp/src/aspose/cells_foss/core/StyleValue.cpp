#include "aspose/cells_foss/core/StyleValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

StyleValue StyleValue::Default()
{
    return StyleValue{};
}

StyleValue StyleValue::Clone() const
{
    StyleValue copy;
    copy._font = _font.Clone();
    copy._pattern = _pattern;
    copy._foregroundColor = _foregroundColor;
    copy._backgroundColor = _backgroundColor;
    copy._borders = _borders.Clone();
    copy._alignment = _alignment.Clone();
    copy._protection = _protection.Clone();
    copy._numberFormat = _numberFormat.Clone();
    return copy;
}

}}}  // namespace Aspose::Cells_FOSS::Core
