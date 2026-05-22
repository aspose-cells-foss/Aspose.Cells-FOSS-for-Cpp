#include "aspose/cells_foss/ValidationCollection.h"
#include "aspose/cells_foss/CellsException.h"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <optional>

namespace Aspose {
namespace Cells_FOSS {

ValidationCollection::ValidationCollection(
    std::vector<Core::ValidationModel>& validations)
    : _validations(validations)
{
}

int ValidationCollection::GetCount() const noexcept
{
    return static_cast<int>(_validations.size());
}

Validation ValidationCollection::operator[](int index)
{
    if (index < 0 || index >= static_cast<int>(_validations.size()))
    {
        throw CellsException("Validation index was out of range.");
    }

    return Validation(_validations, _validations[index]);
}

int ValidationCollection::Add(const CellArea& area)
{
    ValidateArea(area);
    EnsureNoOverlap(_validations, std::nullopt, area);

    Core::ValidationModel validation;
    validation.GetAreas().push_back(area);
    _validations.push_back(std::move(validation));
    return static_cast<int>(_validations.size()) - 1;
}

std::optional<Validation> ValidationCollection::GetValidationInCell(int row, int column)
{
    if (row < 0 || column < 0)
    {
        throw CellsException("Row and column indices must be non-negative.");
    }

    for (std::size_t index = 0; index < _validations.size(); ++index)
    {
        auto& validation = _validations[index];
        const auto& areas = validation.GetAreas();
        for (std::size_t areaIndex = 0; areaIndex < areas.size(); ++areaIndex)
        {
            if (Contains(areas[areaIndex], row, column))
            {
                return Validation(_validations, validation);
            }
        }
    }

    return std::nullopt;
}

void ValidationCollection::RemoveACell(int row, int column)
{
    if (row < 0 || column < 0)
    {
        throw CellsException("Row and column indices must be non-negative.");
    }

    RemoveArea(CellArea(row, column, 1, 1));
}

void ValidationCollection::RemoveArea(const CellArea& cellArea)
{
    ValidateArea(cellArea);

    for (int index = static_cast<int>(_validations.size()) - 1; index >= 0; --index)
    {
        auto& validation = _validations[index];
        ReplaceAreas(validation, SubtractAreas(validation.GetAreas(), cellArea));
        if (validation.GetAreas().empty())
        {
            _validations.erase(_validations.begin() + index);
        }
    }
}

void ValidationCollection::AddAreaToValidation(
    std::vector<Core::ValidationModel>& owner,
    Core::ValidationModel& validation,
    const CellArea& area)
{
    ValidateArea(area);
    EnsureNoOverlap(owner, std::cref(validation), area);

    const auto& existingAreas = validation.GetAreas();
    for (std::size_t index = 0; index < existingAreas.size(); ++index)
    {
        if (AreasOverlap(existingAreas[index], area))
        {
            throw CellsException("Validation areas must not overlap.");
        }
    }

    validation.GetAreas().push_back(area);
    SortAreas(validation.GetAreas());
}

void ValidationCollection::RemoveAreaFromValidation(
    std::vector<Core::ValidationModel>& owner,
    Core::ValidationModel& validation,
    const CellArea& area)
{
    ValidateArea(area);
    ReplaceAreas(validation, SubtractAreas(validation.GetAreas(), area));
    if (validation.GetAreas().empty())
    {
        auto it = std::find_if(owner.begin(), owner.end(),
            [&validation](const Core::ValidationModel& m) {
                return std::addressof(m) == std::addressof(validation);
            });
        if (it != owner.end())
        {
            owner.erase(it);
        }
    }
}

bool ValidationCollection::AreasOverlap(const CellArea& left, const CellArea& right)
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

void ValidationCollection::SortAreas(std::vector<CellArea>& areas)
{
    std::sort(areas.begin(), areas.end(),
        [](const CellArea& lhs, const CellArea& rhs) {
            return CompareAreas(lhs, rhs) < 0;
        });
}

int ValidationCollection::CompareAreas(const CellArea& left, const CellArea& right)
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

void ValidationCollection::ValidateArea(const CellArea& area)
{
    if (area.GetFirstRow() < 0 || area.GetFirstColumn() < 0 ||
        area.GetTotalRows() <= 0 || area.GetTotalColumns() <= 0)
    {
        throw CellsException("Validation area must be a positive cell range.");
    }
}

void ValidationCollection::EnsureNoOverlap(
    std::vector<Core::ValidationModel>& validations,
    std::optional<std::reference_wrapper<const Core::ValidationModel>> currentValidation,
    const CellArea& candidate)
{
    for (std::size_t validationIndex = 0; validationIndex < validations.size(); ++validationIndex)
    {
        const auto& validation = validations[validationIndex];
        if (currentValidation.has_value() &&
            std::addressof(validation) == std::addressof(currentValidation->get()))
        {
            continue;
        }

        const auto& areas = validation.GetAreas();
        for (std::size_t areaIndex = 0; areaIndex < areas.size(); ++areaIndex)
        {
            if (AreasOverlap(areas[areaIndex], candidate))
            {
                throw CellsException("Validation areas must not overlap.");
            }
        }
    }
}

void ValidationCollection::ReplaceAreas(Core::ValidationModel& validation,
                                        const std::vector<CellArea>& areas)
{
    auto& targetAreas = validation.GetAreas();
    targetAreas.clear();
    targetAreas.insert(targetAreas.end(), areas.begin(), areas.end());
}

std::vector<CellArea> ValidationCollection::SubtractAreas(
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

void ValidationCollection::SubtractArea(const CellArea& source, const CellArea& removal,
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

void ValidationCollection::AddIfNonEmpty(std::vector<CellArea>& areas,
                                         int firstRow, int firstColumn,
                                         int lastRow, int lastColumn)
{
    if (lastRow < firstRow || lastColumn < firstColumn)
    {
        return;
    }

    areas.push_back(CellArea::CreateCellArea(firstRow, firstColumn, lastRow, lastColumn));
}

bool ValidationCollection::Contains(const CellArea& area, int row, int column)
{
    return row >= area.GetFirstRow()
        && row < area.GetFirstRow() + area.GetTotalRows()
        && column >= area.GetFirstColumn()
        && column < area.GetFirstColumn() + area.GetTotalColumns();
}

}}  // namespace Aspose::Cells_FOSS
