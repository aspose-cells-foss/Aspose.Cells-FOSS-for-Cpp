#pragma once
#include <string>
#include <vector>

#include "aspose/cells_foss/core/AutoFilterSortConditionModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents auto filter sort state model.
/// </summary>
class AutoFilterSortStateModel {
public:
    /// <summary>
    /// Initializes a new instance of the AutoFilterSortStateModel class.
    /// </summary>
    AutoFilterSortStateModel();

    /// <summary>
    /// Gets or sets a value indicating whether column sort.
    /// </summary>
    bool GetColumnSort() const noexcept { return _columnSort; }
    void SetColumnSort(bool value) noexcept { _columnSort = value; }

    /// <summary>
    /// Gets or sets a value indicating whether case sensitive.
    /// </summary>
    bool GetCaseSensitive() const noexcept { return _caseSensitive; }
    void SetCaseSensitive(bool value) noexcept { _caseSensitive = value; }

    /// <summary>
    /// Gets or sets the sort method.
    /// </summary>
    const std::string& GetSortMethod() const noexcept { return _sortMethod; }
    void SetSortMethod(const std::string& value) { _sortMethod = value; }
    void SetSortMethod(std::string&& value) noexcept { _sortMethod = std::move(value); }

    /// <summary>
    /// Gets or sets the ref.
    /// </summary>
    const std::string& GetRef() const noexcept { return _ref; }
    void SetRef(const std::string& value) { _ref = value; }
    void SetRef(std::string&& value) noexcept { _ref = std::move(value); }

    /// <summary>
    /// Gets the conditions.
    /// </summary>
    const std::vector<AutoFilterSortConditionModel>& GetConditions() const noexcept { return _conditions; }
    std::vector<AutoFilterSortConditionModel>& GetConditions() noexcept { return _conditions; }

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
    bool _columnSort = false;
    bool _caseSensitive = false;
    std::string _sortMethod;
    std::string _ref;
    std::vector<AutoFilterSortConditionModel> _conditions;
};

}}}  // namespace Aspose::Cells_FOSS::Core
