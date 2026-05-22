#pragma once

#include "aspose/cells_foss/Border.h"
#include "aspose/cells_foss/Borders.h"
#include "aspose/cells_foss/Color.h"
#include "aspose/cells_foss/FillPattern.h"
#include "aspose/cells_foss/Font.h"
#include "aspose/cells_foss/HorizontalAlignmentType.h"
#include "aspose/cells_foss/VerticalAlignmentType.h"
#include "aspose/cells_foss/core/StyleValue.h"

#include <string>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents a mutable cell style facade that can be applied to one or more cells.
/// </summary>
class Style {
public:
    /// <summary>
    /// Initializes a new style with default font and border objects.
    /// </summary>
    Style();

    /// <summary>
    /// Gets the font settings.
    /// </summary>
    const Font& GetFont() const noexcept { return _font; }

    /// <summary>
    /// Sets the font settings.
    /// </summary>
    void SetFont(const Font& value) { _font = value; }

    /// <summary>
    /// Gets the border settings.
    /// </summary>
    const Borders& GetBorders() const noexcept { return _borders; }

    /// <summary>
    /// Sets the border settings.
    /// </summary>
    void SetBorders(const Borders& value) { _borders = value; }

    /// <summary>
    /// Gets the fill pattern.
    /// </summary>
    FillPattern GetPattern() const noexcept { return _pattern; }

    /// <summary>
    /// Sets the fill pattern.
    /// </summary>
    void SetPattern(FillPattern value) noexcept { _pattern = value; }

    /// <summary>
    /// Gets the fill foreground color.
    /// </summary>
    const Color& GetForegroundColor() const noexcept { return _foregroundColor; }

    /// <summary>
    /// Sets the fill foreground color.
    /// </summary>
    void SetForegroundColor(const Color& value) noexcept { _foregroundColor = value; }

    /// <summary>
    /// Gets the fill background color.
    /// </summary>
    const Color& GetBackgroundColor() const noexcept { return _backgroundColor; }

    /// <summary>
    /// Sets the fill background color.
    /// </summary>
    void SetBackgroundColor(const Color& value) noexcept { _backgroundColor = value; }

    /// <summary>
    /// Gets the numeric format identifier.
    /// </summary>
    int GetNumber() const noexcept { return _number; }

    /// <summary>
    /// Sets the numeric format identifier.
    /// </summary>
    void SetNumber(int value) noexcept { _number = value; }

    /// <summary>
    /// Gets the custom number format code.
    /// </summary>
    const std::string& GetCustom() const noexcept { return _custom; }

    /// <summary>
    /// Sets the custom number format code.
    /// </summary>
    void SetCustom(const std::string& value) { _custom = value; }

    /// <summary>
    /// Gets the resolved number format string.
    /// </summary>
    std::string GetNumberFormat() const;

    /// <summary>
    /// Sets the number format. Accepts either a built-in format string or a custom format code.
    /// </summary>
    void SetNumberFormat(const std::string& value);

    /// <summary>
    /// Gets the horizontal alignment.
    /// </summary>
    HorizontalAlignmentType GetHorizontalAlignment() const noexcept { return _horizontalAlignment; }

    /// <summary>
    /// Sets the horizontal alignment.
    /// </summary>
    void SetHorizontalAlignment(HorizontalAlignmentType value) noexcept { _horizontalAlignment = value; }

    /// <summary>
    /// Gets the vertical alignment.
    /// </summary>
    VerticalAlignmentType GetVerticalAlignment() const noexcept { return _verticalAlignment; }

    /// <summary>
    /// Sets the vertical alignment.
    /// </summary>
    void SetVerticalAlignment(VerticalAlignmentType value) noexcept { _verticalAlignment = value; }

    /// <summary>
    /// Gets whether text wraps within the cell.
    /// </summary>
    bool GetWrapText() const noexcept { return _wrapText; }

    /// <summary>
    /// Sets whether text wraps within the cell.
    /// </summary>
    void SetWrapText(bool value) noexcept { _wrapText = value; }

    /// <summary>
    /// Gets the indentation level.
    /// </summary>
    int GetIndentLevel() const noexcept { return _indentLevel; }

    /// <summary>
    /// Sets the indentation level. Must be between 0 and 250.
    /// </summary>
    void SetIndentLevel(int value);

    /// <summary>
    /// Gets the text rotation.
    /// </summary>
    int GetTextRotation() const noexcept { return _textRotation; }

    /// <summary>
    /// Sets the text rotation. Must be between 0 and 180, or 255 for vertical text.
    /// </summary>
    void SetTextRotation(int value);

    /// <summary>
    /// Gets whether the cell content shrinks to fit.
    /// </summary>
    bool GetShrinkToFit() const noexcept { return _shrinkToFit; }

    /// <summary>
    /// Sets whether the cell content shrinks to fit.
    /// </summary>
    void SetShrinkToFit(bool value) noexcept { _shrinkToFit = value; }

    /// <summary>
    /// Gets the reading order.
    /// </summary>
    int GetReadingOrder() const noexcept { return _readingOrder; }

    /// <summary>
    /// Sets the reading order. Must be 0, 1, or 2.
    /// </summary>
    void SetReadingOrder(int value);

    /// <summary>
    /// Gets the relative indent.
    /// </summary>
    int GetRelativeIndent() const noexcept { return _relativeIndent; }

    /// <summary>
    /// Sets the relative indent.
    /// </summary>
    void SetRelativeIndent(int value) noexcept { _relativeIndent = value; }

    /// <summary>
    /// Gets whether the cell is locked when worksheet protection is enabled.
    /// </summary>
    bool GetIsLocked() const noexcept { return _isLocked; }

    /// <summary>
    /// Sets whether the cell is locked when worksheet protection is enabled.
    /// </summary>
    void SetIsLocked(bool value) noexcept { _isLocked = value; }

    /// <summary>
    /// Gets whether the cell formula is hidden when worksheet protection is enabled.
    /// </summary>
    bool GetIsHidden() const noexcept { return _isHidden; }

    /// <summary>
    /// Sets whether the cell formula is hidden when worksheet protection is enabled.
    /// </summary>
    void SetIsHidden(bool value) noexcept { _isHidden = value; }

    /// <summary>
    /// Creates a copy of the current style.
    /// </summary>
    /// <returns>The cloned style.</returns>
    Style Clone() const;

    /// <summary>
    /// Converts this style to a Core StyleValue.
    /// </summary>
    Core::StyleValue ToCore() const;

    /// <summary>
    /// Creates a Style from a Core StyleValue.
    /// </summary>
    static Style FromCore(const Core::StyleValue& value);

private:
    static Core::BorderSideValue ToCoreBorder(const Border& border);
    static Border FromCoreBorder(const Core::BorderSideValue& value);

    Font _font;
    Borders _borders;
    FillPattern _pattern = FillPattern::None;
    Color _foregroundColor = Color::Empty();
    Color _backgroundColor = Color::Empty();
    int _number = 0;
    std::string _custom;
    HorizontalAlignmentType _horizontalAlignment = HorizontalAlignmentType::General;
    VerticalAlignmentType _verticalAlignment = VerticalAlignmentType::Bottom;
    bool _wrapText = false;
    int _indentLevel = 0;
    int _textRotation = 0;
    bool _shrinkToFit = false;
    int _readingOrder = 0;
    int _relativeIndent = 0;
    bool _isLocked = true;
    bool _isHidden = false;
};

}}  // namespace Aspose::Cells_FOSS
