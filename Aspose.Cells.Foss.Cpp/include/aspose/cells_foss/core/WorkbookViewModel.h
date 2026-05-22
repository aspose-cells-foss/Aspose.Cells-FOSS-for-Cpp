#pragma once

#include <optional>
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents workbook view model.
/// </summary>
class WorkbookViewModel {
public:
    /// <summary>
    /// Constructs a WorkbookViewModel with default values.
    /// </summary>
    WorkbookViewModel();

    /// <summary>
    /// Gets or sets the x window.
    /// </summary>
    std::optional<int> GetXWindow() const;
    void SetXWindow(std::optional<int> value);

    /// <summary>
    /// Gets or sets the y window.
    /// </summary>
    std::optional<int> GetYWindow() const;
    void SetYWindow(std::optional<int> value);

    /// <summary>
    /// Gets or sets the window width.
    /// </summary>
    std::optional<int> GetWindowWidth() const;
    void SetWindowWidth(std::optional<int> value);

    /// <summary>
    /// Gets or sets the window height.
    /// </summary>
    std::optional<int> GetWindowHeight() const;
    void SetWindowHeight(std::optional<int> value);

    /// <summary>
    /// Gets or sets the first sheet.
    /// </summary>
    std::optional<int> GetFirstSheet() const;
    void SetFirstSheet(std::optional<int> value);

    /// <summary>
    /// Gets or sets a value indicating whether show horizontal scroll.
    /// </summary>
    std::optional<bool> GetShowHorizontalScroll() const;
    void SetShowHorizontalScroll(std::optional<bool> value);

    /// <summary>
    /// Gets or sets a value indicating whether show vertical scroll.
    /// </summary>
    std::optional<bool> GetShowVerticalScroll() const;
    void SetShowVerticalScroll(std::optional<bool> value);

    /// <summary>
    /// Gets or sets a value indicating whether show sheet tabs.
    /// </summary>
    std::optional<bool> GetShowSheetTabs() const;
    void SetShowSheetTabs(std::optional<bool> value);

    /// <summary>
    /// Gets or sets the tab ratio.
    /// </summary>
    std::optional<int> GetTabRatio() const;
    void SetTabRatio(std::optional<int> value);

    /// <summary>
    /// Gets or sets the visibility.
    /// </summary>
    const std::string& GetVisibility() const;
    void SetVisibility(std::string value);

    /// <summary>
    /// Gets or sets a value indicating whether minimized.
    /// </summary>
    bool GetMinimized() const;
    void SetMinimized(bool value);

    /// <summary>
    /// Gets or sets a value indicating whether auto filter date grouping.
    /// </summary>
    bool GetAutoFilterDateGrouping() const;
    void SetAutoFilterDateGrouping(bool value);

    /// <summary>
    /// Copies values from the specified source.
    /// </summary>
    /// <param name="source">The source.</param>
    void CopyFrom(const WorkbookViewModel& source);

    /// <summary>
    /// Performs has stored state.
    /// </summary>
    /// <param name="activeSheetIndex">The active sheet index.</param>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool HasStoredState(int activeSheetIndex) const;

private:
    std::optional<int> m_xWindow;
    std::optional<int> m_yWindow;
    std::optional<int> m_windowWidth;
    std::optional<int> m_windowHeight;
    std::optional<int> m_firstSheet;
    std::optional<bool> m_showHorizontalScroll;
    std::optional<bool> m_showVerticalScroll;
    std::optional<bool> m_showSheetTabs;
    std::optional<int> m_tabRatio;
    std::string m_visibility;
    bool m_minimized;
    bool m_autoFilterDateGrouping;
};

} // namespace Core
} // namespace Cells_FOSS
} // namespace Aspose
