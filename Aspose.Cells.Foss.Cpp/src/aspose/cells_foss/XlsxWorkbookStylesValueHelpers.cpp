#include "aspose/cells_foss/XlsxWorkbookStylesValueHelpers.h"

#include <cstdio>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

namespace {

std::string ToLowerAscii(std::string_view sv)
{
    std::string result(sv);
    for (char& c : result)
    {
        if (c >= 'A' && c <= 'Z')
        {
            c = static_cast<char>(c + ('a' - 'A'));
        }
    }
    return result;
}

}  // anonymous namespace

Core::BorderStyle XlsxWorkbookStylesValueHelpers::ParseBorderStyle(std::string_view value)
{
    const std::string lower = ToLowerAscii(value);

    if (lower == "thin")            return Core::BorderStyle::Thin;
    if (lower == "medium")          return Core::BorderStyle::Medium;
    if (lower == "thick")           return Core::BorderStyle::Thick;
    if (lower == "dotted")          return Core::BorderStyle::Dotted;
    if (lower == "dashed")          return Core::BorderStyle::Dashed;
    if (lower == "double")          return Core::BorderStyle::Double;
    if (lower == "hair")            return Core::BorderStyle::Hair;
    if (lower == "mediumdashed")    return Core::BorderStyle::MediumDashed;
    if (lower == "dashdot")         return Core::BorderStyle::DashDot;
    if (lower == "mediumdashdot")   return Core::BorderStyle::MediumDashDot;
    if (lower == "dashdotdot")      return Core::BorderStyle::DashDotDot;
    if (lower == "mediumdashdotdot") return Core::BorderStyle::MediumDashDotDot;
    if (lower == "slantdashdot")    return Core::BorderStyle::SlantedDashDot;

    return Core::BorderStyle::None;
}

std::string XlsxWorkbookStylesValueHelpers::GetBorderStyleName(Core::BorderStyle value)
{
    switch (value)
    {
    case Core::BorderStyle::Thin:             return "thin";
    case Core::BorderStyle::Medium:           return "medium";
    case Core::BorderStyle::Thick:            return "thick";
    case Core::BorderStyle::Dotted:           return "dotted";
    case Core::BorderStyle::Dashed:           return "dashed";
    case Core::BorderStyle::Double:           return "double";
    case Core::BorderStyle::Hair:             return "hair";
    case Core::BorderStyle::MediumDashed:     return "mediumDashed";
    case Core::BorderStyle::DashDot:          return "dashDot";
    case Core::BorderStyle::MediumDashDot:    return "mediumDashDot";
    case Core::BorderStyle::DashDotDot:       return "dashDotDot";
    case Core::BorderStyle::MediumDashDotDot: return "mediumDashDotDot";
    case Core::BorderStyle::SlantedDashDot:   return "slantDashDot";
    default:                                  return std::string();
    }
}

std::string XlsxWorkbookStylesValueHelpers::GetHorizontalAlignmentName(Core::HorizontalAlignment value)
{
    switch (value)
    {
    case Core::HorizontalAlignment::Left:              return "left";
    case Core::HorizontalAlignment::Center:            return "center";
    case Core::HorizontalAlignment::Right:             return "right";
    case Core::HorizontalAlignment::Fill:              return "fill";
    case Core::HorizontalAlignment::Justify:           return "justify";
    case Core::HorizontalAlignment::CenterContinuous:  return "centerContinuous";
    case Core::HorizontalAlignment::Distributed:       return "distributed";
    default:                                           return std::string();
    }
}

std::string XlsxWorkbookStylesValueHelpers::GetVerticalAlignmentName(Core::VerticalAlignment value)
{
    switch (value)
    {
    case Core::VerticalAlignment::Center:      return "center";
    case Core::VerticalAlignment::Top:         return "top";
    case Core::VerticalAlignment::Justify:     return "justify";
    case Core::VerticalAlignment::Distributed: return "distributed";
    default:                                   return std::string();
    }
}

std::string XlsxWorkbookStylesValueHelpers::ToArgbHex(const Core::ColorValue& color)
{
    char buf[9];
    std::snprintf(buf, sizeof(buf), "%02X%02X%02X%02X",
        static_cast<unsigned>(color.GetA()),
        static_cast<unsigned>(color.GetR()),
        static_cast<unsigned>(color.GetG()),
        static_cast<unsigned>(color.GetB()));
    return std::string(buf, 8);
}

bool XlsxWorkbookStylesValueHelpers::IsEmptyColor(const Core::ColorValue& color)
{
    return color.GetA() == 0
        && color.GetR() == 0
        && color.GetG() == 0
        && color.GetB() == 0;
}

bool XlsxWorkbookStylesValueHelpers::FontEquals(const Core::FontValue& left, const Core::FontValue& right)
{
    return left.GetName() == right.GetName()
        && left.GetSize() == right.GetSize()
        && left.GetBold() == right.GetBold()
        && left.GetItalic() == right.GetItalic()
        && left.GetUnderline() == right.GetUnderline()
        && left.GetStrikeThrough() == right.GetStrikeThrough()
        && left.GetColor() == right.GetColor();
}

bool XlsxWorkbookStylesValueHelpers::BordersEqual(const Core::BordersValue& left, const Core::BordersValue& right)
{
    return BorderSideEquals(left.GetLeft(), right.GetLeft())
        && BorderSideEquals(left.GetRight(), right.GetRight())
        && BorderSideEquals(left.GetTop(), right.GetTop())
        && BorderSideEquals(left.GetBottom(), right.GetBottom())
        && BorderSideEquals(left.GetDiagonal(), right.GetDiagonal())
        && left.GetDiagonalUp() == right.GetDiagonalUp()
        && left.GetDiagonalDown() == right.GetDiagonalDown();
}

bool XlsxWorkbookStylesValueHelpers::BorderSideEquals(const Core::BorderSideValue& left, const Core::BorderSideValue& right)
{
    return left.GetStyle() == right.GetStyle()
        && left.GetColor() == right.GetColor();
}

}}  // namespace Aspose::Cells_FOSS
