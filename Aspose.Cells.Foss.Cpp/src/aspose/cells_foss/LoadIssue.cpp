#include "aspose/cells_foss/LoadIssue.h"

#include <stdexcept>

namespace Aspose {
namespace Cells_FOSS {

LoadIssue::LoadIssue(std::string_view code,
                     DiagnosticSeverity severity,
                     std::string_view message,
                     bool repairApplied,
                     bool dataLossRisk)
    : _code(code)
    , _severity(severity)
    , _message(message)
    , _repairApplied(repairApplied)
    , _dataLossRisk(dataLossRisk)
{
    if (_code.empty()) {
        throw std::invalid_argument("code cannot be null or empty.");
    }
    if (_message.empty()) {
        throw std::invalid_argument("message cannot be null or empty.");
    }
}

const std::string& LoadIssue::GetCode() const noexcept
{
    return _code;
}

DiagnosticSeverity LoadIssue::GetSeverity() const noexcept
{
    return _severity;
}

const std::string& LoadIssue::GetMessage() const noexcept
{
    return _message;
}

bool LoadIssue::GetRepairApplied() const noexcept
{
    return _repairApplied;
}

bool LoadIssue::GetDataLossRisk() const noexcept
{
    return _dataLossRisk;
}

const std::string& LoadIssue::GetPartUri() const noexcept
{
    return _partUri;
}

void LoadIssue::SetPartUri(std::string_view value)
{
    _partUri = value;
}

const std::string& LoadIssue::GetSheetName() const noexcept
{
    return _sheetName;
}

void LoadIssue::SetSheetName(std::string_view value)
{
    _sheetName = value;
}

const std::string& LoadIssue::GetCellRef() const noexcept
{
    return _cellRef;
}

void LoadIssue::SetCellRef(std::string_view value)
{
    _cellRef = value;
}

std::optional<int> LoadIssue::GetRowIndex() const noexcept
{
    return _rowIndex;
}

void LoadIssue::SetRowIndex(std::optional<int> value) noexcept
{
    _rowIndex = value;
}

}}  // namespace Aspose::Cells_FOSS
