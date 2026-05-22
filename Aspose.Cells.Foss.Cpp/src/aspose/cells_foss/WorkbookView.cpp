#include "aspose/cells_foss/WorkbookView.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/WorkbookPropertySupport.h"
#include "aspose/cells_foss/core/WorkbookModel.h"
#include "aspose/cells_foss/core/WorkbookViewModel.h"

#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

WorkbookView::WorkbookView(Core::WorkbookModel& workbookModel)
    : _workbookModel(workbookModel)
    , _model(workbookModel.GetProperties().GetView())
{
}

// ---------------------------------------------------------------------------
// XWindow
// ---------------------------------------------------------------------------

int WorkbookView::GetXWindow() const noexcept
{
    return _model.GetXWindow().value_or(0);
}

void WorkbookView::SetXWindow(int value) noexcept
{
    _model.SetXWindow(value);
}

// ---------------------------------------------------------------------------
// YWindow
// ---------------------------------------------------------------------------

int WorkbookView::GetYWindow() const noexcept
{
    return _model.GetYWindow().value_or(0);
}

void WorkbookView::SetYWindow(int value) noexcept
{
    _model.SetYWindow(value);
}

// ---------------------------------------------------------------------------
// WindowWidth
// ---------------------------------------------------------------------------

int WorkbookView::GetWindowWidth() const noexcept
{
    return _model.GetWindowWidth().value_or(0);
}

void WorkbookView::SetWindowWidth(int value)
{
    if (value < 0) {
        throw CellsException("WindowWidth must be non-negative.");
    }
    _model.SetWindowWidth(value);
}

// ---------------------------------------------------------------------------
// WindowHeight
// ---------------------------------------------------------------------------

int WorkbookView::GetWindowHeight() const noexcept
{
    return _model.GetWindowHeight().value_or(0);
}

void WorkbookView::SetWindowHeight(int value)
{
    if (value < 0) {
        throw CellsException("WindowHeight must be non-negative.");
    }
    _model.SetWindowHeight(value);
}

// ---------------------------------------------------------------------------
// ActiveTab
// ---------------------------------------------------------------------------

int WorkbookView::GetActiveTab() const
{
    return _workbookModel.GetActiveSheetIndex();
}

void WorkbookView::SetActiveTab(int value)
{
    if (value < 0 ||
        value >= static_cast<int>(_workbookModel.GetWorksheets().size())) {
        throw CellsException("ActiveTab must refer to an existing worksheet.");
    }
    _workbookModel.SetActiveSheetIndex(value);
}

// ---------------------------------------------------------------------------
// FirstSheet
// ---------------------------------------------------------------------------

int WorkbookView::GetFirstSheet() const noexcept
{
    return _model.GetFirstSheet().value_or(0);
}

void WorkbookView::SetFirstSheet(int value)
{
    if (value < 0) {
        throw CellsException("FirstSheet must be non-negative.");
    }
    _model.SetFirstSheet(value);
}

// ---------------------------------------------------------------------------
// ShowHorizontalScroll
// ---------------------------------------------------------------------------

bool WorkbookView::GetShowHorizontalScroll() const noexcept
{
    return !_model.GetShowHorizontalScroll().has_value() ||
           _model.GetShowHorizontalScroll().value();
}

void WorkbookView::SetShowHorizontalScroll(bool value) noexcept
{
    _model.SetShowHorizontalScroll(value);
}

// ---------------------------------------------------------------------------
// ShowVerticalScroll
// ---------------------------------------------------------------------------

bool WorkbookView::GetShowVerticalScroll() const noexcept
{
    return !_model.GetShowVerticalScroll().has_value() ||
           _model.GetShowVerticalScroll().value();
}

void WorkbookView::SetShowVerticalScroll(bool value) noexcept
{
    _model.SetShowVerticalScroll(value);
}

// ---------------------------------------------------------------------------
// ShowSheetTabs
// ---------------------------------------------------------------------------

bool WorkbookView::GetShowSheetTabs() const noexcept
{
    return !_model.GetShowSheetTabs().has_value() ||
           _model.GetShowSheetTabs().value();
}

void WorkbookView::SetShowSheetTabs(bool value) noexcept
{
    _model.SetShowSheetTabs(value);
}

// ---------------------------------------------------------------------------
// TabRatio
// ---------------------------------------------------------------------------

int WorkbookView::GetTabRatio() const noexcept
{
    return _model.GetTabRatio().value_or(600);
}

void WorkbookView::SetTabRatio(int value)
{
    if (value < 0 || value > 1000) {
        throw CellsException("TabRatio must be between 0 and 1000.");
    }
    _model.SetTabRatio(value);
}

// ---------------------------------------------------------------------------
// Visibility
// ---------------------------------------------------------------------------

std::string WorkbookView::GetVisibility() const
{
    const auto& vis = _model.GetVisibility();
    return vis.empty() ? std::string("visible") : vis;
}

void WorkbookView::SetVisibility(std::string_view value)
{
    _model.SetVisibility(WorkbookPropertySupport::NormalizeVisibility(value));
}

// ---------------------------------------------------------------------------
// Minimized
// ---------------------------------------------------------------------------

bool WorkbookView::GetMinimized() const noexcept
{
    return _model.GetMinimized();
}

void WorkbookView::SetMinimized(bool value) noexcept
{
    _model.SetMinimized(value);
}

// ---------------------------------------------------------------------------
// AutoFilterDateGrouping
// ---------------------------------------------------------------------------

bool WorkbookView::GetAutoFilterDateGrouping() const noexcept
{
    return _model.GetAutoFilterDateGrouping();
}

void WorkbookView::SetAutoFilterDateGrouping(bool value) noexcept
{
    _model.SetAutoFilterDateGrouping(value);
}

}}  // namespace Aspose::Cells_FOSS
