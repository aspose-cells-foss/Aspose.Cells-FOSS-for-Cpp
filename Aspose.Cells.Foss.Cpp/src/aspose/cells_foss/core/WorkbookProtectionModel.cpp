#include "aspose/cells_foss/core/WorkbookProtectionModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

WorkbookProtectionModel::WorkbookProtectionModel() = default;

void WorkbookProtectionModel::CopyFrom(const WorkbookProtectionModel& source)
{
    _lockStructure = source._lockStructure;
    _lockWindows = source._lockWindows;
    _lockRevision = source._lockRevision;
    _workbookPassword = source._workbookPassword;
    _revisionsPassword = source._revisionsPassword;
}

bool WorkbookProtectionModel::HasStoredState() const
{
    return _lockStructure
        || _lockWindows
        || _lockRevision
        || !_workbookPassword.empty()
        || !_revisionsPassword.empty();
}

}}}  // namespace Aspose::Cells_FOSS::Core
