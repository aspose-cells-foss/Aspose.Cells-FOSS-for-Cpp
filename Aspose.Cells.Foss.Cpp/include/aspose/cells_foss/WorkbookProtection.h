#pragma once

#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {
class WorkbookProtectionModel;
}  // namespace Core

/// Represents workbook protection.
class WorkbookProtection final {
public:
    /// Constructs a WorkbookProtection wrapping the specified model.
    /// @param model The underlying workbook protection model.
    explicit WorkbookProtection(Core::WorkbookProtectionModel& model);

    /// Gets a value indicating whether protected.
    bool IsProtected() const noexcept;

    /// Gets or sets a value indicating whether lock structure.
    bool GetLockStructure() const noexcept;
    void SetLockStructure(bool value) noexcept;

    /// Gets or sets a value indicating whether lock windows.
    bool GetLockWindows() const noexcept;
    void SetLockWindows(bool value) noexcept;

    /// Gets or sets a value indicating whether lock revision.
    bool GetLockRevision() const noexcept;
    void SetLockRevision(bool value) noexcept;

    /// Gets or sets the workbook password.
    std::string GetWorkbookPassword() const;
    void SetWorkbookPassword(std::string_view value);

    /// Gets or sets the revisions password.
    std::string GetRevisionsPassword() const;
    void SetRevisionsPassword(std::string_view value);

private:
    Core::WorkbookProtectionModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
