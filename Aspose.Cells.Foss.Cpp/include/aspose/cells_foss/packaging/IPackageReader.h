#pragma once

#include <cstdint>
#include <vector>

#include "aspose/cells_foss/packaging/PackageModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

/// Defines a reader for package models.
class IPackageReader {
public:
    /// Destructor.
    virtual ~IPackageReader() = default;

    /// Reads a package model from the specified stream.
    /// @param stream The raw byte content of the package.
    /// @return The package model.
    virtual PackageModel Read(const std::vector<std::uint8_t>& stream) = 0;
};

}}}  // namespace Aspose::Cells_FOSS::Packaging
