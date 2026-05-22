#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "aspose/cells_foss/PageOrientationType.h"
#include "aspose/cells_foss/PaperSizeType.h"

namespace Aspose {
namespace Cells_FOSS {

namespace Core {
class PageSetupModel;
}  // namespace Core

/// Represents worksheet print and page-layout settings.
class PageSetup final {
public:
    /// Constructs a PageSetup wrapping the specified model.
    /// @param model The underlying page setup model.
    explicit PageSetup(Core::PageSetupModel& model);

    // -----------------------------------------------------------------------
    // Paper & orientation
    // -----------------------------------------------------------------------

    /// Gets or sets the paper size.
    PaperSizeType GetPaperSize() const noexcept;
    void SetPaperSize(PaperSizeType value) noexcept;

    /// Gets or sets the page orientation.
    PageOrientationType GetOrientation() const noexcept;
    void SetOrientation(PageOrientationType value) noexcept;

    // -----------------------------------------------------------------------
    // Pagination settings
    // -----------------------------------------------------------------------

    /// Gets or sets the first printed page number.
    std::optional<int> GetFirstPageNumber() const noexcept;
    void SetFirstPageNumber(std::optional<int> value);

    /// Gets or sets the print scaling percentage.
    std::optional<int> GetScale() const noexcept;
    void SetScale(std::optional<int> value);

    /// Gets or sets the number of pages wide to fit when printing.
    std::optional<int> GetFitToPagesWide() const noexcept;
    void SetFitToPagesWide(std::optional<int> value);

    /// Gets or sets the number of pages tall to fit when printing.
    std::optional<int> GetFitToPagesTall() const noexcept;
    void SetFitToPagesTall(std::optional<int> value);

    // -----------------------------------------------------------------------
    // Print area & titles
    // -----------------------------------------------------------------------

    /// Gets or sets the print area reference.
    std::string GetPrintArea() const;
    void SetPrintArea(std::string_view value);

    /// Gets or sets the repeating title rows reference.
    std::string GetPrintTitleRows() const;
    void SetPrintTitleRows(std::string_view value);

    /// Gets or sets the repeating title columns reference.
    std::string GetPrintTitleColumns() const;
    void SetPrintTitleColumns(std::string_view value);

    // -----------------------------------------------------------------------
    // Margins (centimeters)
    // -----------------------------------------------------------------------

    /// Gets or sets the left margin in centimeters.
    double GetLeftMargin() const noexcept;
    void SetLeftMargin(double value);

    /// Gets or sets the right margin in centimeters.
    double GetRightMargin() const noexcept;
    void SetRightMargin(double value);

    /// Gets or sets the top margin in centimeters.
    double GetTopMargin() const noexcept;
    void SetTopMargin(double value);

    /// Gets or sets the bottom margin in centimeters.
    double GetBottomMargin() const noexcept;
    void SetBottomMargin(double value);

    /// Gets or sets the header margin in centimeters.
    double GetHeaderMargin() const noexcept;
    void SetHeaderMargin(double value);

    /// Gets or sets the footer margin in centimeters.
    double GetFooterMargin() const noexcept;
    void SetFooterMargin(double value);

    // -----------------------------------------------------------------------
    // Margins (inches)
    // -----------------------------------------------------------------------

    /// Gets or sets the left margin in inches.
    double GetLeftMarginInch() const noexcept;
    void SetLeftMarginInch(double value);

    /// Gets or sets the right margin in inches.
    double GetRightMarginInch() const noexcept;
    void SetRightMarginInch(double value);

    /// Gets or sets the top margin in inches.
    double GetTopMarginInch() const noexcept;
    void SetTopMarginInch(double value);

    /// Gets or sets the bottom margin in inches.
    double GetBottomMarginInch() const noexcept;
    void SetBottomMarginInch(double value);

    /// Gets or sets the header margin in inches.
    double GetHeaderMarginInch() const noexcept;
    void SetHeaderMarginInch(double value);

    /// Gets or sets the footer margin in inches.
    double GetFooterMarginInch() const noexcept;
    void SetFooterMarginInch(double value);

    // -----------------------------------------------------------------------
    // Header / footer text
    // -----------------------------------------------------------------------

    /// Gets or sets the left header text.
    std::string GetLeftHeader() const;
    void SetLeftHeader(std::string_view value);

    /// Gets or sets the center header text.
    std::string GetCenterHeader() const;
    void SetCenterHeader(std::string_view value);

    /// Gets or sets the right header text.
    std::string GetRightHeader() const;
    void SetRightHeader(std::string_view value);

    /// Gets or sets the left footer text.
    std::string GetLeftFooter() const;
    void SetLeftFooter(std::string_view value);

    /// Gets or sets the center footer text.
    std::string GetCenterFooter() const;
    void SetCenterFooter(std::string_view value);

    /// Gets or sets the right footer text.
    std::string GetRightFooter() const;
    void SetRightFooter(std::string_view value);

    // -----------------------------------------------------------------------
    // Print options
    // -----------------------------------------------------------------------

    /// Gets or sets whether gridlines are printed.
    bool GetPrintGridlines() const noexcept;
    void SetPrintGridlines(bool value) noexcept;

    /// Gets or sets whether row and column headings are printed.
    bool GetPrintHeadings() const noexcept;
    void SetPrintHeadings(bool value) noexcept;

    /// Gets or sets whether content is centered horizontally on the page.
    bool GetCenterHorizontally() const noexcept;
    void SetCenterHorizontally(bool value) noexcept;

    /// Gets or sets whether content is centered vertically on the page.
    bool GetCenterVertically() const noexcept;
    void SetCenterVertically(bool value) noexcept;

    // -----------------------------------------------------------------------
    // Page breaks
    // -----------------------------------------------------------------------

    /// Gets the horizontal page breaks as sorted zero-based row indexes.
    std::vector<int> GetHorizontalPageBreaks() const;

    /// Gets the vertical page breaks as sorted zero-based column indexes.
    std::vector<int> GetVerticalPageBreaks() const;

    /// Adds a horizontal page break at the specified zero-based row index.
    /// @param rowIndex The zero-based row index.
    void AddHorizontalPageBreak(int rowIndex);

    /// Adds a vertical page break at the specified zero-based column index.
    /// @param columnIndex The zero-based column index.
    void AddVerticalPageBreak(int columnIndex);

    /// Removes all horizontal page breaks.
    void ClearHorizontalPageBreaks();

    /// Removes all vertical page breaks.
    void ClearVerticalPageBreaks();

private:
    static constexpr double kCentimetersPerInch = 2.54;

    static double ToCentimeters(double inches) noexcept;
    static double ToInches(double centimeters) noexcept;
    static double ValidateMargin(double value, const char* propertyName);
    static std::string NormalizeText(std::string_view value);
    static std::vector<int> GetOrderedBreaks(const std::vector<int>& breaks);
    static void AddDistinct(std::vector<int>& collection, int value);

    Core::PageSetupModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
