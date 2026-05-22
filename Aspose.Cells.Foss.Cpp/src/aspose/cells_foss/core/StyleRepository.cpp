#include "aspose/cells_foss/core/StyleRepository.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

StyleValue StyleRepository::Normalize(const StyleValue& style)
{
    return style.Clone();
}

}}}  // namespace Aspose::Cells_FOSS::Core
