#pragma once
#include "aspose/cells_foss/CellsException.h"
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

/// Represents an error that occurs during formula.
class FormulaException : public CellsException {
public:
    /// Initializes a new instance of the FormulaException class.
    /// @param message The error message.
    explicit FormulaException(std::string_view message);
};

}}  // namespace Aspose::Cells_FOSS
