#pragma once

#include "aspose/cells_foss/CellFormatValue.h"
#include "aspose/cells_foss/FillValue.h"
#include "aspose/cells_foss/XmlDocument.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/core/BordersValue.h"
#include "aspose/cells_foss/core/FontValue.h"
#include "aspose/cells_foss/core/HorizontalAlignment.h"
#include "aspose/cells_foss/core/StyleValue.h"
#include "aspose/cells_foss/core/VerticalAlignment.h"

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Provides XML read/write methods for the workbook styles part.
/// </summary>
class XlsxWorkbookStylesXml {
public:
    XlsxWorkbookStylesXml() = delete;

    /// <summary>
    /// Builds the stylesheet XML document from the given style collections.
    /// </summary>
    static XmlDocument BuildStylesheetDocument(
        const std::vector<Core::FontValue>& fonts,
        const std::vector<FillValue>& fills,
        const std::vector<Core::BordersValue>& borders,
        const CellFormatValue& normalCellFormat,
        const std::vector<CellFormatValue>& cellFormats,
        const std::vector<std::pair<int, std::string>>& customNumberFormats,
        const std::vector<Core::StyleValue>& differentialFormats);

    /// <summary>
    /// Reads font values from the stylesheet root element.
    /// </summary>
    static std::vector<Core::FontValue> ReadFontValues(const XmlElement& root);

    /// <summary>
    /// Reads fill values from the stylesheet root element.
    /// </summary>
    static std::vector<FillValue> ReadFillValues(const XmlElement& root);

    /// <summary>
    /// Reads borders values from the stylesheet root element.
    /// </summary>
    static std::vector<Core::BordersValue> ReadBordersValues(const XmlElement& root);

    /// <summary>
    /// Reads differential style values from the stylesheet root element.
    /// </summary>
    static std::vector<Core::StyleValue> ReadDifferentialStyleValues(const XmlElement& root);

    /// <summary>
    /// Builds XML elements for the given font values.
    /// </summary>
    static std::vector<XmlElement> BuildFontElements(
        const std::vector<Core::FontValue>& fonts);

    /// <summary>
    /// Builds XML elements for the given fill values.
    /// </summary>
    static std::vector<XmlElement> BuildFillElements(
        const std::vector<FillValue>& fills);

    /// <summary>
    /// Builds XML elements for the given border values.
    /// </summary>
    static std::vector<XmlElement> BuildBorderElements(
        const std::vector<Core::BordersValue>& borders);

    /// <summary>
    /// Builds XML elements for the given cell format values.
    /// </summary>
    static std::vector<XmlElement> BuildCellFormatElements(
        const std::vector<CellFormatValue>& cellFormats);

    /// <summary>
    /// Builds XML elements for the given differential format values.
    /// </summary>
    static std::vector<XmlElement> BuildDifferentialFormatElements(
        const std::vector<Core::StyleValue>& differentialFormats);

    /// <summary>
    /// Parses a horizontal alignment name string to the corresponding enum value.
    /// </summary>
    static Core::HorizontalAlignment ParseHorizontalAlignment(std::string_view value);

    /// <summary>
    /// Parses a vertical alignment name string to the corresponding enum value.
    /// </summary>
    static Core::VerticalAlignment ParseVerticalAlignment(std::string_view value);

    /// <summary>
    /// Parses an optional boolean attribute. Returns nullopt if the attribute is null.
    /// </summary>
    static std::optional<bool> ParseOptionalBoolAttribute(const XmlAttribute& attribute);

private:
    static XmlElement BuildCellStyleFormatElement(const CellFormatValue& cellFormat);
    static XmlElement BuildCellFormatElement(const CellFormatValue& cellFormat, bool includeXfId);
    static Core::StyleValue ReadDifferentialStyleValue(const XmlElement& dxf);
    static Core::FontValue ReadFontValue(const XmlElement& font);
    static FillValue ReadFillValue(const XmlElement& fill);
    static Core::BordersValue ReadBordersValue(const XmlElement& border);
    static Core::BorderSideValue ReadBorderSideValue(const XmlElement& side);
    static Core::ColorValue ReadColorValue(const XmlElement& colorElement);
    static XmlElement BuildFontElement(const Core::FontValue& font);
    static XmlElement BuildFillElement(const FillValue& fill);
    static XmlElement BuildBorderElement(const Core::BordersValue& borders);
    static XmlElement BuildColorElement(const std::string& elementName, const Core::ColorValue& color);
    static XmlElement BuildBorderSideElement(const std::string& sideName, const Core::BorderSideValue& side);
    static XmlElement BuildAlignmentElement(const Core::AlignmentValue& alignment);
    static XmlElement BuildProtectionElement(const Core::ProtectionValue& protection);
};

}}  // namespace Aspose::Cells_FOSS
