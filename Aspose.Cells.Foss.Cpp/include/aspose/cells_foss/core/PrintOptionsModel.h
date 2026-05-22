#pragma once

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents print options model.
/// </summary>
class PrintOptionsModel final {
public:
    /// <summary>
    /// Initializes a new instance of the PrintOptionsModel class.
    /// </summary>
    PrintOptionsModel();

    /// <summary>
    /// Gets or sets a value indicating whether grid lines.
    /// </summary>
    bool GetGridLines() const noexcept { return _gridLines; }
    void SetGridLines(bool value) noexcept { _gridLines = value; }

    /// <summary>
    /// Gets or sets a value indicating whether headings.
    /// </summary>
    bool GetHeadings() const noexcept { return _headings; }
    void SetHeadings(bool value) noexcept { _headings = value; }

    /// <summary>
    /// Gets or sets a value indicating whether horizontal centered.
    /// </summary>
    bool GetHorizontalCentered() const noexcept { return _horizontalCentered; }
    void SetHorizontalCentered(bool value) noexcept { _horizontalCentered = value; }

    /// <summary>
    /// Gets or sets a value indicating whether vertical centered.
    /// </summary>
    bool GetVerticalCentered() const noexcept { return _verticalCentered; }
    void SetVerticalCentered(bool value) noexcept { _verticalCentered = value; }

private:
    bool _gridLines = false;
    bool _headings = false;
    bool _horizontalCentered = false;
    bool _verticalCentered = false;
};

}}}  // namespace Aspose::Cells_FOSS::Core
