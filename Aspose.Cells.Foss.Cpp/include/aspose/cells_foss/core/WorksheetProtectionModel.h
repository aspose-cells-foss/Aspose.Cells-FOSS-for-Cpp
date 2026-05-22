#pragma once
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents worksheet protection model.
/// </summary>
class WorksheetProtectionModel final {
public:
    /// <summary>
    /// Initializes a new instance of the WorksheetProtectionModel class.
    /// </summary>
    WorksheetProtectionModel();

    /// <summary>
    /// Gets or sets a value indicating whether protected.
    /// </summary>
    bool GetIsProtected() const noexcept { return _isProtected; }
    void SetIsProtected(bool value) noexcept { _isProtected = value; }

    /// <summary>
    /// Gets or sets a value indicating whether objects.
    /// </summary>
    bool GetObjects() const noexcept { return _objects; }
    void SetObjects(bool value) noexcept { _objects = value; }

    /// <summary>
    /// Gets or sets a value indicating whether scenarios.
    /// </summary>
    bool GetScenarios() const noexcept { return _scenarios; }
    void SetScenarios(bool value) noexcept { _scenarios = value; }

    /// <summary>
    /// Gets or sets a value indicating whether format cells.
    /// </summary>
    bool GetFormatCells() const noexcept { return _formatCells; }
    void SetFormatCells(bool value) noexcept { _formatCells = value; }

    /// <summary>
    /// Gets or sets a value indicating whether format columns.
    /// </summary>
    bool GetFormatColumns() const noexcept { return _formatColumns; }
    void SetFormatColumns(bool value) noexcept { _formatColumns = value; }

    /// <summary>
    /// Gets or sets a value indicating whether format rows.
    /// </summary>
    bool GetFormatRows() const noexcept { return _formatRows; }
    void SetFormatRows(bool value) noexcept { _formatRows = value; }

    /// <summary>
    /// Gets or sets a value indicating whether insert columns.
    /// </summary>
    bool GetInsertColumns() const noexcept { return _insertColumns; }
    void SetInsertColumns(bool value) noexcept { _insertColumns = value; }

    /// <summary>
    /// Gets or sets a value indicating whether insert rows.
    /// </summary>
    bool GetInsertRows() const noexcept { return _insertRows; }
    void SetInsertRows(bool value) noexcept { _insertRows = value; }

    /// <summary>
    /// Gets or sets a value indicating whether insert hyperlinks.
    /// </summary>
    bool GetInsertHyperlinks() const noexcept { return _insertHyperlinks; }
    void SetInsertHyperlinks(bool value) noexcept { _insertHyperlinks = value; }

    /// <summary>
    /// Gets or sets a value indicating whether delete columns.
    /// </summary>
    bool GetDeleteColumns() const noexcept { return _deleteColumns; }
    void SetDeleteColumns(bool value) noexcept { _deleteColumns = value; }

    /// <summary>
    /// Gets or sets a value indicating whether delete rows.
    /// </summary>
    bool GetDeleteRows() const noexcept { return _deleteRows; }
    void SetDeleteRows(bool value) noexcept { _deleteRows = value; }

    /// <summary>
    /// Gets or sets a value indicating whether select locked cells.
    /// </summary>
    bool GetSelectLockedCells() const noexcept { return _selectLockedCells; }
    void SetSelectLockedCells(bool value) noexcept { _selectLockedCells = value; }

    /// <summary>
    /// Gets or sets a value indicating whether sort.
    /// </summary>
    bool GetSort() const noexcept { return _sort; }
    void SetSort(bool value) noexcept { _sort = value; }

    /// <summary>
    /// Gets or sets a value indicating whether auto filter.
    /// </summary>
    bool GetAutoFilter() const noexcept { return _autoFilter; }
    void SetAutoFilter(bool value) noexcept { _autoFilter = value; }

    /// <summary>
    /// Gets or sets a value indicating whether pivot tables.
    /// </summary>
    bool GetPivotTables() const noexcept { return _pivotTables; }
    void SetPivotTables(bool value) noexcept { _pivotTables = value; }

    /// <summary>
    /// Gets or sets a value indicating whether select unlocked cells.
    /// </summary>
    bool GetSelectUnlockedCells() const noexcept { return _selectUnlockedCells; }
    void SetSelectUnlockedCells(bool value) noexcept { _selectUnlockedCells = value; }

    /// <summary>
    /// Gets or sets the password hash.
    /// </summary>
    const std::string& GetPasswordHash() const noexcept { return _passwordHash; }
    void SetPasswordHash(std::string value) { _passwordHash = std::move(value); }

    /// <summary>
    /// Gets or sets the algorithm name.
    /// </summary>
    const std::string& GetAlgorithmName() const noexcept { return _algorithmName; }
    void SetAlgorithmName(std::string value) { _algorithmName = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether h value.
    /// </summary>
    const std::string& GetHashValue() const noexcept { return _hashValue; }
    void SetHashValue(std::string value) { _hashValue = std::move(value); }

    /// <summary>
    /// Gets or sets the salt value.
    /// </summary>
    const std::string& GetSaltValue() const noexcept { return _saltValue; }
    void SetSaltValue(std::string value) { _saltValue = std::move(value); }

    /// <summary>
    /// Gets or sets the spin count.
    /// </summary>
    const std::string& GetSpinCount() const noexcept { return _spinCount; }
    void SetSpinCount(std::string value) { _spinCount = std::move(value); }

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear();

    /// <summary>
    /// Performs has stored state.
    /// </summary>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool HasStoredState() const;

private:
    bool _isProtected = false;
    bool _objects = false;
    bool _scenarios = false;
    bool _formatCells = false;
    bool _formatColumns = false;
    bool _formatRows = false;
    bool _insertColumns = false;
    bool _insertRows = false;
    bool _insertHyperlinks = false;
    bool _deleteColumns = false;
    bool _deleteRows = false;
    bool _selectLockedCells = false;
    bool _sort = false;
    bool _autoFilter = false;
    bool _pivotTables = false;
    bool _selectUnlockedCells = false;
    std::string _passwordHash;
    std::string _algorithmName;
    std::string _hashValue;
    std::string _saltValue;
    std::string _spinCount;
};

}}}  // namespace Aspose::Cells_FOSS::Core
