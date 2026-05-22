#pragma once
#include "aspose/cells_foss/CellsException.h"

namespace Aspose {
namespace Cells_FOSS {

/// Represents an error that occurs during unsupported feature.
class UnsupportedFeatureException : public CellsException {
public:
    /// Initializes a new instance of the UnsupportedFeatureException class.
    /// @param message The error message.
    explicit UnsupportedFeatureException(std::string_view message);
};

}}  // namespace Aspose::Cells_FOSS
