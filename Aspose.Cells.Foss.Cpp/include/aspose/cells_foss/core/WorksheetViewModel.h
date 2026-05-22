#pragma once

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents worksheet view model.
/// </summary>
class WorksheetViewModel {
public:
    /// <summary>
    /// Constructs a WorksheetViewModel with default values.
    /// </summary>
    WorksheetViewModel();

    /// <summary>
    /// Gets or sets a value indicating whether show grid lines.
    /// </summary>
    bool GetShowGridLines() const;
    void SetShowGridLines(bool value);

    /// <summary>
    /// Gets or sets a value indicating whether show row column headers.
    /// </summary>
    bool GetShowRowColumnHeaders() const;
    void SetShowRowColumnHeaders(bool value);

    /// <summary>
    /// Gets or sets a value indicating whether show zeros.
    /// </summary>
    bool GetShowZeros() const;
    void SetShowZeros(bool value);

    /// <summary>
    /// Gets or sets a value indicating whether right to left.
    /// </summary>
    bool GetRightToLeft() const;
    void SetRightToLeft(bool value);

    /// <summary>
    /// Gets or sets the zoom scale.
    /// </summary>
    int GetZoomScale() const;
    void SetZoomScale(int value);

private:
    bool m_showGridLines;
    bool m_showRowColumnHeaders;
    bool m_showZeros;
    bool m_rightToLeft;
    int m_zoomScale;
};

} // namespace Core
} // namespace Cells_FOSS
} // namespace Aspose
