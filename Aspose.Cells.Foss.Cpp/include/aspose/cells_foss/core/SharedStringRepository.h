#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents shared string repository.
/// </summary>
class SharedStringRepository final {
public:
    /// <summary>
    /// Initializes a new instance of the SharedStringRepository class.
    /// </summary>
    SharedStringRepository() = default;

    /// <summary>
    /// Gets the values.
    /// </summary>
    const std::vector<std::string>& GetValues() const noexcept { return _values; }

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear();

    /// <summary>
    /// Attempts to get value.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    /// <param name="value">The value.</param>
    /// <returns>true if the operation succeeds; otherwise, false.</returns>
    bool TryGetValue(int index, std::string& value) const;

    /// <summary>
    /// Performs intern.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns>The zero-based index of the interned string.</returns>
    int Intern(std::string_view value);

private:
    std::unordered_map<std::string, int> _indices;
    std::vector<std::string> _values;
};

}}}  // namespace Aspose::Cells_FOSS::Core
