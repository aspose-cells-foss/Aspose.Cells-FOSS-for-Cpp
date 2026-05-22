#include "aspose/cells_foss/Cells.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/Worksheet.h"
#include "aspose/cells_foss/core/CellAddress.h"
#include "aspose/cells_foss/core/MergeRegion.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

namespace Aspose {
namespace Cells_FOSS {

Cells::Cells(Worksheet& worksheet)
    : _worksheet(worksheet)
    , _rows(worksheet)
    , _columns(worksheet)
{
}

Cell Cells::operator[](std::string_view cellName) const
{
    try
    {
        return Cell(_worksheet, Core::CellAddress::Parse(cellName));
    }
    catch (const std::invalid_argument& exception)
    {
        throw CellsException(exception.what(), exception);
    }
}

Cell Cells::operator()(int row, int column) const
{
    if (row < 0 || column < 0)
    {
        throw CellsException("Row and column indices must be non-negative.");
    }
    return Cell(_worksheet, Core::CellAddress(row, column));
}

const RowCollection& Cells::GetRows() const
{
    return _rows;
}

const ColumnCollection& Cells::GetColumns() const
{
    return _columns;
}

std::vector<CellArea> Cells::GetMergedCells() const
{
    const auto& mergeRegions = _worksheet.GetModel().GetMergeRegions();
    std::vector<CellArea> result;
    result.reserve(mergeRegions.size());
    for (const auto& region : mergeRegions)
    {
        result.emplace_back(
            region.GetFirstRow(),
            region.GetFirstColumn(),
            region.GetTotalRows(),
            region.GetTotalColumns());
    }
    return result;
}

void Cells::Merge(int firstRow, int firstColumn, int totalRows, int totalColumns)
{
    if (firstRow < 0 || firstColumn < 0)
    {
        throw CellsException("Merge origin indices must be non-negative.");
    }
    if (totalRows <= 0 || totalColumns <= 0)
    {
        throw CellsException("Merge range dimensions must be positive.");
    }

    Core::MergeRegion candidate(firstRow, firstColumn, totalRows, totalColumns);
    const auto& existingRegions = _worksheet.GetModel().GetMergeRegions();
    for (const auto& existing : existingRegions)
    {
        if (Overlaps(existing, candidate))
        {
            throw CellsException("Merge ranges must not overlap.");
        }
    }

    _worksheet.GetModel().GetMergeRegions().push_back(candidate);
}

bool Cells::Overlaps(const Core::MergeRegion& left, const Core::MergeRegion& right)
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

}}  // namespace Aspose::Cells_FOSS
