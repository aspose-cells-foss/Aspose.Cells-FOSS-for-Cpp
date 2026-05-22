#pragma once

#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/StylesheetLoadContext.h"
#include "aspose/cells_foss/StylesheetSaveContext.h"
#include "aspose/cells_foss/ZipArchive.h"
#include "aspose/cells_foss/core/CellRecord.h"
#include "aspose/cells_foss/core/StyleValue.h"
#include "aspose/cells_foss/core/WorkbookModel.h"

#include <string>
#include <unordered_map>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Provides static methods for building and loading workbook stylesheets.
/// </summary>
class XlsxWorkbookStyles {
public:
    XlsxWorkbookStyles() = delete;

    /// <summary>
    /// Builds the stylesheet save context from the workbook model,
    /// collecting all fonts, fills, borders, cell formats, and differential formats.
    /// </summary>
    static StylesheetSaveContext BuildStylesheet(const Core::WorkbookModel& model);

    /// <summary>
    /// Gets the style value for serialization, adjusting number format
    /// for date/time cells that do not already use a date format.
    /// </summary>
    static Core::StyleValue GetStyleForSerialization(const Core::CellRecord& record);

    /// <summary>
    /// Computes a canonical string key for the given style value,
    /// suitable for use as a dictionary key for deduplication.
    /// </summary>
    static std::string GetStyleKey(const Core::StyleValue& style);

    /// <summary>
    /// Loads the stylesheet from the archive, parsing cell formats,
    /// differential formats, fonts, fills, borders, and number formats.
    /// </summary>
    static StylesheetLoadContext LoadStylesheet(
        const ZipArchive& archive,
        const std::unordered_map<std::string, std::string>& workbookRelationships,
        const LoadOptions& options,
        LoadDiagnostics& diagnostics);

    /// <summary>
    /// Returns true if two style values are equivalent (same canonical key).
    /// </summary>
    static bool StylesEqual(const Core::StyleValue& left, const Core::StyleValue& right);
};

}}  // namespace Aspose::Cells_FOSS
