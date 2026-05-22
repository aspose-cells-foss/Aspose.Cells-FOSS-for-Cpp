#include "aspose/cells_foss/PageSetup.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/core/PageSetupModel.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

PageSetup::PageSetup(Core::PageSetupModel& model)
    : _model(model)
{
}

// ---------------------------------------------------------------------------
// PaperSize
// ---------------------------------------------------------------------------

PaperSizeType PageSetup::GetPaperSize() const noexcept
{
    return static_cast<PaperSizeType>(_model.GetPaperSize());
}

void PageSetup::SetPaperSize(PaperSizeType value) noexcept
{
    _model.SetPaperSize(static_cast<int>(value));
}

// ---------------------------------------------------------------------------
// Orientation
// ---------------------------------------------------------------------------

PageOrientationType PageSetup::GetOrientation() const noexcept
{
    switch (_model.GetOrientation()) {
    case Core::PageOrientation::Portrait:
        return PageOrientationType::Portrait;
    case Core::PageOrientation::Landscape:
        return PageOrientationType::Landscape;
    default:
        return PageOrientationType::Default;
    }
}

void PageSetup::SetOrientation(PageOrientationType value) noexcept
{
    switch (value) {
    case PageOrientationType::Portrait:
        _model.SetOrientation(Core::PageOrientation::Portrait);
        break;
    case PageOrientationType::Landscape:
        _model.SetOrientation(Core::PageOrientation::Landscape);
        break;
    default:
        _model.SetOrientation(Core::PageOrientation::Default);
        break;
    }
}

// ---------------------------------------------------------------------------
// FirstPageNumber
// ---------------------------------------------------------------------------

std::optional<int> PageSetup::GetFirstPageNumber() const noexcept
{
    return _model.GetFirstPageNumber();
}

void PageSetup::SetFirstPageNumber(std::optional<int> value)
{
    if (value.has_value() && value.value() <= 0) {
        throw CellsException("FirstPageNumber must be positive.");
    }
    _model.SetFirstPageNumber(value);
}

// ---------------------------------------------------------------------------
// Scale
// ---------------------------------------------------------------------------

std::optional<int> PageSetup::GetScale() const noexcept
{
    return _model.GetScale();
}

void PageSetup::SetScale(std::optional<int> value)
{
    if (value.has_value() && (value.value() < 10 || value.value() > 400)) {
        throw CellsException("Scale must be between 10 and 400.");
    }
    _model.SetScale(value);
}

// ---------------------------------------------------------------------------
// FitToPagesWide
// ---------------------------------------------------------------------------

std::optional<int> PageSetup::GetFitToPagesWide() const noexcept
{
    return _model.GetFitToWidth();
}

void PageSetup::SetFitToPagesWide(std::optional<int> value)
{
    if (value.has_value() && value.value() < 0) {
        throw CellsException("FitToPagesWide must be zero or greater.");
    }
    _model.SetFitToWidth(value);
}

// ---------------------------------------------------------------------------
// FitToPagesTall
// ---------------------------------------------------------------------------

std::optional<int> PageSetup::GetFitToPagesTall() const noexcept
{
    return _model.GetFitToHeight();
}

void PageSetup::SetFitToPagesTall(std::optional<int> value)
{
    if (value.has_value() && value.value() < 0) {
        throw CellsException("FitToPagesTall must be zero or greater.");
    }
    _model.SetFitToHeight(value);
}

// ---------------------------------------------------------------------------
// PrintArea
// ---------------------------------------------------------------------------

std::string PageSetup::GetPrintArea() const
{
    const auto& area = _model.GetPrintArea();
    return area.empty() ? std::string() : area;
}

void PageSetup::SetPrintArea(std::string_view value)
{
    _model.SetPrintArea(NormalizeText(value));
}

// ---------------------------------------------------------------------------
// PrintTitleRows
// ---------------------------------------------------------------------------

std::string PageSetup::GetPrintTitleRows() const
{
    const auto& rows = _model.GetPrintTitleRows();
    return rows.empty() ? std::string() : rows;
}

void PageSetup::SetPrintTitleRows(std::string_view value)
{
    _model.SetPrintTitleRows(NormalizeText(value));
}

