#pragma once
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents workbook protection model.
/// </summary>
class WorkbookProtectionModel final {
public:
    /// <summary>
    /// Initializes a new instance of the WorkbookProtectionModel class.
    /// </summary>
    WorkbookProtectionModel();

    /// <summary>
    /// Gets or sets a value indicating whether lock structure.
    /// </summary>
    bool GetLockStructure() const noexcept { return _lockStructure; }
    void SetLockStructure(bool value) noexcept { _lockStructure = value; }

    /// <summary>
    /// Gets or sets a value indicating whether lock windows.
    /// </summary>
    bool GetLockWindows() const noexcept { return _lockWindows; }
    void SetLockWindows(bool value) noexcept { _lockWindows = value; }

    /// <summary>
    /// Gets or sets a value indicating whether lock revision.
    /// </summary>
    bool GetLockRevision() const noexcept { return _lockRevision; }
    void SetLockRevision(bool value) noexcept { _lockRevision = value; }

    /// <summary>
    /// Gets or sets the workbook password.
    /// </summary>
    const std::string& GetWorkbookPassword() const noexcept { return _workbookPassword; }
    void SetWorkbookPassword(std::string value) { _workbookPassword = std::move(value); }

    /// <summary>
    /// Gets or sets the revisions password.
    /// </summary>
    const std::string& GetRevisionsPassword() const noexcept { return _revisionsPassword; }
    void SetRevisionsPassword(std::string value) { _revisionsPassword = std::move(value); }

    /// <summary>
    /// Copies values from the specified source.
    /// </summary>
    /// <param name="source">The source.</param>
    void CopyFrom(const WorkbookProtectionModel& source);

    /// <summary>
    /// Performs has stored state.
    /// </summary>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool HasStoredState() const;

private:
    bool _lockStructure = false;
    bool _lockWindows = false;
    bool _lockRevision = false;
    std::string _workbookPassword;
    std::string _revisionsPassword;
};

}}}  // namespace Aspose::Cells_FOSS::Core
