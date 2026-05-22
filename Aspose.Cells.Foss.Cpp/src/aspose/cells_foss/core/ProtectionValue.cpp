#include "aspose/cells_foss/core/ProtectionValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

ProtectionValue ProtectionValue::Clone() const
{
    ProtectionValue copy;
    copy._isLocked = _isLocked;
    copy._isHidden = _isHidden;
    return copy;
}

}}}  // namespace Aspose::Cells_FOSS::Core
