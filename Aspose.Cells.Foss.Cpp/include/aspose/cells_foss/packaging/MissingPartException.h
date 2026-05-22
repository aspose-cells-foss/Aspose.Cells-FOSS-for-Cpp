#pragma once
#include "aspose/cells_foss/packaging/PackageStructureException.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

/// Represents an error that occurs during missing part.
class MissingPartException : public PackageStructureException {
public:
    /// Initializes a new instance of the MissingPartException class.
    /// @param message The error message.
    explicit MissingPartException(std::string_view message);
};

}}}  // namespace Aspose::Cells_FOSS::Packaging
