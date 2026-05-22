#pragma once

#include <cstdint>

namespace Aspose {
namespace Cells_FOSS {
namespace InternalValidation {

/// <summary>
/// Specifies validation message severity.
/// </summary>
enum class ValidationMessageSeverity : int32_t
{
    /// <summary>
    /// Information message.
    /// </summary>
    Info = 0,
    
    /// <summary>
    /// Warning message.
    /// </summary>
    Warning = 1,
    
    /// <summary>
    /// Error message.
    /// </summary>
    Error = 2
};

} // namespace InternalValidation
} // namespace Cells_FOSS
} // namespace Aspose