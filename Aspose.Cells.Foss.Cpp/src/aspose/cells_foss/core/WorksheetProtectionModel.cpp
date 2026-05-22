#include "aspose/cells_foss/core/WorksheetProtectionModel.h"
#include <algorithm>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

namespace {

bool IsBlankOrWhiteSpace(const std::string& s) noexcept
{
    return std::all_of(s.begin(), s.end(), [](unsigned char ch) {
        return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'
            || ch == '\f' || ch == '\v';
    });
}

}  // anonymous namespace

WorksheetProtectionModel::WorksheetProtectionModel() = default;

void WorksheetProtectionModel::Clear()
{
    _isProtected = false;
    _objects = false;
    _scenarios = false;
    _formatCells = false;
    _formatColumns = false;
    _formatRows = false;
    _insertColumns = false;
    _insertRows = false;
    _insertHyperlinks = false;
    _deleteColumns = false;
    _deleteRows = false;
    _selectLockedCells = false;
    _sort = false;
    _autoFilter = false;
    _pivotTables = false;
    _selectUnlockedCells = false;
    _passwordHash.clear();
    _algorithmName.clear();
    _hashValue.clear();
    _saltValue.clear();
    _spinCount.clear();
}

bool WorksheetProtectionModel::HasStoredState() const
{
    return _isProtected
        || _objects
        || _scenarios
        || _formatCells
        || _formatColumns
        || _formatRows
        || _insertColumns
        || _insertRows
        || _insertHyperlinks
        || _deleteColumns
        || _deleteRows
        || _selectLockedCells
        || _sort
        || _autoFilter
        || _pivotTables
        || _selectUnlockedCells
        || !IsBlankOrWhiteSpace(_passwordHash)
        || !IsBlankOrWhiteSpace(_algorithmName)
        || !IsBlankOrWhiteSpace(_hashValue)
        || !IsBlankOrWhiteSpace(_saltValue)
        || !IsBlankOrWhiteSpace(_spinCount);
}

}}}  // namespace Aspose::Cells_FOSS::Core
