#pragma once

#include "aspose/cells_foss/core/ColorValue.h"
#include "aspose/cells_foss/core/FillPatternKind.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents fill value.
/// </summary>
class FillValue final {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="FillValue"/> class.
    /// </summary>
    FillValue() = default;

    /// <summary>
    /// Gets or sets the pattern.
    /// </summary>
    Core::FillPatternKind GetPattern() const noexcept { return _pattern; }
    void SetPattern(Core::FillPatternKind value) noexcept { _pattern = value; }

    /// <summary>
    /// Gets or sets the foreground color.
    /// </summary>
    const Core::ColorValue& GetForegroundColor() const noexcept { return _foregroundColor; }
    void SetForegroundColor(const Core::ColorValue& value) noexcept { _foregroundColor = value; }

    /// <summary>
    /// Gets or sets the background color.
    /// </summary>
    const Core::ColorValue& GetBackgroundColor() const noexcept { return _backgroundColor; }
    void SetBackgroundColor(const Core::ColorValue& value) noexcept { _backgroundColor = value; }

private:
    Core::FillPatternKind _pattern = Core::FillPatternKind::None;
    Core::ColorValue _foregroundColor;
    Core::ColorValue _backgroundColor;
};

}}  // namespace Aspose::Cells_FOSS
