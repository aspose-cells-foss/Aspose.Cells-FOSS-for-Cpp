#pragma once

#include "aspose/cells_foss/Border.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents borders.
/// </summary>
class Borders {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="Borders"/> class.
    /// </summary>
    Borders() noexcept = default;

    /// <summary>
    /// Gets the left border.
    /// </summary>
    const Border& GetLeft() const noexcept { return _left; }

    /// <summary>
    /// Sets the left border.
    /// </summary>
    void SetLeft(const Border& value) noexcept { _left = value; }

    /// <summary>
    /// Gets the right border.
    /// </summary>
    const Border& GetRight() const noexcept { return _right; }

    /// <summary>
    /// Sets the right border.
    /// </summary>
    void SetRight(const Border& value) noexcept { _right = value; }

    /// <summary>
    /// Gets the top border.
    /// </summary>
    const Border& GetTop() const noexcept { return _top; }

    /// <summary>
    /// Sets the top border.
    /// </summary>
    void SetTop(const Border& value) noexcept { _top = value; }

    /// <summary>
    /// Gets the bottom border.
    /// </summary>
    const Border& GetBottom() const noexcept { return _bottom; }

    /// <summary>
    /// Sets the bottom border.
    /// </summary>
    void SetBottom(const Border& value) noexcept { _bottom = value; }

    /// <summary>
    /// Gets the diagonal border.
    /// </summary>
    const Border& GetDiagonal() const noexcept { return _diagonal; }

    /// <summary>
    /// Sets the diagonal border.
    /// </summary>
    void SetDiagonal(const Border& value) noexcept { _diagonal = value; }

    /// <summary>
    /// Gets or sets a value indicating whether diagonal up.
    /// </summary>
    bool GetDiagonalUp() const noexcept { return _diagonalUp; }

    /// <summary>
    /// Sets a value indicating whether diagonal up.
    /// </summary>
    void SetDiagonalUp(bool value) noexcept { _diagonalUp = value; }

    /// <summary>
    /// Gets or sets a value indicating whether diagonal down.
    /// </summary>
    bool GetDiagonalDown() const noexcept { return _diagonalDown; }

    /// <summary>
    /// Sets a value indicating whether diagonal down.
    /// </summary>
    void SetDiagonalDown(bool value) noexcept { _diagonalDown = value; }

    /// <summary>
    /// Creates a copy of the current instance.
    /// </summary>
    /// <returns>The borders.</returns>
    Borders Clone() const;

private:
    Border _left;
    Border _right;
    Border _top;
    Border _bottom;
    Border _diagonal;
    bool _diagonalUp = false;
    bool _diagonalDown = false;
};

}}  // namespace Aspose::Cells_FOSS
