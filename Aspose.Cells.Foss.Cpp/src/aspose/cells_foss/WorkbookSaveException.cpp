#include "aspose/cells_foss/WorkbookSaveException.h"

namespace Aspose {
namespace Cells_FOSS {

WorkbookSaveException::WorkbookSaveException(std::string_view message)
    : CellsException(message)
{
}

WorkbookSaveException::WorkbookSaveException(std::string_view message, const std::exception& innerException)
    : CellsException(message, innerException)
{
}

}}  // namespace Aspose::Cells_FOSS
