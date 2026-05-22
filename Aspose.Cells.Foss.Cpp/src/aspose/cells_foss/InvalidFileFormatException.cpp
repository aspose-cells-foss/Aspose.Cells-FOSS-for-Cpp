#include "aspose/cells_foss/InvalidFileFormatException.h"

namespace Aspose {
namespace Cells_FOSS {

InvalidFileFormatException::InvalidFileFormatException(std::string_view message)
    : CellsException(message)
{
}

InvalidFileFormatException::InvalidFileFormatException(std::string_view message, const std::exception& innerException)
    : CellsException(message, innerException)
{
}

}}  // namespace Aspose::Cells_FOSS
