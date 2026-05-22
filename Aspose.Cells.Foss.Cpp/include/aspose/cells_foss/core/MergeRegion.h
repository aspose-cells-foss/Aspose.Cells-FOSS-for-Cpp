#pragma once

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents merge region.
/// </summary>
class MergeRegion {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="MergeRegion"/> class.
    /// </summary>
    MergeRegion() noexcept = default;

    /// <summary>
    /// Initializes a new instance of the <see cref="MergeRegion"/> class.
    /// </summary>
    /// <param name="firstRow">The zero-based first row index.</param>
    /// <param name="firstColumn">The zero-based first column index.</param>
    /// <param name="totalRows">The total number of rows.</param>
    /// <param name="totalColumns">The total number of columns.</param>
    MergeRegion(int firstRow, int firstColumn, int totalRows, int totalColumns) noexcept;

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

private:
    int _firstRow = 0;
    int _firstColumn = 0;
    int _totalRows = 0;
    int _totalColumns = 0;
};

}}}  // namespace Aspose::Cells_FOSS::Core
