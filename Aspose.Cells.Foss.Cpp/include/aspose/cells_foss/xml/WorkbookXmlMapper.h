#pragma once

#include "aspose/cells_foss/CellValue.h"

#include <cstdint>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {
namespace Xml {

/// Represents workbook xml mapper.
class WorkbookXmlMapper {
public:
    WorkbookXmlMapper() = default;

    /// Reads data from the specified source.
    /// @param stream The stream.
    /// @param workbookModel The workbook model.
    /// @param packageModel The package model.
    void Read(const std::vector<std::uint8_t>& stream, const CellValue& workbookModel, const CellValue& packageModel);

    /// Writes data to the specified target.
    /// @param stream The stream.
    /// @param workbookModel The workbook model.
    /// @param packageModel The package model.
    void Write(std::vector<std::uint8_t>& stream, const CellValue& workbookModel, const CellValue& packageModel);
};

}}}  // namespace Aspose::Cells_FOSS::Xml
