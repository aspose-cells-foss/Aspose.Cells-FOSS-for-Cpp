#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

#include "aspose/cells_foss/AutoFilterSortCondition.h"
#include "aspose/cells_foss/core/AutoFilterSortConditionModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents a collection of auto filter sort condition objects.
/// </summary>
class AutoFilterSortConditionCollection final {
public:
    class Iterator;

    /// <summary>
    /// Initializes a new instance of the AutoFilterSortConditionCollection class.
    /// </summary>
    /// <param name="models">The underlying sort condition models.</param>
    explicit AutoFilterSortConditionCollection(
        std::vector<Core::AutoFilterSortConditionModel>& models);

    /// <summary>
    /// Gets the number of items.
    /// </summary>
    int GetCount() const noexcept;

    /// <summary>
    /// Gets the element at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    /// <returns>The sort condition at the given index.</returns>
    AutoFilterSortCondition operator[](int index);

    /// <summary>
    /// Adds the specified item.
    /// </summary>
    /// <param name="reference">The reference.</param>
    /// <returns>The zero-based index of the added item.</returns>
    int Add(std::string_view reference);

    /// <summary>
    /// Removes the specified item.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    void RemoveAt(int index);

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear();

    /// <summary>
    /// Returns a snapshot vector that iterates through the collection.
    /// </summary>
    /// <returns>A vector of sort conditions that can be used to iterate through the collection.</returns>
    std::vector<AutoFilterSortCondition> GetEnumerator();

    /// <summary>
    /// Returns an iterator to the beginning.
    /// </summary>
    Iterator begin();

    /// <summary>
    /// Returns an iterator to the end.
    /// </summary>
    Iterator end();

private:
    std::vector<Core::AutoFilterSortConditionModel>& _models;
};

/// <summary>
/// Iterator for AutoFilterSortConditionCollection.
/// </summary>
class AutoFilterSortConditionCollection::Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = AutoFilterSortCondition;
    using difference_type = std::ptrdiff_t;

    Iterator() noexcept;
    AutoFilterSortCondition operator*() const;
    Iterator& operator++();
    bool operator==(const Iterator& other) const noexcept;
    bool operator!=(const Iterator& other) const noexcept;

private:
    friend class AutoFilterSortConditionCollection;
    Iterator(std::vector<Core::AutoFilterSortConditionModel>& models, int index) noexcept;

    std::vector<Core::AutoFilterSortConditionModel>* _models;
    int _index;
};

}}  // namespace Aspose::Cells_FOSS
