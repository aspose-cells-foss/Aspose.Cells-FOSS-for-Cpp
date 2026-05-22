#include "aspose/cells_foss/xml/StylesheetXmlMapper.h"
#include "aspose/cells_foss/UnsupportedFeatureException.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Xml {

void StylesheetXmlMapper::Read(
    const std::vector<std::uint8_t>& /*stream*/,
    const CellValue& /*styleRepository*/)
{
    throw UnsupportedFeatureException(
        "Stylesheet XML reading is not supported in this initial solution skeleton.");
}

void StylesheetXmlMapper::Write(
    std::vector<std::uint8_t>& /*stream*/,
    const CellValue& /*styleRepository*/)
{
    throw UnsupportedFeatureException(
        "Stylesheet XML writing is not supported in this initial solution skeleton.");
}

}}}  // namespace Aspose::Cells_FOSS::Xml
