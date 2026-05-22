#include "aspose/cells_foss/WorksheetProtection.h"
#include "aspose/cells_foss/core/WorksheetProtectionModel.h"

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

WorksheetProtection::WorksheetProtection(
    Core::WorksheetProtectionModel& model)
    : _model(model)
{
}

// ---------------------------------------------------------------------------
// IsProtected
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetIsProtected() const noexcept
{
    return _model.GetIsProtected();
}

void WorksheetProtection::SetIsProtected(bool value) noexcept
{
    _model.SetIsProtected(value);
}

// ---------------------------------------------------------------------------
// Objects
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetObjects() const noexcept
{
    return _model.GetObjects();
}

void WorksheetProtection::SetObjects(bool value) noexcept
{
    _model.SetObjects(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// Scenarios
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetScenarios() const noexcept
{
    return _model.GetScenarios();
}

void WorksheetProtection::SetScenarios(bool value) noexcept
{
    _model.SetScenarios(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// FormatCells
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetFormatCells() const noexcept
{
    return _model.GetFormatCells();
}

void WorksheetProtection::SetFormatCells(bool value) noexcept
{
    _model.SetFormatCells(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// FormatColumns
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetFormatColumns() const noexcept
{
    return _model.GetFormatColumns();
}

void WorksheetProtection::SetFormatColumns(bool value) noexcept
{
    _model.SetFormatColumns(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// FormatRows
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetFormatRows() const noexcept
{
    return _model.GetFormatRows();
}

void WorksheetProtection::SetFormatRows(bool value) noexcept
{
    _model.SetFormatRows(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// InsertColumns
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetInsertColumns() const noexcept
{
    return _model.GetInsertColumns();
}

void WorksheetProtection::SetInsertColumns(bool value) noexcept
{
    _model.SetInsertColumns(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// InsertRows
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetInsertRows() const noexcept
{
    return _model.GetInsertRows();
}

void WorksheetProtection::SetInsertRows(bool value) noexcept
{
    _model.SetInsertRows(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// InsertHyperlinks
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetInsertHyperlinks() const noexcept
{
    return _model.GetInsertHyperlinks();
}

void WorksheetProtection::SetInsertHyperlinks(bool value) noexcept
{
    _model.SetInsertHyperlinks(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// DeleteColumns
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetDeleteColumns() const noexcept
{
    return _model.GetDeleteColumns();
}

void WorksheetProtection::SetDeleteColumns(bool value) noexcept
{
    _model.SetDeleteColumns(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// DeleteRows
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetDeleteRows() const noexcept
{
    return _model.GetDeleteRows();
}

void WorksheetProtection::SetDeleteRows(bool value) noexcept
{
    _model.SetDeleteRows(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// SelectLockedCells
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetSelectLockedCells() const noexcept
{
    return _model.GetSelectLockedCells();
}

void WorksheetProtection::SetSelectLockedCells(bool value) noexcept
{
    _model.SetSelectLockedCells(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// Sort
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetSort() const noexcept
{
    return _model.GetSort();
}

void WorksheetProtection::SetSort(bool value) noexcept
{
    _model.SetSort(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// AutoFilter
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetAutoFilter() const noexcept
{
    return _model.GetAutoFilter();
}

void WorksheetProtection::SetAutoFilter(bool value) noexcept
{
    _model.SetAutoFilter(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// PivotTables
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetPivotTables() const noexcept
{
    return _model.GetPivotTables();
}

void WorksheetProtection::SetPivotTables(bool value) noexcept
{
    _model.SetPivotTables(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// SelectUnlockedCells
// ---------------------------------------------------------------------------

bool WorksheetProtection::GetSelectUnlockedCells() const noexcept
{
    return _model.GetSelectUnlockedCells();
}

void WorksheetProtection::SetSelectUnlockedCells(bool value) noexcept
{
    _model.SetSelectUnlockedCells(value);
    MarkProtectedWhenEnabled(value);
}

// ---------------------------------------------------------------------------
// Reset
// ---------------------------------------------------------------------------

void WorksheetProtection::Reset()
{
    _model.Clear();
}

// ---------------------------------------------------------------------------
// MarkProtectedWhenEnabled (private)
// ---------------------------------------------------------------------------

void WorksheetProtection::MarkProtectedWhenEnabled(bool value) noexcept
{
    if (value)
    {
        _model.SetIsProtected(true);
    }
}

}}  // namespace Aspose::Cells_FOSS
