#pragma once

#include "aspose/cells_foss/Row.h"

namespace Aspose {
namespace Cells_FOSS {

class Worksheet;

/// <summary>
/// Represents a collection of row objects.
/// </summary>
class RowCollection final {
public:
    /// <summary>
    /// Constructs a RowCollection for the given worksheet.
    /// </summary>
    /// <param name="worksheet">The parent worksheet.</param>
    explicit RowCollection(Worksheet& worksheet);

    /// <summary>
    /// Gets the element at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based row index.</param>
    /// <returns>The row at the given index.</returns>
    Row operator[](int index) const;

private:
    Worksheet& _worksheet;
};

}}  // namespace Aspose::Cells_FOSS
