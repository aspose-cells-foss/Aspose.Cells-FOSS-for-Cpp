#include "aspose/cells_foss/XlsxWorkbookStyles.h"

#include "aspose/cells_foss/FillValue.h"
#include "aspose/cells_foss/StyleValueSanitizer.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"
#include "aspose/cells_foss/XlsxWorkbookStylesXml.h"
#include "aspose/cells_foss/core/BordersValue.h"
#include "aspose/cells_foss/core/CellAddress.h"
#include "aspose/cells_foss/core/CellValueKind.h"
#include "aspose/cells_foss/core/FontValue.h"
#include "aspose/cells_foss/core/NumberFormatValue.h"

#include <algorithm>
#include <cstdio>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

namespace {

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

const XNamespace& MainNs = XlsxWorkbookSerializerCommon::MainNs;

/// Formats a double with C# "0.####" InvariantCulture semantics:
/// at least one digit before the decimal, up to 4 decimal places, trailing zeros removed.
std::string FormatFont(double value)
{
    char buf[64];
    const int n = std::snprintf(buf, sizeof(buf), "%.4f", value);
    if (n < 0 || n >= static_cast<int>(sizeof(buf)))
    {
        return std::to_string(value);
    }
    std::string result(buf, static_cast<std::size_t>(n));
    const auto dotPos = result.find('.');
    if (dotPos != std::string::npos)
    {
        const auto lastNonZero = result.find_last_not_of('0');
        if (lastNonZero == dotPos)
        {
            result.erase(dotPos);
        }
        else
        {
            result.erase(lastNonZero + 1);
        }
    }
    return result;
}

/// Returns ARGB hex string for a ColorValue (uppercase, 8 characters).
std::string ToArgbHex(const Core::ColorValue& color)
{
    char buf[9];
    std::snprintf(buf, sizeof(buf), "%02X%02X%02X%02X",
        static_cast<unsigned>(color.GetA()),
        static_cast<unsigned>(color.GetR()),
        static_cast<unsigned>(color.GetG()),
        static_cast<unsigned>(color.GetB()));
    return std::string(buf, 8);
}

/// Maps a FillValue from a StyleValue's pattern/colors.
FillValue ToFillValue(const Core::StyleValue& style)
{
    FillValue fv;
    fv.SetPattern(style.GetPattern());
    fv.SetForegroundColor(style.GetForegroundColor());
    fv.SetBackgroundColor(style.GetBackgroundColor());
    return fv;
}

/// Returns canonical key for a FontValue.
std::string GetFontKey(const Core::FontValue& font)
{
    std::ostringstream os;
    os << font.GetName() << '|'
       << FormatFont(font.GetSize()) << '|'
       << (font.GetBold() ? '1' : '0') << '|'
       << (font.GetItalic() ? '1' : '0') << '|'
       << (font.GetUnderline() ? '1' : '0') << '|'
       << (font.GetStrikeThrough() ? '1' : '0') << '|'
       << ToArgbHex(font.GetColor());
    return os.str();
}

/// Returns canonical key for a FillValue.
std::string GetFillKey(const FillValue& fill)
{
    std::ostringstream os;
    os << static_cast<int>(fill.GetPattern()) << '|'
       << ToArgbHex(fill.GetForegroundColor()) << '|'
       << ToArgbHex(fill.GetBackgroundColor());
    return os.str();
}

/// Returns canonical key for a BorderSideValue.
std::string GetBorderSideKey(const Core::BorderSideValue& border)
{
    std::ostringstream os;
    os << static_cast<int>(border.GetStyle()) << '|'
       << ToArgbHex(border.GetColor());
    return os.str();
}

/// Returns canonical key for a BordersValue.
std::string GetBordersKey(const Core::BordersValue& borders)
{
    std::ostringstream os;
    os << GetBorderSideKey(borders.GetLeft()) << '|'
       << GetBorderSideKey(borders.GetRight()) << '|'
       << GetBorderSideKey(borders.GetTop()) << '|'
       << GetBorderSideKey(borders.GetBottom()) << '|'
       << GetBorderSideKey(borders.GetDiagonal()) << '|'
       << (borders.GetDiagonalUp() ? '1' : '0') << '|'
       << (borders.GetDiagonalDown() ? '1' : '0');
    return os.str();
}

/// Template helper equivalent to C# Intern<T>.
template<typename T, typename Func>
int Intern(std::unordered_map<std::string, int>& indices,
           std::vector<T>& values,
           const T& value,
           Func getKey)
{
    const std::string key = getKey(value);
    const auto it = indices.find(key);
    if (it != indices.end())
    {
        return it->second;
    }
    const int id = static_cast<int>(values.size());
    values.push_back(value);
    indices[key] = id;
    return id;
}

/// Returns the effective style for a cell record.
Core::StyleValue GetEffectiveStyle(const Core::CellRecord& record)
{
    // In the C++ model, every CellRecord always has a StyleValue.
    // If the style equals default and the record is not explicitly stored,
    // we still return it as-is (the C# code checks for null style).
    return record.GetStyle();
}

/// Resolves or registers a number format id.
int ResolveNumberFormatId(
    const Core::NumberFormatValue& numberFormat,
    std::unordered_map<std::string, int>& customNumberFormatIds,
    std::vector<std::pair<int, std::string>>& customNumberFormats,
    int& nextCustomNumberFormatId)
{
    const std::string& customFormat = numberFormat.GetCustom();
    if (!customFormat.empty())
    {
        const auto it = customNumberFormatIds.find(customFormat);
        if (it != customNumberFormatIds.end())
        {
            return it->second;
        }
        const int customId = nextCustomNumberFormatId++;
        customNumberFormatIds[customFormat] = customId;
        customNumberFormats.emplace_back(customId, customFormat);
        return customId;
    }
    return numberFormat.GetNumber() >= 0 ? numberFormat.GetNumber() : 0;
}

/// Returns true if the number format represents a date/time format.
bool IsDateNumberFormat(const Core::NumberFormatValue& numberFormat)
{
    if (XlsxWorkbookSerializerCommon::BuiltInDateFormats.count(numberFormat.GetNumber()) > 0)
    {
        return true;
    }
    const std::string& customFormat = numberFormat.GetCustom();
    return !customFormat.empty()
        && XlsxWorkbookArchiveHelpers::LooksLikeDateFormat(customFormat);
}

/// Returns true if the cell should be persisted to the output.
bool ShouldPersistCell(const Core::StyleValue& workbookDefaultStyle,
                       const Core::CellRecord& record)
{
    return record.GetIsExplicitlyStored()
        || !record.GetFormula().empty()
        || !record.GetValue().IsEmpty()
        || record.GetKind() != Core::CellValueKind::Blank
        || !XlsxWorkbookStyles::StylesEqual(record.GetStyle(), workbookDefaultStyle);
}

/// Comparison function for sorting cell entries by (row, column).
bool CompareCellEntries(const std::pair<Core::CellAddress, Core::CellRecord>& left,
                        const std::pair<Core::CellAddress, Core::CellRecord>& right)
{
    const int rowCmp = left.first.GetRowIndex() - right.first.GetRowIndex();
    if (rowCmp != 0)
    {
        return rowCmp < 0;
    }
    return left.first.GetColumnIndex() < right.first.GetColumnIndex();
}

/// Collects cell entries that should be persisted, sorted by address.
std::vector<std::pair<Core::CellAddress, Core::CellRecord>> CollectPersistedCellEntries(
    const Core::WorksheetModel& worksheet,
    const Core::StyleValue& workbookDefaultStyle)
{
    std::vector<std::pair<Core::CellAddress, Core::CellRecord>> persistedEntries;
    for (const auto& pair : worksheet.GetCells())
    {
        if (ShouldPersistCell(workbookDefaultStyle, pair.second))
        {
            persistedEntries.push_back(pair);
        }
    }
    std::sort(persistedEntries.begin(), persistedEntries.end(), CompareCellEntries);
    return persistedEntries;
}

/// Checks if a string is null/empty/whitespace (matching C# string.IsNullOrWhiteSpace).
bool IsNullOrWhiteSpace(const std::string& s)
{
    for (char c : s)
    {
        if (static_cast<unsigned char>(c) > 32)
        {
            return false;
        }
    }
    return true;
}

}  // anonymous namespace

