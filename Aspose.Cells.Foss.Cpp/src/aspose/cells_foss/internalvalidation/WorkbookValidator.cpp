#include "aspose/cells_foss/internalvalidation/WorkbookValidator.h"

namespace Aspose {
namespace Cells_FOSS {
namespace InternalValidation {

WorkbookValidator::WorkbookValidator() = default;

std::vector<ValidationMessage> WorkbookValidator::ValidateForSave(
    const CellValue& /*workbookModel*/,
    const CellValue& /*packageModel*/) const
{
    return {};
}

}}}  // namespace Aspose::Cells_FOSS::InternalValidation
