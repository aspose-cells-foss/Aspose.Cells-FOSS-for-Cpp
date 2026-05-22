#include "aspose/cells_foss/Style.h"

#include "aspose/cells_foss/NumberFormat.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/core/AlignmentValue.h"
#include "aspose/cells_foss/core/BorderStyle.h"
#include "aspose/cells_foss/core/BorderSideValue.h"
#include "aspose/cells_foss/core/BordersValue.h"
#include "aspose/cells_foss/core/ColorValue.h"
#include "aspose/cells_foss/core/FillPatternKind.h"
#include "aspose/cells_foss/core/FontValue.h"
#include "aspose/cells_foss/core/HorizontalAlignment.h"
#include "aspose/cells_foss/core/NumberFormatValue.h"
#include "aspose/cells_foss/core/ProtectionValue.h"
#include "aspose/cells_foss/core/VerticalAlignment.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

namespace {

bool IsNullOrWhiteSpace(std::string_view s) noexcept
{
    return std::all_of(s.begin(), s.end(),
                       [](unsigned char ch) { return std::isspace(ch) != 0; });
}

std::string Trim(std::string_view s)
{
    auto start = s.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string_view::npos) {
        return {};
    }
    auto end = s.find_last_not_of(" \t\n\r\f\v");
    return std::string(s.substr(start, end - start + 1));
}

}  // namespace

Style::Style() = default;

std::string Style::GetNumberFormat() const
{
    return NumberFormat::ResolveFormatCode(_number, _custom);
}

void Style::SetNumberFormat(const std::string& value)
{
    auto builtInId = NumberFormat::GetBuiltInFormatId(value);
    if (builtInId.has_value())
    {
        _number = builtInId.value();
        _custom.clear();
        return;
    }

    _number = 0;
    if (IsNullOrWhiteSpace(value))
    {
        _custom.clear();
    }
    else
    {
        _custom = Trim(value);
    }
}

void Style::SetIndentLevel(int value)
{
    if (value < 0 || value > 250)
    {
        throw CellsException("IndentLevel must be between 0 and 250.");
    }
    _indentLevel = value;
}

void Style::SetTextRotation(int value)
{
    if (value != 255 && (value < 0 || value > 180))
    {
        throw CellsException("TextRotation must be between 0 and 180, or 255 for vertical text.");
    }
    _textRotation = value;
}

void Style::SetReadingOrder(int value)
{
    if (value < 0 || value > 2)
    {
        throw CellsException("ReadingOrder must be 0, 1, or 2.");
    }
    _readingOrder = value;
}

Style Style::Clone() const
{
    Style copy;
    copy._font = _font.Clone();
    copy._borders = _borders.Clone();
    copy._pattern = _pattern;
    copy._foregroundColor = _foregroundColor;
    copy._backgroundColor = _backgroundColor;
    copy._number = _number;
    copy._custom = _custom;
    copy._horizontalAlignment = _horizontalAlignment;
    copy._verticalAlignment = _verticalAlignment;
    copy._wrapText = _wrapText;
    copy._indentLevel = _indentLevel;
    copy._textRotation = _textRotation;
    copy._shrinkToFit = _shrinkToFit;
    copy._readingOrder = _readingOrder;
    copy._relativeIndent = _relativeIndent;
    copy._isLocked = _isLocked;
    copy._isHidden = _isHidden;
    return copy;
}

