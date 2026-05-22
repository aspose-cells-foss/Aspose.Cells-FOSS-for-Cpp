#include "aspose/cells_foss/packaging/PackageStructureException.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

PackageStructureException::PackageStructureException(std::string_view message)
    : _message(message)
{
}

const char* PackageStructureException::what() const noexcept
{
    return _message.c_str();
}

}}}  // namespace Aspose::Cells_FOSS::Packaging
