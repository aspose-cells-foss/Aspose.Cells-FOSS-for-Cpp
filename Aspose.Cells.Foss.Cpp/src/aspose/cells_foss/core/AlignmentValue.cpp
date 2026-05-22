#include "aspose/cells_foss/core/AlignmentValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

AlignmentValue AlignmentValue::Clone() const
{
    AlignmentValue copy;
    copy._horizontal = _horizontal;
    copy._vertical = _vertical;
    copy._wrapText = _wrapText;
    copy._indentLevel = _indentLevel;
    copy._textRotation = _textRotation;
    copy._shrinkToFit = _shrinkToFit;
    copy._readingOrder = _readingOrder;
    copy._relativeIndent = _relativeIndent;
    return copy;
}

}}}  // namespace Aspose::Cells_FOSS::Core
