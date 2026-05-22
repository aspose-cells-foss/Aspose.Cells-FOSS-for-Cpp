#pragma once

#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/core/WorkbookModel.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Provides static methods for building and loading workbook-level defined names.
class XlsxWorkbookDefinedNames {
public:
    XlsxWorkbookDefinedNames() = delete;

    /// Builds the definedNames element containing page-setup, auto-filter, and
    /// user-defined names. Returns a null XmlElement when no defined names exist.
    static XmlElement BuildDefinedNames(const Core::WorkbookModel& model);

    /// Loads workbook-level defined names from the workbook root element.
    static void LoadWorkbookDefinedNames(
        const XmlElement& workbookRoot,
        Core::WorkbookModel& workbookModel,
        int sheetCount,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options);

private:
    static bool ContainsDuplicate(
        const std::vector<Core::DefinedNameModel>& definedNames,
        std::string_view name,
        std::optional<int> localSheetIndex);

    static void AddInvalidDefinedNameIssue(
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view message);

    static void HandleInvalidDefinedName(
        const LoadOptions& options,
        std::string_view message);

    static XmlElement BuildAutoFilterDefinedNames(const Core::WorkbookModel& model);

    static std::string QualifyAutoFilterRange(
        std::string_view sheetName,
        std::string_view range);

    static std::string QuoteWorksheetName(std::string_view sheetName);

    static std::string ToAbsoluteCellReference(std::string_view value);
};

}}  // namespace Aspose::Cells_FOSS
