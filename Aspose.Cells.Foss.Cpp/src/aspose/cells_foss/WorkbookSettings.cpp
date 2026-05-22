#include "aspose/cells_foss/WorkbookSettings.h"
#include "aspose/cells_foss/core/DateSystem.h"
#include "aspose/cells_foss/core/WorkbookSettingsModel.h"

#include <locale>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

WorkbookSettings::WorkbookSettings(Core::WorkbookSettingsModel& model)
    : _model(model)
{
}

// ---------------------------------------------------------------------------
// Date1904
// ---------------------------------------------------------------------------

bool WorkbookSettings::GetDate1904() const noexcept
{
    return _model.GetDateSystem() == Core::DateSystem::Mac1904;
}

void WorkbookSettings::SetDate1904(bool value) noexcept
{
    _model.SetDateSystem(value ? Core::DateSystem::Mac1904
                               : Core::DateSystem::Windows1900);
}

// ---------------------------------------------------------------------------
// Culture
// ---------------------------------------------------------------------------

std::locale WorkbookSettings::GetCulture() const
{
    // Return a copy so callers can mutate the returned locale without
    // changing workbook formatting rules until they assign it back.
    return _model.GetDisplayCulture();
}

void WorkbookSettings::SetCulture(std::locale value)
{
    _model.SetDisplayCulture(std::move(value));
}

}}  // namespace Aspose::Cells_FOSS
