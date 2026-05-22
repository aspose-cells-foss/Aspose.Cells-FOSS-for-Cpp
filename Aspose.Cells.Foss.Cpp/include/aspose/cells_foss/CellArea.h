#pragma once

#include <string_view>
#include <stdexcept>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents cell area.
/// </summary>
class CellArea {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="CellArea"/> class.
    /// </summary>
    CellArea() noexcept = default;

    /// <summary>
    /// Initializes a new instance of the <see cref="CellArea"/> class.
    /// </summary>
    /// <param name="firstRow">The zero-based first row index.</param>
    /// <param name="firstColumn">The zero-based first column index.</param>
    /// <param name="totalRows">The total number of rows.</param>
    /// <param name="totalColumns">The total number of columns.</param>
    CellArea(int firstRow, int firstColumn, int totalRows, int totalColumns);

    /// <summary>
    /// Gets the first row.
    /// </summary>
    int GetFirstRow() const noexcept { return _firstRow; }

    /// <summary>
    /// Gets the first column.
    /// </summary>
    int GetFirstColumn() const noexcept { return _firstColumn; }

    /// <summary>
    /// Gets the total rows.
    /// </summary>
    int GetTotalRows() const noexcept { return _totalRows; }

    /// <summary>
    /// Gets the total columns.
    /// </summary>
    int GetTotalColumns() const noexcept { return _totalColumns; }

    /// <summary>
    /// Creates the cell area.
    /// </summary>
    /// <param name="startRow">The start row.</param>
    /// <param name="startColumn">The start column.</param>
    /// <param name="endRow">The end row.</param>
    /// <param name="endColumn">The end column.</param>
    /// <returns>The cell area.</returns>
    static CellArea CreateCellArea(int startRow, int startColumn, int endRow, int endColumn);

    /// <summary>
    /// Creates the cell area.
    /// </summary>
    /// <param name="startCellName">The start cell reference.</param>
    /// <param name="endCellName">The end cell reference.</param>
    /// <returns>The cell area.</returns>
    static CellArea CreateCellArea(std::string_view startCellName, std::string_view endCellName);

private:
    int _firstRow = 0;
    int _firstColumn = 0;
    int _totalRows = 0;
    int _totalColumns = 0;
};

}}  // namespace Aspose::Cells_FOSS
