#include "aspose/cells_foss/core/NumberFormatValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

NumberFormatValue NumberFormatValue::Clone() const
{
    NumberFormatValue copy;
    copy._number = _number;
    copy._custom = _custom;
    return copy;
}

}}}  // namespace Aspose::Cells_FOSS::Core
