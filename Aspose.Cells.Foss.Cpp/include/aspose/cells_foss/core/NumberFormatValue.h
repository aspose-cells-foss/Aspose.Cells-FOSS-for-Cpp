#pragma once

#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents number format value.
/// </summary>
class NumberFormatValue {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="NumberFormatValue"/> class.
    /// </summary>
    NumberFormatValue() = default;

    /// <summary>
    /// Gets or sets the number.
    /// </summary>
    int GetNumber() const noexcept { return _number; }
    void SetNumber(int value) noexcept { _number = value; }

    /// <summary>
    /// Gets or sets the custom.
    /// </summary>
    const std::string& GetCustom() const noexcept { return _custom; }
    void SetCustom(const std::string& value) { _custom = value; }

    /// <summary>
    /// Creates a copy of the current instance.
    /// </summary>
    /// <returns>The number format value.</returns>
    NumberFormatValue Clone() const;

private:
    int _number = 0;
    std::string _custom;
};

}}}  // namespace Aspose::Cells_FOSS::Core
