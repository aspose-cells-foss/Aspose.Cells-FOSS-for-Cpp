#include "aspose/cells_foss/ExtendedDocumentProperties.h"

#include "aspose/cells_foss/CellsException.h"

namespace Aspose {
namespace Cells_FOSS {

ExtendedDocumentProperties::ExtendedDocumentProperties(Core::ExtendedDocumentPropertiesModel& model)
    : _model(model)
{
}

void ExtendedDocumentProperties::SetDocSecurity(int value)
{
    if (value < 0)
    {
        throw CellsException("DocSecurity must be non-negative.");
    }

    _model.SetDocSecurity(value);
}

}}  // namespace Aspose::Cells_FOSS
