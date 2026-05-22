#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

#include "aspose/cells_foss/AutoFilterCustomFilter.h"
#include "aspose/cells_foss/FilterOperatorType.h"
#include "aspose/cells_foss/core/AutoFilterCustomFilterModel.h"
#include "aspose/cells_foss/core/FilterColumnModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents a collection of auto filter custom filter objects.
/// </summary>
class AutoFilterCustomFilterCollection final {
public:
    class Iterator;

    /// <summary>
    /// Initializes a new instance of the AutoFilterCustomFilterCollection class.
    /// </summary>
    /// <param name="models">The underlying custom filter models.</param>
    /// <param name="columnModel">The filter column model.</param>
    AutoFilterCustomFilterCollection(
        std::vector<Core::AutoFilterCustomFilterModel>& models,
        Core::FilterColumnModel& columnModel);

    /// <summary>
    /// Gets or sets a value indicating whether match all.
    /// </summary>
    bool GetMatchAll() const noexcept;
    void SetMatchAll(bool value) noexcept;

    /// <summary>
    /// Gets the number of items.
    /// </summary>
    int GetCount() const noexcept;

    /// <summary>
    /// Gets the element at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    /// <returns>The custom filter at the given index.</returns>
    AutoFilterCustomFilter operator[](int index);

    /// <summary>
    /// Adds the specified item.
    /// </summary>
    /// <param name="operatorType">The operator type.</param>
    /// <param name="value">The value.</param>
    /// <returns>The zero-based index of the added item.</returns>
    int Add(FilterOperatorType operatorType, std::string_view value);

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
    /// <returns>A vector of custom filters that can be used to iterate through the collection.</returns>
    std::vector<AutoFilterCustomFilter> GetEnumerator();

    /// <summary>
    /// Returns an iterator to the beginning.
    /// </summary>
    Iterator begin();

    /// <summary>
    /// Returns an iterator to the end.
    /// </summary>
    Iterator end();

private:
    std::vector<Core::AutoFilterCustomFilterModel>& _models;
    Core::FilterColumnModel& _columnModel;
};

/// <summary>
/// Iterator for AutoFilterCustomFilterCollection.
/// </summary>
class AutoFilterCustomFilterCollection::Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = AutoFilterCustomFilter;
    using difference_type = std::ptrdiff_t;

    Iterator() noexcept;
    AutoFilterCustomFilter operator*() const;
    Iterator& operator++();
    bool operator==(const Iterator& other) const noexcept;
    bool operator!=(const Iterator& other) const noexcept;

private:
    friend class AutoFilterCustomFilterCollection;
    Iterator(std::vector<Core::AutoFilterCustomFilterModel>& models, int index) noexcept;

    std::vector<Core::AutoFilterCustomFilterModel>* _models;
    int _index;
};

}}  // namespace Aspose::Cells_FOSS
