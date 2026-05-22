#pragma once

#include "aspose/cells_foss/core/BorderSideValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents borders value.
/// </summary>
class BordersValue {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="BordersValue"/> class.
    /// </summary>
    BordersValue() noexcept = default;

    /// <summary>
    /// Performs border side value.
    /// </summary>
    const BorderSideValue& GetLeft() const noexcept { return _left; }
    void SetLeft(const BorderSideValue& value) noexcept { _left = value; }

    /// <summary>
    /// Performs border side value.
    /// </summary>
    const BorderSideValue& GetRight() const noexcept { return _right; }
    void SetRight(const BorderSideValue& value) noexcept { _right = value; }

    /// <summary>
    /// Performs border side value.
    /// </summary>
    const BorderSideValue& GetTop() const noexcept { return _top; }
    void SetTop(const BorderSideValue& value) noexcept { _top = value; }

    /// <summary>
    /// Performs border side value.
    /// </summary>
    const BorderSideValue& GetBottom() const noexcept { return _bottom; }
    void SetBottom(const BorderSideValue& value) noexcept { _bottom = value; }

    /// <summary>
    /// Performs border side value.
    /// </summary>
    const BorderSideValue& GetDiagonal() const noexcept { return _diagonal; }
    void SetDiagonal(const BorderSideValue& value) noexcept { _diagonal = value; }

    /// <summary>
    /// Gets or sets a value indicating whether diagonal up.
    /// </summary>
    bool GetDiagonalUp() const noexcept { return _diagonalUp; }
    void SetDiagonalUp(bool value) noexcept { _diagonalUp = value; }

    /// <summary>
    /// Gets or sets a value indicating whether diagonal down.
    /// </summary>
    bool GetDiagonalDown() const noexcept { return _diagonalDown; }
    void SetDiagonalDown(bool value) noexcept { _diagonalDown = value; }

    /// <summary>
    /// Creates a copy of the current instance.
    /// </summary>
    /// <returns>The borders value.</returns>
    BordersValue Clone() const;

private:
    BorderSideValue _left;
    BorderSideValue _right;
    BorderSideValue _top;
    BorderSideValue _bottom;
    BorderSideValue _diagonal;
    bool _diagonalUp = false;
    bool _diagonalDown = false;
};

}}}  // namespace Aspose::Cells_FOSS::Core