// ---------------------------------------------------------------------------
// BuildStylesheet
// ---------------------------------------------------------------------------

StylesheetSaveContext XlsxWorkbookStyles::BuildStylesheet(const Core::WorkbookModel& model)
{
    const Core::StyleValue defaultStyle = model.GetDefaultStyle().Clone();

    std::vector<Core::FontValue> fonts;
    fonts.push_back(defaultStyle.GetFont().Clone());
    std::unordered_map<std::string, int> fontIds;
    fontIds[GetFontKey(fonts[0])] = 0;

    std::vector<FillValue> fills;
    fills.push_back(ToFillValue(defaultStyle));
    {
        FillValue gray;
        gray.SetPattern(Core::FillPatternKind::Gray125);
        fills.push_back(gray);
    }
    std::unordered_map<std::string, int> fillIds;
    fillIds[GetFillKey(fills[0])] = 0;
    fillIds[GetFillKey(fills[1])] = 1;

    std::vector<Core::BordersValue> borders;
    borders.push_back(defaultStyle.GetBorders().Clone());
    std::unordered_map<std::string, int> borderIds;
    borderIds[GetBordersKey(borders[0])] = 0;

    std::unordered_map<std::string, int> customNumberFormatIds;
    std::vector<std::pair<int, std::string>> customNumberFormats;
    int nextCustomNumberFormatId = 164;
    const int defaultNumberFormatId = ResolveNumberFormatId(
        defaultStyle.GetNumberFormat(), customNumberFormatIds,
        customNumberFormats, nextCustomNumberFormatId);

    CellFormatValue normalCellFormat;
    normalCellFormat.SetNumFmtId(defaultNumberFormatId);
    normalCellFormat.SetFontId(0);
    normalCellFormat.SetFillId(0);
    normalCellFormat.SetBorderId(0);
    normalCellFormat.SetAlignment(defaultStyle.GetAlignment().Clone());
    normalCellFormat.SetProtection(defaultStyle.GetProtection().Clone());

    std::vector<CellFormatValue> cellFormats;
    cellFormats.push_back(normalCellFormat);

    std::unordered_map<std::string, int> styleIndices;
    styleIndices[GetStyleKey(defaultStyle)] = 0;

    std::vector<Core::StyleValue> differentialFormats;
    std::unordered_map<std::string, int> differentialStyleIndices;

    for (const auto& worksheet : model.GetWorksheets())
    {
        const auto persistedEntries = CollectPersistedCellEntries(worksheet, model.GetDefaultStyle());
        for (std::size_t index = 0; index < persistedEntries.size(); ++index)
        {
            const auto& pair = persistedEntries[index];
            const Core::StyleValue style = GetStyleForSerialization(pair.second);
            const std::string styleKey = GetStyleKey(style);
            if (styleIndices.count(styleKey) > 0)
            {
                continue;
            }

            const int fontId = Intern(fontIds, fonts, style.GetFont(), GetFontKey);
            const int fillId = Intern(fillIds, fills, ToFillValue(style), GetFillKey);
            const int borderId = Intern(borderIds, borders, style.GetBorders(), GetBordersKey);
            const int numFmtId = ResolveNumberFormatId(
                style.GetNumberFormat(), customNumberFormatIds,
                customNumberFormats, nextCustomNumberFormatId);

            styleIndices[styleKey] = static_cast<int>(cellFormats.size());
            CellFormatValue cf;
            cf.SetNumFmtId(numFmtId);
            cf.SetFontId(fontId);
            cf.SetFillId(fillId);
            cf.SetBorderId(borderId);
            cf.SetAlignment(style.GetAlignment().Clone());
            cf.SetProtection(style.GetProtection().Clone());
            cellFormats.push_back(std::move(cf));
        }

        for (std::size_t fi = 0; fi < worksheet.GetConditionalFormattings().size(); ++fi)
        {
            const auto& formatting = worksheet.GetConditionalFormattings()[fi];
            for (std::size_t ci = 0; ci < formatting.GetConditions().size(); ++ci)
            {
                const auto& style = formatting.GetConditions()[ci].GetStyle();
                if (StylesEqual(style, Core::StyleValue::Default()))
                {
                    continue;
                }

                const std::string styleKey = GetStyleKey(style);
                if (differentialStyleIndices.count(styleKey) > 0)
                {
                    continue;
                }

                differentialStyleIndices[styleKey] = static_cast<int>(differentialFormats.size());
                differentialFormats.push_back(style.Clone());
            }
        }
    }

    const bool hasStyles = styleIndices.size() > 1
        || !StylesEqual(defaultStyle, Core::StyleValue::Default())
        || !differentialFormats.empty();

    return StylesheetSaveContext(
        XlsxWorkbookStylesXml::BuildStylesheetDocument(
            fonts, fills, borders, normalCellFormat, cellFormats,
            customNumberFormats, differentialFormats),
        std::move(styleIndices),
        std::move(differentialStyleIndices),
        static_cast<int>(differentialFormats.size()),
        hasStyles);
}

