#pragma once
#include "aspose/cells_foss/CellsException.h"
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

/// Represents an error that occurs during style.
class StyleException : public CellsException {
public:
    /// Initializes a new instance of the StyleException class.
    /// @param message The error message.
    explicit StyleException(std::string_view message);
};

}}  // namespace Aspose::Cells_FOSS
