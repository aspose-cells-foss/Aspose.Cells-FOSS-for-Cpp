#include "aspose/cells_foss/CellArea.h"
#include "aspose/cells_foss/core/CellAddress.h"

#include <algorithm>
#include <stdexcept>

namespace Aspose {
namespace Cells_FOSS {

CellArea::CellArea(int firstRow, int firstColumn, int totalRows, int totalColumns)
{
    if (firstRow < 0)
        throw std::out_of_range("firstRow");
    if (firstColumn < 0)
        throw std::out_of_range("firstColumn");
    if (totalRows <= 0)
        throw std::out_of_range("totalRows");
    if (totalColumns <= 0)
        throw std::out_of_range("totalColumns");

    _firstRow = firstRow;
    _firstColumn = firstColumn;
    _totalRows = totalRows;
    _totalColumns = totalColumns;
}

CellArea CellArea::CreateCellArea(int startRow, int startColumn, int endRow, int endColumn)
{
    if (endRow < startRow)
        throw std::out_of_range("endRow");
    if (endColumn < startColumn)
        throw std::out_of_range("endColumn");

    return CellArea(startRow, startColumn, endRow - startRow + 1, endColumn - startColumn + 1);
}

CellArea CellArea::CreateCellArea(std::string_view startCellName, std::string_view endCellName)
{
    Core::CellAddress start = Core::CellAddress::Parse(startCellName);
    Core::CellAddress end = Core::CellAddress::Parse(endCellName);

    return CreateCellArea(
        std::min(start.GetRowIndex(), end.GetRowIndex()),
        std::min(start.GetColumnIndex(), end.GetColumnIndex()),
        std::max(start.GetRowIndex(), end.GetRowIndex()),
        std::max(start.GetColumnIndex(), end.GetColumnIndex()));
}

}}  // namespace Aspose::Cells_FOSS
