#include "aspose/cells_foss/DocumentProperties.h"

namespace Aspose {
namespace Cells_FOSS {

DocumentProperties::DocumentProperties(Core::DocumentPropertiesModel& model)
    : _model(model)
    , _core(model.GetCore())
    , _extended(model.GetExtended())
{
}

}}  // namespace Aspose::Cells_FOSS
