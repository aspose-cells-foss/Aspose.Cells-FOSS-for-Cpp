#include "aspose/cells_foss/FilterValueCollection.h"
#include "aspose/cells_foss/AutoFilterSupport.h"
#include "aspose/cells_foss/CellsException.h"

#include <cstddef>
#include <string_view>
#include <utility>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

FilterValueCollection::FilterValueCollection(std::vector<std::string>& values)
    : _values(values)
{
}

// ---------------------------------------------------------------------------
// GetCount
// ---------------------------------------------------------------------------

int FilterValueCollection::GetCount() const noexcept
{
    return static_cast<int>(_values.size());
}

// ---------------------------------------------------------------------------
// operator[]
// ---------------------------------------------------------------------------

const std::string& FilterValueCollection::operator[](int index) const
{
    if (index < 0 || index >= static_cast<int>(_values.size()))
    {
        throw CellsException("Filter value index was out of range.");
    }

    return _values[static_cast<std::size_t>(index)];
}

// ---------------------------------------------------------------------------
// Add
// ---------------------------------------------------------------------------

int FilterValueCollection::Add(std::string_view value)
{
    std::string normalized = AutoFilterSupport::NormalizeText(value, "value");
    _values.push_back(std::move(normalized));
    return static_cast<int>(_values.size()) - 1;
}

// ---------------------------------------------------------------------------
// RemoveAt
// ---------------------------------------------------------------------------

void FilterValueCollection::RemoveAt(int index)
{
    if (index < 0 || index >= static_cast<int>(_values.size()))
    {
        throw CellsException("Filter value index was out of range.");
    }

    _values.erase(_values.begin() + index);
}

// ---------------------------------------------------------------------------
// Clear
// ---------------------------------------------------------------------------

void FilterValueCollection::Clear()
{
    _values.clear();
}

// ---------------------------------------------------------------------------
// GetEnumerator
// ---------------------------------------------------------------------------

std::vector<std::string> FilterValueCollection::GetEnumerator() const
{
    return _values;
}

// ---------------------------------------------------------------------------
// begin / end
// ---------------------------------------------------------------------------

std::vector<std::string>::const_iterator FilterValueCollection::begin() const noexcept
{
    return _values.cbegin();
}

std::vector<std::string>::const_iterator FilterValueCollection::end() const noexcept
{
    return _values.cend();
}

}}  // namespace Aspose::Cells_FOSS
