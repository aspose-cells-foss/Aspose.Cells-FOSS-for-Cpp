#include "aspose/cells_foss/xml/SharedStringTableXmlMapper.h"
#include "aspose/cells_foss/UnsupportedFeatureException.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Xml {

void SharedStringTableXmlMapper::Read(
    const std::vector<std::uint8_t>& /*stream*/,
    const CellValue& /*sharedStrings*/)
{
    throw UnsupportedFeatureException(
        "Shared string XML reading is not supported in this initial solution skeleton.");
}

void SharedStringTableXmlMapper::Write(
    std::vector<std::uint8_t>& /*stream*/,
    const CellValue& /*sharedStrings*/)
{
    throw UnsupportedFeatureException(
        "Shared string XML writing is not supported in this initial solution skeleton.");
}

}}}  // namespace Aspose::Cells_FOSS::Xml