// ---------------------------------------------------------------------------
// GetStyleForSerialization
// ---------------------------------------------------------------------------

Core::StyleValue XlsxWorkbookStyles::GetStyleForSerialization(const Core::CellRecord& record)
{
    const Core::StyleValue style = GetEffectiveStyle(record);
    if (record.GetKind() != Core::CellValueKind::DateTime
        || IsDateNumberFormat(style.GetNumberFormat()))
    {
        return style;
    }

    Core::StyleValue serializedStyle = style.Clone();
    Core::NumberFormatValue nfv;
    nfv.SetNumber(14);
    nfv.SetCustom(std::string());
    serializedStyle.SetNumberFormat(std::move(nfv));
    return serializedStyle;
}

// ---------------------------------------------------------------------------
// GetStyleKey
// ---------------------------------------------------------------------------

std::string XlsxWorkbookStyles::GetStyleKey(const Core::StyleValue& style)
{
    std::ostringstream os;
    os << GetFontKey(style.GetFont()) << '|'
       << GetFillKey(ToFillValue(style)) << '|'
       << GetBordersKey(style.GetBorders()) << '|'
       << style.GetNumberFormat().GetNumber() << '|'
       << style.GetNumberFormat().GetCustom() << '|'
       << static_cast<int>(style.GetAlignment().GetHorizontal()) << '|'
       << static_cast<int>(style.GetAlignment().GetVertical()) << '|'
       << (style.GetAlignment().GetWrapText() ? '1' : '0') << '|'
       << style.GetAlignment().GetIndentLevel() << '|'
       << style.GetAlignment().GetTextRotation() << '|'
       << (style.GetAlignment().GetShrinkToFit() ? '1' : '0') << '|'
       << style.GetAlignment().GetReadingOrder() << '|'
       << style.GetAlignment().GetRelativeIndent() << '|'
       << (style.GetProtection().GetIsLocked() ? '1' : '0') << '|'
       << (style.GetProtection().GetIsHidden() ? '1' : '0');
    return os.str();
}

