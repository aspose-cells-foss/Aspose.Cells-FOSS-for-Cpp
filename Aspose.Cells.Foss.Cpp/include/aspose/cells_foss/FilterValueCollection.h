#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Represents a collection of filter value objects.
class FilterValueCollection final {
public:
    /// Initializes a new instance of the FilterValueCollection class.
    /// @param values The underlying string values.
    explicit FilterValueCollection(std::vector<std::string>& values);

    /// Gets the number of items.
    int GetCount() const noexcept;

    /// Gets the element at the specified zero-based index.
    /// @param index The zero-based index.
    /// @return The string value at the given index.
    const std::string& operator[](int index) const;

    /// Adds the specified item.
    /// @param value The value.
    /// @return The zero-based index of the added item.
    int Add(std::string_view value);

    /// Removes the specified item.
    /// @param index The zero-based index.
    void RemoveAt(int index);

    /// Clears the current state.
    void Clear();

    /// Returns a snapshot vector that iterates through the collection.
    /// @return A vector of strings that can be used to iterate through the collection.
    std::vector<std::string> GetEnumerator() const;

    /// Returns a const iterator to the beginning.
    std::vector<std::string>::const_iterator begin() const noexcept;

    /// Returns a const iterator to the end.
    std::vector<std::string>::const_iterator end() const noexcept;

private:
    std::vector<std::string>& _values;
};

}}  // namespace Aspose::Cells_FOSS
