#include "aspose/cells_foss/ColumnCollection.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/Worksheet.h"

namespace Aspose {
namespace Cells_FOSS {

ColumnCollection::ColumnCollection(Worksheet& worksheet)
    : _worksheet(worksheet)
{
}

Column ColumnCollection::operator[](int index) const
{
    if (index < 0) {
        throw CellsException("Column index must be non-negative.");
    }
    return Column(_worksheet, index);
}

}}  // namespace Aspose::Cells_FOSS
