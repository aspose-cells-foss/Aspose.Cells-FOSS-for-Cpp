#include "aspose/cells_foss/packaging/PackageLoadContext.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

PackageLoadContext::PackageLoadContext(CellValue workbook, PackageModel package)
    : _workbook(std::move(workbook))
    , _package(std::move(package))
{
}

// ---------------------------------------------------------------------------
// Properties
// ---------------------------------------------------------------------------

const CellValue& PackageLoadContext::GetWorkbook() const noexcept
{
    return _workbook;
}

const PackageModel& PackageLoadContext::GetPackage() const noexcept
{
    return _package;
}

}}}  // namespace Aspose::Cells_FOSS::Packaging
