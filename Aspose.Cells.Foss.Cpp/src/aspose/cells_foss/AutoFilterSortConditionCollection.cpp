#include "aspose/cells_foss/AutoFilterSortConditionCollection.h"
#include "aspose/cells_foss/AutoFilterSupport.h"
#include "aspose/cells_foss/CellsException.h"

#include <cstddef>
#include <string_view>
#include <utility>

namespace Aspose {
namespace Cells_FOSS {

AutoFilterSortConditionCollection::AutoFilterSortConditionCollection(
    std::vector<Core::AutoFilterSortConditionModel>& models)
    : _models(models)
{
}

int AutoFilterSortConditionCollection::GetCount() const noexcept
{
    return static_cast<int>(_models.size());
}

AutoFilterSortCondition AutoFilterSortConditionCollection::operator[](int index)
{
    if (index < 0 || index >= static_cast<int>(_models.size()))
    {
        throw CellsException("Sort condition index was out of range.");
    }

    return AutoFilterSortCondition(_models[static_cast<std::size_t>(index)]);
}

int AutoFilterSortConditionCollection::Add(std::string_view reference)
{
    Core::AutoFilterSortConditionModel model;
    model.SetRef(AutoFilterSupport::NormalizeRequiredRange(reference, "reference"));
    _models.push_back(std::move(model));
    return static_cast<int>(_models.size()) - 1;
}

void AutoFilterSortConditionCollection::RemoveAt(int index)
{
    if (index < 0 || index >= static_cast<int>(_models.size()))
    {
        throw CellsException("Sort condition index was out of range.");
    }

    _models.erase(_models.begin() + index);
}

void AutoFilterSortConditionCollection::Clear()
{
    _models.clear();
}

std::vector<AutoFilterSortCondition> AutoFilterSortConditionCollection::GetEnumerator()
{
    std::vector<AutoFilterSortCondition> conditions;
    conditions.reserve(_models.size());
    for (std::size_t index = 0; index < _models.size(); ++index)
    {
        conditions.emplace_back(_models[index]);
    }
    return conditions;
}

AutoFilterSortConditionCollection::Iterator
AutoFilterSortConditionCollection::begin()
{
    return Iterator(_models, 0);
}

AutoFilterSortConditionCollection::Iterator
AutoFilterSortConditionCollection::end()
{
    return Iterator(_models, static_cast<int>(_models.size()));
}

// -- Iterator implementation --

AutoFilterSortConditionCollection::Iterator::Iterator() noexcept
    : _models(nullptr)
    , _index(0)
{
}

AutoFilterSortConditionCollection::Iterator::Iterator(
    std::vector<Core::AutoFilterSortConditionModel>& models,
    int index) noexcept
    : _models(&models)
    , _index(index)
{
}

AutoFilterSortCondition AutoFilterSortConditionCollection::Iterator::operator*() const
{
    return AutoFilterSortCondition((*_models)[static_cast<std::size_t>(_index)]);
}

AutoFilterSortConditionCollection::Iterator&
AutoFilterSortConditionCollection::Iterator::operator++()
{
    ++_index;
    return *this;
}

bool AutoFilterSortConditionCollection::Iterator::operator==(
    const Iterator& other) const noexcept
{
    return _models == other._models && _index == other._index;
}

bool AutoFilterSortConditionCollection::Iterator::operator!=(
    const Iterator& other) const noexcept
{
    return !(*this == other);
}

}}  // namespace Aspose::Cells_FOSS
