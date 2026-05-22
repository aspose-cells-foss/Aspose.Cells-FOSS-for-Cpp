#include "aspose/cells_foss/FormatConditionCollection.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/ConditionalFormattingCollection.h"
#include "aspose/cells_foss/FormatCondition.h"

#include <algorithm>
#include <cstddef>

namespace Aspose {
namespace Cells_FOSS {

FormatConditionCollection::FormatConditionCollection(
    std::vector<Core::ConditionalFormattingModel>& owner,
    Core::ConditionalFormattingModel& model)
    : _owner(owner)
    , _model(model)
{
}

FormatCondition FormatConditionCollection::operator[](int index)
{
    auto& conditions = _model.GetConditions();
    if (index < 0 || index >= static_cast<int>(conditions.size()))
    {
        throw CellsException("Format condition index was out of range.");
    }
    return FormatCondition(_owner, _model,
                           conditions[static_cast<std::size_t>(index)]);
}

int FormatConditionCollection::GetCount() const noexcept
{
    return static_cast<int>(_model.GetConditions().size());
}

int FormatConditionCollection::GetRangeCount() const noexcept
{
    return static_cast<int>(_model.GetAreas().size());
}

int FormatConditionCollection::Add(const CellArea& area, FormatConditionType type,
                                   OperatorType operatorType,
                                   std::string_view formula1, std::string_view formula2)
{
    AddArea(area);
    return AddCondition(type, operatorType, formula1, formula2);
}

int FormatConditionCollection::AddCondition(FormatConditionType type)
{
    return AddCondition(type, OperatorType::None, std::string_view{}, std::string_view{});
}

int FormatConditionCollection::AddCondition(FormatConditionType type, OperatorType operatorType,
                                             std::string_view formula1, std::string_view formula2)
{
    Core::FormatConditionModel condition;
    condition.SetType(type);
    condition.SetOperator(operatorType);
    condition.SetFormula1(NormalizeFormula(formula1));
    condition.SetFormula2(NormalizeFormula(formula2));
    condition.SetPriority(ConditionalFormattingCollection::GetNextPriority(_owner));
    condition.SetStyle(Core::StyleValue::Default().Clone());
    InitializeDefaults(condition);
    _model.GetConditions().push_back(std::move(condition));
    return static_cast<int>(_model.GetConditions().size()) - 1;
}

void FormatConditionCollection::AddArea(const CellArea& area)
{
    ValidateArea(area);
    _model.GetAreas().push_back(area);
    SortAreas(_model.GetAreas());
}

CellArea FormatConditionCollection::GetCellArea(int index) const
{
    const auto& areas = _model.GetAreas();
    if (index < 0 || index >= static_cast<int>(areas.size()))
    {
        throw CellsException("Conditional formatting area index was out of range.");
    }
    return areas[static_cast<std::size_t>(index)];
}

void FormatConditionCollection::RemoveArea(int index)
{
    auto& areas = _model.GetAreas();
    if (index < 0 || index >= static_cast<int>(areas.size()))
    {
        throw CellsException("Conditional formatting area index was out of range.");
    }
    areas.erase(areas.begin() + index);
    RemoveCollectionIfEmpty(_owner, _model);
}

void FormatConditionCollection::RemoveArea(int startRow, int startColumn,
                                            int totalRows, int totalColumns)
{
    RemoveArea(CellArea(startRow, startColumn, totalRows, totalColumns));
}

void FormatConditionCollection::RemoveArea(const CellArea& area)
{
    ValidateArea(area);
    ReplaceAreas(_model, SubtractAreas(_model.GetAreas(), area));
    RemoveCollectionIfEmpty(_owner, _model);
}

void FormatConditionCollection::RemoveCondition(int index)
{
    auto& conditions = _model.GetConditions();
    if (index < 0 || index >= static_cast<int>(conditions.size()))
    {
        throw CellsException("Format condition index was out of range.");
    }
    conditions.erase(conditions.begin() + index);
    RemoveCollectionIfEmpty(_owner, _model);
}

void FormatConditionCollection::RemoveCondition(
    std::vector<Core::ConditionalFormattingModel>& owner,
    Core::ConditionalFormattingModel& collection,
    Core::FormatConditionModel& model)
{
    auto& conditions = collection.GetConditions();
    auto it = std::find_if(conditions.begin(), conditions.end(),
        [&model](const Core::FormatConditionModel& m) {
            return std::addressof(m) == std::addressof(model);
        });
    if (it != conditions.end())
    {
        conditions.erase(it);
    }
    RemoveCollectionIfEmpty(owner, collection);
}

void FormatConditionCollection::InitializeDefaults(Core::FormatConditionModel& condition)
{
    switch (condition.GetType())
    {
    case FormatConditionType::DuplicateValues:
        condition.SetDuplicate(true);
        break;
    case FormatConditionType::UniqueValues:
        condition.SetDuplicate(false);
        break;
    case FormatConditionType::Top10:
        condition.SetTop(true);
        condition.SetRank(10);
        break;
    case FormatConditionType::Bottom10:
        condition.SetTop(false);
        condition.SetRank(10);
        break;
    case FormatConditionType::AboveAverage:
        condition.SetAbove(true);
        break;
    case FormatConditionType::BelowAverage:
        condition.SetAbove(false);
        break;
    case FormatConditionType::ColorScale:
        condition.SetColorScaleCount(2);
        break;
    case FormatConditionType::DataBar:
        condition.SetBarColor(Core::ColorValue(255, 99, 142, 198));
        break;
    case FormatConditionType::IconSet:
        condition.SetIconSetType("3TrafficLights1");
        break;
    default:
        break;
    }
}

void FormatConditionCollection::RemoveCollectionIfEmpty(
    std::vector<Core::ConditionalFormattingModel>& owner,
    Core::ConditionalFormattingModel& collection)
{
    if (collection.GetAreas().empty() || collection.GetConditions().empty())
    {
        auto it = std::find_if(owner.begin(), owner.end(),
            [&collection](const Core::ConditionalFormattingModel& m) {
                return std::addressof(m) == std::addressof(collection);
            });
        if (it != owner.end())
        {
            owner.erase(it);
        }
    }
}

void FormatConditionCollection::ReplaceAreas(
    Core::ConditionalFormattingModel& model,
    const std::vector<CellArea>& areas)
{
    auto& targetAreas = model.GetAreas();
    targetAreas.clear();
    targetAreas.insert(targetAreas.end(), areas.begin(), areas.end());
}

std::string FormatConditionCollection::NormalizeFormula(std::string_view value)
{
    if (value.empty())
    {
        return {};
    }

    const auto start = value.find_first_not_of(" \t\n\r");
    if (start == std::string_view::npos)
    {
        return {};
    }
    const auto end = value.find_last_not_of(" \t\n\r");
    const auto trimmed = value.substr(start, end - start + 1);

    if (trimmed.empty())
    {
        return {};
    }

    if (trimmed[0] == '=')
    {
        return std::string(trimmed.substr(1));
    }

    return std::string(trimmed);
}

std::string FormatConditionCollection::NormalizeText(std::string_view value)
{
    if (value.empty())
    {
        return {};
    }

    const auto start = value.find_first_not_of(" \t\n\r");
    if (start == std::string_view::npos)
    {
        return {};
    }
    const auto end = value.find_last_not_of(" \t\n\r");
    return std::string(value.substr(start, end - start + 1));
}

void FormatConditionCollection::ValidateArea(const CellArea& area)
{
    if (area.GetFirstRow() < 0 || area.GetFirstColumn() < 0 ||
        area.GetTotalRows() <= 0 || area.GetTotalColumns() <= 0)
    {
        throw CellsException("Conditional formatting area must be a positive cell range.");
    }
}

std::vector<CellArea> FormatConditionCollection::SubtractAreas(
    const std::vector<CellArea>& sourceAreas,
    const CellArea& removal)
{
    std::vector<CellArea> remaining;
    remaining.reserve(sourceAreas.size());
    for (const auto& source : sourceAreas)
    {
        SubtractArea(source, removal, remaining);
    }
    SortAreas(remaining);
    return remaining;
}

void FormatConditionCollection::SubtractArea(const CellArea& source, const CellArea& removal,
                                              std::vector<CellArea>& output)
{
    if (!AreasOverlap(source, removal))
    {
        output.push_back(source);
        return;
    }

    const int sourceLastRow = source.GetFirstRow() + source.GetTotalRows() - 1;
    const int sourceLastColumn = source.GetFirstColumn() + source.GetTotalColumns() - 1;
    const int removalLastRow = removal.GetFirstRow() + removal.GetTotalRows() - 1;
    const int removalLastColumn = removal.GetFirstColumn() + removal.GetTotalColumns() - 1;

    const int overlapFirstRow = (std::max)(source.GetFirstRow(), removal.GetFirstRow());
    const int overlapFirstColumn = (std::max)(source.GetFirstColumn(), removal.GetFirstColumn());
    const int overlapLastRow = (std::min)(sourceLastRow, removalLastRow);
    const int overlapLastColumn = (std::min)(sourceLastColumn, removalLastColumn);

    AddIfNonEmpty(output, source.GetFirstRow(), source.GetFirstColumn(),
                  overlapFirstRow - 1, sourceLastColumn);
    AddIfNonEmpty(output, overlapLastRow + 1, source.GetFirstColumn(),
                  sourceLastRow, sourceLastColumn);
    AddIfNonEmpty(output, overlapFirstRow, source.GetFirstColumn(),
                  overlapLastRow, overlapFirstColumn - 1);
    AddIfNonEmpty(output, overlapFirstRow, overlapLastColumn + 1,
                  overlapLastRow, sourceLastColumn);
}

bool FormatConditionCollection::AreasOverlap(const CellArea& left, const CellArea& right)
{
    const int leftLastRow = left.GetFirstRow() + left.GetTotalRows() - 1;
    const int leftLastColumn = left.GetFirstColumn() + left.GetTotalColumns() - 1;
    const int rightLastRow = right.GetFirstRow() + right.GetTotalRows() - 1;
    const int rightLastColumn = right.GetFirstColumn() + right.GetTotalColumns() - 1;

    return left.GetFirstRow() <= rightLastRow
        && right.GetFirstRow() <= leftLastRow
        && left.GetFirstColumn() <= rightLastColumn
        && right.GetFirstColumn() <= leftLastColumn;
}

void FormatConditionCollection::SortAreas(std::vector<CellArea>& areas)
{
    std::sort(areas.begin(), areas.end(),
        [](const CellArea& lhs, const CellArea& rhs) {
            return CompareAreas(lhs, rhs) < 0;
        });
}

int FormatConditionCollection::CompareAreas(const CellArea& left, const CellArea& right)
{
    const int rowComparison = left.GetFirstRow() - right.GetFirstRow();
    if (rowComparison != 0)
    {
        return rowComparison;
    }

    const int columnComparison = left.GetFirstColumn() - right.GetFirstColumn();
    if (columnComparison != 0)
    {
        return columnComparison;
    }

    const int rowCountComparison = left.GetTotalRows() - right.GetTotalRows();
    if (rowCountComparison != 0)
    {
        return rowCountComparison;
    }

    return left.GetTotalColumns() - right.GetTotalColumns();
}

void FormatConditionCollection::AddIfNonEmpty(std::vector<CellArea>& areas,
                                               int firstRow, int firstColumn,
                                               int lastRow, int lastColumn)
{
    if (lastRow < firstRow || lastColumn < firstColumn)
    {
        return;
    }

    areas.push_back(CellArea::CreateCellArea(firstRow, firstColumn, lastRow, lastColumn));
}

}}  // namespace Aspose::Cells_FOSS
