#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "aspose/cells_foss/CalculationProperties.h"
#include "aspose/cells_foss/WorkbookProtection.h"
#include "aspose/cells_foss/WorkbookView.h"

namespace Aspose {
namespace Cells_FOSS {

namespace Core {
class WorkbookModel;
class WorkbookPropertiesModel;
}  // namespace Core

/// Represents workbook properties.
class WorkbookProperties final {
public:
    /// Constructs a WorkbookProperties wrapping the specified workbook model.
    /// @param workbookModel The underlying workbook model.
    explicit WorkbookProperties(Core::WorkbookModel& workbookModel);

    // Non-copyable: owns non-copyable sub-objects (WorkbookView).
    WorkbookProperties(const WorkbookProperties&) = delete;
    WorkbookProperties& operator=(const WorkbookProperties&) = delete;

    /// Gets or sets the code name.
    std::string GetCodeName() const;
    void SetCodeName(std::string_view value);

    /// Gets or sets the show objects.
    std::string GetShowObjects() const;
    void SetShowObjects(std::string_view value);

    /// Gets or sets a value indicating whether filter privacy.
    bool GetFilterPrivacy() const noexcept;
    void SetFilterPrivacy(bool value) noexcept;

    /// Gets or sets a value indicating whether show border unselected tables.
    bool GetShowBorderUnselectedTables() const noexcept;
    void SetShowBorderUnselectedTables(bool value) noexcept;

    /// Gets or sets a value indicating whether show ink annotation.
    bool GetShowInkAnnotation() const noexcept;
    void SetShowInkAnnotation(bool value) noexcept;

    /// Gets or sets a value indicating whether backup file.
    bool GetBackupFile() const noexcept;
    void SetBackupFile(bool value) noexcept;

    /// Gets or sets a value indicating whether save external link values.
    bool GetSaveExternalLinkValues() const noexcept;
    void SetSaveExternalLinkValues(bool value) noexcept;

    /// Gets or sets the update links.
    std::string GetUpdateLinks() const;
    void SetUpdateLinks(std::string_view value);

    /// Gets or sets a value indicating whether hide pivot field list.
    bool GetHidePivotFieldList() const noexcept;
    void SetHidePivotFieldList(bool value) noexcept;

    /// Gets or sets the default theme version.
    std::optional<int> GetDefaultThemeVersion() const noexcept;
    void SetDefaultThemeVersion(std::optional<int> value);

    /// Gets the protection.
    WorkbookProtection& GetProtection() noexcept;
    const WorkbookProtection& GetProtection() const noexcept;

    /// Gets the view.
    WorkbookView& GetView() noexcept;
    const WorkbookView& GetView() const noexcept;

    /// Gets the calculation.
    CalculationProperties& GetCalculation() noexcept;
    const CalculationProperties& GetCalculation() const noexcept;

private:
    Core::WorkbookModel& _workbookModel;
    Core::WorkbookPropertiesModel& _model;
    WorkbookProtection _protection;
    WorkbookView _view;
    CalculationProperties _calculation;
};

}}  // namespace Aspose::Cells_FOSS
