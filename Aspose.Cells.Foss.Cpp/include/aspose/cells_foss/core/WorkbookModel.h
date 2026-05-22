#pragma once

#include <deque>
#include <vector>

#include "aspose/cells_foss/core/DefinedNameModel.h"
#include "aspose/cells_foss/core/DiagnosticBag.h"
#include "aspose/cells_foss/core/DocumentPropertiesModel.h"
#include "aspose/cells_foss/core/SharedStringRepository.h"
#include "aspose/cells_foss/core/StyleRepository.h"
#include "aspose/cells_foss/core/StyleValue.h"
#include "aspose/cells_foss/core/WorkbookPropertiesModel.h"
#include "aspose/cells_foss/core/WorkbookSettingsModel.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

// NOTE: std::deque<WorksheetModel> is used instead of std::vector so that
// emplace_back / push_back never invalidates references or pointers to
// existing elements.  Worksheet and HyperlinkCollection store references
// into this container and must survive worksheet collection mutations.

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents workbook model.
/// </summary>
class WorkbookModel final {
public:
    /// <summary>
    /// Initializes a new instance of the WorkbookModel class.
    /// </summary>
    WorkbookModel();

    /// <summary>
    /// Gets the worksheets.
    /// </summary>
    const std::deque<WorksheetModel>& GetWorksheets() const noexcept { return _worksheets; }
    std::deque<WorksheetModel>& GetWorksheets() noexcept { return _worksheets; }

    /// <summary>
    /// Gets the settings.
    /// </summary>
    const WorkbookSettingsModel& GetSettings() const noexcept { return _settings; }
    WorkbookSettingsModel& GetSettings() noexcept { return _settings; }

    /// <summary>
    /// Gets the properties.
    /// </summary>
    const WorkbookPropertiesModel& GetProperties() const noexcept { return _properties; }
    WorkbookPropertiesModel& GetProperties() noexcept { return _properties; }

    /// <summary>
    /// Gets the document properties.
    /// </summary>
    const DocumentPropertiesModel& GetDocumentProperties() const noexcept { return _documentProperties; }
    DocumentPropertiesModel& GetDocumentProperties() noexcept { return _documentProperties; }

    /// <summary>
    /// Gets the diagnostics.
    /// </summary>
    const DiagnosticBag& GetDiagnostics() const noexcept { return _diagnostics; }
    DiagnosticBag& GetDiagnostics() noexcept { return _diagnostics; }

    /// <summary>
    /// Gets the styles.
    /// </summary>
    const StyleRepository& GetStyles() const noexcept { return _styles; }
    StyleRepository& GetStyles() noexcept { return _styles; }

    /// <summary>
    /// Gets or sets the shared strings.
    /// </summary>
    const SharedStringRepository& GetSharedStrings() const noexcept { return _sharedStrings; }
    SharedStringRepository& GetSharedStrings() noexcept { return _sharedStrings; }

    /// <summary>
    /// Gets or sets the default style.
    /// </summary>
    const StyleValue& GetDefaultStyle() const noexcept { return _defaultStyle; }
    void SetDefaultStyle(StyleValue value) { _defaultStyle = std::move(value); }

    /// <summary>
    /// Gets or sets the active sheet index.
    /// </summary>
    int GetActiveSheetIndex() const noexcept { return _activeSheetIndex; }
    void SetActiveSheetIndex(int value) noexcept { _activeSheetIndex = value; }

    /// <summary>
    /// Gets the defined names.
    /// </summary>
    const std::vector<DefinedNameModel>& GetDefinedNames() const noexcept { return _definedNames; }
    std::vector<DefinedNameModel>& GetDefinedNames() noexcept { return _definedNames; }

private:
    std::deque<WorksheetModel> _worksheets;
    WorkbookSettingsModel _settings;
    WorkbookPropertiesModel _properties;
    DocumentPropertiesModel _documentProperties;
    DiagnosticBag _diagnostics;
    StyleRepository _styles;
    SharedStringRepository _sharedStrings;
    StyleValue _defaultStyle;
    int _activeSheetIndex = 0;
    std::vector<DefinedNameModel> _definedNames;
};

}}}  // namespace Aspose::Cells_FOSS::Core
