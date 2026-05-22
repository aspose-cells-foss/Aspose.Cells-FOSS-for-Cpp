#pragma once
#include <exception>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

/// Represents an error that occurs during cells.
class CellsException : public std::exception {
public:
    /// Initializes a new instance of the CellsException class.
    /// @param message The error message.
    explicit CellsException(std::string_view message);

    /// Initializes a new instance of the CellsException class.
    /// @param message The error message.
    /// @param innerException The exception that caused the current exception.
    CellsException(std::string_view message, const std::exception& innerException);

    const char* what() const noexcept override;

    /// Gets the exception that caused the current exception.
    const std::exception_ptr& GetInnerException() const noexcept;

private:
    std::string _message;
    std::exception_ptr _innerException;
};

}}  // namespace Aspose::Cells_FOSS
