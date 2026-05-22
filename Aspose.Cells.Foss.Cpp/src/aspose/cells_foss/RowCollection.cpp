#include "aspose/cells_foss/RowCollection.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/Worksheet.h"

namespace Aspose {
namespace Cells_FOSS {

RowCollection::RowCollection(Worksheet& worksheet)
    : _worksheet(worksheet)
{
}

Row RowCollection::operator[](int index) const
{
    if (index < 0) {
        throw CellsException("Row index must be non-negative.");
    }
    return Row(_worksheet, index);
}

}}  // namespace Aspose::Cells_FOSS
