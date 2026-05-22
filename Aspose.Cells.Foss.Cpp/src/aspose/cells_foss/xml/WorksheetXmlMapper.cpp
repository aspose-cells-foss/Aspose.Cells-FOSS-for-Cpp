#include "aspose/cells_foss/xml/WorksheetXmlMapper.h"
#include "aspose/cells_foss/UnsupportedFeatureException.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Xml {

void WorksheetXmlMapper::Read(
    const std::vector<std::uint8_t>& /*stream*/,
    const CellValue& /*worksheetModel*/)
{
    throw UnsupportedFeatureException(
        "SpreadsheetML worksheet reading is not supported in this initial solution skeleton.");
}

void WorksheetXmlMapper::Write(
    std::vector<std::uint8_t>& /*stream*/,
    const CellValue& /*worksheetModel*/)
{
    throw UnsupportedFeatureException(
        "SpreadsheetML worksheet writing is not supported in this initial solution skeleton.");
}

}}}  // namespace Aspose::Cells_FOSS::Xml