// ---------------------------------------------------------------------------
// PrintTitleColumns
// ---------------------------------------------------------------------------

std::string PageSetup::GetPrintTitleColumns() const
{
    const auto& cols = _model.GetPrintTitleColumns();
    return cols.empty() ? std::string() : cols;
}

void PageSetup::SetPrintTitleColumns(std::string_view value)
{
    _model.SetPrintTitleColumns(NormalizeText(value));
}

// ---------------------------------------------------------------------------
// LeftMargin (centimeters)
// ---------------------------------------------------------------------------

double PageSetup::GetLeftMargin() const noexcept
{
    return ToCentimeters(_model.GetMargins().GetLeft());
}

void PageSetup::SetLeftMargin(double value)
{
    _model.GetMargins().SetLeft(ValidateMargin(ToInches(value), "LeftMargin"));
}

// ---------------------------------------------------------------------------
// RightMargin (centimeters)
// ---------------------------------------------------------------------------

double PageSetup::GetRightMargin() const noexcept
{
    return ToCentimeters(_model.GetMargins().GetRight());
}

void PageSetup::SetRightMargin(double value)
{
    _model.GetMargins().SetRight(ValidateMargin(ToInches(value), "RightMargin"));
}

// ---------------------------------------------------------------------------
// TopMargin (centimeters)
// ---------------------------------------------------------------------------

double PageSetup::GetTopMargin() const noexcept
{
    return ToCentimeters(_model.GetMargins().GetTop());
}

void PageSetup::SetTopMargin(double value)
{
    _model.GetMargins().SetTop(ValidateMargin(ToInches(value), "TopMargin"));
}

// ---------------------------------------------------------------------------
// BottomMargin (centimeters)
// ---------------------------------------------------------------------------

double PageSetup::GetBottomMargin() const noexcept
{
    return ToCentimeters(_model.GetMargins().GetBottom());
}

void PageSetup::SetBottomMargin(double value)
{
    _model.GetMargins().SetBottom(ValidateMargin(ToInches(value), "BottomMargin"));
}

// ---------------------------------------------------------------------------
// HeaderMargin (centimeters)
// ---------------------------------------------------------------------------

double PageSetup::GetHeaderMargin() const noexcept
{
    return ToCentimeters(_model.GetMargins().GetHeader());
}

void PageSetup::SetHeaderMargin(double value)
{
    _model.GetMargins().SetHeader(ValidateMargin(ToInches(value), "HeaderMargin"));
}

// ---------------------------------------------------------------------------
// FooterMargin (centimeters)
// ---------------------------------------------------------------------------

double PageSetup::GetFooterMargin() const noexcept
{
    return ToCentimeters(_model.GetMargins().GetFooter());
}

void PageSetup::SetFooterMargin(double value)
{
    _model.GetMargins().SetFooter(ValidateMargin(ToInches(value), "FooterMargin"));
}

// ---------------------------------------------------------------------------
// LeftMarginInch
// ---------------------------------------------------------------------------

double PageSetup::GetLeftMarginInch() const noexcept
{
    return _model.GetMargins().GetLeft();
}

void PageSetup::SetLeftMarginInch(double value)
{
    _model.GetMargins().SetLeft(ValidateMargin(value, "LeftMarginInch"));
}

// ---------------------------------------------------------------------------
// RightMarginInch
// ---------------------------------------------------------------------------

double PageSetup::GetRightMarginInch() const noexcept
{
    return _model.GetMargins().GetRight();
}

void PageSetup::SetRightMarginInch(double value)
{
    _model.GetMargins().SetRight(ValidateMargin(value, "RightMarginInch"));
}

// ---------------------------------------------------------------------------
// TopMarginInch
// ---------------------------------------------------------------------------

double PageSetup::GetTopMarginInch() const noexcept
{
    return _model.GetMargins().GetTop();
}

void PageSetup::SetTopMarginInch(double value)
{
    _model.GetMargins().SetTop(ValidateMargin(value, "TopMarginInch"));
}

// ---------------------------------------------------------------------------
// BottomMarginInch
// ---------------------------------------------------------------------------

double PageSetup::GetBottomMarginInch() const noexcept
{
    return _model.GetMargins().GetBottom();
}

