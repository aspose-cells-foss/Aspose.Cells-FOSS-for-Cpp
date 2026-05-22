#pragma once

#include <optional>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Provides normalization helpers for style integer values.
/// </summary>
class StyleValueSanitizer {
public:
    /// <summary>
    /// Normalizes an indent level value to the valid range [0, 250].
    /// Returns 0 if the value is absent or out of range.
    /// </summary>
    static int NormalizeIndentLevel(std::optional<int> value);

    /// <summary>
    /// Normalizes a reading order value to the valid range [0, 2].
    /// Returns 0 if the value is absent or out of range.
    /// </summary>
    static int NormalizeReadingOrder(std::optional<int> value);

    /// <summary>
    /// Normalizes a text rotation value. Accepts 255 (vertical text) or [0, 180].
    /// Returns 0 if the value is absent or out of range.
    /// </summary>
    static int NormalizeTextRotation(std::optional<int> value);
};

}}  // namespace Aspose::Cells_FOSS
