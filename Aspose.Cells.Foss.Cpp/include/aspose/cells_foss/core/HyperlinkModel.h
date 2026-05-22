#pragma once
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents hyperlink model.
/// </summary>
class HyperlinkModel final {
public:
    /// <summary>
    /// Initializes a new instance of the HyperlinkModel class.
    /// </summary>
    HyperlinkModel();

    /// <summary>
    /// Gets or sets the first row.
    /// </summary>
    int GetFirstRow() const noexcept { return _firstRow; }
    void SetFirstRow(int value) noexcept { _firstRow = value; }

    /// <summary>
    /// Gets or sets the first column.
    /// </summary>
    int GetFirstColumn() const noexcept { return _firstColumn; }
    void SetFirstColumn(int value) noexcept { _firstColumn = value; }

    /// <summary>
    /// Gets or sets the total rows.
    /// </summary>
    int GetTotalRows() const noexcept { return _totalRows; }
    void SetTotalRows(int value) noexcept { _totalRows = value; }

    /// <summary>
    /// Gets or sets the total columns.
    /// </summary>
    int GetTotalColumns() const noexcept { return _totalColumns; }
    void SetTotalColumns(int value) noexcept { _totalColumns = value; }

    /// <summary>
    /// Gets or sets the address.
    /// </summary>
    const std::string& GetAddress() const noexcept { return _address; }
    void SetAddress(std::string value) { _address = std::move(value); }

    /// <summary>
    /// Gets or sets the sub address.
    /// </summary>
    const std::string& GetSubAddress() const noexcept { return _subAddress; }
    void SetSubAddress(std::string value) { _subAddress = std::move(value); }

    /// <summary>
    /// Gets or sets the screen tip.
    /// </summary>
    const std::string& GetScreenTip() const noexcept { return _screenTip; }
    void SetScreenTip(std::string value) { _screenTip = std::move(value); }

    /// <summary>
    /// Gets or sets the text to display.
    /// </summary>
    const std::string& GetTextToDisplay() const noexcept { return _textToDisplay; }
    void SetTextToDisplay(std::string value) { _textToDisplay = std::move(value); }

private:
    int _firstRow = 0;
    int _firstColumn = 0;
    int _totalRows = 1;
    int _totalColumns = 1;
    std::string _address;
    std::string _subAddress;
    std::string _screenTip;
    std::string _textToDisplay;
};

}}}  // namespace Aspose::Cells_FOSS::Core
