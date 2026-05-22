#include "aspose/cells_foss/WorkbookLoadException.h"

namespace Aspose {
namespace Cells_FOSS {

WorkbookLoadException::WorkbookLoadException(std::string_view message)
    : CellsException(message)
{
}

WorkbookLoadException::WorkbookLoadException(std::string_view message, const std::exception& innerException)
    : CellsException(message, innerException)
{
}

}}  // namespace Aspose::Cells_FOSS
