#include "aspose/cells_foss/xml/WorkbookXmlMapper.h"
#include "aspose/cells_foss/UnsupportedFeatureException.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Xml {

void WorkbookXmlMapper::Read(
    const std::vector<std::uint8_t>& /*stream*/,
    const CellValue& /*workbookModel*/,
    const CellValue& /*packageModel*/)
{
    throw UnsupportedFeatureException(
        "SpreadsheetML reading is not supported in this initial solution skeleton.");
}

void WorkbookXmlMapper::Write(
    std::vector<std::uint8_t>& /*stream*/,
    const CellValue& /*workbookModel*/,
    const CellValue& /*packageModel*/)
{
    throw UnsupportedFeatureException(
        "SpreadsheetML writing is not supported in this initial solution skeleton.");
}

}}}  // namespace Aspose::Cells_FOSS::Xml
