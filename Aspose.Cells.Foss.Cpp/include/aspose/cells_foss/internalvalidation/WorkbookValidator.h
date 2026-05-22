#pragma once

#include <vector>

#include "aspose/cells_foss/CellValue.h"
#include "aspose/cells_foss/internalvalidation/ValidationMessage.h"

namespace Aspose {
namespace Cells_FOSS {
namespace InternalValidation {

/// <summary>
/// Represents workbook validator.
/// </summary>
class WorkbookValidator final {
public:
    /// <summary>
    /// Initializes a new instance of the WorkbookValidator class.
    /// </summary>
    WorkbookValidator();

    /// <summary>
    /// Validates the current state.
    /// </summary>
    /// <param name="workbookModel">The workbook model.</param>
    /// <param name="packageModel">The package model.</param>
    /// <returns>The validation message list.</returns>
    std::vector<ValidationMessage> ValidateForSave(const CellValue& workbookModel, const CellValue& packageModel) const;
};

}}}  // namespace Aspose::Cells_FOSS::InternalValidation
