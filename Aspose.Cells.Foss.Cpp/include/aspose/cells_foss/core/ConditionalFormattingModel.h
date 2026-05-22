#pragma once
#include <vector>

#include "aspose/cells_foss/CellArea.h"
#include "aspose/cells_foss/core/FormatConditionModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents conditional formatting model.
/// </summary>
class ConditionalFormattingModel {
public:
    /// <summary>
    /// Initializes a new instance of the ConditionalFormattingModel class.
    /// </summary>
    ConditionalFormattingModel();

    /// <summary>
    /// Gets the areas.
    /// </summary>
    const std::vector<Aspose::Cells_FOSS::CellArea>& GetAreas() const noexcept { return _areas; }
    std::vector<Aspose::Cells_FOSS::CellArea>& GetAreas() noexcept { return _areas; }

    /// <summary>
    /// Gets the conditions.
    /// </summary>
    const std::vector<FormatConditionModel>& GetConditions() const noexcept { return _conditions; }
    std::vector<FormatConditionModel>& GetConditions() noexcept { return _conditions; }

private:
    std::vector<Aspose::Cells_FOSS::CellArea> _areas;
    std::vector<FormatConditionModel> _conditions;
};

}}}  // namespace Aspose::Cells_FOSS::Core
