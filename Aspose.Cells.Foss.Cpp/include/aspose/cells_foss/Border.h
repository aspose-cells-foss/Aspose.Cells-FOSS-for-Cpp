#pragma once

#include "aspose/cells_foss/BorderStyleType.h"
#include "aspose/cells_foss/Color.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents border.
/// </summary>
class Border {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="Border"/> class.
    /// </summary>
    Border() noexcept = default;

    /// <summary>
    /// Gets or sets the line style.
    /// </summary>
    BorderStyleType GetLineStyle() const noexcept { return _lineStyle; }

    /// <summary>
    /// Sets the line style.
    /// </summary>
    void SetLineStyle(BorderStyleType value) noexcept { _lineStyle = value; }

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
    /// <returns>The border.</returns>
    Border Clone() const;

private:
    BorderStyleType _lineStyle = BorderStyleType::None;
    Color _color = Color::Empty();
};

}}  // namespace Aspose::Cells_FOSS
