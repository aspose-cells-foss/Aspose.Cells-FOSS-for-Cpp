#include "aspose/cells_foss/core/CellAddress.h"

#include <algorithm>
#include <stack>
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

CellAddress::CellAddress(int rowIndex, int columnIndex)
{
    if (rowIndex < 0)
        throw std::out_of_range("rowIndex is out of range.");
    if (columnIndex < 0)
        throw std::out_of_range("columnIndex is out of range.");
    _rowIndex = rowIndex;
    _columnIndex = columnIndex;
}

CellAddress CellAddress::Parse(std::string_view cellReference)
{
    if (cellReference.empty())
        throw std::invalid_argument("Cell reference must be non-empty.");

    auto isSpace = [](char ch) noexcept -> bool {
        return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f' || ch == '\v';
    };

    // Check that the view is not all whitespace (matches C# IsNullOrWhiteSpace).
    bool allWhitespace = true;
    for (char ch : cellReference)
    {
        if (!isSpace(ch))
        {
            allWhitespace = false;
            break;
        }
    }
    if (allWhitespace)
        throw std::invalid_argument("Cell reference must be non-empty.");

    // Trim leading and trailing whitespace only (matches C# Trim()).
    auto start = std::find_if_not(cellReference.begin(), cellReference.end(), isSpace);
    auto end = std::find_if_not(cellReference.rbegin(), cellReference.rend(), isSpace).base();
    if (start >= end)
        throw std::invalid_argument("Cell reference must be non-empty.");
    std::string reference(start, end);

    std::size_t index = 0;
    int column = 0;

    // Convert the A1 column prefix into a zero-based index using base-26 letters.
    while (index < reference.size() && ((reference[index] >= 'A' && reference[index] <= 'Z') || (reference[index] >= 'a' && reference[index] <= 'z')))
    {
        char letter = reference[index];
        if (letter >= 'a' && letter <= 'z')
            letter = static_cast<char>(letter - 'a' + 'A');
        if (letter < 'A' || letter > 'Z')
            throw std::invalid_argument("Cell reference is invalid.");
        column = (column * 26) + (letter - 'A' + 1);
        index++;
    }

    if (column == 0 || index == reference.size())
        throw std::invalid_argument("Cell reference is invalid.");

    int row = 0;
    while (index < reference.size() && reference[index] >= '0' && reference[index] <= '9')
    {
        row = (row * 10) + (reference[index] - '0');
        index++;
    }

    if (index != reference.size() || row <= 0)
        throw std::invalid_argument("Cell reference is invalid.");

    return CellAddress(row - 1, column - 1);
}

bool CellAddress::Equals(const CellAddress& other) const noexcept
{
    return _rowIndex == other._rowIndex && _columnIndex == other._columnIndex;
}

int CellAddress::GetHashCode() const noexcept
{
    return (_rowIndex * 397) ^ _columnIndex;
}

std::string CellAddress::ToString() const
{
    return ColumnIndexToName(_columnIndex) + std::to_string(_rowIndex + 1);
}

std::string CellAddress::ColumnIndexToName(int columnIndex)
{
    int index = columnIndex + 1;
    std::stack<char> characters;
    // Convert the zero-based column index back to Excel's repeated-letter form.
    while (index > 0)
    {
        index--;
        characters.push(static_cast<char>('A' + (index % 26)));
        index /= 26;
    }

    std::string result;
    result.reserve(characters.size());
    while (!characters.empty())
    {
        result.push_back(characters.top());
        characters.pop();
    }
    return result;
}

}}}  // namespace Aspose::Cells_FOSS::Core
