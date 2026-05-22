#pragma once

#include "aspose/cells_foss/core/HorizontalAlignment.h"
#include "aspose/cells_foss/core/VerticalAlignment.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents alignment value.
/// </summary>
class AlignmentValue {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="AlignmentValue"/> class.
    /// </summary>
    AlignmentValue() noexcept = default;

    /// <summary>
    /// Gets or sets the horizontal.
    /// </summary>
    HorizontalAlignment GetHorizontal() const noexcept { return _horizontal; }
    void SetHorizontal(HorizontalAlignment value) noexcept { _horizontal = value; }

    /// <summary>
    /// Gets or sets the vertical.
    /// </summary>
    VerticalAlignment GetVertical() const noexcept { return _vertical; }
    void SetVertical(VerticalAlignment value) noexcept { _vertical = value; }

    /// <summary>
    /// Gets or sets a value indicating whether wrap text.
    /// </summary>
    bool GetWrapText() const noexcept { return _wrapText; }
    void SetWrapText(bool value) noexcept { _wrapText = value; }

    /// <summary>
    /// Gets or sets the indent level.
    /// </summary>
    int GetIndentLevel() const noexcept { return _indentLevel; }
    void SetIndentLevel(int value) noexcept { _indentLevel = value; }

    /// <summary>
    /// Gets or sets the text rotation.
    /// </summary>
    int GetTextRotation() const noexcept { return _textRotation; }
    void SetTextRotation(int value) noexcept { _textRotation = value; }

    /// <summary>
    /// Gets or sets a value indicating whether shrink to fit.
    /// </summary>
    bool GetShrinkToFit() const noexcept { return _shrinkToFit; }
    void SetShrinkToFit(bool value) noexcept { _shrinkToFit = value; }

    /// <summary>
    /// Gets or sets the reading order.
    /// </summary>
    int GetReadingOrder() const noexcept { return _readingOrder; }
    void SetReadingOrder(int value) noexcept { _readingOrder = value; }

    /// <summary>
    /// Gets or sets the relative indent.
    /// </summary>
    int GetRelativeIndent() const noexcept { return _relativeIndent; }
    void SetRelativeIndent(int value) noexcept { _relativeIndent = value; }

    /// <summary>
    /// Creates a copy of the current instance.
    /// </summary>
    /// <returns>The alignment value.</returns>
    AlignmentValue Clone() const;

private:
    HorizontalAlignment _horizontal = HorizontalAlignment::General;
    VerticalAlignment _vertical = VerticalAlignment::Bottom;
    bool _wrapText = false;
    int _indentLevel = 0;
    int _textRotation = 0;
    bool _shrinkToFit = false;
    int _readingOrder = 0;
    int _relativeIndent = 0;
};

}}}  // namespace Aspose::Cells_FOSS::Core
