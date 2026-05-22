#pragma once

namespace Aspose {
namespace Cells_FOSS {
namespace Core {
class WorksheetProtectionModel;
}  // namespace Core

/// Represents worksheet protection.
class WorksheetProtection final {
public:
    /// Constructs a WorksheetProtection wrapping the specified model.
    /// @param model The underlying worksheet protection model.
    explicit WorksheetProtection(Core::WorksheetProtectionModel& model);

    /// Gets or sets a value indicating whether protected.
    bool GetIsProtected() const noexcept;
    void SetIsProtected(bool value) noexcept;

    /// Gets or sets a value indicating whether objects.
    bool GetObjects() const noexcept;
    void SetObjects(bool value) noexcept;

    /// Gets or sets a value indicating whether scenarios.
    bool GetScenarios() const noexcept;
    void SetScenarios(bool value) noexcept;

    /// Gets or sets a value indicating whether format cells.
    bool GetFormatCells() const noexcept;
    void SetFormatCells(bool value) noexcept;

    /// Gets or sets a value indicating whether format columns.
    bool GetFormatColumns() const noexcept;
    void SetFormatColumns(bool value) noexcept;

    /// Gets or sets a value indicating whether format rows.
    bool GetFormatRows() const noexcept;
    void SetFormatRows(bool value) noexcept;

    /// Gets or sets a value indicating whether insert columns.
    bool GetInsertColumns() const noexcept;
    void SetInsertColumns(bool value) noexcept;

    /// Gets or sets a value indicating whether insert rows.
    bool GetInsertRows() const noexcept;
    void SetInsertRows(bool value) noexcept;

    /// Gets or sets a value indicating whether insert hyperlinks.
    bool GetInsertHyperlinks() const noexcept;
    void SetInsertHyperlinks(bool value) noexcept;

    /// Gets or sets a value indicating whether delete columns.
    bool GetDeleteColumns() const noexcept;
    void SetDeleteColumns(bool value) noexcept;

    /// Gets or sets a value indicating whether delete rows.
    bool GetDeleteRows() const noexcept;
    void SetDeleteRows(bool value) noexcept;

    /// Gets or sets a value indicating whether select locked cells.
    bool GetSelectLockedCells() const noexcept;
    void SetSelectLockedCells(bool value) noexcept;

    /// Gets or sets a value indicating whether sort.
    bool GetSort() const noexcept;
    void SetSort(bool value) noexcept;

    /// Gets or sets a value indicating whether auto filter.
    bool GetAutoFilter() const noexcept;
    void SetAutoFilter(bool value) noexcept;

    /// Gets or sets a value indicating whether pivot tables.
    bool GetPivotTables() const noexcept;
    void SetPivotTables(bool value) noexcept;

    /// Gets or sets a value indicating whether select unlocked cells.
    bool GetSelectUnlockedCells() const noexcept;
    void SetSelectUnlockedCells(bool value) noexcept;

    /// Resets all protection settings to their defaults.
    void Reset();

private:
    void MarkProtectedWhenEnabled(bool value) noexcept;

    Core::WorksheetProtectionModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
