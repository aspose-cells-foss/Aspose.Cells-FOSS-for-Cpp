#pragma once

#include <string>
#include <string_view>

#include "aspose/cells_foss/core/BordersValue.h"
#include "aspose/cells_foss/core/BorderStyle.h"
#include "aspose/cells_foss/core/ColorValue.h"
#include "aspose/cells_foss/core/FontValue.h"
#include "aspose/cells_foss/core/HorizontalAlignment.h"
#include "aspose/cells_foss/core/VerticalAlignment.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Provides helper methods for workbook style value conversions and comparisons.
/// </summary>
class XlsxWorkbookStylesValueHelpers {
public:
    /// <summary>
    /// Parses a border style name string to the corresponding BorderStyle enum value.
    /// Matching is case-insensitive.
    /// </summary>
    static Core::BorderStyle ParseBorderStyle(std::string_view value);

    /// <summary>
    /// Returns the canonical name for the given BorderStyle value.
    /// </summary>
    static std::string GetBorderStyleName(Core::BorderStyle value);

    /// <summary>
    /// Returns the canonical name for the given HorizontalAlignment value.
    /// </summary>
    static std::string GetHorizontalAlignmentName(Core::HorizontalAlignment value);

    /// <summary>
    /// Returns the canonical name for the given VerticalAlignment value.
    /// </summary>
    static std::string GetVerticalAlignmentName(Core::VerticalAlignment value);

    /// <summary>
    /// Converts a ColorValue to an 8-character uppercase ARGB hex string.
    /// </summary>
    static std::string ToArgbHex(const Core::ColorValue& color);

    /// <summary>
    /// Returns true if all ARGB components of the color are zero.
    /// </summary>
    static bool IsEmptyColor(const Core::ColorValue& color);

    /// <summary>
    /// Compares two FontValue instances for field-by-field equality.
    /// </summary>
    static bool FontEquals(const Core::FontValue& left, const Core::FontValue& right);

    /// <summary>
    /// Compares two BordersValue instances for field-by-field equality.
    /// </summary>
    static bool BordersEqual(const Core::BordersValue& left, const Core::BordersValue& right);

private:
    static bool BorderSideEquals(const Core::BorderSideValue& left, const Core::BorderSideValue& right);
};

}}  // namespace Aspose::Cells_FOSS
