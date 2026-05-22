#pragma once

#include <cstddef>
#include <vector>

#include "aspose/cells_foss/FilterColumn.h"
#include "aspose/cells_foss/core/FilterColumnModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents a collection of filter column objects.
/// </summary>
class FilterColumnCollection final {
public:
    class Iterator;

    /// <summary>
    /// Initializes a new instance of the FilterColumnCollection class.
    /// </summary>
    /// <param name="models">The underlying filter column models.</param>
    explicit FilterColumnCollection(std::vector<Core::FilterColumnModel>& models);

    /// <summary>
    /// Gets the number of items.
    /// </summary>
    int GetCount() const noexcept;

    /// <summary>
    /// Gets the element at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    /// <returns>The filter column at the given index.</returns>
    FilterColumn operator[](int index);

    /// <summary>
    /// Adds the specified item.
    /// </summary>
    /// <param name="columnIndex">The zero-based column index.</param>
    /// <returns>The zero-based index of the added item.</returns>
    int Add(int columnIndex);

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
    /// <returns>A vector of filter columns that can be used to iterate through the collection.</returns>
    std::vector<FilterColumn> GetEnumerator();

    /// <summary>
    /// Returns an iterator to the beginning.
    /// </summary>
    Iterator begin();

    /// <summary>
    /// Returns an iterator to the end.
    /// </summary>
    Iterator end();

private:
    std::vector<Core::FilterColumnModel>& _models;
};

/// <summary>
/// Iterator for FilterColumnCollection.
/// </summary>
class FilterColumnCollection::Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = FilterColumn;
    using difference_type = std::ptrdiff_t;

    Iterator() noexcept;
    FilterColumn operator*() const;
    Iterator& operator++();
    bool operator==(const Iterator& other) const noexcept;
    bool operator!=(const Iterator& other) const noexcept;

private:
    friend class FilterColumnCollection;
    Iterator(std::vector<Core::FilterColumnModel>& models, int index) noexcept;

    std::vector<Core::FilterColumnModel>* _models;
    int _index;
};

}}  // namespace Aspose::Cells_FOSS
