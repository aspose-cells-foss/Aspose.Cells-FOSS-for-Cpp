#pragma once

#include "aspose/cells_foss/Color.h"

#include <string>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents font.
/// </summary>
class Font {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="Font"/> class.
    /// </summary>
    Font() = default;

    /// <summary>
    /// Gets or sets the name.
    /// </summary>
    const std::string& GetName() const { return _name; }

    /// <summary>
    /// Sets the name.
    /// </summary>
    void SetName(const std::string& value) { _name = value; }

    /// <summary>
    /// Gets or sets the size.
    /// </summary>
    double GetSize() const noexcept { return _size; }

    /// <summary>
    /// Sets the size.
    /// </summary>
    void SetSize(double value) noexcept { _size = value; }

    /// <summary>
    /// Gets or sets a value indicating whether bold.
    /// </summary>
    bool GetBold() const noexcept { return _bold; }

    /// <summary>
    /// Sets a value indicating whether bold.
    /// </summary>
    void SetBold(bool value) noexcept { _bold = value; }

    /// <summary>
    /// Gets or sets a value indicating whether italic.
    /// </summary>
    bool GetItalic() const noexcept { return _italic; }

    /// <summary>
    /// Sets a value indicating whether italic.
    /// </summary>
    void SetItalic(bool value) noexcept { _italic = value; }

    /// <summary>
    /// Gets or sets a value indicating whether underline.
    /// </summary>
    bool GetUnderline() const noexcept { return _underline; }

    /// <summary>
    /// Sets a value indicating whether underline.
    /// </summary>
    void SetUnderline(bool value) noexcept { _underline = value; }

    /// <summary>
    /// Gets or sets a value indicating whether strike through.
    /// </summary>
    bool GetStrikeThrough() const noexcept { return _strikeThrough; }

    /// <summary>
    /// Sets a value indicating whether strike through.
    /// </summary>
    void SetStrikeThrough(bool value) noexcept { _strikeThrough = value; }

    /// <summary>
    /// Gets or sets the color.
    /// </summary>
    const Color& GetColor() const noexcept { return _color; }

    /// <summary>
    /// Sets the color.
    /// </summary>
    void SetColor(const Color& value) noexcept { _color = value; }

    /// <summary>
    /// Creates a copy of the current instance.
    /// </summary>
    /// <returns>The font.</returns>
    Font Clone() const;

private:
    std::string _name = "Calibri";
    double _size = 11.0;
    bool _bold = false;
    bool _italic = false;
    bool _underline = false;
    bool _strikeThrough = false;
    Color _color = Color::Empty();
};

}}  // namespace Aspose::Cells_FOSS
