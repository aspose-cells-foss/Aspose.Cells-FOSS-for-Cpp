#pragma once

#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

namespace Core {
class WorkbookModel;
class WorkbookViewModel;
}  // namespace Core

/// Represents workbook view.
class WorkbookView final {
public:
    /// Constructs a WorkbookView wrapping the specified workbook model.
    /// @param workbookModel The underlying workbook model.
    explicit WorkbookView(Core::WorkbookModel& workbookModel);

    // WorkbookView is non-copyable (matches C# sealed + readonly refs).
    WorkbookView(const WorkbookView&) = delete;
    WorkbookView& operator=(const WorkbookView&) = delete;

    /// Gets or sets the x window.
    int GetXWindow() const noexcept;
    void SetXWindow(int value) noexcept;

    /// Gets or sets the y window.
    int GetYWindow() const noexcept;
    void SetYWindow(int value) noexcept;

    /// Gets or sets the window width.
    int GetWindowWidth() const noexcept;
    void SetWindowWidth(int value);

    /// Gets or sets the window height.
    int GetWindowHeight() const noexcept;
    void SetWindowHeight(int value);

    /// Gets or sets the active tab.
    int GetActiveTab() const;
    void SetActiveTab(int value);

    /// Gets or sets the first sheet.
    int GetFirstSheet() const noexcept;
    void SetFirstSheet(int value);

    /// Gets or sets a value indicating whether show horizontal scroll.
    bool GetShowHorizontalScroll() const noexcept;
    void SetShowHorizontalScroll(bool value) noexcept;

    /// Gets or sets a value indicating whether show vertical scroll.
    bool GetShowVerticalScroll() const noexcept;
    void SetShowVerticalScroll(bool value) noexcept;

    /// Gets or sets a value indicating whether show sheet tabs.
    bool GetShowSheetTabs() const noexcept;
    void SetShowSheetTabs(bool value) noexcept;

    /// Gets or sets the tab ratio.
    int GetTabRatio() const noexcept;
    void SetTabRatio(int value);

    /// Gets or sets the visibility.
    std::string GetVisibility() const;
    void SetVisibility(std::string_view value);

    /// Gets or sets a value indicating whether minimized.
    bool GetMinimized() const noexcept;
    void SetMinimized(bool value) noexcept;

    /// Gets or sets a value indicating whether auto filter date grouping.
    bool GetAutoFilterDateGrouping() const noexcept;
    void SetAutoFilterDateGrouping(bool value) noexcept;

private:
    Core::WorkbookModel& _workbookModel;
    Core::WorkbookViewModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
