#pragma once

#include "aspose/cells_foss/Cell.h"
#include "aspose/cells_foss/CellArea.h"
#include "aspose/cells_foss/ColumnCollection.h"
#include "aspose/cells_foss/RowCollection.h"

#include <string_view>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

class Worksheet;

namespace Core {
class MergeRegion;
}

/// <summary>
/// Provides access to worksheet cells, rows, columns, and merged ranges.
/// </summary>
class Cells final {
public:
    /// <summary>
    /// Constructs a Cells object for the given worksheet.
    /// </summary>
    /// <param name="worksheet">The parent worksheet.</param>
    explicit Cells(Worksheet& worksheet);

    /// <summary>
    /// Gets a cell by A1 reference such as "A1" or "BC12".
    /// </summary>
    /// <param name="cellName">The cell reference string.</param>
    /// <returns>The cell at the specified reference.</returns>
    Cell operator[](std::string_view cellName) const;

    /// <summary>
    /// Gets a cell by zero-based row and column index.
    /// </summary>
    /// <param name="row">The zero-based row index.</param>
    /// <param name="column">The zero-based column index.</param>
    /// <returns>The cell at the specified position.</returns>
    Cell operator()(int row, int column) const;

    /// <summary>
    /// Gets row-level settings for the worksheet.
    /// </summary>
    /// <returns>A reference to the row collection.</returns>
    const RowCollection& GetRows() const;

    /// <summary>
    /// Gets column-level settings for the worksheet.
    /// </summary>
    /// <returns>A reference to the column collection.</returns>
    const ColumnCollection& GetColumns() const;

    /// <summary>
    /// Gets the current merged-cell regions in worksheet order.
    /// </summary>
    /// <returns>A vector of merged cell areas.</returns>
    std::vector<CellArea> GetMergedCells() const;

    /// <summary>
    /// Merges a rectangular cell region using zero-based coordinates.
    /// </summary>
    /// <param name="firstRow">The zero-based first row index.</param>
    /// <param name="firstColumn">The zero-based first column index.</param>
    /// <param name="totalRows">The total number of rows.</param>
    /// <param name="totalColumns">The total number of columns.</param>
    void Merge(int firstRow, int firstColumn, int totalRows, int totalColumns);

private:
    Worksheet& _worksheet;
    RowCollection _rows;
    ColumnCollection _columns;

    static bool Overlaps(const Core::MergeRegion& left, const Core::MergeRegion& right);
};

}}  // namespace Aspose::Cells_FOSS
