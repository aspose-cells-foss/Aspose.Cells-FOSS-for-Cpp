#pragma once

#include <cstdint>
#include <vector>

#include "aspose/cells_foss/packaging/PackageModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

/// Defines a writer for package models.
class IPackageWriter {
public:
    /// Destructor.
    virtual ~IPackageWriter() = default;

    /// Writes the specified package model to the target stream.
    /// @param stream The output byte buffer that receives the package content.
    /// @param packageModel The package model.
    virtual void Write(std::vector<std::uint8_t>& stream,
                       const PackageModel& packageModel) = 0;
};

}}}  // namespace Aspose::Cells_FOSS::Packaging
