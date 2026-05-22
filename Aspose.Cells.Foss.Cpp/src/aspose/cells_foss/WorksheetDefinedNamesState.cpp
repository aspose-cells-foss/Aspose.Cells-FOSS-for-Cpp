#include "aspose/cells_foss/WorksheetDefinedNamesState.h"

namespace Aspose {
namespace Cells_FOSS {

WorksheetDefinedNamesState::WorksheetDefinedNamesState() = default;

const std::string& WorksheetDefinedNamesState::GetPrintArea() const noexcept
{
    return _printArea;
}

void WorksheetDefinedNamesState::SetPrintArea(const std::string& value)
{
    _printArea = value;
}

const std::string& WorksheetDefinedNamesState::GetPrintTitleRows() const noexcept
{
    return _printTitleRows;
}

void WorksheetDefinedNamesState::SetPrintTitleRows(const std::string& value)
{
    _printTitleRows = value;
}

const std::string& WorksheetDefinedNamesState::GetPrintTitleColumns() const noexcept
{
    return _printTitleColumns;
}

void WorksheetDefinedNamesState::SetPrintTitleColumns(const std::string& value)
{
    _printTitleColumns = value;
}

}}  // namespace Aspose::Cells_FOSS