void PageSetup::SetBottomMarginInch(double value)
{
    _model.GetMargins().SetBottom(ValidateMargin(value, "BottomMarginInch"));
}

// ---------------------------------------------------------------------------
// HeaderMarginInch
// ---------------------------------------------------------------------------

double PageSetup::GetHeaderMarginInch() const noexcept
{
    return _model.GetMargins().GetHeader();
}

void PageSetup::SetHeaderMarginInch(double value)
{
    _model.GetMargins().SetHeader(ValidateMargin(value, "HeaderMarginInch"));
}

// ---------------------------------------------------------------------------
// FooterMarginInch
// ---------------------------------------------------------------------------

double PageSetup::GetFooterMarginInch() const noexcept
{
    return _model.GetMargins().GetFooter();
}

void PageSetup::SetFooterMarginInch(double value)
{
    _model.GetMargins().SetFooter(ValidateMargin(value, "FooterMarginInch"));
}

// ---------------------------------------------------------------------------
// LeftHeader
// ---------------------------------------------------------------------------

std::string PageSetup::GetLeftHeader() const
{
    const auto& h = _model.GetHeaderFooter().GetLeftHeader();
    return h.empty() ? std::string() : h;
}

void PageSetup::SetLeftHeader(std::string_view value)
{
    _model.GetHeaderFooter().SetLeftHeader(NormalizeText(value));
}

// ---------------------------------------------------------------------------
// CenterHeader
// ---------------------------------------------------------------------------

std::string PageSetup::GetCenterHeader() const
{
    const auto& h = _model.GetHeaderFooter().GetCenterHeader();
    return h.empty() ? std::string() : h;
}

void PageSetup::SetCenterHeader(std::string_view value)
{
    _model.GetHeaderFooter().SetCenterHeader(NormalizeText(value));
}

// ---------------------------------------------------------------------------
// RightHeader
// ---------------------------------------------------------------------------

std::string PageSetup::GetRightHeader() const
{
    const auto& h = _model.GetHeaderFooter().GetRightHeader();
    return h.empty() ? std::string() : h;
}

void PageSetup::SetRightHeader(std::string_view value)
{
    _model.GetHeaderFooter().SetRightHeader(NormalizeText(value));
}

// ---------------------------------------------------------------------------
// LeftFooter
// ---------------------------------------------------------------------------

std::string PageSetup::GetLeftFooter() const
{
    const auto& f = _model.GetHeaderFooter().GetLeftFooter();
    return f.empty() ? std::string() : f;
}

void PageSetup::SetLeftFooter(std::string_view value)
{
    _model.GetHeaderFooter().SetLeftFooter(NormalizeText(value));
}

// ---------------------------------------------------------------------------
// CenterFooter
// ---------------------------------------------------------------------------

std::string PageSetup::GetCenterFooter() const
{
    const auto& f = _model.GetHeaderFooter().GetCenterFooter();
    return f.empty() ? std::string() : f;
}

void PageSetup::SetCenterFooter(std::string_view value)
{
    _model.GetHeaderFooter().SetCenterFooter(NormalizeText(value));
}

// ---------------------------------------------------------------------------
// RightFooter
// ---------------------------------------------------------------------------

std::string PageSetup::GetRightFooter() const
{
    const auto& f = _model.GetHeaderFooter().GetRightFooter();
    return f.empty() ? std::string() : f;
}

void PageSetup::SetRightFooter(std::string_view value)
{
    _model.GetHeaderFooter().SetRightFooter(NormalizeText(value));
}

// ---------------------------------------------------------------------------
// PrintGridlines
// ---------------------------------------------------------------------------

bool PageSetup::GetPrintGridlines() const noexcept
{
    return _model.GetPrintOptions().GetGridLines();
}

void PageSetup::SetPrintGridlines(bool value) noexcept
{
    _model.GetPrintOptions().SetGridLines(value);
}

// ---------------------------------------------------------------------------
// PrintHeadings
// ---------------------------------------------------------------------------

bool PageSetup::GetPrintHeadings() const noexcept
{
    return _model.GetPrintOptions().GetHeadings();
}

