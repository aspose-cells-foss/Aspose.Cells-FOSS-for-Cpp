#pragma once

#include <optional>
#include <string>

#include "aspose/cells_foss/core/CalculationPropertiesModel.h"
#include "aspose/cells_foss/core/WorkbookProtectionModel.h"
#include "aspose/cells_foss/core/WorkbookViewModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents workbook properties model.
/// </summary>
class WorkbookPropertiesModel final {
public:
    /// <summary>
    /// Initializes a new instance of the WorkbookPropertiesModel class.
    /// </summary>
    WorkbookPropertiesModel();

    /// <summary>
    /// Gets or sets the code name.
    /// </summary>
    const std::string& GetCodeName() const noexcept { return _codeName; }
    void SetCodeName(std::string value) { _codeName = std::move(value); }

    /// <summary>
    /// Gets or sets the show objects.
    /// </summary>
    const std::string& GetShowObjects() const noexcept { return _showObjects; }
    void SetShowObjects(std::string value) { _showObjects = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether filter privacy.
    /// </summary>
    bool GetFilterPrivacy() const noexcept { return _filterPrivacy; }
    void SetFilterPrivacy(bool value) noexcept { _filterPrivacy = value; }

    /// <summary>
    /// Gets or sets a value indicating whether show border unselected tables.
    /// </summary>
    bool GetShowBorderUnselectedTables() const noexcept { return _showBorderUnselectedTables; }
    void SetShowBorderUnselectedTables(bool value) noexcept { _showBorderUnselectedTables = value; }

    /// <summary>
    /// Gets or sets a value indicating whether show ink annotation.
    /// </summary>
    bool GetShowInkAnnotation() const noexcept { return _showInkAnnotation; }
    void SetShowInkAnnotation(bool value) noexcept { _showInkAnnotation = value; }

    /// <summary>
    /// Gets or sets a value indicating whether backup file.
    /// </summary>
    bool GetBackupFile() const noexcept { return _backupFile; }
    void SetBackupFile(bool value) noexcept { _backupFile = value; }

    /// <summary>
    /// Gets or sets a value indicating whether save external link values.
    /// </summary>
    bool GetSaveExternalLinkValues() const noexcept { return _saveExternalLinkValues; }
    void SetSaveExternalLinkValues(bool value) noexcept { _saveExternalLinkValues = value; }

    /// <summary>
    /// Gets or sets the update links.
    /// </summary>
    const std::string& GetUpdateLinks() const noexcept { return _updateLinks; }
    void SetUpdateLinks(std::string value) { _updateLinks = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether hide pivot field list.
    /// </summary>
    bool GetHidePivotFieldList() const noexcept { return _hidePivotFieldList; }
    void SetHidePivotFieldList(bool value) noexcept { _hidePivotFieldList = value; }

    /// <summary>
    /// Gets or sets the default theme version.
    /// </summary>
    std::optional<int> GetDefaultThemeVersion() const noexcept { return _defaultThemeVersion; }
    void SetDefaultThemeVersion(std::optional<int> value) noexcept { _defaultThemeVersion = value; }

    /// <summary>
    /// Gets the protection.
    /// </summary>
    WorkbookProtectionModel& GetProtection() noexcept { return _protection; }
    const WorkbookProtectionModel& GetProtection() const noexcept { return _protection; }

    /// <summary>
    /// Gets the view.
    /// </summary>
    WorkbookViewModel& GetView() noexcept { return _view; }
    const WorkbookViewModel& GetView() const noexcept { return _view; }

    /// <summary>
    /// Gets the calculation.
    /// </summary>
    CalculationPropertiesModel& GetCalculation() noexcept { return _calculation; }
    const CalculationPropertiesModel& GetCalculation() const noexcept { return _calculation; }

    /// <summary>
    /// Copies values from the specified source.
    /// </summary>
    /// <param name="source">The source.</param>
    void CopyFrom(const WorkbookPropertiesModel& source);

    /// <summary>
    /// Performs has workbook properties state.
    /// </summary>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool HasWorkbookPropertiesState() const;

private:
    std::string _codeName;
    std::string _showObjects;
    bool _filterPrivacy = false;
    bool _showBorderUnselectedTables = true;
    bool _showInkAnnotation = true;
    bool _backupFile = false;
    bool _saveExternalLinkValues = true;
    std::string _updateLinks;
    bool _hidePivotFieldList = false;
    std::optional<int> _defaultThemeVersion;
    WorkbookProtectionModel _protection;
    WorkbookViewModel _view;
    CalculationPropertiesModel _calculation;
};

}}}  // namespace Aspose::Cells_FOSS::Core
