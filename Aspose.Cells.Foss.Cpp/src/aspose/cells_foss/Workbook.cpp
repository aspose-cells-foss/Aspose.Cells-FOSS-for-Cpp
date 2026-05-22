#include "aspose/cells_foss/Workbook.h"

#include <cctype>
#include <fstream>
#include <iterator>
#include <string>

#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/DefinedNameCollection.h"
#include "aspose/cells_foss/DefinedNameUtility.h"
#include "aspose/cells_foss/DocumentProperties.h"
#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/SaveOptions.h"
#include "aspose/cells_foss/WorkbookLoadException.h"
#include "aspose/cells_foss/WorkbookProperties.h"
#include "aspose/cells_foss/WorkbookSaveException.h"
#include "aspose/cells_foss/WorkbookSettings.h"
#include "aspose/cells_foss/WorksheetCollection.h"
#include "aspose/cells_foss/XlsxWorkbookSerializer.h"
#include "aspose/cells_foss/core/WorkbookModel.h"
#include "aspose/cells_foss/core/WorkbookSettingsModel.h"
#include "aspose/cells_foss/core/WorkbookPropertiesModel.h"
#include "aspose/cells_foss/core/DocumentPropertiesModel.h"

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Impl definition -- holds all state for the Workbook facade.
// ---------------------------------------------------------------------------
struct Workbook::Impl {
    Core::WorkbookModel model;
    WorkbookSettings settings;
    WorkbookProperties properties;
    DocumentProperties documentProperties;
    LoadDiagnostics loadDiagnostics;
    std::unique_ptr<WorksheetCollection> worksheets;
    std::unique_ptr<DefinedNameCollection> definedNames;

    Impl()
        : model()
        , settings(model.GetSettings())
        , properties(model)
        , documentProperties(model.GetDocumentProperties())
        , loadDiagnostics()
    {
    }
};

// ---------------------------------------------------------------------------
// Constructors
// ---------------------------------------------------------------------------

Workbook::Workbook()
    : _impl(std::make_unique<Impl>())
{
    _impl->worksheets = std::make_unique<WorksheetCollection>(*this, _impl->model);
    _impl->definedNames = std::make_unique<DefinedNameCollection>(*this);
}

Workbook::Workbook(std::string_view fileName)
    : Workbook(fileName, LoadOptions())
{
}

Workbook::Workbook(const std::vector<std::uint8_t>& stream)
    : Workbook(stream, LoadOptions())
{
}

