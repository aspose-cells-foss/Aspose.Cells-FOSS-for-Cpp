#pragma once

#include <optional>
#include <string>
#include <vector>

#include "aspose/cells_foss/core/HeaderFooterModel.h"
#include "aspose/cells_foss/core/PageMarginsModel.h"
#include "aspose/cells_foss/core/PageOrientation.h"
#include "aspose/cells_foss/core/PrintOptionsModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents page setup model.
/// </summary>
class PageSetupModel final {
public:
    /// <summary>
    /// Initializes a new instance of the PageSetupModel class.
    /// </summary>
    PageSetupModel();

    /// <summary>
    /// Performs page margins model.
    /// </summary>
    const PageMarginsModel& GetMargins() const noexcept { return _margins; }
    PageMarginsModel& GetMargins() noexcept { return _margins; }

    /// <summary>
    /// Performs print options model.
    /// </summary>
    const PrintOptionsModel& GetPrintOptions() const noexcept { return _printOptions; }
    PrintOptionsModel& GetPrintOptions() noexcept { return _printOptions; }

    /// <summary>
    /// Performs header footer model.
    /// </summary>
    const HeaderFooterModel& GetHeaderFooter() const noexcept { return _headerFooter; }
    HeaderFooterModel& GetHeaderFooter() noexcept { return _headerFooter; }

    /// <summary>
    /// Gets or sets the paper size.
    /// </summary>
    int GetPaperSize() const noexcept { return _paperSize; }
    void SetPaperSize(int value) noexcept { _paperSize = value; }

    /// <summary>
    /// Gets or sets the orientation.
    /// </summary>
    PageOrientation GetOrientation() const noexcept { return _orientation; }
    void SetOrientation(PageOrientation value) noexcept { _orientation = value; }

    /// <summary>
    /// Gets or sets the first page number.
    /// </summary>
    const std::optional<int>& GetFirstPageNumber() const noexcept { return _firstPageNumber; }
    void SetFirstPageNumber(std::optional<int> value) noexcept { _firstPageNumber = value; }

    /// <summary>
    /// Gets or sets the scale.
    /// </summary>
    const std::optional<int>& GetScale() const noexcept { return _scale; }
    void SetScale(std::optional<int> value) noexcept { _scale = value; }

    /// <summary>
    /// Gets or sets the fit to width.
    /// </summary>
    const std::optional<int>& GetFitToWidth() const noexcept { return _fitToWidth; }
    void SetFitToWidth(std::optional<int> value) noexcept { _fitToWidth = value; }

    /// <summary>
    /// Gets or sets the fit to height.
    /// </summary>
    const std::optional<int>& GetFitToHeight() const noexcept { return _fitToHeight; }
    void SetFitToHeight(std::optional<int> value) noexcept { _fitToHeight = value; }

    /// <summary>
    /// Gets or sets the print area.
    /// </summary>
    const std::string& GetPrintArea() const noexcept { return _printArea; }
    void SetPrintArea(std::string value) { _printArea = std::move(value); }

    /// <summary>
    /// Gets or sets the print title rows.
    /// </summary>
    const std::string& GetPrintTitleRows() const noexcept { return _printTitleRows; }
    void SetPrintTitleRows(std::string value) { _printTitleRows = std::move(value); }

    /// <summary>
    /// Gets or sets the print title columns.
    /// </summary>
    const std::string& GetPrintTitleColumns() const noexcept { return _printTitleColumns; }
    void SetPrintTitleColumns(std::string value) { _printTitleColumns = std::move(value); }

    /// <summary>
    /// Gets the horizontal page breaks.
    /// </summary>
    const std::vector<int>& GetHorizontalPageBreaks() const noexcept { return _horizontalPageBreaks; }
    std::vector<int>& GetHorizontalPageBreaks() noexcept { return _horizontalPageBreaks; }

    /// <summary>
    /// Gets the vertical page breaks.
    /// </summary>
    const std::vector<int>& GetVerticalPageBreaks() const noexcept { return _verticalPageBreaks; }
    std::vector<int>& GetVerticalPageBreaks() noexcept { return _verticalPageBreaks; }

private:
    PageMarginsModel _margins;
    PrintOptionsModel _printOptions;
    HeaderFooterModel _headerFooter;
    int _paperSize = 0;
    PageOrientation _orientation = PageOrientation::Default;
    std::optional<int> _firstPageNumber;
    std::optional<int> _scale;
    std::optional<int> _fitToWidth;
    std::optional<int> _fitToHeight;
    std::string _printArea;
    std::string _printTitleRows;
    std::string _printTitleColumns;
    std::vector<int> _horizontalPageBreaks;
    std::vector<int> _verticalPageBreaks;
};

}}}  // namespace Aspose::Cells_FOSS::Core
