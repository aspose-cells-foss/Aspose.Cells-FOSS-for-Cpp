#pragma once

#include "aspose/cells_foss/Column.h"

namespace Aspose {
namespace Cells_FOSS {

class Worksheet;

/// <summary>
/// Represents a collection of column objects.
/// </summary>
class ColumnCollection final {
public:
    /// <summary>
    /// Constructs a ColumnCollection for the given worksheet.
    /// </summary>
    /// <param name="worksheet">The parent worksheet.</param>
    explicit ColumnCollection(Worksheet& worksheet);

    /// <summary>
    /// Gets the element at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based column index.</param>
    /// <returns>The column at the given index.</returns>
    Column operator[](int index) const;

private:
    Worksheet& _worksheet;
};

}}  // namespace Aspose::Cells_FOSS
