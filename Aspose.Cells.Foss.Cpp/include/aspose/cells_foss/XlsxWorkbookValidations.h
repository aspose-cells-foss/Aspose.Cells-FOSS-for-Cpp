#pragma once

#include "aspose/cells_foss/CellArea.h"
#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/OperatorType.h"
#include "aspose/cells_foss/ValidationAlertType.h"
#include "aspose/cells_foss/ValidationType.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <string>
#include <string_view>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Provides static methods for building and loading worksheet data validation
/// elements in the XLSX workbook serializer.
class XlsxWorkbookValidations {
public:
    XlsxWorkbookValidations() = delete;

    /// Builds the dataValidations XML element for a worksheet. Returns a null
    /// XmlElement when no validations exist.
    static XmlElement BuildDataValidationsElement(
        const Core::WorksheetModel& worksheet);

    /// Loads data validations from the worksheet XML root into the model.
    static void LoadValidations(
        Core::WorksheetModel& worksheetModel,
        const XmlElement& worksheetRoot,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

private:
    static bool TryLoadAreas(
        const std::vector<Core::ValidationModel>& existingValidations,
        Core::ValidationModel& candidate,
        std::string_view sqref,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

    static bool ConflictsWithExisting(
        const std::vector<Core::ValidationModel>& existingValidations,
        const Core::ValidationModel& candidate,
        const CellArea& area);

    static std::vector<Core::ValidationModel> GetOrderedValidations(
        const std::vector<Core::ValidationModel>& validations);

    static int CompareValidations(const Core::ValidationModel& left,
                                  const Core::ValidationModel& right);

    static std::string BuildSqref(const std::vector<CellArea>& areas);

    static std::string ToAreaReference(const CellArea& area);

    static ValidationType ParseValidationType(
        std::string_view text,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view cellRef);

    static OperatorType ParseOperatorType(
        std::string_view text,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view cellRef);

    static ValidationAlertType ParseAlertStyle(
        std::string_view text,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view cellRef);

    static ValidationType WarnAndReturnAnyValue(
        std::string_view attributeName,
        std::string_view text,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view cellRef);

    static OperatorType WarnAndReturnNone(
        std::string_view attributeName,
        std::string_view text,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view cellRef);

    static std::string NormalizeToken(std::string_view value);
    static std::string NormalizeFormulaText(std::string_view value);
    static std::string NormalizeText(std::string_view value);

    static const char* ToValidationTypeName(ValidationType type);
    static const char* ToOperatorName(OperatorType type);
    static const char* ToAlertStyleName(ValidationAlertType type);
};

}}  // namespace Aspose::Cells_FOSS
