#pragma once

#include <locale>

#include "aspose/cells_foss/core/DateSystem.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents workbook settings model.
/// </summary>
class WorkbookSettingsModel final {
public:
    /// <summary>
    /// Initializes a new instance of the WorkbookSettingsModel class.
    /// </summary>
    WorkbookSettingsModel();

    /// <summary>
    /// Gets or sets the date system.
    /// </summary>
    DateSystem GetDateSystem() const noexcept { return _dateSystem; }
    void SetDateSystem(DateSystem value) noexcept { _dateSystem = value; }

    /// <summary>
    /// Gets or sets the display culture.
    /// </summary>
    const std::locale& GetDisplayCulture() const noexcept { return _displayCulture; }
    void SetDisplayCulture(std::locale value) { _displayCulture = std::move(value); }

private:
    DateSystem _dateSystem = DateSystem::Windows1900;
    std::locale _displayCulture;
};

}}}  // namespace Aspose::Cells_FOSS::Core
