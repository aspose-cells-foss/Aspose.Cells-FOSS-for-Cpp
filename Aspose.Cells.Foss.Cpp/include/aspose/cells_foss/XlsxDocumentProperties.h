#pragma once

#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/XmlDocument.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/ZipArchive.h"

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Aspose {
namespace Cells_FOSS {

namespace Core {
class WorkbookModel;
}  // namespace Core

/// Provides static methods for building and loading XLSX document properties
/// (core and extended) from/to a ZIP archive.
class XlsxDocumentProperties {
public:
    XlsxDocumentProperties() = delete;

    /// Builds the core properties XML document. Returns a null XmlDocument
    /// when no core state is present.
    static XmlDocument BuildCorePropertiesDocument(
        const Core::WorkbookModel& model);

    /// Builds the extended properties XML document. Returns a null XmlDocument
    /// when no extended state is present.
    static XmlDocument BuildExtendedPropertiesDocument(
        const Core::WorkbookModel& model);

    /// Loads document properties (core and extended) from the ZIP archive.
    static void LoadDocumentProperties(
        const ZipArchive& archive,
        Core::WorkbookModel& model,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options);

private:
    static std::unordered_map<std::string, std::string>
    LoadRootDocumentPropertiesRelationshipTargets(
        const ZipArchive& archive,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options);

    static void LoadCoreProperties(
        const ZipArchive& archive,
        Core::WorkbookModel& model,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        const std::string& partUri);

    static void LoadExtendedProperties(
        const ZipArchive& archive,
        Core::WorkbookModel& model,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        const std::string& partUri);

    static void AddStringElement(XmlElement& parent,
                                 const std::string& qualifiedName,
                                 const std::string& value);

    static void AddIntElement(XmlElement& parent,
                              const std::string& qualifiedName,
                              std::optional<int> value);

    static void AddBoolElement(XmlElement& parent,
                               const std::string& qualifiedName,
                               std::optional<bool> value);

    static void AddDateElement(XmlElement& parent,
                               const std::string& qualifiedName,
                               std::optional<DateTime> value);

    static std::string ReadElementValue(const XmlElement& parent,
                                        const std::string& qualifiedName);

    static std::optional<DateTime> ReadDateElement(
        const XmlElement& parent,
        const std::string& qualifiedName,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        const std::string& partUri);

    static std::optional<int> ReadIntElement(
        const XmlElement& parent,
        const std::string& qualifiedName,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        const std::string& partUri);

    static std::optional<bool> ReadBoolElement(
        const XmlElement& parent,
        const std::string& qualifiedName,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        const std::string& partUri);

    static void AddDocumentPropertiesIssue(
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        const std::string& partUri,
        const std::string& message);
};

}}  // namespace Aspose::Cells_FOSS
