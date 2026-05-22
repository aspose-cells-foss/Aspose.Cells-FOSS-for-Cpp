#pragma once

#include <locale>

namespace Aspose {
namespace Cells_FOSS {

namespace Core {
class WorkbookSettingsModel;
}  // namespace Core

/// <summary>
/// Represents workbook-level settings that affect date handling and display formatting.
/// </summary>
class WorkbookSettings final {
public:
    /// <summary>
    /// Constructs a WorkbookSettings wrapping the specified model.
    /// </summary>
    /// <param name="model">The underlying workbook settings model.</param>
    explicit WorkbookSettings(Core::WorkbookSettingsModel& model);

    // WorkbookSettings is non-copyable (matches C# sealed + readonly refs).
    WorkbookSettings(const WorkbookSettings&) = delete;
    WorkbookSettings& operator=(const WorkbookSettings&) = delete;

    /// <summary>
    /// Gets or sets whether the workbook uses the 1904 date system.
    /// </summary>
    bool GetDate1904() const noexcept;
    void SetDate1904(bool value) noexcept;

    /// <summary>
    /// Gets or sets the culture used for display-string formatting.
    /// </summary>
    std::locale GetCulture() const;
    void SetCulture(std::locale value);

private:
    Core::WorkbookSettingsModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
