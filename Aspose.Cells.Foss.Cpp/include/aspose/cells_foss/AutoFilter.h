#pragma once

#include <string>

#include "aspose/cells_foss/AutoFilterSortState.h"
#include "aspose/cells_foss/FilterColumnCollection.h"
#include "aspose/cells_foss/core/AutoFilterModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents auto filter.
/// </summary>
class AutoFilter final {
public:
    /// <summary>
    /// Initializes a new instance of the AutoFilter class.
    /// </summary>
    /// <param name="model">The underlying auto filter model.</param>
    explicit AutoFilter(Core::AutoFilterModel& model);

    /// <summary>
    /// Gets or sets the range.
    /// </summary>
    std::string GetRange() const;
    void SetRange(const std::string& value);

    /// <summary>
    /// Gets the filter columns.
    /// </summary>
    FilterColumnCollection& GetFilterColumns() noexcept;

    /// <summary>
    /// Gets the sort state.
    /// </summary>
    AutoFilterSortState& GetSortState() noexcept;

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear();

private:
    Core::AutoFilterModel& _model;
    FilterColumnCollection _filterColumns;
    AutoFilterSortState _sortState;
};

}}  // namespace Aspose::Cells_FOSS
