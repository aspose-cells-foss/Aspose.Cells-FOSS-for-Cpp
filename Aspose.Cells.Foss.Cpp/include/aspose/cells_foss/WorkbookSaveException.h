#pragma once
#include "aspose/cells_foss/CellsException.h"

namespace Aspose {
namespace Cells_FOSS {

/// Represents an error that occurs during workbook save.
class WorkbookSaveException : public CellsException {
public:
    /// Initializes a new instance of the WorkbookSaveException class.
    /// @param message The error message.
    explicit WorkbookSaveException(std::string_view message);

    /// Initializes a new instance of the WorkbookSaveException class.
    /// @param message The message.
    /// @param innerException The exception that caused the current exception.
    WorkbookSaveException(std::string_view message, const std::exception& innerException);
};

}}  // namespace Aspose::Cells_FOSS
