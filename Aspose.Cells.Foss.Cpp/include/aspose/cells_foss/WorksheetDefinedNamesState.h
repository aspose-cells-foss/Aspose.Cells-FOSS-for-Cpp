#pragma once

#include <string>

namespace Aspose {
namespace Cells_FOSS {

/// Stores the defined names state for a worksheet (print area, title rows, title columns).
class WorksheetDefinedNamesState final {
public:
    /// Initializes a new instance of the WorksheetDefinedNamesState class.
    WorksheetDefinedNamesState();

    /// Gets or sets the print area.
    const std::string& GetPrintArea() const noexcept;
    void SetPrintArea(const std::string& value);

    /// Gets or sets the print title rows.
    const std::string& GetPrintTitleRows() const noexcept;
    void SetPrintTitleRows(const std::string& value);

    /// Gets or sets the print title columns.
    const std::string& GetPrintTitleColumns() const noexcept;
    void SetPrintTitleColumns(const std::string& value);

private:
    std::string _printArea;
    std::string _printTitleRows;
    std::string _printTitleColumns;
};

}}  // namespace Aspose::Cells_FOSS
