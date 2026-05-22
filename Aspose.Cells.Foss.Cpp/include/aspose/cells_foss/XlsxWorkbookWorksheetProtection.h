#pragma once

#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// Provides static methods for building and loading worksheet-protection XML elements.
class XlsxWorkbookWorksheetProtection {
public:
    XlsxWorkbookWorksheetProtection() = delete;

    /// Builds the sheetProtection element from the worksheet model's protection state.
    /// Returns a null XmlElement if the protection has no stored state.
    static XmlElement BuildSheetProtectionElement(Core::WorksheetModel& worksheet);

    /// Loads worksheet protection attributes from the worksheet XML root into the model.
    static void LoadWorksheetProtection(
        Core::WorksheetModel& worksheetModel,
        const XmlElement& worksheetRoot,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);
};

}}  // namespace Aspose::Cells_FOSS
