#include "aspose/cells_foss/WorkbookProtection.h"
#include "aspose/cells_foss/core/WorkbookProtectionModel.h"

#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

WorkbookProtection::WorkbookProtection(
    Core::WorkbookProtectionModel& model)
    : _model(model)
{
}

// ---------------------------------------------------------------------------
// IsProtected
// ---------------------------------------------------------------------------

bool WorkbookProtection::IsProtected() const noexcept
{
    return _model.HasStoredState();
}

// ---------------------------------------------------------------------------
// LockStructure
// ---------------------------------------------------------------------------

bool WorkbookProtection::GetLockStructure() const noexcept
{
    return _model.GetLockStructure();
}

void WorkbookProtection::SetLockStructure(bool value) noexcept
{
    _model.SetLockStructure(value);
}

// ---------------------------------------------------------------------------
// LockWindows
// ---------------------------------------------------------------------------

bool WorkbookProtection::GetLockWindows() const noexcept
{
    return _model.GetLockWindows();
}

void WorkbookProtection::SetLockWindows(bool value) noexcept
{
    _model.SetLockWindows(value);
}

// ---------------------------------------------------------------------------
// LockRevision
// ---------------------------------------------------------------------------

bool WorkbookProtection::GetLockRevision() const noexcept
{
    return _model.GetLockRevision();
}

void WorkbookProtection::SetLockRevision(bool value) noexcept
{
    _model.SetLockRevision(value);
}

// ---------------------------------------------------------------------------
// WorkbookPassword
// ---------------------------------------------------------------------------

std::string WorkbookProtection::GetWorkbookPassword() const
{
    return _model.GetWorkbookPassword();
}

void WorkbookProtection::SetWorkbookPassword(std::string_view value)
{
    _model.SetWorkbookPassword(std::string(value));
}

// ---------------------------------------------------------------------------
// RevisionsPassword
// ---------------------------------------------------------------------------

std::string WorkbookProtection::GetRevisionsPassword() const
{
    return _model.GetRevisionsPassword();
}

void WorkbookProtection::SetRevisionsPassword(std::string_view value)
{
    _model.SetRevisionsPassword(std::string(value));
}

}}  // namespace Aspose::Cells_FOSS
