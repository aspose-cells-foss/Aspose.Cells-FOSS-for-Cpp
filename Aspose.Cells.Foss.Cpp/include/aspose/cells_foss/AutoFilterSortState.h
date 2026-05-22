#pragma once

#include <string>

#include "aspose/cells_foss/AutoFilterSortConditionCollection.h"
#include "aspose/cells_foss/core/AutoFilterSortStateModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents auto filter sort state.
/// </summary>
class AutoFilterSortState final {
public:
    /// <summary>
    /// Initializes a new instance of the AutoFilterSortState class.
    /// </summary>
    /// <param name="model">The underlying sort state model.</param>
    explicit AutoFilterSortState(Core::AutoFilterSortStateModel& model);

    /// <summary>
    /// Gets or sets a value indicating whether column sort.
    /// </summary>
    bool GetColumnSort() const noexcept;
    void SetColumnSort(bool value) noexcept;

    /// <summary>
    /// Gets or sets a value indicating whether case sensitive.
    /// </summary>
    bool GetCaseSensitive() const noexcept;
    void SetCaseSensitive(bool value) noexcept;

    /// <summary>
    /// Gets or sets the sort method.
    /// </summary>
    std::string GetSortMethod() const;
    void SetSortMethod(const std::string& value);

    /// <summary>
    /// Gets or sets the ref.
    /// </summary>
    std::string GetRef() const;
    void SetRef(const std::string& value);

    /// <summary>
    /// Gets the sort conditions.
    /// </summary>
    AutoFilterSortConditionCollection& GetSortConditions() noexcept;

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear();

private:
    Core::AutoFilterSortStateModel& _model;
    AutoFilterSortConditionCollection _conditions;
};

}}  // namespace Aspose::Cells_FOSS
