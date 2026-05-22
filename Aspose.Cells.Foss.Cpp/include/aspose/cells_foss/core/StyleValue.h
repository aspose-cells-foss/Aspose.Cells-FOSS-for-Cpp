#pragma once

#include "aspose/cells_foss/core/AlignmentValue.h"
#include "aspose/cells_foss/core/BordersValue.h"
#include "aspose/cells_foss/core/ColorValue.h"
#include "aspose/cells_foss/core/FillPatternKind.h"
#include "aspose/cells_foss/core/FontValue.h"
#include "aspose/cells_foss/core/NumberFormatValue.h"
#include "aspose/cells_foss/core/ProtectionValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents style value.
/// </summary>
class StyleValue final {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="StyleValue"/> class.
    /// </summary>
    StyleValue() = default;

    /// <summary>
    /// Gets the default.
    /// </summary>
    static StyleValue Default();

    /// <summary>
    /// Performs font value.
    /// </summary>
    const FontValue& GetFont() const noexcept { return _font; }
    void SetFont(const FontValue& value) { _font = value; }

    /// <summary>
    /// Gets or sets the pattern.
    /// </summary>
    FillPatternKind GetPattern() const noexcept { return _pattern; }
    void SetPattern(FillPatternKind value) noexcept { _pattern = value; }

    /// <summary>
    /// Gets or sets the foreground color.
    /// </summary>
    const ColorValue& GetForegroundColor() const noexcept { return _foregroundColor; }
    void SetForegroundColor(const ColorValue& value) noexcept { _foregroundColor = value; }

    /// <summary>
    /// Gets or sets the background color.
    /// </summary>
    const ColorValue& GetBackgroundColor() const noexcept { return _backgroundColor; }
    void SetBackgroundColor(const ColorValue& value) noexcept { _backgroundColor = value; }

    /// <summary>
    /// Performs borders value.
    /// </summary>
    const BordersValue& GetBorders() const noexcept { return _borders; }
    void SetBorders(const BordersValue& value) { _borders = value; }

    /// <summary>
    /// Performs alignment value.
    /// </summary>
    const AlignmentValue& GetAlignment() const noexcept { return _alignment; }
    void SetAlignment(const AlignmentValue& value) { _alignment = value; }

    /// <summary>
    /// Performs protection value.
    /// </summary>
    const ProtectionValue& GetProtection() const noexcept { return _protection; }
    void SetProtection(const ProtectionValue& value) { _protection = value; }

    /// <summary>
    /// Performs number format value.
    /// </summary>
    const NumberFormatValue& GetNumberFormat() const noexcept { return _numberFormat; }
    void SetNumberFormat(const NumberFormatValue& value) { _numberFormat = value; }

    /// <summary>
    /// Creates a copy of the current instance.
    /// </summary>
    /// <returns>The style value.</returns>
    StyleValue Clone() const;

private:
    FontValue _font;
    FillPatternKind _pattern = FillPatternKind::None;
    ColorValue _foregroundColor;
    ColorValue _backgroundColor;
    BordersValue _borders;
    AlignmentValue _alignment;
    ProtectionValue _protection;
    NumberFormatValue _numberFormat;
};

}}}  // namespace Aspose::Cells_FOSS::Core
