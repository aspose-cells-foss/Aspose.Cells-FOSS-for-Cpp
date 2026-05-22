#pragma once

#include "aspose/cells_foss/CellValue.h"

#include <cstdint>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {
namespace Xml {

/// Represents stylesheet xml mapper.
class StylesheetXmlMapper {
public:
    StylesheetXmlMapper() = default;

    /// Reads data from the specified source.
    /// @param stream The stream.
    /// @param styleRepository The style repository.
    void Read(const std::vector<std::uint8_t>& stream, const CellValue& styleRepository);

    /// Writes data to the specified target.
    /// @param stream The stream.
    /// @param styleRepository The style repository.
    void Write(std::vector<std::uint8_t>& stream, const CellValue& styleRepository);
};

}}}  // namespace Aspose::Cells_FOSS::Xml
