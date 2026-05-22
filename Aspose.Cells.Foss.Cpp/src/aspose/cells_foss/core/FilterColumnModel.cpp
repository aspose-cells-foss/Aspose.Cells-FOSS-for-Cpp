#include "aspose/cells_foss/core/FilterColumnModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

FilterColumnModel::FilterColumnModel() = default;

void FilterColumnModel::ClearCriteria()
{
    _hiddenButton = false;
    _filters.clear();
    _customFilters.clear();
    _customFiltersAnd = false;
    _colorFilter.Clear();
    _dynamicFilter.Clear();
    _top10.Clear();
}

bool FilterColumnModel::HasStoredState() const
{
    return _hiddenButton
        || !_filters.empty()
        || !_customFilters.empty()
        || _colorFilter.GetEnabled()
        || _dynamicFilter.GetEnabled()
        || _top10.GetEnabled();
}

}}}  // namespace Aspose::Cells_FOSS::Core
