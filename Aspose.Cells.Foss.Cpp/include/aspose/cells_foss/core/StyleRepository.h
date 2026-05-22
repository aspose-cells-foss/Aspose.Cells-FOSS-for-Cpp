#pragma once

#include "aspose/cells_foss/core/StyleValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents style repository.
/// </summary>
class StyleRepository final {
public:
    /// <summary>
    /// Initializes a new instance of the StyleRepository class.
    /// </summary>
    StyleRepository() = default;

    /// <summary>
    /// Normalizes the specified value.
    /// </summary>
    /// <param name="style">The style.</param>
    /// <returns>The style value.</returns>
    StyleValue Normalize(const StyleValue& style);
};

}}}  // namespace Aspose::Cells_FOSS::Core
