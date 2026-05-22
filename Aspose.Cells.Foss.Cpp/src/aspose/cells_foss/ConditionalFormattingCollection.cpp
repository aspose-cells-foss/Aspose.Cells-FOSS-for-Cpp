#include "aspose/cells_foss/ConditionalFormattingCollection.h"
#include "aspose/cells_foss/CellArea.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/FormatConditionCollection.h"

#include <algorithm>
#include <cstddef>

namespace Aspose {
namespace Cells_FOSS {

ConditionalFormattingCollection::ConditionalFormattingCollection(
    std::vector<Core::ConditionalFormattingModel>& collections)
    : _collections(collections)
{
}

int ConditionalFormattingCollection::GetCount() const noexcept
{
    return static_cast<int>(_collections.size());
}

FormatConditionCollection ConditionalFormattingCollection::operator[](int index) const
{
    if (index < 0 || index >= static_cast<int>(_collections.size()))
    {
        throw CellsException("Conditional formatting index was out of range.");
    }
    return FormatConditionCollection(
        _collections,
        _collections[static_cast<std::size_t>(index)]);
}

int ConditionalFormattingCollection::Add()
{
    _collections.emplace_back();
    return static_cast<int>(_collections.size()) - 1;
}

void ConditionalFormattingCollection::RemoveAt(int index)
{
    if (index < 0 || index >= static_cast<int>(_collections.size()))
    {
        throw CellsException("Conditional formatting index was out of range.");
    }
    _collections.erase(_collections.begin() + index);
}

void ConditionalFormattingCollection::RemoveArea(int startRow, int startColumn,
                                                  int totalRows, int totalColumns)
{
    const CellArea area(startRow, startColumn, totalRows, totalColumns);
    for (int index = static_cast<int>(_collections.size()) - 1; index >= 0; --index)
    {
        FormatConditionCollection collection(
            _collections,
            _collections[static_cast<std::size_t>(index)]);
        collection.RemoveArea(area);
    }
}

int ConditionalFormattingCollection::GetNextPriority(
    const std::vector<Core::ConditionalFormattingModel>& collections)
{
    int maxPriority = 0;
    for (std::size_t collectionIndex = 0; collectionIndex < collections.size(); ++collectionIndex)
    {
        const auto& collection = collections[collectionIndex];
        const auto& conditions = collection.GetConditions();
        for (std::size_t conditionIndex = 0; conditionIndex < conditions.size(); ++conditionIndex)
        {
            maxPriority = (std::max)(maxPriority, conditions[conditionIndex].GetPriority());
        }
    }
    return maxPriority + 1;
}

}}  // namespace Aspose::Cells_FOSS
