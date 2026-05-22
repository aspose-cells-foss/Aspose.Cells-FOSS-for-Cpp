#pragma once

#include "aspose/cells_foss/CellValue.h"
#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/core/StyleValue.h"

#include <locale>
#include <string>

namespace Aspose {
namespace Cells_FOSS {

/// Internal static helper for formatting display text of cell values.
class DisplayTextFormatter {
public:
    DisplayTextFormatter() = delete;

    /// Formats a raw cell value into its invariant-culture string
    /// representation (used when no style applies).
    static std::string FormatStringValue(const CellValue& value);

    /// Formats a cell value for display using the given style and workbook
    /// culture.  Applies number formats, date formats, text formats, and
    /// fraction formats as appropriate.
    static std::string FormatDisplayValue(const CellValue& value,
                                          const Core::StyleValue& style,
                                          const std::locale& workbookCulture);

private:
    static std::string FormatTextValue(std::string_view value,
                                       const Core::StyleValue& style);
    static std::string FormatNumericValue(const CellValue& value,
                                          const Core::StyleValue& style,
                                          const std::locale& workbookCulture);
    static std::string FormatDateTimeValue(const DateTime& value,
                                           const Core::StyleValue& style,
                                           const std::locale& workbookCulture);
    static std::string FormatRawDateTimeValue(const DateTime& value);
    static bool TryFormatFraction(double numericValue, std::string_view section,
                                  bool useAbsoluteValue, std::string& result);
    static std::string FormatWholeFractionResult(long wholePart,
                                                 bool useAbsoluteValue,
                                                 double numericValue);
    static int GreatestCommonDivisor(int left, int right);

    /// Formats a numeric value (int or double) with a .NET-style numeric
    /// format pattern string and the given locale.
    static std::string FormatWithNumericPattern(double value,
                                                std::string_view pattern,
                                                const std::locale& culture);
};

}}  // namespace Aspose::Cells_FOSS
