#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Provides number format operations.
/// </summary>
class NumberFormat {
public:
    NumberFormat() = delete;

    /// <summary>
    /// Gets the built in format.
    /// </summary>
    /// <param name="formatId">The format id.</param>
    /// <returns>The string.</returns>
    static std::string GetBuiltInFormat(int formatId);

    /// <summary>
    /// Gets the built in format id.
    /// </summary>
    /// <param name="formatCode">The format code.</param>
    /// <returns>The int.</returns>
    static std::optional<int> GetBuiltInFormatId(std::string_view formatCode);

    /// <summary>
    /// Performs is built in format.
    /// </summary>
    /// <param name="formatCode">The format code.</param>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    static bool IsBuiltInFormat(std::string_view formatCode);

    /// <summary>
    /// Resolves the format code.
    /// </summary>
    /// <param name="number">The number.</param>
    /// <param name="custom">The custom format code.</param>
    /// <returns>The resolved format code string.</returns>
    static std::string ResolveFormatCode(int number, std::string_view custom);
};

}}  // namespace Aspose::Cells_FOSS
