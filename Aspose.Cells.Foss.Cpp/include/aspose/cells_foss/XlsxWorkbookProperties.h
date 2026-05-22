#pragma once

#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/XmlElement.h"

#include <optional>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {
class WorkbookModel;
}  // namespace Core

/// Provides static methods for building and loading workbook-level metadata.
class XlsxWorkbookProperties {
public:
    XlsxWorkbookProperties() = delete;

    /// Builds the workbookPr element. Returns a null XmlElement when no state
    /// is present.
    static XmlElement BuildWorkbookPropertiesElement(
        const Core::WorkbookModel& model);

    /// Builds the workbookProtection element. Returns a null XmlElement when
    /// no protection state is present.
    static XmlElement BuildWorkbookProtectionElement(
        const Core::WorkbookModel& model);

    /// Builds the bookViews element. Returns a null XmlElement when no view
    /// state is present.
    static XmlElement BuildBookViewsElement(
        const Core::WorkbookModel& model);

    /// Builds the calcPr element. Returns a null XmlElement when no
    /// calculation state is present.
    static XmlElement BuildCalculationPropertiesElement(
        const Core::WorkbookModel& model);

    /// Loads workbook metadata from the workbook XML root element.
    static void LoadWorkbookMetadata(
        const XmlElement& workbookRoot,
        Core::WorkbookModel& workbookModel,
        int sheetCount,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options);

private:
    static std::string ReadStringAttribute(const XmlElement& element,
                                           std::string_view name);

    static std::string ReadChoiceAttribute(
        const XmlElement& element,
        std::string_view attributeName,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view partUri,
        std::string (*normalizer)(std::string_view));

    static bool ReadBoolAttribute(
        const XmlElement& element,
        std::string_view attributeName,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        bool defaultValue,
        std::string_view partUri);

    static std::optional<bool> ReadNullableBoolAttribute(
        const XmlElement& element,
        std::string_view attributeName,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view partUri);

    static std::optional<int> ReadNonNegativeIntAttribute(
        const XmlElement& element,
        std::string_view attributeName,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view partUri);

    static std::optional<double> ReadNonNegativeDoubleAttribute(
        const XmlElement& element,
        std::string_view attributeName,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view partUri);

    static bool TryReadBoolean(std::string_view rawValue, bool& value);

    static void AddWorkbookMetadataIssue(
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view partUri,
        std::string_view message);

    static std::string FormatDecimalFraction(double value);
};

}}  // namespace Aspose::Cells_FOSS
