#pragma once

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents page margins model.
/// </summary>
class PageMarginsModel final {
public:
    /// <summary>
    /// Initializes a new instance of the PageMarginsModel class.
    /// </summary>
    PageMarginsModel();

    /// <summary>
    /// Gets or sets the left.
    /// </summary>
    double GetLeft() const noexcept { return _left; }
    void SetLeft(double value) noexcept { _left = value; }

    /// <summary>
    /// Gets or sets the right.
    /// </summary>
    double GetRight() const noexcept { return _right; }
    void SetRight(double value) noexcept { _right = value; }

    /// <summary>
    /// Gets or sets the top.
    /// </summary>
    double GetTop() const noexcept { return _top; }
    void SetTop(double value) noexcept { _top = value; }

    /// <summary>
    /// Gets or sets the bottom.
    /// </summary>
    double GetBottom() const noexcept { return _bottom; }
    void SetBottom(double value) noexcept { _bottom = value; }

    /// <summary>
    /// Gets or sets the header.
    /// </summary>
    double GetHeader() const noexcept { return _header; }
    void SetHeader(double value) noexcept { _header = value; }

    /// <summary>
    /// Gets or sets the footer.
    /// </summary>
    double GetFooter() const noexcept { return _footer; }
    void SetFooter(double value) noexcept { _footer = value; }

private:
    double _left = 0.7;
    double _right = 0.7;
    double _top = 0.75;
    double _bottom = 0.75;
    double _header = 0.3;
    double _footer = 0.3;
};

}}}  // namespace Aspose::Cells_FOSS::Core
