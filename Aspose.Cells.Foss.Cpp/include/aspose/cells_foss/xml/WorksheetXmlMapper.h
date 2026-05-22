#pragma once

#include "aspose/cells_foss/CellValue.h"

#include <cstdint>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {
namespace Xml {

/// Represents worksheet xml mapper.
class WorksheetXmlMapper {
public:
    WorksheetXmlMapper() = default;

    /// Reads data from the specified source.
    /// @param stream The stream.
    /// @param worksheetModel The worksheet model.
    void Read(const std::vector<std::uint8_t>& stream, const CellValue& worksheetModel);

    /// Writes data to the specified target.
    /// @param stream The stream.
    /// @param worksheetModel The worksheet model.
    void Write(std::vector<std::uint8_t>& stream, const CellValue& worksheetModel);
};

}}}  // namespace Aspose::Cells_FOSS::Xml
