#pragma once
#include <string>
#include <vector>

#include "aspose/cells_foss/core/AutoFilterSortStateModel.h"
#include "aspose/cells_foss/core/FilterColumnModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents auto filter model.
/// </summary>
class AutoFilterModel {
public:
    /// <summary>
    /// Initializes a new instance of the AutoFilterModel class.
    /// </summary>
    AutoFilterModel();

    /// <summary>
    /// Gets or sets the range.
    /// </summary>
    const std::string& GetRange() const noexcept { return _range; }
    void SetRange(const std::string& value) { _range = value; }
    void SetRange(std::string&& value) noexcept { _range = std::move(value); }

    /// <summary>
    /// Gets the filter columns.
    /// </summary>
    const std::vector<FilterColumnModel>& GetFilterColumns() const noexcept { return _filterColumns; }
    std::vector<FilterColumnModel>& GetFilterColumns() noexcept { return _filterColumns; }

    /// <summary>
    /// Gets the sort state.
    /// </summary>
    const AutoFilterSortStateModel& GetSortState() const noexcept { return _sortState; }
    AutoFilterSortStateModel& GetSortState() noexcept { return _sortState; }

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear();

    /// <summary>
    /// Performs has stored state.
    /// </summary>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool HasStoredState() const;

private:
    std::string _range;
    std::vector<FilterColumnModel> _filterColumns;
    AutoFilterSortStateModel _sortState;
};

}}}  // namespace Aspose::Cells_FOSS::Core
