#pragma once

#include "aspose/cells_foss/core/BorderStyle.h"
#include "aspose/cells_foss/core/ColorValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents border side value.
/// </summary>
class BorderSideValue {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="BorderSideValue"/> class.
    /// </summary>
    BorderSideValue() noexcept = default;

    /// <summary>
    /// Gets or sets the style.
    /// </summary>
    BorderStyle GetStyle() const noexcept { return _style; }
    void SetStyle(BorderStyle value) noexcept { _style = value; }

    /// <summary>
    /// Gets or sets the color.
    /// </summary>
    const ColorValue& GetColor() const noexcept { return _color; }
    void SetColor(const ColorValue& value) noexcept { _color = value; }

    /// <summary>
    /// Creates a copy of the current instance.
    /// </summary>
    /// <returns>The border side value.</returns>
    BorderSideValue Clone() const;

private:
    BorderStyle _style = BorderStyle::None;
    ColorValue _color;
};

}}}  // namespace Aspose::Cells_FOSS::Core
