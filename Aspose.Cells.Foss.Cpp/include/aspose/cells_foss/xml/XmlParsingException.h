#pragma once
#include "aspose/cells_foss/CellsException.h"
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {
namespace Xml {

/// Represents an error that occurs during xml parsing.
class XmlParsingException : public CellsException {
public:
    /// Initializes a new instance of the XmlParsingException class.
    /// @param message The error message.
    explicit XmlParsingException(std::string_view message);
};

}}}  // namespace Aspose::Cells_FOSS::Xml
