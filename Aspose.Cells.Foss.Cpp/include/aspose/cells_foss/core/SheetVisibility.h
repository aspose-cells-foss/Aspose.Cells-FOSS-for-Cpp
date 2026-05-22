#pragma once

#include <cstdint>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Specifies sheet visibility.
/// </summary>
enum class SheetVisibility : int32_t
{
    /// <summary>
    /// Indicates the sheet is visible.
    /// </summary>
    Visible = 0,

    /// <summary>
    /// Indicates the sheet is hidden but can be shown by the user via the user interface.
    /// </summary>
    Hidden = 1,

    /// <summary>
    /// Indicates the sheet is hidden and cannot be shown in the user interface.
    /// </summary>
    VeryHidden = 2
};

} // namespace Core
} // namespace Cells_FOSS
} // namespace Aspose
