#include "aspose/cells_foss/WorkbookProperties.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/WorkbookPropertySupport.h"
#include "aspose/cells_foss/core/WorkbookModel.h"
#include "aspose/cells_foss/core/WorkbookPropertiesModel.h"

#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

WorkbookProperties::WorkbookProperties(Core::WorkbookModel& workbookModel)
    : _workbookModel(workbookModel)
    , _model(workbookModel.GetProperties())
    , _protection(_model.GetProtection())
    , _view(workbookModel)
    , _calculation(_model.GetCalculation())
{
}

// ---------------------------------------------------------------------------
// CodeName
// ---------------------------------------------------------------------------

std::string WorkbookProperties::GetCodeName() const
{
    return _model.GetCodeName();
}

void WorkbookProperties::SetCodeName(std::string_view value)
{
    _model.SetCodeName(std::string(value));
}

// ---------------------------------------------------------------------------
// ShowObjects
// ---------------------------------------------------------------------------

std::string WorkbookProperties::GetShowObjects() const
{
    const auto& val = _model.GetShowObjects();
    return val.empty() ? std::string("all") : val;
}

void WorkbookProperties::SetShowObjects(std::string_view value)
{
    _model.SetShowObjects(WorkbookPropertySupport::NormalizeShowObjects(value));
}

// ---------------------------------------------------------------------------
// FilterPrivacy
// ---------------------------------------------------------------------------

bool WorkbookProperties::GetFilterPrivacy() const noexcept
{
    return _model.GetFilterPrivacy();
}

void WorkbookProperties::SetFilterPrivacy(bool value) noexcept
{
    _model.SetFilterPrivacy(value);
}

// ---------------------------------------------------------------------------
// ShowBorderUnselectedTables
// ---------------------------------------------------------------------------

bool WorkbookProperties::GetShowBorderUnselectedTables() const noexcept
{
    return _model.GetShowBorderUnselectedTables();
}

void WorkbookProperties::SetShowBorderUnselectedTables(bool value) noexcept
{
    _model.SetShowBorderUnselectedTables(value);
}

// ---------------------------------------------------------------------------
// ShowInkAnnotation
// ---------------------------------------------------------------------------

bool WorkbookProperties::GetShowInkAnnotation() const noexcept
{
    return _model.GetShowInkAnnotation();
}

void WorkbookProperties::SetShowInkAnnotation(bool value) noexcept
{
    _model.SetShowInkAnnotation(value);
}

// ---------------------------------------------------------------------------
// BackupFile
// ---------------------------------------------------------------------------

bool WorkbookProperties::GetBackupFile() const noexcept
{
    return _model.GetBackupFile();
}

void WorkbookProperties::SetBackupFile(bool value) noexcept
{
    _model.SetBackupFile(value);
}

// ---------------------------------------------------------------------------
// SaveExternalLinkValues
// ---------------------------------------------------------------------------

bool WorkbookProperties::GetSaveExternalLinkValues() const noexcept
{
    return _model.GetSaveExternalLinkValues();
}

void WorkbookProperties::SetSaveExternalLinkValues(bool value) noexcept
{
    _model.SetSaveExternalLinkValues(value);
}

// ---------------------------------------------------------------------------
// UpdateLinks
// ---------------------------------------------------------------------------

std::string WorkbookProperties::GetUpdateLinks() const
{
    const auto& val = _model.GetUpdateLinks();
    return val.empty() ? std::string("userSet") : val;
}

void WorkbookProperties::SetUpdateLinks(std::string_view value)
{
    _model.SetUpdateLinks(WorkbookPropertySupport::NormalizeUpdateLinks(value));
}

// ---------------------------------------------------------------------------
// HidePivotFieldList
// ---------------------------------------------------------------------------

bool WorkbookProperties::GetHidePivotFieldList() const noexcept
{
    return _model.GetHidePivotFieldList();
}

void WorkbookProperties::SetHidePivotFieldList(bool value) noexcept
{
    _model.SetHidePivotFieldList(value);
}

// ---------------------------------------------------------------------------
// DefaultThemeVersion
// ---------------------------------------------------------------------------

std::optional<int> WorkbookProperties::GetDefaultThemeVersion() const noexcept
{
    return _model.GetDefaultThemeVersion();
}

void WorkbookProperties::SetDefaultThemeVersion(std::optional<int> value)
{
    if (value.has_value() && value.value() < 0) {
        throw CellsException("DefaultThemeVersion must be non-negative.");
    }
    _model.SetDefaultThemeVersion(value);
}

// ---------------------------------------------------------------------------
// Protection
// ---------------------------------------------------------------------------

WorkbookProtection& WorkbookProperties::GetProtection() noexcept
{
    return _protection;
}

const WorkbookProtection& WorkbookProperties::GetProtection() const noexcept
{
    return _protection;
}

// ---------------------------------------------------------------------------
// View
// ---------------------------------------------------------------------------

WorkbookView& WorkbookProperties::GetView() noexcept
{
    return _view;
}

const WorkbookView& WorkbookProperties::GetView() const noexcept
{
    return _view;
}

// ---------------------------------------------------------------------------
// Calculation
// ---------------------------------------------------------------------------

CalculationProperties& WorkbookProperties::GetCalculation() noexcept
{
    return _calculation;
}

const CalculationProperties& WorkbookProperties::GetCalculation() const noexcept
{
    return _calculation;
}

}}  // namespace Aspose::Cells_FOSS
