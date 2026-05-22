#pragma once

#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/SaveOptions.h"

#include <cstdint>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

namespace Core {
class WorkbookModel;
}  // namespace Core

/// Serializes and deserializes workbook models in XLSX format.
class XlsxWorkbookSerializer {
public:
    XlsxWorkbookSerializer() = delete;

    /// Saves the current state.
    /// @param model The model.
    /// @param stream The output byte buffer.
    /// @param options The options to use.
    static void Save(Core::WorkbookModel& model,
                     std::vector<std::uint8_t>& stream,
                     const SaveOptions& options);

    /// Loads the current state.
    /// @param stream The input byte buffer.
    /// @param options The options to use.
    /// @param diagnostics The diagnostics.
    /// @return The workbook model.
    static Core::WorkbookModel Load(const std::vector<std::uint8_t>& stream,
                                    const LoadOptions& options,
                                    LoadDiagnostics& diagnostics);
};

}}  // namespace Aspose::Cells_FOSS
