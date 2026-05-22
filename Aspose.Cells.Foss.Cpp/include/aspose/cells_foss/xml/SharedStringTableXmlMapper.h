#pragma once

#include "aspose/cells_foss/CellValue.h"

#include <cstdint>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {
namespace Xml {

/// Represents shared string table xml mapper.
class SharedStringTableXmlMapper {
public:
    SharedStringTableXmlMapper() = default;

    /// Reads data from the specified source.
    /// @param stream The stream.
    /// @param sharedStrings The shared strings.
    void Read(const std::vector<std::uint8_t>& stream, const CellValue& sharedStrings);

    /// Writes data to the specified target.
    /// @param stream The stream.
    /// @param sharedStrings The shared strings.
    void Write(std::vector<std::uint8_t>& stream, const CellValue& sharedStrings);
};

}}}  // namespace Aspose::Cells_FOSS::Xml