void PageSetup::SetPrintHeadings(bool value) noexcept
{
    _model.GetPrintOptions().SetHeadings(value);
}

// ---------------------------------------------------------------------------
// CenterHorizontally
// ---------------------------------------------------------------------------

bool PageSetup::GetCenterHorizontally() const noexcept
{
    return _model.GetPrintOptions().GetHorizontalCentered();
}

void PageSetup::SetCenterHorizontally(bool value) noexcept
{
    _model.GetPrintOptions().SetHorizontalCentered(value);
}

// ---------------------------------------------------------------------------
// CenterVertically
// ---------------------------------------------------------------------------

bool PageSetup::GetCenterVertically() const noexcept
{
    return _model.GetPrintOptions().GetVerticalCentered();
}

void PageSetup::SetCenterVertically(bool value) noexcept
{
    _model.GetPrintOptions().SetVerticalCentered(value);
}

// ---------------------------------------------------------------------------
// HorizontalPageBreaks
// ---------------------------------------------------------------------------

std::vector<int> PageSetup::GetHorizontalPageBreaks() const
{
    return GetOrderedBreaks(_model.GetHorizontalPageBreaks());
}

// ---------------------------------------------------------------------------
// VerticalPageBreaks
// ---------------------------------------------------------------------------

std::vector<int> PageSetup::GetVerticalPageBreaks() const
{
    return GetOrderedBreaks(_model.GetVerticalPageBreaks());
}

// ---------------------------------------------------------------------------
// AddHorizontalPageBreak
// ---------------------------------------------------------------------------

void PageSetup::AddHorizontalPageBreak(int rowIndex)
{
    if (rowIndex < 0) {
        throw CellsException("Horizontal page break row index must be non-negative.");
    }
    AddDistinct(_model.GetHorizontalPageBreaks(), rowIndex);
}

// ---------------------------------------------------------------------------
// AddVerticalPageBreak
// ---------------------------------------------------------------------------

void PageSetup::AddVerticalPageBreak(int columnIndex)
{
    if (columnIndex < 0) {
        throw CellsException("Vertical page break column index must be non-negative.");
    }
    AddDistinct(_model.GetVerticalPageBreaks(), columnIndex);
}

// ---------------------------------------------------------------------------
// ClearHorizontalPageBreaks
// ---------------------------------------------------------------------------

void PageSetup::ClearHorizontalPageBreaks()
{
    _model.GetHorizontalPageBreaks().clear();
}

// ---------------------------------------------------------------------------
// ClearVerticalPageBreaks
// ---------------------------------------------------------------------------

void PageSetup::ClearVerticalPageBreaks()
{
    _model.GetVerticalPageBreaks().clear();
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

double PageSetup::ToCentimeters(double inches) noexcept
{
    return inches * kCentimetersPerInch;
}

double PageSetup::ToInches(double centimeters) noexcept
{
    return centimeters / kCentimetersPerInch;
}

double PageSetup::ValidateMargin(double value, const char* propertyName)
{
    if (value < 0.0) {
        throw CellsException(std::string(propertyName) + " must be zero or greater.");
    }
    return value;
}

std::string PageSetup::NormalizeText(std::string_view value)
{
    // Find first non-whitespace character.
    std::string_view::size_type start = 0;
    while (start < value.size() &&
           std::isspace(static_cast<unsigned char>(value[start])) != 0) {
        ++start;
    }

    // If all whitespace or empty, return empty (C# model stores null -> C++ empty).
    if (start == value.size()) {
        return std::string();
    }

    // Find last non-whitespace character.
    std::string_view::size_type end = value.size();
    while (end > start &&
           std::isspace(static_cast<unsigned char>(value[end - 1])) != 0) {
        --end;
    }

    return std::string(value.substr(start, end - start));
}

std::vector<int> PageSetup::GetOrderedBreaks(const std::vector<int>& breaks)
{
    std::vector<int> ordered(breaks);
    std::sort(ordered.begin(), ordered.end());
    return ordered;
}

void PageSetup::AddDistinct(std::vector<int>& collection, int value)
{
    if (std::find(collection.begin(), collection.end(), value) == collection.end()) {
        collection.push_back(value);
    }
}

}}  // namespace Aspose::Cells_FOSS
