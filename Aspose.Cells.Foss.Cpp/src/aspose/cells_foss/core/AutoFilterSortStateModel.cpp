#include "aspose/cells_foss/core/AutoFilterSortStateModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

AutoFilterSortStateModel::AutoFilterSortStateModel() = default;

void AutoFilterSortStateModel::Clear()
{
    _columnSort = false;
    _caseSensitive = false;
    _sortMethod.clear();
    _ref.clear();
    _conditions.clear();
}

bool AutoFilterSortStateModel::HasStoredState() const
{
    return !_ref.empty()
        || _columnSort
        || _caseSensitive
        || !_sortMethod.empty()
        || !_conditions.empty();
}

}}}  // namespace Aspose::Cells_FOSS::Core
