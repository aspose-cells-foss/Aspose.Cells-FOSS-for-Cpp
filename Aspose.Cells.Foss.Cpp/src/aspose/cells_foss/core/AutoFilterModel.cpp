#include "aspose/cells_foss/core/AutoFilterModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

AutoFilterModel::AutoFilterModel()
    : _range()
    , _filterColumns()
    , _sortState()
{
}

void AutoFilterModel::Clear()
{
    _range.clear();
    _filterColumns.clear();
    _sortState.Clear();
}

bool AutoFilterModel::HasStoredState() const
{
    return !_range.empty();
}

}}}  // namespace Aspose::Cells_FOSS::Core