// ---------------------------------------------------------------------------
// LoadStylesheet
// ---------------------------------------------------------------------------

StylesheetLoadContext XlsxWorkbookStyles::LoadStylesheet(
    const ZipArchive& archive,
    const std::unordered_map<std::string, std::string>& workbookRelationships,
    const LoadOptions& options,
    LoadDiagnostics& diagnostics)
{
    (void)options;
    (void)diagnostics;

    std::string stylesUri = XlsxWorkbookArchiveHelpers::FindRelationshipTarget(
        workbookRelationships, "/xl/styles.xml");
    if (stylesUri.empty())
    {
        if (workbookRelationships.empty())
        {
            stylesUri = "/xl/styles.xml";
        }
        else
        {
            stylesUri = XlsxWorkbookArchiveHelpers::FindRelationshipTargetByType(
                workbookRelationships,
                XlsxWorkbookSerializerCommon::StylesRelationshipType);
        }
    }
    if (stylesUri.empty())
    {
        stylesUri = "/xl/styles.xml";
    }

    const auto entry = XlsxWorkbookArchiveHelpers::GetEntry(archive, stylesUri);
    if (entry.IsNull())
    {
        return StylesheetLoadContext();
    }

    const auto document = XlsxWorkbookArchiveHelpers::LoadDocument(entry);
    const auto& root = document.GetRoot();
    if (root.IsNull())
    {
        return StylesheetLoadContext();
    }

    // Parse custom number formats.
    std::unordered_map<int, std::string> customFormats;
    {
        const auto numFmts = root.GetElement(MainNs + "numFmts");
        if (!numFmts.IsNull())
        {
            for (const auto& numFmt : numFmts.GetElements(MainNs + "numFmt"))
            {
                const auto idOpt = XlsxWorkbookArchiveHelpers::ParseIntAttribute(
                    numFmt.GetAttribute("numFmtId"));
                const auto codeAttr = numFmt.GetAttribute("formatCode");
                if (idOpt.has_value() && !codeAttr.IsNull() && !codeAttr.GetValue().empty())
                {
                    customFormats[idOpt.value()] = codeAttr.GetValue();
                }
            }
        }
    }

    // Parse fonts, fills, borders.
    auto fonts = XlsxWorkbookStylesXml::ReadFontValues(root);
    if (fonts.empty())
    {
        fonts.push_back(Core::StyleValue::Default().GetFont().Clone());
    }

    auto fills = XlsxWorkbookStylesXml::ReadFillValues(root);
    if (fills.empty())
    {
        FillValue none;
        none.SetPattern(Core::FillPatternKind::None);
        fills.push_back(none);
        FillValue gray;
        gray.SetPattern(Core::FillPatternKind::Gray125);
        fills.push_back(gray);
    }

    auto borders = XlsxWorkbookStylesXml::ReadBordersValues(root);
    if (borders.empty())
    {
        borders.push_back(Core::StyleValue::Default().GetBorders().Clone());
    }

    StylesheetLoadContext context;
    context.GetCellFormats().clear();
    {
        const auto diffFormats = XlsxWorkbookStylesXml::ReadDifferentialStyleValues(root);
        context.GetDifferentialFormats().insert(
            context.GetDifferentialFormats().end(),
            diffFormats.begin(), diffFormats.end());
    }

    // Parse cellXfs.
    const auto cellXfsElem = root.GetElement(MainNs + "cellXfs");
    std::vector<XmlElement> cellXfs;
    if (!cellXfsElem.IsNull())
    {
        cellXfs = cellXfsElem.GetElements(MainNs + "xf");
    }
    if (cellXfs.empty())
    {
        context.GetCellFormats().push_back(Core::StyleValue::Default().Clone());
        context.SetDefaultCellStyle(context.GetCellFormats()[0].Clone());
        return context;
    }

    for (std::size_t index = 0; index < cellXfs.size(); ++index)
    {
        const auto& xf = cellXfs[index];
        Core::StyleValue style = Core::StyleValue::Default().Clone();

        const auto fontIdOpt = XlsxWorkbookArchiveHelpers::ParseIntAttribute(
            xf.GetAttribute("fontId"));
        if (fontIdOpt.has_value() && fontIdOpt.value() >= 0
            && fontIdOpt.value() < static_cast<int>(fonts.size()))
        {
            style.SetFont(fonts[fontIdOpt.value()].Clone());
        }

        const auto fillIdOpt = XlsxWorkbookArchiveHelpers::ParseIntAttribute(
            xf.GetAttribute("fillId"));
        if (fillIdOpt.has_value() && fillIdOpt.value() >= 0
            && fillIdOpt.value() < static_cast<int>(fills.size()))
        {
            const auto& fill = fills[fillIdOpt.value()];
            style.SetPattern(fill.GetPattern());
            style.SetForegroundColor(fill.GetForegroundColor());
            style.SetBackgroundColor(fill.GetBackgroundColor());
        }

        const auto borderIdOpt = XlsxWorkbookArchiveHelpers::ParseIntAttribute(
            xf.GetAttribute("borderId"));
        if (borderIdOpt.has_value() && borderIdOpt.value() >= 0
            && borderIdOpt.value() < static_cast<int>(borders.size()))
        {
            style.SetBorders(borders[borderIdOpt.value()].Clone());
        }

        const int numFmtId = XlsxWorkbookArchiveHelpers::ParseIntAttribute(
            xf.GetAttribute("numFmtId")).value_or(0);
        std::string customFormat;
        const auto cfIt = customFormats.find(numFmtId);
        if (cfIt != customFormats.end())
        {
            customFormat = cfIt->second;
        }
        Core::NumberFormatValue nfv;
        nfv.SetNumber(numFmtId);
        nfv.SetCustom(std::move(customFormat));
        style.SetNumberFormat(std::move(nfv));

        const auto alignment = xf.GetElement(MainNs + "alignment");
        if (!alignment.IsNull())
        {
            Core::AlignmentValue av;
            av.SetHorizontal(
                XlsxWorkbookStylesXml::ParseHorizontalAlignment(
                    alignment.GetAttribute("horizontal").GetValue()));
            av.SetVertical(
                XlsxWorkbookStylesXml::ParseVerticalAlignment(
                    alignment.GetAttribute("vertical").GetValue()));
            av.SetWrapText(
                XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
                    alignment.GetAttribute("wrapText")));
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

        const auto protection = xf.GetElement(MainNs + "protection");
        if (!protection.IsNull())
        {
            Core::ProtectionValue pv;
            pv.SetIsLocked(
                XlsxWorkbookStylesXml::ParseOptionalBoolAttribute(
                    protection.GetAttribute("locked")).value_or(true));
            pv.SetIsHidden(
                XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
                    protection.GetAttribute("hidden")));
            style.SetProtection(std::move(pv));
        }

        context.GetCellFormats().push_back(style);
        if (IsDateNumberFormat(style.GetNumberFormat()))
        {
            context.GetDateStyleIndexes().insert(static_cast<int>(index));
        }
    }

    if (context.GetCellFormats().empty())
    {
        context.GetCellFormats().push_back(Core::StyleValue::Default().Clone());
    }

    context.SetDefaultCellStyle(context.GetCellFormats()[0].Clone());
    return context;
}

// ---------------------------------------------------------------------------
// StylesEqual
// ---------------------------------------------------------------------------

bool XlsxWorkbookStyles::StylesEqual(const Core::StyleValue& left,
                                     const Core::StyleValue& right)
{
    return GetStyleKey(left) == GetStyleKey(right);
}

}}  // namespace Aspose::Cells_FOSS
