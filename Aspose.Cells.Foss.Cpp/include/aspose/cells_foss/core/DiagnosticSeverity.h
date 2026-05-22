#pragma once

#include <cstdint>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Specifies diagnostic severity.
/// </summary>
enum class DiagnosticSeverity : int32_t
{
    /// <summary>
    /// Warning severity.
    /// </summary>
    Warning = 0,

    /// <summary>
    /// Recoverable severity.
    /// </summary>
    Recoverable = 1,

    /// <summary>
    /// Lossy recoverable severity.
    /// </summary>
    LossyRecoverable = 2,

    /// <summary>
    /// Fatal severity.
    /// </summary>
    Fatal = 3
};

} // namespace Core
} // namespace Cells_FOSS
} // namespace Aspose
