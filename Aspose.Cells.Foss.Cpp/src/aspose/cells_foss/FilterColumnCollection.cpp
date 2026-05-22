#include "aspose/cells_foss/FilterColumnCollection.h"
#include "aspose/cells_foss/CellsException.h"

#include <cstddef>
#include <utility>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

FilterColumnCollection::FilterColumnCollection(
    std::vector<Core::FilterColumnModel>& models)
    : _models(models)
{
}

// ---------------------------------------------------------------------------
// Count
// ---------------------------------------------------------------------------

int FilterColumnCollection::GetCount() const noexcept
{
    return static_cast<int>(_models.size());
}

// ---------------------------------------------------------------------------
// Indexer
// ---------------------------------------------------------------------------

FilterColumn FilterColumnCollection::operator[](int index)
{
    if (index < 0 || index >= static_cast<int>(_models.size()))
    {
        throw CellsException("Filter column index was out of range.");
    }

    return FilterColumn(_models[static_cast<std::size_t>(index)]);
}

// ---------------------------------------------------------------------------
// Add
// ---------------------------------------------------------------------------

int FilterColumnCollection::Add(int columnIndex)
{
    if (columnIndex < 0)
    {
        throw CellsException("Filter column index must be zero or greater.");
    }

    for (std::size_t index = 0; index < _models.size(); ++index)
    {
        if (_models[index].GetColumnIndex() == columnIndex)
        {
            throw CellsException(
                "A filter column for the specified column index already exists.");
        }
    }

    Core::FilterColumnModel model;
    model.SetColumnIndex(columnIndex);

    std::size_t insertIndex = 0;
    while (insertIndex < _models.size() &&
           _models[insertIndex].GetColumnIndex() < columnIndex)
    {
        ++insertIndex;
    }

    _models.insert(
        _models.begin() + static_cast<std::ptrdiff_t>(insertIndex),
        std::move(model));
    return static_cast<int>(insertIndex);
}

// ---------------------------------------------------------------------------
// RemoveAt
// ---------------------------------------------------------------------------

void FilterColumnCollection::RemoveAt(int index)
{
    if (index < 0 || index >= static_cast<int>(_models.size()))
    {
        throw CellsException("Filter column index was out of range.");
    }

    _models.erase(_models.begin() + static_cast<std::ptrdiff_t>(index));
}

// ---------------------------------------------------------------------------
// Clear
// ---------------------------------------------------------------------------

void FilterColumnCollection::Clear()
{
    _models.clear();
}

// ---------------------------------------------------------------------------
// GetEnumerator
// ---------------------------------------------------------------------------

std::vector<FilterColumn> FilterColumnCollection::GetEnumerator()
{
    std::vector<FilterColumn> columns;
    columns.reserve(_models.size());
    for (std::size_t index = 0; index < _models.size(); ++index)
    {
        columns.emplace_back(_models[index]);
    }
    return columns;
}

// ---------------------------------------------------------------------------
// begin / end
// ---------------------------------------------------------------------------

FilterColumnCollection::Iterator FilterColumnCollection::begin()
{
    return Iterator(_models, 0);
}

FilterColumnCollection::Iterator FilterColumnCollection::end()
{
    return Iterator(_models, static_cast<int>(_models.size()));
}

// ---------------------------------------------------------------------------
// Iterator implementation
// ---------------------------------------------------------------------------

FilterColumnCollection::Iterator::Iterator() noexcept
    : _models(nullptr)
    , _index(0)
{
}

FilterColumnCollection::Iterator::Iterator(
    std::vector<Core::FilterColumnModel>& models,
    int index) noexcept
    : _models(&models)
    , _index(index)
{
}

FilterColumn FilterColumnCollection::Iterator::operator*() const
{
    return FilterColumn((*_models)[static_cast<std::size_t>(_index)]);
}

FilterColumnCollection::Iterator&
FilterColumnCollection::Iterator::operator++()
{
    ++_index;
    return *this;
}

bool FilterColumnCollection::Iterator::operator==(
    const Iterator& other) const noexcept
{
    return _models == other._models && _index == other._index;
}

bool FilterColumnCollection::Iterator::operator!=(
    const Iterator& other) const noexcept
{
    return !(*this == other);
}

}}  // namespace Aspose::Cells_FOSS
