#pragma once

#include "aspose/cells_foss/CellValue.h"
#include "aspose/cells_foss/packaging/PackageModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

/// Represents package load context.
class PackageLoadContext final {
public:
    /// Initializes a new instance of the PackageLoadContext class.
    /// @param workbook The workbook.
    /// @param package The package.
    PackageLoadContext(CellValue workbook, PackageModel package);

    /// Gets the workbook.
    const CellValue& GetWorkbook() const noexcept;

    /// Gets the package.
    const PackageModel& GetPackage() const noexcept;

private:
    CellValue _workbook;
    PackageModel _package;
};

}}}  // namespace Aspose::Cells_FOSS::Packaging