Workbook::Workbook(std::string_view fileName, const LoadOptions& options)
    : Workbook()
{
    if (fileName.empty()) {
        throw CellsException("fileName must not be empty.");
    }

    std::ifstream file(std::string(fileName), std::ios::binary);
    if (!file.is_open()) {
        throw CellsException("Failed to open file.");
    }

    std::vector<std::uint8_t> fileBytes(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();

    LoadFromStream(fileBytes, options);
}

Workbook::Workbook(const std::vector<std::uint8_t>& stream, const LoadOptions& options)
    : Workbook()
{
    if (stream.empty()) {
        throw CellsException("stream must not be empty.");
    }

    LoadFromStream(stream, options);
}

Workbook::~Workbook() = default;

// ---------------------------------------------------------------------------
// Properties
// ---------------------------------------------------------------------------

WorksheetCollection& Workbook::GetWorksheets()
{
    return *_impl->worksheets;
}

const WorksheetCollection& Workbook::GetWorksheets() const
{
    return *_impl->worksheets;
}

WorkbookSettings& Workbook::GetSettings()
{
    return _impl->settings;
}

const WorkbookSettings& Workbook::GetSettings() const
{
    return _impl->settings;
}

WorkbookProperties& Workbook::GetProperties()
{
    return _impl->properties;
}

const WorkbookProperties& Workbook::GetProperties() const
{
    return _impl->properties;
}

DocumentProperties& Workbook::GetDocumentProperties()
{
    return _impl->documentProperties;
}

const DocumentProperties& Workbook::GetDocumentProperties() const
{
    return _impl->documentProperties;
}

DefinedNameCollection& Workbook::GetDefinedNames()
{
    return *_impl->definedNames;
}

const DefinedNameCollection& Workbook::GetDefinedNames() const
{
    return *_impl->definedNames;
}

LoadDiagnostics& Workbook::GetLoadDiagnostics()
{
    return _impl->loadDiagnostics;
}

const LoadDiagnostics& Workbook::GetLoadDiagnostics() const
{
    return _impl->loadDiagnostics;
}

Core::WorkbookModel& Workbook::GetModel()
{
    return _impl->model;
}

const Core::WorkbookModel& Workbook::GetModel() const
{
    return _impl->model;
}

std::vector<Core::DefinedNameModel>& Workbook::GetDefinedNamesModel()
{
    return _impl->model.GetDefinedNames();
}

// ---------------------------------------------------------------------------
// Save methods
// ---------------------------------------------------------------------------

void Workbook::Save(std::string_view fileName) const
{
    SaveOptions defaultOptions;
    Save(fileName, defaultOptions);
}

void Workbook::Save(std::string_view fileName, SaveFormat format) const
{
    SaveOptions options;
    options.SetSaveFormat(format);
    Save(fileName, options);
}

void Workbook::Save(std::string_view fileName, const SaveOptions& options) const
{
    std::vector<std::uint8_t> buffer;
    Save(buffer, options);

    std::ofstream file(std::string(fileName), std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        throw CellsException("Failed to create output file.");
    }

    file.write(reinterpret_cast<const char*>(buffer.data()),
               static_cast<std::streamsize>(buffer.size()));
    file.close();
}

void Workbook::Save(std::vector<std::uint8_t>& stream, SaveFormat format) const
{
    SaveOptions options;
    options.SetSaveFormat(format);
    Save(stream, options);
}

void Workbook::Save(std::vector<std::uint8_t>& stream, const SaveOptions& options) const
{
    try {
        XlsxWorkbookSerializer::Save(_impl->model, stream, options);
    } catch (const CellsException&) {
        throw;
    } catch (const std::exception& exception) {
        throw WorkbookSaveException("Failed to save XLSX workbook.", exception);
    }
}

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

void Workbook::EnsureUniqueSheetName(
    std::string_view sheetName,
    std::optional<std::reference_wrapper<const Core::WorksheetModel>> currentSheet)
{
    const auto& worksheets = _impl->model.GetWorksheets();
    for (std::size_t index = 0; index < worksheets.size(); ++index) {
        const auto& existing = worksheets[index];

        // Skip the current sheet being renamed.
        if (currentSheet.has_value() &&
            std::addressof(existing) == std::addressof(currentSheet->get())) {
            continue;
        }

        // Case-insensitive comparison.
        const auto& existingName = existing.GetName();
        if (existingName.size() == sheetName.size()) {
            bool match = true;
            for (std::size_t i = 0; i < existingName.size(); ++i) {
                if (std::tolower(static_cast<unsigned char>(existingName[i])) !=
                    std::tolower(static_cast<unsigned char>(sheetName[i]))) {
                    match = false;
                    break;
                }
            }
            if (match) {
                throw CellsException(
                    std::string("Worksheet name '") +
                    std::string(sheetName) +
                    "' already exists.");
            }
        }
    }
}

void Workbook::EnsureValidDefinedNameScope(std::optional<int> localSheetIndex)
{
    if (!localSheetIndex.has_value()) {
        return;
    }

    const int count = static_cast<int>(_impl->model.GetWorksheets().size());
    if (*localSheetIndex < 0 || *localSheetIndex >= count) {
        throw CellsException("Defined name scope must refer to an existing worksheet.");
    }
}

void Workbook::EnsureUniqueDefinedName(
    const Core::DefinedNameModel& currentDefinedName,
    std::string_view name,
    std::optional<int> localSheetIndex)
{
    const auto& definedNames = _impl->model.GetDefinedNames();
    for (std::size_t index = 0; index < definedNames.size(); ++index) {
        const auto& existing = definedNames[index];

        // Skip the current defined name being renamed.
        if (&existing == &currentDefinedName) {
            continue;
        }

        // Case-insensitive name comparison.
        const auto& existingName = existing.GetName();
        if (existingName.size() != name.size()) {
            continue;
        }
        bool nameMatch = true;
        for (std::size_t i = 0; i < existingName.size(); ++i) {
            if (std::tolower(static_cast<unsigned char>(existingName[i])) !=
                std::tolower(static_cast<unsigned char>(name[i]))) {
                nameMatch = false;
                break;
            }
        }
        if (!nameMatch) {
            continue;
        }

        if (DefinedNameUtility::SameScope(existing.GetLocalSheetIndex(), localSheetIndex)) {
            throw CellsException(
                std::string("Defined name '") +
                std::string(name) +
                "' already exists in the same scope.");
        }
    }
}

void Workbook::Dispose()
{
    // Release resources. In C++ the unique_ptr destructor handles cleanup.
    // This method is provided for API parity with C# IDisposable.
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

void Workbook::LoadFromStream(const std::vector<std::uint8_t>& stream,
                              const LoadOptions& options)
{
    try {
        Core::WorkbookModel loadedModel =
            XlsxWorkbookSerializer::Load(stream, options, _impl->loadDiagnostics);

        // Keep the Workbook facade and child facade instances stable after
        // load by copying the loaded state into the existing model graph.
        auto& ws = _impl->model.GetWorksheets();
        ws.clear();
        const auto& loadedWs = loadedModel.GetWorksheets();
        ws.insert(ws.end(), loadedWs.begin(), loadedWs.end());

        _impl->model.GetSettings().SetDateSystem(
            loadedModel.GetSettings().GetDateSystem());
        _impl->model.GetSettings().SetDisplayCulture(
            loadedModel.GetSettings().GetDisplayCulture());

        _impl->model.GetProperties().CopyFrom(loadedModel.GetProperties());
        _impl->model.GetDocumentProperties().CopyFrom(
            loadedModel.GetDocumentProperties());

        _impl->model.SetDefaultStyle(loadedModel.GetDefaultStyle());
        _impl->model.SetActiveSheetIndex(loadedModel.GetActiveSheetIndex());

        auto& dn = _impl->model.GetDefinedNames();
        dn.clear();
        const auto& loadedDn = loadedModel.GetDefinedNames();
        dn.insert(dn.end(), loadedDn.begin(), loadedDn.end());

        // Rebuild WorksheetCollection wrappers so they reference the
        // newly-inserted WorksheetModel objects instead of the destroyed ones.
        _impl->worksheets = std::make_unique<WorksheetCollection>(*this, _impl->model);
    } catch (const CellsException&) {
        throw;
    } catch (const std::exception& exception) {
        throw WorkbookLoadException("Failed to load XLSX workbook.", exception);
    }
}

}}  // namespace Aspose::Cells_FOSS
