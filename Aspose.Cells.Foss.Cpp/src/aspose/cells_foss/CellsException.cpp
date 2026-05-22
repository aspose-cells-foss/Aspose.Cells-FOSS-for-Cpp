#include "aspose/cells_foss/CellsException.h"

namespace Aspose {
namespace Cells_FOSS {

CellsException::CellsException(std::string_view message)
    : _message(message)
{
}

CellsException::CellsException(std::string_view message, const std::exception& innerException)
    : _message(message)
{
    try {
        throw innerException;
    } catch (...) {
        _innerException = std::current_exception();
    }
}

const char* CellsException::what() const noexcept
{
    return _message.c_str();
}

const std::exception_ptr& CellsException::GetInnerException() const noexcept
{
    return _innerException;
}

}}  // namespace Aspose::Cells_FOSS
