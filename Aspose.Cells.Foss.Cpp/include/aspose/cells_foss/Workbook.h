#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "aspose/cells_foss/core/DefinedNameModel.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

namespace Aspose {
namespace Cells_FOSS {

namespace Core {
class WorkbookModel;
}

enum class SaveFormat;
class DefinedNameCollection;
class DocumentProperties;
class LoadDiagnostics;
class LoadOptions;
class SaveOptions;
class WorksheetCollection;
class WorkbookProperties;
class WorkbookSettings;

/// <summary>
/// Represents the root spreadsheet object used to create, load, modify, and
/// save an XLSX workbook.
/// </summary>
class Workbook {
public:
    /// <summary>
    /// Initializes a new workbook with one default worksheet.
    /// </summary>
    Workbook();

    /// <summary>
    /// Opens an existing workbook from a file path using default load options.
    /// </summary>
    explicit Workbook(std::string_view fileName);

    /// <summary>
    /// Opens an existing workbook from a stream using default load options.
    /// </summary>
    explicit Workbook(const std::vector<std::uint8_t>& stream);

    /// <summary>
    /// Opens an existing workbook from a file path using explicit load options.
    /// </summary>
    Workbook(std::string_view fileName, const LoadOptions& options);

    /// <summary>
    /// Opens an existing workbook from a stream using explicit load options.
    /// </summary>
    Workbook(const std::vector<std::uint8_t>& stream, const LoadOptions& options);

    ~Workbook();

    Workbook(const Workbook&) = delete;
    Workbook& operator=(const Workbook&) = delete;

    // ---- Properties ----

    /// <summary>
    /// Gets the worksheets in workbook order.
    /// </summary>
    WorksheetCollection& GetWorksheets();
    const WorksheetCollection& GetWorksheets() const;

    /// <summary>
    /// Gets workbook-level settings such as the date system and display culture.
    /// </summary>
    WorkbookSettings& GetSettings();
    const WorkbookSettings& GetSettings() const;

    /// <summary>
    /// Gets workbook metadata and view settings exposed by the supported public API.
    /// </summary>
    WorkbookProperties& GetProperties();
    const WorkbookProperties& GetProperties() const;

    /// <summary>
    /// Gets the document properties facade for core and extended metadata.
    /// </summary>
    DocumentProperties& GetDocumentProperties();
    const DocumentProperties& GetDocumentProperties() const;

    /// <summary>
    /// Gets the workbook-defined names collection.
    /// </summary>
    DefinedNameCollection& GetDefinedNames();
    const DefinedNameCollection& GetDefinedNames() const;

    /// <summary>
    /// Gets diagnostics collected while loading the current workbook.
    /// </summary>
    LoadDiagnostics& GetLoadDiagnostics();
    const LoadDiagnostics& GetLoadDiagnostics() const;

    // ---- Save methods ----

    /// <summary>
    /// Saves the workbook to an XLSX file using default save options.
    /// </summary>
    void Save(std::string_view fileName) const;

    /// <summary>
    /// Saves the workbook to a file using the specified save format.
    /// </summary>
    void Save(std::string_view fileName, SaveFormat format) const;

    /// <summary>
    /// Saves the workbook to a file using explicit save options.
    /// </summary>
    void Save(std::string_view fileName, const SaveOptions& options) const;

    /// <summary>
    /// Saves the workbook to a stream using the specified save format.
    /// </summary>
    void Save(std::vector<std::uint8_t>& stream, SaveFormat format) const;

    /// <summary>
    /// Saves the workbook to a stream using explicit save options.
    /// </summary>
    void Save(std::vector<std::uint8_t>& stream, const SaveOptions& options) const;

    // ---- Internal helpers (exposed for use by other port classes) ----

    /// <summary>
    /// Gets the underlying workbook model.
    /// </summary>
    Core::WorkbookModel& GetModel();
    const Core::WorkbookModel& GetModel() const;

    /// <summary>
    /// Gets the underlying defined names model vector.
    /// </summary>
    std::vector<Core::DefinedNameModel>& GetDefinedNamesModel();

    /// <summary>
    /// Ensures no other worksheet in the workbook shares the given name.
    /// Throws CellsException when a duplicate is found.
    /// </summary>
    void EnsureUniqueSheetName(
        std::string_view sheetName,
        std::optional<std::reference_wrapper<const Core::WorksheetModel>> currentSheet = std::nullopt);

    /// <summary>
    /// Validates that the local sheet index is absent (global scope) or
    /// within the valid worksheet range. Throws CellsException otherwise.
    /// </summary>
    void EnsureValidDefinedNameScope(std::optional<int> localSheetIndex);

    /// <summary>
    /// Ensures no other defined name in the workbook shares the given name
    /// and scope. Throws CellsException when a duplicate is found.
    /// </summary>
    void EnsureUniqueDefinedName(const Core::DefinedNameModel& currentDefinedName,
                                 std::string_view name,
                                 std::optional<int> localSheetIndex);

    /// <summary>
    /// Releases resources associated with the workbook instance.
    /// </summary>
    void Dispose();

private:
    struct Impl;
    std::unique_ptr<Impl> _impl;

    void LoadFromStream(const std::vector<std::uint8_t>& stream,
                        const LoadOptions& options);
};

}}  // namespace Aspose::Cells_FOSS
