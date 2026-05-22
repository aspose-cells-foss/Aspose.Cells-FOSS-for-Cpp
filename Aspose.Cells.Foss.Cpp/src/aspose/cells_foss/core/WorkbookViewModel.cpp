#include "aspose/cells_foss/core/WorkbookViewModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

WorkbookViewModel::WorkbookViewModel()
    : m_minimized(false)
    , m_autoFilterDateGrouping(true)
{
}

std::optional<int> WorkbookViewModel::GetXWindow() const { return m_xWindow; }
void WorkbookViewModel::SetXWindow(std::optional<int> value) { m_xWindow = value; }

std::optional<int> WorkbookViewModel::GetYWindow() const { return m_yWindow; }
void WorkbookViewModel::SetYWindow(std::optional<int> value) { m_yWindow = value; }

std::optional<int> WorkbookViewModel::GetWindowWidth() const { return m_windowWidth; }
void WorkbookViewModel::SetWindowWidth(std::optional<int> value) { m_windowWidth = value; }

std::optional<int> WorkbookViewModel::GetWindowHeight() const { return m_windowHeight; }
void WorkbookViewModel::SetWindowHeight(std::optional<int> value) { m_windowHeight = value; }

std::optional<int> WorkbookViewModel::GetFirstSheet() const { return m_firstSheet; }
void WorkbookViewModel::SetFirstSheet(std::optional<int> value) { m_firstSheet = value; }

std::optional<bool> WorkbookViewModel::GetShowHorizontalScroll() const { return m_showHorizontalScroll; }
void WorkbookViewModel::SetShowHorizontalScroll(std::optional<bool> value) { m_showHorizontalScroll = value; }

std::optional<bool> WorkbookViewModel::GetShowVerticalScroll() const { return m_showVerticalScroll; }
void WorkbookViewModel::SetShowVerticalScroll(std::optional<bool> value) { m_showVerticalScroll = value; }

std::optional<bool> WorkbookViewModel::GetShowSheetTabs() const { return m_showSheetTabs; }
void WorkbookViewModel::SetShowSheetTabs(std::optional<bool> value) { m_showSheetTabs = value; }

std::optional<int> WorkbookViewModel::GetTabRatio() const { return m_tabRatio; }
void WorkbookViewModel::SetTabRatio(std::optional<int> value) { m_tabRatio = value; }

const std::string& WorkbookViewModel::GetVisibility() const { return m_visibility; }
void WorkbookViewModel::SetVisibility(std::string value) { m_visibility = std::move(value); }

bool WorkbookViewModel::GetMinimized() const { return m_minimized; }
void WorkbookViewModel::SetMinimized(bool value) { m_minimized = value; }

bool WorkbookViewModel::GetAutoFilterDateGrouping() const { return m_autoFilterDateGrouping; }
void WorkbookViewModel::SetAutoFilterDateGrouping(bool value) { m_autoFilterDateGrouping = value; }

void WorkbookViewModel::CopyFrom(const WorkbookViewModel& source)
{
    m_xWindow = source.m_xWindow;
    m_yWindow = source.m_yWindow;
    m_windowWidth = source.m_windowWidth;
    m_windowHeight = source.m_windowHeight;
    m_firstSheet = source.m_firstSheet;
    m_showHorizontalScroll = source.m_showHorizontalScroll;
    m_showVerticalScroll = source.m_showVerticalScroll;
    m_showSheetTabs = source.m_showSheetTabs;
    m_tabRatio = source.m_tabRatio;
    m_visibility = source.m_visibility;
    m_minimized = source.m_minimized;
    m_autoFilterDateGrouping = source.m_autoFilterDateGrouping;
}

bool WorkbookViewModel::HasStoredState(int activeSheetIndex) const
{
    return activeSheetIndex > 0
        || m_xWindow.has_value()
        || m_yWindow.has_value()
        || m_windowWidth.has_value()
        || m_windowHeight.has_value()
        || m_firstSheet.has_value()
        || m_showHorizontalScroll.has_value()
        || m_showVerticalScroll.has_value()
        || m_showSheetTabs.has_value()
        || m_tabRatio.has_value()
        || !m_visibility.empty()
        || m_minimized
        || !m_autoFilterDateGrouping;
}

} // namespace Core
} // namespace Cells_FOSS
} // namespace Aspose