Core::StyleValue Style::ToCore() const
{
    Core::StyleValue sv;

    Core::FontValue fv;
    fv.SetName(_font.GetName());
    fv.SetSize(_font.GetSize());
    fv.SetBold(_font.GetBold());
    fv.SetItalic(_font.GetItalic());
    fv.SetUnderline(_font.GetUnderline());
    fv.SetStrikeThrough(_font.GetStrikeThrough());
    fv.SetColor(_font.GetColor().ToCore());
    sv.SetFont(fv);

    sv.SetPattern(static_cast<Core::FillPatternKind>(_pattern));
    sv.SetForegroundColor(_foregroundColor.ToCore());
    sv.SetBackgroundColor(_backgroundColor.ToCore());

    Core::BordersValue bv;
    bv.SetLeft(ToCoreBorder(_borders.GetLeft()));
    bv.SetRight(ToCoreBorder(_borders.GetRight()));
    bv.SetTop(ToCoreBorder(_borders.GetTop()));
    bv.SetBottom(ToCoreBorder(_borders.GetBottom()));
    bv.SetDiagonal(ToCoreBorder(_borders.GetDiagonal()));
    bv.SetDiagonalUp(_borders.GetDiagonalUp());
    bv.SetDiagonalDown(_borders.GetDiagonalDown());
    sv.SetBorders(bv);

    Core::AlignmentValue av;
    av.SetHorizontal(static_cast<Core::HorizontalAlignment>(_horizontalAlignment));
    av.SetVertical(static_cast<Core::VerticalAlignment>(_verticalAlignment));
    av.SetWrapText(_wrapText);
    av.SetIndentLevel(_indentLevel);
    av.SetTextRotation(_textRotation);
    av.SetShrinkToFit(_shrinkToFit);
    av.SetReadingOrder(_readingOrder);
    av.SetRelativeIndent(_relativeIndent);
    sv.SetAlignment(av);

    Core::ProtectionValue pv;
    pv.SetIsLocked(_isLocked);
    pv.SetIsHidden(_isHidden);
    sv.SetProtection(pv);

    Core::NumberFormatValue nfv;
    nfv.SetNumber(_number);
    nfv.SetCustom(_custom);
    sv.SetNumberFormat(nfv);

    return sv;
}

Style Style::FromCore(const Core::StyleValue& value)
{
    Style s;

    Font f;
    f.SetName(value.GetFont().GetName());
    f.SetSize(value.GetFont().GetSize());
    f.SetBold(value.GetFont().GetBold());
    f.SetItalic(value.GetFont().GetItalic());
    f.SetUnderline(value.GetFont().GetUnderline());
    f.SetStrikeThrough(value.GetFont().GetStrikeThrough());
    f.SetColor(Color::FromCore(value.GetFont().GetColor()));
    s._font = f;

    s._pattern = static_cast<FillPattern>(value.GetPattern());
    s._foregroundColor = Color::FromCore(value.GetForegroundColor());
    s._backgroundColor = Color::FromCore(value.GetBackgroundColor());

    Borders b;
    b.SetLeft(FromCoreBorder(value.GetBorders().GetLeft()));
    b.SetRight(FromCoreBorder(value.GetBorders().GetRight()));
    b.SetTop(FromCoreBorder(value.GetBorders().GetTop()));
    b.SetBottom(FromCoreBorder(value.GetBorders().GetBottom()));
    b.SetDiagonal(FromCoreBorder(value.GetBorders().GetDiagonal()));
    b.SetDiagonalUp(value.GetBorders().GetDiagonalUp());
    b.SetDiagonalDown(value.GetBorders().GetDiagonalDown());
    s._borders = b;

    s._horizontalAlignment = static_cast<HorizontalAlignmentType>(value.GetAlignment().GetHorizontal());
    s._verticalAlignment = static_cast<VerticalAlignmentType>(value.GetAlignment().GetVertical());
    s._wrapText = value.GetAlignment().GetWrapText();
    s._indentLevel = value.GetAlignment().GetIndentLevel();
    s._textRotation = value.GetAlignment().GetTextRotation();
    s._shrinkToFit = value.GetAlignment().GetShrinkToFit();
    s._readingOrder = value.GetAlignment().GetReadingOrder();
    s._relativeIndent = value.GetAlignment().GetRelativeIndent();

    s._isLocked = value.GetProtection().GetIsLocked();
    s._isHidden = value.GetProtection().GetIsHidden();

    s._number = value.GetNumberFormat().GetNumber();
    s._custom = value.GetNumberFormat().GetCustom();

    return s;
}

Core::BorderSideValue Style::ToCoreBorder(const Border& border)
{
    Core::BorderSideValue bsv;
    bsv.SetStyle(static_cast<Core::BorderStyle>(border.GetLineStyle()));
    bsv.SetColor(border.GetColor().ToCore());
    return bsv;
}

Border Style::FromCoreBorder(const Core::BorderSideValue& value)
{
    Border b;
    b.SetLineStyle(static_cast<BorderStyleType>(value.GetStyle()));
    b.SetColor(Color::FromCore(value.GetColor()));
    return b;
}

}}  // namespace Aspose::Cells_FOSS
