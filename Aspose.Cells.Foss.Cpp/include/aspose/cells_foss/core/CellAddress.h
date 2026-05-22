#pragma once

#include <string>
#include <string_view>
#include <stdexcept>
#include <functional>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents cell address.
/// </summary>
class CellAddress {
public:
    /// <summary>
    /// Initializes a new instance of the CellAddress class.
    /// </summary>
    CellAddress() noexcept = default;

    /// <summary>
    /// Initializes a new instance of the CellAddress class.
    /// </summary>
    /// <param name="rowIndex">The zero-based row index.</param>
    /// <param name="columnIndex">The zero-based column index.</param>
    CellAddress(int rowIndex, int columnIndex);

    /// <summary>
    /// Gets the row index.
    /// </summary>
    int GetRowIndex() const noexcept { return _rowIndex; }

    /// <summary>
    /// Gets the column index.
    /// </summary>
    int GetColumnIndex() const noexcept { return _columnIndex; }

    /// <summary>
    /// Parses the specified value.
    /// </summary>
    /// <param name="cellReference">The cell reference.</param>
    /// <returns>The cell address.</returns>
    static CellAddress Parse(std::string_view cellReference);

    /// <summary>
    /// Determines whether the specified value is equal to the current instance.
    /// </summary>
    /// <param name="other">The other cell address.</param>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool Equals(const CellAddress& other) const noexcept;

    /// <summary>
    /// Returns a hash code for the current instance.
    /// </summary>
    /// <returns>The hash code.</returns>
    int GetHashCode() const noexcept;

    /// <summary>
    /// Returns the string representation of the current instance.
    /// </summary>
    /// <returns>The string.</returns>
    std::string ToString() const;

    friend bool operator==(const CellAddress& lhs, const CellAddress& rhs) noexcept
    {
        return lhs.Equals(rhs);
    }

    friend bool operator!=(const CellAddress& lhs, const CellAddress& rhs) noexcept
    {
        return !lhs.Equals(rhs);
    }

private:
    int _rowIndex = 0;
    int _columnIndex = 0;

    static std::string ColumnIndexToName(int columnIndex);
};

}}}  // namespace Aspose::Cells_FOSS::Core

namespace std {

template<>
struct hash<Aspose::Cells_FOSS::Core::CellAddress> {
    std::size_t operator()(const Aspose::Cells_FOSS::Core::CellAddress& addr) const noexcept
    {
        return static_cast<std::size_t>(addr.GetHashCode());
    }
};

}  // namespace std
