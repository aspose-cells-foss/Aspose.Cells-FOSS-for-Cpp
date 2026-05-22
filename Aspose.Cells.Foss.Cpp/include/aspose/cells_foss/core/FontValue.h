#pragma once

#include <string>

#include "aspose/cells_foss/core/ColorValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents font value.
/// </summary>
class FontValue {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="FontValue"/> class.
    /// </summary>
    FontValue() = default;

    /// <summary>
    /// Gets or sets the name.
    /// </summary>
    const std::string& GetName() const noexcept { return _name; }
    void SetName(const std::string& value) { _name = value; }

    /// <summary>
    /// Gets or sets the size.
    /// </summary>
    double GetSize() const noexcept { return _size; }
    void SetSize(double value) noexcept { _size = value; }

    /// <summary>
    /// Gets or sets a value indicating whether bold.
    /// </summary>
    bool GetBold() const noexcept { return _bold; }
    void SetBold(bool value) noexcept { _bold = value; }

    /// <summary>
    /// Gets or sets a value indicating whether italic.
    /// </summary>
    bool GetItalic() const noexcept { return _italic; }
    void SetItalic(bool value) noexcept { _italic = value; }

    /// <summary>
    /// Gets or sets a value indicating whether underline.
    /// </summary>
    bool GetUnderline() const noexcept { return _underline; }
    void SetUnderline(bool value) noexcept { _underline = value; }

    /// <summary>
    /// Gets or sets a value indicating whether strike through.
    /// </summary>
    bool GetStrikeThrough() const noexcept { return _strikeThrough; }
    void SetStrikeThrough(bool value) noexcept { _strikeThrough = value; }

    /// <summary>
    /// Gets or sets the color.
    /// </summary>
    const ColorValue& GetColor() const noexcept { return _color; }
    void SetColor(const ColorValue& value) noexcept { _color = value; }

    /// <summary>
    /// Creates a copy of the current instance.
    /// </summary>
    /// <returns>The font value.</returns>
    FontValue Clone() const;

private:
    std::string _name = "Calibri";
    double _size = 11.0;
    bool _bold = false;
    bool _italic = false;
    bool _underline = false;
    bool _strikeThrough = false;
    ColorValue _color;
};

}}}  // namespace Aspose::Cells_FOSS::Core
