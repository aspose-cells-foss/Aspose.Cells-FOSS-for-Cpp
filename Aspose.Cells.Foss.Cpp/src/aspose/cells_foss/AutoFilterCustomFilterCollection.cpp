#include "aspose/cells_foss/AutoFilterCustomFilterCollection.h"
#include "aspose/cells_foss/AutoFilterSupport.h"
#include "aspose/cells_foss/CellsException.h"

#include <cstddef>
#include <string_view>
#include <utility>

namespace Aspose {
namespace Cells_FOSS {

AutoFilterCustomFilterCollection::AutoFilterCustomFilterCollection(
    std::vector<Core::AutoFilterCustomFilterModel>& models,
    Core::FilterColumnModel& columnModel)
    : _models(models)
    , _columnModel(columnModel)
{
}

bool AutoFilterCustomFilterCollection::GetMatchAll() const noexcept
{
    return _columnModel.GetCustomFiltersAnd();
}

void AutoFilterCustomFilterCollection::SetMatchAll(bool value) noexcept
{
    _columnModel.SetCustomFiltersAnd(value);
}

int AutoFilterCustomFilterCollection::GetCount() const noexcept
{
    return static_cast<int>(_models.size());
}

AutoFilterCustomFilter AutoFilterCustomFilterCollection::operator[](int index)
{
    if (index < 0 || index >= static_cast<int>(_models.size()))
    {
        throw CellsException("Custom filter index was out of range.");
    }

    return AutoFilterCustomFilter(_models[static_cast<std::size_t>(index)]);
}

int AutoFilterCustomFilterCollection::Add(FilterOperatorType operatorType, std::string_view value)
{
    if (static_cast<int>(_models.size()) >= 2)
    {
        throw CellsException("Custom filters support at most two filter conditions.");
    }

    Core::AutoFilterCustomFilterModel model;
    model.SetOperator(AutoFilterSupport::ToOperatorName(operatorType));
    model.SetValue(AutoFilterSupport::NormalizeText(value, "value"));
    _models.push_back(std::move(model));
    return static_cast<int>(_models.size()) - 1;
}

void AutoFilterCustomFilterCollection::RemoveAt(int index)
{
    if (index < 0 || index >= static_cast<int>(_models.size()))
    {
        throw CellsException("Custom filter index was out of range.");
    }

    _models.erase(_models.begin() + index);
}

void AutoFilterCustomFilterCollection::Clear()
{
    _models.clear();
    _columnModel.SetCustomFiltersAnd(false);
}

std::vector<AutoFilterCustomFilter> AutoFilterCustomFilterCollection::GetEnumerator()
{
    std::vector<AutoFilterCustomFilter> filters;
    filters.reserve(_models.size());
    for (std::size_t index = 0; index < _models.size(); ++index)
    {
        filters.emplace_back(_models[index]);
    }
    return filters;
}

AutoFilterCustomFilterCollection::Iterator
AutoFilterCustomFilterCollection::begin()
{
    return Iterator(_models, 0);
}

AutoFilterCustomFilterCollection::Iterator
AutoFilterCustomFilterCollection::end()
{
    return Iterator(_models, static_cast<int>(_models.size()));
}

// -- Iterator implementation --

AutoFilterCustomFilterCollection::Iterator::Iterator() noexcept
    : _models(nullptr)
    , _index(0)
{
}

AutoFilterCustomFilterCollection::Iterator::Iterator(
    std::vector<Core::AutoFilterCustomFilterModel>& models,
    int index) noexcept
    : _models(&models)
    , _index(index)
{
}

AutoFilterCustomFilter AutoFilterCustomFilterCollection::Iterator::operator*() const
{
    return AutoFilterCustomFilter((*_models)[static_cast<std::size_t>(_index)]);
}

AutoFilterCustomFilterCollection::Iterator&
AutoFilterCustomFilterCollection::Iterator::operator++()
{
    ++_index;
    return *this;
}

bool AutoFilterCustomFilterCollection::Iterator::operator==(
    const Iterator& other) const noexcept
{
    return _models == other._models && _index == other._index;
}

bool AutoFilterCustomFilterCollection::Iterator::operator!=(
    const Iterator& other) const noexcept
{
    return !(*this == other);
}

}}  // namespace Aspose::Cells_FOSS
