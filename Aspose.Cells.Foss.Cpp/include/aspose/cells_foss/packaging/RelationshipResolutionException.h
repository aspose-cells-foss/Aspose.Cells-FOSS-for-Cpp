#pragma once
#include "aspose/cells_foss/packaging/PackageStructureException.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

/// Represents an error that occurs during relationship resolution.
class RelationshipResolutionException : public PackageStructureException {
public:
    /// Initializes a new instance of the RelationshipResolutionException class.
    /// @param message The error message.
    explicit RelationshipResolutionException(std::string_view message);
};

}}}  // namespace Aspose::Cells_FOSS::Packaging
