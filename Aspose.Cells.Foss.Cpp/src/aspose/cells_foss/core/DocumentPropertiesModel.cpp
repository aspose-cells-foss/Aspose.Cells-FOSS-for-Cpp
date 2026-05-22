#include "aspose/cells_foss/core/DocumentPropertiesModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

DocumentPropertiesModel::DocumentPropertiesModel() = default;

void DocumentPropertiesModel::CopyFrom(const DocumentPropertiesModel& source)
{
    _core.CopyFrom(source._core);
    _extended.CopyFrom(source._extended);
}

bool DocumentPropertiesModel::HasStoredState() const
{
    return _core.HasStoredState() || _extended.HasStoredState();
}

}}}  // namespace Aspose::Cells_FOSS::Core
