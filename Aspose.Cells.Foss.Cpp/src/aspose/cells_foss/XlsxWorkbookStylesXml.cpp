#include "aspose/cells_foss/XlsxWorkbookStylesXml.h"
#include "aspose/cells_foss/StyleValueSanitizer.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"
#include "aspose/cells_foss/XlsxWorkbookStylesValueHelpers.h"

#include <charconv>
#include <cstdio>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

namespace {

const XNamespace& MainNs = XlsxWorkbookSerializerCommon::MainNs;

// ---------------------------------------------------------------------------
// XML element construction helpers
// ---------------------------------------------------------------------------

XmlElement MakeElem(const std::string& qualifiedName)
{
    return XmlElement::MakeElement(qualifiedName);
}

XmlElement MakeElem(const std::string& qualifiedName,
                    const std::string& attrLocalName,
                    const std::string& attrValue)
{
    return XmlElement::MakeElement(
        qualifiedName,
        {{attrLocalName, attrValue}},
        {});
}

XmlElement MakeElem(const std::string& qualifiedName,
                    const std::string& attr1Local, const std::string& attr1Value,
                    const std::vector<XmlElement>& children)
{
    auto elem = XmlElement::MakeElement(qualifiedName);
    elem.SetAttribute(attr1Local, attr1Value);
    for (const auto& child : children) {
        elem.AddChild(child);
    }
    return elem;
}

XmlElement MakeElemWithAttrs(const std::string& qualifiedName,
                             std::vector<std::pair<std::string, std::string>> attrs)
{
    return XmlElement::MakeElement(qualifiedName, std::move(attrs), {});
}

void AddAttr(XmlElement& elem, const std::string& localName, const std::string& value)
{
    elem.SetAttribute(localName, value);
}

void AddAttrInt(XmlElement& elem, const std::string& localName, int value)
{
    elem.SetAttribute(localName, std::to_string(value));
}

// ---------------------------------------------------------------------------
// ToLower helper
// ---------------------------------------------------------------------------

std::string ToLowerAscii(std::string_view sv)
{
    std::string result(sv);
    for (char& c : result) {
        if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>(c + ('a' - 'A'));
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
// Double formatting matching C# "0.####" InvariantCulture pattern
// ---------------------------------------------------------------------------

std::string FormatDoubleInvariant(double value)
{
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.4g", value);
    return std::string(buf);
}

// ---------------------------------------------------------------------------
// Trim edge whitespace only (matching C# string.Trim() semantics exactly)
// ---------------------------------------------------------------------------

std::string_view TrimEdge(std::string_view sv)
{
    while (!sv.empty() && static_cast<unsigned char>(sv.front()) <= 32) {
        sv.remove_prefix(1);
    }
    while (!sv.empty() && static_cast<unsigned char>(sv.back()) <= 32) {
        sv.remove_suffix(1);
    }
    return sv;
}

// ---------------------------------------------------------------------------
// Check if a string is null/empty/whitespace (matching C# string.IsNullOrWhiteSpace)
// ---------------------------------------------------------------------------

bool IsNullOrWhiteSpace(std::string_view sv)
{
    for (char c : sv) {
        if (static_cast<unsigned char>(c) > 32) {
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// Parse a two-character hex string to a byte value
// ---------------------------------------------------------------------------

bool ParseHexByte(std::string_view hex, std::uint8_t& out)
{
    if (hex.size() < 2) {
        return false;
    }
    unsigned int val = 0;
    auto [ptr, ec] = std::from_chars(hex.data(), hex.data() + 2, val, 16);
    if (ec != std::errc{} || ptr != hex.data() + 2) {
        return false;
    }
    out = static_cast<std::uint8_t>(val);
    return true;
}

}  // anonymous namespace

// ---------------------------------------------------------------------------
// BuildStylesheetDocument
// ---------------------------------------------------------------------------

XmlDocument XlsxWorkbookStylesXml::BuildStylesheetDocument(
    const std::vector<Core::FontValue>& fonts,
    const std::vector<FillValue>& fills,
    const std::vector<Core::BordersValue>& borders,
    const CellFormatValue& normalCellFormat,
    const std::vector<CellFormatValue>& cellFormats,
    const std::vector<std::pair<int, std::string>>& customNumberFormats,
    const std::vector<Core::StyleValue>& differentialFormats)
{
    auto stylesheet = MakeElem(MainNs + "styleSheet");

    if (!customNumberFormats.empty()) {
        std::vector<XmlElement> numFmtElements;
        numFmtElements.reserve(customNumberFormats.size());
        for (const auto& pair : customNumberFormats) {
            auto nf = MakeElem(MainNs + "numFmt");
            AddAttrInt(nf, "numFmtId", pair.first);
            AddAttr(nf, "formatCode", pair.second);
            numFmtElements.push_back(std::move(nf));
        }
        auto numFmts = MakeElem(MainNs + "numFmts",
                                "count", std::to_string(customNumberFormats.size()),
                                numFmtElements);
        stylesheet.AddChild(std::move(numFmts));
    }

    {
        auto el = MakeElem(MainNs + "fonts",
                           "count", std::to_string(fonts.size()),
                           BuildFontElements(fonts));
        stylesheet.AddChild(std::move(el));
    }
    {
        auto el = MakeElem(MainNs + "fills",
                           "count", std::to_string(fills.size()),
                           BuildFillElements(fills));
        stylesheet.AddChild(std::move(el));
    }
    {
        auto el = MakeElem(MainNs + "borders",
                           "count", std::to_string(borders.size()),
                           BuildBorderElements(borders));
        stylesheet.AddChild(std::move(el));
    }
    {
        std::vector<XmlElement> csxf{BuildCellStyleFormatElement(normalCellFormat)};
        auto el = MakeElem(MainNs + "cellStyleXfs", "count", "1", csxf);
        stylesheet.AddChild(std::move(el));
    }
    {
        auto el = MakeElem(MainNs + "cellXfs",
                           "count", std::to_string(cellFormats.size()),
                           BuildCellFormatElements(cellFormats));
        stylesheet.AddChild(std::move(el));
    }
    {
        auto cellStyle = MakeElemWithAttrs(MainNs + "cellStyle",
                                           {{"name", "Normal"}, {"xfId", "0"}, {"builtinId", "0"}});
        std::vector<XmlElement> csChild{std::move(cellStyle)};
        auto cellStyles = MakeElem(MainNs + "cellStyles", "count", "1", csChild);
        stylesheet.AddChild(std::move(cellStyles));
    }

    if (!differentialFormats.empty()) {
        auto el = MakeElem(MainNs + "dxfs",
                           "count", std::to_string(differentialFormats.size()),
                           BuildDifferentialFormatElements(differentialFormats));
        stylesheet.AddChild(std::move(el));
    }

    return XmlDocument::Build(stylesheet);
}

// ---------------------------------------------------------------------------
// ReadFontValues
// ---------------------------------------------------------------------------

std::vector<Core::FontValue> XlsxWorkbookStylesXml::ReadFontValues(const XmlElement& root)
{
    std::vector<Core::FontValue> fonts;
    const auto fontsElem = root.GetElement(MainNs + "fonts");
    const auto fontElems = fontsElem.IsNull()
        ? std::vector<XmlElement>()
        : fontsElem.GetElements(MainNs + "font");
    fonts.reserve(fontElems.size());
    for (const auto& font : fontElems) {
        fonts.push_back(ReadFontValue(font));
    }
    return fonts;
}

// ---------------------------------------------------------------------------
// ReadFillValues
// ---------------------------------------------------------------------------

std::vector<FillValue> XlsxWorkbookStylesXml::ReadFillValues(const XmlElement& root)
{
    std::vector<FillValue> fills;
    const auto fillsElem = root.GetElement(MainNs + "fills");
    const auto fillElems = fillsElem.IsNull()
        ? std::vector<XmlElement>()
        : fillsElem.GetElements(MainNs + "fill");
    fills.reserve(fillElems.size());
    for (const auto& fill : fillElems) {
        fills.push_back(ReadFillValue(fill));
    }
    return fills;
}

// ---------------------------------------------------------------------------
// ReadBordersValues
// ---------------------------------------------------------------------------

std::vector<Core::BordersValue> XlsxWorkbookStylesXml::ReadBordersValues(const XmlElement& root)
{
    std::vector<Core::BordersValue> borders;
    const auto bordersElem = root.GetElement(MainNs + "borders");
    const auto borderElems = bordersElem.IsNull()
        ? std::vector<XmlElement>()
        : bordersElem.GetElements(MainNs + "border");
    borders.reserve(borderElems.size());
    for (const auto& border : borderElems) {
        borders.push_back(ReadBordersValue(border));
    }
    return borders;
}

// ---------------------------------------------------------------------------
// ReadDifferentialStyleValues
// ---------------------------------------------------------------------------

std::vector<Core::StyleValue> XlsxWorkbookStylesXml::ReadDifferentialStyleValues(
    const XmlElement& root)
{
    std::vector<Core::StyleValue> styles;
    const auto dxfsElem = root.GetElement(MainNs + "dxfs");
    const auto dxfElems = dxfsElem.IsNull()
        ? std::vector<XmlElement>()
        : dxfsElem.GetElements(MainNs + "dxf");
    styles.reserve(dxfElems.size());
    for (const auto& dxf : dxfElems) {
        styles.push_back(ReadDifferentialStyleValue(dxf));
    }
    return styles;
}

// ---------------------------------------------------------------------------
// BuildFontElements
// ---------------------------------------------------------------------------

std::vector<XmlElement> XlsxWorkbookStylesXml::BuildFontElements(
    const std::vector<Core::FontValue>& fonts)
{
    std::vector<XmlElement> elements;
    elements.reserve(fonts.size());
    for (const auto& font : fonts) {
        elements.push_back(BuildFontElement(font));
    }
    return elements;
}

// ---------------------------------------------------------------------------
// BuildFillElements
// ---------------------------------------------------------------------------

std::vector<XmlElement> XlsxWorkbookStylesXml::BuildFillElements(
    const std::vector<FillValue>& fills)
{
    std::vector<XmlElement> elements;
    elements.reserve(fills.size());
    for (const auto& fill : fills) {
        elements.push_back(BuildFillElement(fill));
    }
    return elements;
}

// ---------------------------------------------------------------------------
// BuildBorderElements
// ---------------------------------------------------------------------------

std::vector<XmlElement> XlsxWorkbookStylesXml::BuildBorderElements(
    const std::vector<Core::BordersValue>& borders)
{
    std::vector<XmlElement> elements;
    elements.reserve(borders.size());
    for (const auto& bdr : borders) {
        elements.push_back(BuildBorderElement(bdr));
    }
    return elements;
}

// ---------------------------------------------------------------------------
// BuildCellFormatElements
// ---------------------------------------------------------------------------

std::vector<XmlElement> XlsxWorkbookStylesXml::BuildCellFormatElements(
    const std::vector<CellFormatValue>& cellFormats)
{
    std::vector<XmlElement> elements;
    elements.reserve(cellFormats.size());
    for (const auto& cf : cellFormats) {
        elements.push_back(BuildCellFormatElement(cf, true));
    }
    return elements;
}

// ---------------------------------------------------------------------------
// BuildDifferentialFormatElements
// ---------------------------------------------------------------------------

std::vector<XmlElement> XlsxWorkbookStylesXml::BuildDifferentialFormatElements(
    const std::vector<Core::StyleValue>& differentialFormats)
{
    std::vector<XmlElement> elements;
    elements.reserve(differentialFormats.size());
    for (const auto& style : differentialFormats) {
        auto element = MakeElem(MainNs + "dxf");

        if (!XlsxWorkbookStylesValueHelpers::FontEquals(
                style.GetFont(), Core::StyleValue::Default().GetFont())) {
            element.AddChild(BuildFontElement(style.GetFont()));
        }
        if (style.GetPattern() != Core::FillPatternKind::None ||
            !XlsxWorkbookStylesValueHelpers::IsEmptyColor(style.GetForegroundColor()) ||
            !XlsxWorkbookStylesValueHelpers::IsEmptyColor(style.GetBackgroundColor())) {
            FillValue fv;
            fv.SetPattern(style.GetPattern());
            fv.SetForegroundColor(style.GetForegroundColor());
            fv.SetBackgroundColor(style.GetBackgroundColor());
            element.AddChild(BuildFillElement(fv));
        }
        if (!XlsxWorkbookStylesValueHelpers::BordersEqual(
                style.GetBorders(), Core::StyleValue::Default().GetBorders())) {
            element.AddChild(BuildBorderElement(style.GetBorders()));
        }
        if (style.GetNumberFormat().GetNumber() != 0 ||
            !style.GetNumberFormat().GetCustom().empty()) {
            auto numFmtEl = MakeElem(MainNs + "numFmt");
            AddAttrInt(numFmtEl, "numFmtId",
                       style.GetNumberFormat().GetNumber() >= 0
                           ? style.GetNumberFormat().GetNumber()
                           : 0);
            if (!style.GetNumberFormat().GetCustom().empty()) {
                AddAttr(numFmtEl, "formatCode", style.GetNumberFormat().GetCustom());
            }
            element.AddChild(std::move(numFmtEl));
        }

        auto alignmentEl = BuildAlignmentElement(style.GetAlignment());
        if (!alignmentEl.IsNull()) {
            element.AddChild(std::move(alignmentEl));
        }
        auto protectionEl = BuildProtectionElement(style.GetProtection());
        if (!protectionEl.IsNull()) {
            element.AddChild(std::move(protectionEl));
        }

        elements.push_back(std::move(element));
    }
    return elements;
}

// ---------------------------------------------------------------------------
// ParseHorizontalAlignment
// ---------------------------------------------------------------------------

Core::HorizontalAlignment XlsxWorkbookStylesXml::ParseHorizontalAlignment(std::string_view value)
{
    const std::string lower = ToLowerAscii(value);
    if (lower == "left")              return Core::HorizontalAlignment::Left;
    if (lower == "center")            return Core::HorizontalAlignment::Center;
    if (lower == "right")             return Core::HorizontalAlignment::Right;
    if (lower == "fill")              return Core::HorizontalAlignment::Fill;
    if (lower == "justify")           return Core::HorizontalAlignment::Justify;
    if (lower == "centercontinuous")  return Core::HorizontalAlignment::CenterContinuous;
    if (lower == "distributed")       return Core::HorizontalAlignment::Distributed;
    return Core::HorizontalAlignment::General;
}

// ---------------------------------------------------------------------------
// ParseVerticalAlignment
// ---------------------------------------------------------------------------

Core::VerticalAlignment XlsxWorkbookStylesXml::ParseVerticalAlignment(std::string_view value)
{
    const std::string lower = ToLowerAscii(value);
    if (lower == "center")      return Core::VerticalAlignment::Center;
    if (lower == "top")         return Core::VerticalAlignment::Top;
    if (lower == "justify")     return Core::VerticalAlignment::Justify;
    if (lower == "distributed") return Core::VerticalAlignment::Distributed;
    return Core::VerticalAlignment::Bottom;
}

// ---------------------------------------------------------------------------
// ParseOptionalBoolAttribute
// ---------------------------------------------------------------------------

std::optional<bool> XlsxWorkbookStylesXml::ParseOptionalBoolAttribute(
    const XmlAttribute& attribute)
{
    if (attribute.IsNull()) {
        return std::nullopt;
    }
    return XlsxWorkbookArchiveHelpers::ParseBoolAttribute(attribute);
}

// ---------------------------------------------------------------------------
// Private: BuildCellStyleFormatElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookStylesXml::BuildCellStyleFormatElement(const CellFormatValue& cellFormat)
{
    return BuildCellFormatElement(cellFormat, false);
}

// ---------------------------------------------------------------------------
// Private: ReadDifferentialStyleValue
// ---------------------------------------------------------------------------

Core::StyleValue XlsxWorkbookStylesXml::ReadDifferentialStyleValue(const XmlElement& dxf)
{
    Core::StyleValue style = Core::StyleValue::Default().Clone();

    const auto font = dxf.GetElement(MainNs + "font");
    if (!font.IsNull()) {
        style.SetFont(ReadFontValue(font));
    }

    const auto fill = dxf.GetElement(MainNs + "fill");
    if (!fill.IsNull()) {
        const auto fillValue = ReadFillValue(fill);
        style.SetPattern(fillValue.GetPattern());
        style.SetForegroundColor(fillValue.GetForegroundColor());
        style.SetBackgroundColor(fillValue.GetBackgroundColor());
    }

    const auto border = dxf.GetElement(MainNs + "border");
    if (!border.IsNull()) {
        style.SetBorders(ReadBordersValue(border));
    }

    const auto numFmt = dxf.GetElement(MainNs + "numFmt");
    if (!numFmt.IsNull()) {
        Core::NumberFormatValue nfv;
        nfv.SetNumber(
            XlsxWorkbookArchiveHelpers::ParseIntAttribute(numFmt.GetAttribute("numFmtId"))
                .value_or(0));
        const auto codeAttr = numFmt.GetAttribute("formatCode");
        if (!codeAttr.IsNull()) {
            nfv.SetCustom(codeAttr.GetValue());
        }
        style.SetNumberFormat(std::move(nfv));
    }

    const auto alignment = dxf.GetElement(MainNs + "alignment");
    if (!alignment.IsNull()) {
        Core::AlignmentValue av;
        av.SetHorizontal(
            ParseHorizontalAlignment(alignment.GetAttribute("horizontal").GetValue()));
        av.SetVertical(
            ParseVerticalAlignment(alignment.GetAttribute("vertical").GetValue()));
        av.SetWrapText(
            XlsxWorkbookArchiveHelpers::ParseBoolAttribute(alignment.GetAttribute("wrapText")));
        av.SetIndentLevel(
            StyleValueSanitizer::NormalizeIndentLevel(
                XlsxWorkbookArchiveHelpers::ParseIntAttribute(
                    alignment.GetAttribute("indent"))));
        av.SetTextRotation(
            StyleValueSanitizer::NormalizeTextRotation(
                XlsxWorkbookArchiveHelpers::ParseIntAttribute(
                    alignment.GetAttribute("textRotation"))));
        av.SetShrinkToFit(
            XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
                alignment.GetAttribute("shrinkToFit")));
        av.SetReadingOrder(
            StyleValueSanitizer::NormalizeReadingOrder(
                XlsxWorkbookArchiveHelpers::ParseIntAttribute(
                    alignment.GetAttribute("readingOrder"))));
        av.SetRelativeIndent(
            XlsxWorkbookArchiveHelpers::ParseIntAttribute(
                alignment.GetAttribute("relativeIndent")).value_or(0));
        style.SetAlignment(std::move(av));
    }

    const auto protection = dxf.GetElement(MainNs + "protection");
    if (!protection.IsNull()) {
        Core::ProtectionValue pv;
        pv.SetIsLocked(
            ParseOptionalBoolAttribute(protection.GetAttribute("locked")).value_or(true));
        pv.SetIsHidden(
            XlsxWorkbookArchiveHelpers::ParseBoolAttribute(protection.GetAttribute("hidden")));
        style.SetProtection(std::move(pv));
    }

    return style;
}

// ---------------------------------------------------------------------------
// Private: ReadFontValue
// ---------------------------------------------------------------------------

Core::FontValue XlsxWorkbookStylesXml::ReadFontValue(const XmlElement& font)
{
    Core::FontValue fv;

    const auto nameElem = font.GetElement(MainNs + "name");
    const auto nameAttr = nameElem.IsNull() ? XmlAttribute() : nameElem.GetAttribute("val");
    fv.SetName(nameAttr.IsNull() ? std::string("Calibri") : nameAttr.GetValue());

    const auto szElem = font.GetElement(MainNs + "sz");
    const auto szAttr = szElem.IsNull() ? XmlAttribute() : szElem.GetAttribute("val");
    fv.SetSize(szAttr.IsNull() ? 11.0
                               : XlsxWorkbookArchiveHelpers::ParseDoubleAttribute(szAttr).value_or(11.0));

    fv.SetBold(!font.GetElement(MainNs + "b").IsNull());
    fv.SetItalic(!font.GetElement(MainNs + "i").IsNull());
    fv.SetUnderline(!font.GetElement(MainNs + "u").IsNull());
    fv.SetStrikeThrough(!font.GetElement(MainNs + "strike").IsNull());
    fv.SetColor(ReadColorValue(font.GetElement(MainNs + "color")));

    return fv;
}

// ---------------------------------------------------------------------------
// Private: ReadFillValue
// ---------------------------------------------------------------------------

FillValue XlsxWorkbookStylesXml::ReadFillValue(const XmlElement& fill)
{
    const auto patternFill = fill.GetElement(MainNs + "patternFill");
    if (patternFill.IsNull()) {
        return FillValue();
    }

    const auto ptAttr = patternFill.GetAttribute("patternType");
    const std::string patternType = ToLowerAscii(ptAttr.IsNull() ? "none" : ptAttr.GetValue());

    Core::FillPatternKind pattern;
    if (patternType == "solid")                pattern = Core::FillPatternKind::Solid;
    else if (patternType == "mediumgray")      pattern = Core::FillPatternKind::MediumGray;
    else if (patternType == "darkgray")        pattern = Core::FillPatternKind::DarkGray;
    else if (patternType == "gray125")         pattern = Core::FillPatternKind::Gray125;
    else if (patternType == "gray0625")        pattern = Core::FillPatternKind::Gray0625;
    else if (patternType == "darkhorizontal")  pattern = Core::FillPatternKind::DarkHorizontal;
    else if (patternType == "darkvertical")    pattern = Core::FillPatternKind::DarkVertical;
    else if (patternType == "darkdown")        pattern = Core::FillPatternKind::DarkDown;
    else if (patternType == "darkup")          pattern = Core::FillPatternKind::DarkUp;
    else if (patternType == "darkgrid")        pattern = Core::FillPatternKind::DarkGrid;
    else if (patternType == "darktrellis")     pattern = Core::FillPatternKind::DarkTrellis;
    else if (patternType == "lighthorizontal") pattern = Core::FillPatternKind::LightHorizontal;
    else if (patternType == "lightvertical")   pattern = Core::FillPatternKind::LightVertical;
    else if (patternType == "lightdown")       pattern = Core::FillPatternKind::LightDown;
    else if (patternType == "lightup")         pattern = Core::FillPatternKind::LightUp;
    else if (patternType == "lightgrid")       pattern = Core::FillPatternKind::LightGrid;
    else if (patternType == "lighttrellis")    pattern = Core::FillPatternKind::LightTrellis;
    else                                       pattern = Core::FillPatternKind::None;

    FillValue fv;
    fv.SetPattern(pattern);
    fv.SetForegroundColor(ReadColorValue(patternFill.GetElement(MainNs + "fgColor")));
    fv.SetBackgroundColor(ReadColorValue(patternFill.GetElement(MainNs + "bgColor")));
    return fv;
}

// ---------------------------------------------------------------------------
// Private: ReadBordersValue
// ---------------------------------------------------------------------------

Core::BordersValue XlsxWorkbookStylesXml::ReadBordersValue(const XmlElement& border)
{
    Core::BordersValue bv;
    bv.SetLeft(ReadBorderSideValue(border.GetElement(MainNs + "left")));
    bv.SetRight(ReadBorderSideValue(border.GetElement(MainNs + "right")));
    bv.SetTop(ReadBorderSideValue(border.GetElement(MainNs + "top")));
    bv.SetBottom(ReadBorderSideValue(border.GetElement(MainNs + "bottom")));
    bv.SetDiagonal(ReadBorderSideValue(border.GetElement(MainNs + "diagonal")));
    bv.SetDiagonalUp(
        XlsxWorkbookArchiveHelpers::ParseBoolAttribute(border.GetAttribute("diagonalUp")));
    bv.SetDiagonalDown(
        XlsxWorkbookArchiveHelpers::ParseBoolAttribute(border.GetAttribute("diagonalDown")));
    return bv;
}

// ---------------------------------------------------------------------------
// Private: BuildFontElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookStylesXml::BuildFontElement(const Core::FontValue& font)
{
    auto element = MakeElem(MainNs + "font");

    if (font.GetBold()) {
        element.AddChild(MakeElem(MainNs + "b"));
    }
    if (font.GetItalic()) {
        element.AddChild(MakeElem(MainNs + "i"));
    }
    if (font.GetStrikeThrough()) {
        element.AddChild(MakeElem(MainNs + "strike"));
    }
    if (font.GetUnderline()) {
        element.AddChild(MakeElem(MainNs + "u"));
    }

    element.AddChild(MakeElem(MainNs + "sz", "val", FormatDoubleInvariant(font.GetSize())));

    auto colorEl = BuildColorElement("color", font.GetColor());
    if (!colorEl.IsNull()) {
        element.AddChild(std::move(colorEl));
    }

    element.AddChild(MakeElem(MainNs + "name", "val", font.GetName()));

    return element;
}

// ---------------------------------------------------------------------------
// Private: BuildFillElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookStylesXml::BuildFillElement(const FillValue& fill)
{
    auto patternFill = MakeElem(MainNs + "patternFill");

    switch (fill.GetPattern()) {
    case Core::FillPatternKind::Solid:
        AddAttr(patternFill, "patternType", "solid");
        break;
    case Core::FillPatternKind::MediumGray:
        AddAttr(patternFill, "patternType", "mediumGray");
        break;
    case Core::FillPatternKind::DarkGray:
        AddAttr(patternFill, "patternType", "darkGray");
        break;
    case Core::FillPatternKind::Gray125:
        AddAttr(patternFill, "patternType", "gray125");
        break;
    case Core::FillPatternKind::Gray0625:
        AddAttr(patternFill, "patternType", "gray0625");
        break;
    case Core::FillPatternKind::DarkHorizontal:
        AddAttr(patternFill, "patternType", "darkHorizontal");
        break;
    case Core::FillPatternKind::DarkVertical:
        AddAttr(patternFill, "patternType", "darkVertical");
        break;
    case Core::FillPatternKind::DarkDown:
        AddAttr(patternFill, "patternType", "darkDown");
        break;
    case Core::FillPatternKind::DarkUp:
        AddAttr(patternFill, "patternType", "darkUp");
        break;
    case Core::FillPatternKind::DarkGrid:
        AddAttr(patternFill, "patternType", "darkGrid");
        break;
    case Core::FillPatternKind::DarkTrellis:
        AddAttr(patternFill, "patternType", "darkTrellis");
        break;
    case Core::FillPatternKind::LightHorizontal:
        AddAttr(patternFill, "patternType", "lightHorizontal");
        break;
    case Core::FillPatternKind::LightVertical:
        AddAttr(patternFill, "patternType", "lightVertical");
        break;
    case Core::FillPatternKind::LightDown:
        AddAttr(patternFill, "patternType", "lightDown");
        break;
    case Core::FillPatternKind::LightUp:
        AddAttr(patternFill, "patternType", "lightUp");
        break;
    case Core::FillPatternKind::LightGrid:
        AddAttr(patternFill, "patternType", "lightGrid");
        break;
    case Core::FillPatternKind::LightTrellis:
        AddAttr(patternFill, "patternType", "lightTrellis");
        break;
    default:
        AddAttr(patternFill, "patternType", "none");
        break;
    }

    auto fgColor = BuildColorElement("fgColor", fill.GetForegroundColor());
    if (!fgColor.IsNull()) {
        patternFill.AddChild(std::move(fgColor));
    }
    auto bgColor = BuildColorElement("bgColor", fill.GetBackgroundColor());
    if (!bgColor.IsNull()) {
        patternFill.AddChild(std::move(bgColor));
    }

    auto fillEl = MakeElem(MainNs + "fill");
    fillEl.AddChild(std::move(patternFill));
    return fillEl;
}

// ---------------------------------------------------------------------------
// Private: BuildBorderElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookStylesXml::BuildBorderElement(const Core::BordersValue& borders)
{
    auto element = MakeElem(MainNs + "border");
    element.AddChild(BuildBorderSideElement("left", borders.GetLeft()));
    element.AddChild(BuildBorderSideElement("right", borders.GetRight()));
    element.AddChild(BuildBorderSideElement("top", borders.GetTop()));
    element.AddChild(BuildBorderSideElement("bottom", borders.GetBottom()));
    element.AddChild(BuildBorderSideElement("diagonal", borders.GetDiagonal()));

    if (borders.GetDiagonalUp()) {
        AddAttrInt(element, "diagonalUp", 1);
    }
    if (borders.GetDiagonalDown()) {
        AddAttrInt(element, "diagonalDown", 1);
    }

    return element;
}

// ---------------------------------------------------------------------------
// Private: BuildCellFormatElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookStylesXml::BuildCellFormatElement(const CellFormatValue& cellFormat,
                                                         bool includeXfId)
{
    auto element = MakeElemWithAttrs(MainNs + "xf",
                                     {{"numFmtId", std::to_string(cellFormat.GetNumFmtId())},
                                      {"fontId", std::to_string(cellFormat.GetFontId())},
                                      {"fillId", std::to_string(cellFormat.GetFillId())},
                                      {"borderId", std::to_string(cellFormat.GetBorderId())}});

    if (includeXfId) {
        AddAttrInt(element, "xfId", 0);
    }
    if (cellFormat.GetNumFmtId() != 0) {
        AddAttrInt(element, "applyNumberFormat", 1);
    }
    if (cellFormat.GetFontId() != 0) {
        AddAttrInt(element, "applyFont", 1);
    }
    if (cellFormat.GetFillId() != 0) {
        AddAttrInt(element, "applyFill", 1);
    }
    if (cellFormat.GetBorderId() != 0) {
        AddAttrInt(element, "applyBorder", 1);
    }

    auto alignmentEl = BuildAlignmentElement(cellFormat.GetAlignment());
    if (!alignmentEl.IsNull()) {
        AddAttrInt(element, "applyAlignment", 1);
        element.AddChild(std::move(alignmentEl));
    }

    auto protectionEl = BuildProtectionElement(cellFormat.GetProtection());
    if (!protectionEl.IsNull()) {
        AddAttrInt(element, "applyProtection", 1);
        element.AddChild(std::move(protectionEl));
    }

    return element;
}

// ---------------------------------------------------------------------------
// Private: ReadBorderSideValue
// ---------------------------------------------------------------------------

Core::BorderSideValue XlsxWorkbookStylesXml::ReadBorderSideValue(const XmlElement& side)
{
    if (side.IsNull()) {
        return Core::BorderSideValue();
    }
    Core::BorderSideValue bsv;
    bsv.SetStyle(
        XlsxWorkbookStylesValueHelpers::ParseBorderStyle(
            side.GetAttribute("style").GetValue()));
    bsv.SetColor(ReadColorValue(side.GetElement(MainNs + "color")));
    return bsv;
}

// ---------------------------------------------------------------------------
// Private: ReadColorValue
// ---------------------------------------------------------------------------

Core::ColorValue XlsxWorkbookStylesXml::ReadColorValue(const XmlElement& colorElement)
{
    if (colorElement.IsNull()) {
        return Core::ColorValue();
    }
    const auto rgbAttr = colorElement.GetAttribute("rgb");
    if (rgbAttr.IsNull()) {
        return Core::ColorValue();
    }
    const std::string raw = rgbAttr.GetValue();

    if (IsNullOrWhiteSpace(raw)) {
        return Core::ColorValue();
    }

    // Trim edge whitespace (matching C# string.Trim()) then uppercase.
    const std::string_view trimmed = TrimEdge(raw);
    std::string rgbUpper;
    rgbUpper.reserve(trimmed.size());
    for (char c : trimmed) {
        if (c >= 'a' && c <= 'f') {
            rgbUpper.push_back(static_cast<char>(c - ('a' - 'A')));
        } else {
            rgbUpper.push_back(c);
        }
    }

    if (rgbUpper.size() == 6) {
        rgbUpper = "FF" + rgbUpper;
    }
    if (rgbUpper.size() != 8) {
        return Core::ColorValue();
    }

    std::uint8_t a = 0, r = 0, g = 0, b = 0;
    if (!ParseHexByte(rgbUpper.substr(0, 2), a) ||
        !ParseHexByte(rgbUpper.substr(2, 2), r) ||
        !ParseHexByte(rgbUpper.substr(4, 2), g) ||
        !ParseHexByte(rgbUpper.substr(6, 2), b)) {
        return Core::ColorValue();
    }

    return Core::ColorValue(a, r, g, b);
}

// ---------------------------------------------------------------------------
// Private: BuildColorElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookStylesXml::BuildColorElement(const std::string& elementName,
                                                     const Core::ColorValue& color)
{
    if (XlsxWorkbookStylesValueHelpers::IsEmptyColor(color)) {
        return XmlElement();
    }
    return MakeElem(MainNs + elementName, "rgb",
                    XlsxWorkbookStylesValueHelpers::ToArgbHex(color));
}

// ---------------------------------------------------------------------------
// Private: BuildBorderSideElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookStylesXml::BuildBorderSideElement(const std::string& sideName,
                                                          const Core::BorderSideValue& side)
{
    auto element = MakeElem(MainNs + sideName);

    const std::string styleName =
        XlsxWorkbookStylesValueHelpers::GetBorderStyleName(side.GetStyle());
    if (!styleName.empty()) {
        AddAttr(element, "style", styleName);
    }

    auto colorEl = BuildColorElement("color", side.GetColor());
    if (!colorEl.IsNull()) {
        element.AddChild(std::move(colorEl));
    }

    return element;
}

// ---------------------------------------------------------------------------
// Private: BuildAlignmentElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookStylesXml::BuildAlignmentElement(const Core::AlignmentValue& alignment)
{
    auto element = MakeElem(MainNs + "alignment");
    bool hasValue = false;

    const std::string horizontal =
        XlsxWorkbookStylesValueHelpers::GetHorizontalAlignmentName(alignment.GetHorizontal());
    if (!horizontal.empty()) {
        AddAttr(element, "horizontal", horizontal);
        hasValue = true;
    }

    const std::string vertical =
        XlsxWorkbookStylesValueHelpers::GetVerticalAlignmentName(alignment.GetVertical());
    if (!vertical.empty()) {
        AddAttr(element, "vertical", vertical);
        hasValue = true;
    }

    if (alignment.GetWrapText()) {
        AddAttrInt(element, "wrapText", 1);
        hasValue = true;
    }
    if (alignment.GetIndentLevel() > 0) {
        AddAttrInt(element, "indent", alignment.GetIndentLevel());
        hasValue = true;
    }
    if (alignment.GetTextRotation() != 0) {
        AddAttrInt(element, "textRotation", alignment.GetTextRotation());
        hasValue = true;
    }
    if (alignment.GetShrinkToFit()) {
        AddAttrInt(element, "shrinkToFit", 1);
        hasValue = true;
    }
    if (alignment.GetReadingOrder() != 0) {
        AddAttrInt(element, "readingOrder", alignment.GetReadingOrder());
        hasValue = true;
    }
    if (alignment.GetRelativeIndent() != 0) {
        AddAttrInt(element, "relativeIndent", alignment.GetRelativeIndent());
        hasValue = true;
    }

    return hasValue ? element : XmlElement();
}

// ---------------------------------------------------------------------------
// Private: BuildProtectionElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookStylesXml::BuildProtectionElement(const Core::ProtectionValue& protection)
{
    auto element = MakeElem(MainNs + "protection");
    bool hasValue = false;

    if (!protection.GetIsLocked()) {
        AddAttrInt(element, "locked", 0);
        hasValue = true;
    }
    if (protection.GetIsHidden()) {
        AddAttrInt(element, "hidden", 1);
        hasValue = true;
    }

    return hasValue ? element : XmlElement();
}

}}  // namespace Aspose::Cells_FOSS
