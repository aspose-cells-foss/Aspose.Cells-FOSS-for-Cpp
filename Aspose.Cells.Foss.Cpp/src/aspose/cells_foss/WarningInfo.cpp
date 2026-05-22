#include "aspose/cells_foss/WarningInfo.h"

namespace Aspose {
namespace Cells_FOSS {

WarningInfo::WarningInfo()
    : _severity(DiagnosticSeverity::Warning)
    , _dataLossRisk(false)
{
}

const std::string& WarningInfo::GetCode() const noexcept
{
    return _code;
}

void WarningInfo::SetCode(std::string_view value)
{
    _code = value;
}

DiagnosticSeverity WarningInfo::GetSeverity() const noexcept
{
    return _severity;
}

void WarningInfo::SetSeverity(DiagnosticSeverity value) noexcept
{
    _severity = value;
}

const std::string& WarningInfo::GetMessage() const noexcept
{
    return _message;
}

void WarningInfo::SetMessage(std::string_view value)
{
    _message = value;
}

bool WarningInfo::GetDataLossRisk() const noexcept
{
    return _dataLossRisk;
}

void WarningInfo::SetDataLossRisk(bool value) noexcept
{
    _dataLossRisk = value;
}

const std::string& WarningInfo::GetPartUri() const noexcept
{
    return _partUri;
}

void WarningInfo::SetPartUri(std::string_view value)
{
    _partUri = value;
}

const std::string& WarningInfo::GetSheetName() const noexcept
{
    return _sheetName;
}

void WarningInfo::SetSheetName(std::string_view value)
{
    _sheetName = value;
}

const std::string& WarningInfo::GetCellRef() const noexcept
{
    return _cellRef;
}

void WarningInfo::SetCellRef(std::string_view value)
{
    _cellRef = value;
}

std::optional<int> WarningInfo::GetRowIndex() const noexcept
{
    return _rowIndex;
}

void WarningInfo::SetRowIndex(std::optional<int> value) noexcept
{
    _rowIndex = value;
}

}}  // namespace Aspose::Cells_FOSS
