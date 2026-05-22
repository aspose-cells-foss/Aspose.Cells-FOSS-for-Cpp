#include "aspose/cells_foss/DisplayTextFormatter.h"
#include "aspose/cells_foss/DisplayTextDateFormatSupport.h"
#include "aspose/cells_foss/DisplayTextFormatterSupport.h"
#include "aspose/cells_foss/DisplayTextLocaleSupport.h"
#include "aspose/cells_foss/NumberFormat.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// File-scoped helpers
// ---------------------------------------------------------------------------

namespace {

bool IsWhitespaceOrEmpty(std::string_view s) noexcept
{
    for (char c : s) {
        if (std::isspace(static_cast<unsigned char>(c)) == 0) {
            return false;
        }
    }
    return true;
}

bool EqualsOrdinal(std::string_view a, std::string_view b) noexcept
{
    return a == b;
}

// Formats a double in "G15" style (up to 15 significant digits, no trailing
// zeros, uses 'E' for exponent).  Matches .NET Double.ToString(null,
// CultureInfo.InvariantCulture).
std::string FormatDoubleInvariant(double value)
{
    if (std::isnan(value)) {
        return "NaN";
    }
    if (std::isinf(value)) {
        return value < 0.0 ? "-Infinity" : "Infinity";
    }

    char buf[64];
    int len = std::snprintf(buf, sizeof(buf), "%.15g", value);
    if (len < 0 || static_cast<size_t>(len) >= sizeof(buf)) {
        return std::to_string(value);
    }

    // .NET uses uppercase 'E' for scientific notation; C uses lowercase 'e'.
    for (int i = 0; i < len; ++i) {
        if (buf[i] == 'e') {
            buf[i] = 'E';
        }
    }

    return std::string(buf, static_cast<size_t>(len));
}

// Computes the time-of-day duration from a DateTime value, returned as
// a std::chrono::duration<double> measured in seconds.
std::chrono::duration<double> GetTimeOfDay(const DateTime& value)
{
    constexpr std::int64_t TICKS_PER_DAY = 86400LL * 10000000LL;
    std::int64_t todTicks = value.GetTicks() % TICKS_PER_DAY;
    if (todTicks < 0) {
        todTicks += TICKS_PER_DAY;
    }
    double seconds = static_cast<double>(todTicks) / 10000000.0;
    return std::chrono::duration<double>(seconds);
}

// Returns true if the DateTime has zero time-of-day component.
bool IsDateOnly(const DateTime& value)
{
    return value.GetHour() == 0 && value.GetMinute() == 0 &&
           value.GetSecond() == 0 &&
           (value.GetTicks() % 10000000LL) == 0;
}

// Appends an integer to a string (no padding).
void AppendInt(int value, std::string& out)
{
    out.append(std::to_string(value));
}

// Appends an integer with minimum zero-padding.
void AppendIntPadded(int value, int minWidth, std::string& out)
{
    if (value < 0) {
        out.push_back('-');
        value = -value;
    }
    std::string num = std::to_string(value);
    for (int i = static_cast<int>(num.size()); i < minWidth; ++i) {
        out.push_back('0');
    }
    out.append(num);
}

// ---------------------------------------------------------------------------
// .NET-style numeric format pattern engine
// ---------------------------------------------------------------------------

struct ParsedNumericFormat {
    bool isPercent = false;
    bool isScientific = false;
    char expChar = '\0';      // 'E' or 'e'
    int  expDigits = 0;       // number of exponent digit placeholders

    // Integer part (digits left of decimal point).
    int  intDigitCount = 0;   // total digit placeholders (0 + #)
    int  intZeroCount = 0;    // '0' placeholders only
    int  groupSize = 0;       // digits between commas (rightmost group)

    // Fraction part (digits right of decimal point).
    int  fracDigitCount = 0;
    int  fracZeroCount = 0;

    bool hasDecimal = false;
};

// Parses a numeric format pattern into its structural components.
ParsedNumericFormat ParseNumericPattern(std::string_view pattern)
{
    ParsedNumericFormat fmt;
    std::string_view p = pattern;

    // Detect percentage.
    for (char c : p) {
        if (c == '%') {
            fmt.isPercent = true;
            break;
        }
    }

    // Detect scientific notation.
    for (size_t i = 0; i < p.size(); ++i) {
        char c = p[i];
        if (c == 'E' || c == 'e') {
            if (i + 1 < p.size() && (p[i + 1] == '+' || p[i + 1] == '-')) {
                int digCount = 0;
                for (size_t j = i + 2; j < p.size(); ++j) {
                    if (p[j] == '0' || p[j] == '#') {
                        ++digCount;
                    } else {
                        break;
                    }
                }
                if (digCount > 0) {
                    fmt.isScientific = true;
                    fmt.expChar = c;
                    fmt.expDigits = digCount;
                    p = p.substr(0, i);
                    break;
                }
            }
        }
    }

    // Parse integer and fraction parts, skipping literal characters.
    enum class Phase { Integer, Fraction };
    Phase phase = Phase::Integer;
    int lastCommaPos = -1;
    int intPos = 0;

    for (size_t i = 0; i < p.size(); ++i) {
        char c = p[i];
        if (c == '%') {
            continue;
        }
        if (c == '.') {
            if (phase == Phase::Integer) {
                fmt.hasDecimal = true;
                phase = Phase::Fraction;
            }
            continue;
        }
        if (phase == Phase::Integer) {
            if (c == '0') {
                fmt.intZeroCount++;
                fmt.intDigitCount++;
                intPos++;
            } else if (c == '#') {
                fmt.intDigitCount++;
                intPos++;
            } else if (c == ',') {
                lastCommaPos = intPos;
            }
        } else {
            if (c == '0') {
                fmt.fracZeroCount++;
                fmt.fracDigitCount++;
            } else if (c == '#') {
                fmt.fracDigitCount++;
            }
        }
    }

    // Compute group size: count of integer digit placeholders after the
    // last comma.  If no comma, groupSize stays 0 (no grouping).
    if (lastCommaPos >= 0) {
        fmt.groupSize = intPos - lastCommaPos;
    }

    return fmt;
}

// Formats a double value using a parsed .NET-style numeric format.
std::string ApplyNumericFormat(double value, const ParsedNumericFormat& fmt,
                               const std::locale& culture)
{
    // Use locale-specific numeric separators.
    char decimalSep = '.';
    char thousandsSep = ',';
    try {
        const auto& np = std::use_facet<std::numpunct<char>>(culture);
        decimalSep = np.decimal_point();
        thousandsSep = np.thousands_sep();
    } catch (...) {
        // Fallback to C-locale defaults.
    }
    // Normalize non-breaking space (0xA0 in Windows-1252) to regular space.
    if (static_cast<unsigned char>(thousandsSep) == 0xA0) {
        thousandsSep = ' ';
    }

    double workValue = value;

    if (fmt.isPercent) {
        workValue *= 100.0;
    }

    if (fmt.isScientific) {
        // Scientific notation.
        int exponent = 0;
        if (workValue != 0.0) {
            double logVal = std::log10(std::fabs(workValue));
            exponent = static_cast<int>(std::floor(logVal));
            workValue /= std::pow(10.0, exponent);
        }

        int intDigits = fmt.intDigitCount;
        if (intDigits <= 0) {
            intDigits = 1;
        }
        int fracDigits = fmt.fracDigitCount;

        // Round to the total number of significant digits.
        double totalDigits = static_cast<double>(intDigits + fracDigits);
        double scale = std::pow(10.0, totalDigits - 1.0);
        workValue = std::round(workValue * scale) / scale;

        double absMantissa = std::fabs(workValue);
        double mantissaIntPart = std::floor(absMantissa);
        double mantissaFracPart = absMantissa - mantissaIntPart;

        std::string result;

        // Sign.
        if (value < 0.0) {
            result.push_back('-');
        }

        // Integer part of mantissa.
        result.append(std::to_string(static_cast<int>(mantissaIntPart)));

        // Decimal part of mantissa.
        if (fracDigits > 0) {
            result.push_back('.');
            double fracScale = std::pow(10.0, fracDigits);
            int fracInt = static_cast<int>(
                std::round(mantissaFracPart * fracScale));
            std::string fracStr = std::to_string(fracInt);
            while (static_cast<int>(fracStr.size()) < fracDigits) {
                fracStr.insert(fracStr.begin(), '0');
            }
            int zerosToKeep = fmt.fracZeroCount;
            if (zerosToKeep < fracDigits) {
                while (static_cast<int>(fracStr.size()) > zerosToKeep &&
                       fracStr.back() == '0') {
                    fracStr.pop_back();
                }
            }
            result.append(fracStr);
        }

        // Exponent.
        result.push_back(fmt.expChar);
        if (exponent >= 0) {
            result.push_back('+');
            AppendIntPadded(exponent, fmt.expDigits, result);
        } else {
            result.push_back('-');
            AppendIntPadded(-exponent, fmt.expDigits, result);
        }

        if (fmt.isPercent) {
            result.push_back('%');
        }

        return result;
    }

    // Fixed-point formatting.
    int fracDigits = fmt.fracDigitCount;

    // If no integer digits and no decimal point, treat as integer format.
    if (fmt.intDigitCount == 0 && !fmt.hasDecimal) {
        workValue = std::round(workValue);
    }

    // Round to fraction precision.
    double roundScale = std::pow(10.0, fracDigits);
    workValue = std::round(workValue * roundScale) / roundScale;

    double absValue = std::fabs(workValue);
    double intPartF = std::floor(absValue);
    double fracPart = absValue - intPartF;
    auto intPart = static_cast<long>(intPartF);

    // Build integer digits.
    std::string intStr;
    {
        long tmp = intPart;
        if (tmp == 0) {
            int minDigits = std::max(fmt.intZeroCount,
                                     fmt.intDigitCount > 0 ? 1 : 0);
            for (int i = 0; i < minDigits; ++i) {
                intStr.push_back('0');
            }
        } else {
            while (tmp > 0) {
                intStr.push_back(static_cast<char>('0' + (tmp % 10)));
                tmp /= 10;
            }
            std::reverse(intStr.begin(), intStr.end());
        }
    }

    // Pad integer part to minimum width for '0' placeholders.
    while (static_cast<int>(intStr.size()) < fmt.intZeroCount) {
        intStr.insert(intStr.begin(), '0');
    }

    // Apply grouping.
    if (fmt.groupSize > 0 &&
        static_cast<int>(intStr.size()) > fmt.groupSize) {
        std::string grouped;
        int count = 0;
        for (int i = static_cast<int>(intStr.size()) - 1; i >= 0; --i) {
            if (count > 0 && count % fmt.groupSize == 0) {
                grouped.push_back(thousandsSep);
            }
            grouped.push_back(intStr[static_cast<size_t>(i)]);
            ++count;
        }
        std::reverse(grouped.begin(), grouped.end());
        intStr = grouped;
    }

    // Build fraction digits.
    std::string fracStr;
    if (fracDigits > 0 || fmt.hasDecimal) {
        double fracScale = std::pow(10.0, fracDigits);
        auto fracInt = static_cast<int>(std::round(fracPart * fracScale));
        // Handle rounding overflow (e.g., 0.999 rounds to 1.0).
        if (fracInt >= static_cast<int>(fracScale)) {
            fracInt = 0;
            intPart++;
            // Rebuild integer part.
            intStr.clear();
            {
                long tmp = intPart;
                while (tmp > 0) {
                    intStr.push_back(static_cast<char>('0' + (tmp % 10)));
                    tmp /= 10;
                }
                std::reverse(intStr.begin(), intStr.end());
            }
            while (static_cast<int>(intStr.size()) < fmt.intZeroCount) {
                intStr.insert(intStr.begin(), '0');
            }
            if (fmt.groupSize > 0 &&
                static_cast<int>(intStr.size()) > fmt.groupSize) {
                std::string grouped;
                int count = 0;
                for (int i = static_cast<int>(intStr.size()) - 1; i >= 0;
                     --i) {
                    if (count > 0 && count % fmt.groupSize == 0) {
                        grouped.push_back(thousandsSep);
                    }
                    grouped.push_back(intStr[static_cast<size_t>(i)]);
                    ++count;
                }
                std::reverse(grouped.begin(), grouped.end());
                intStr = grouped;
            }
        }

        if (fracDigits > 0) {
            fracStr = std::to_string(fracInt);
            while (static_cast<int>(fracStr.size()) < fracDigits) {
                fracStr.insert(fracStr.begin(), '0');
            }
            // Trim trailing zeros for '#' placeholders.
            int zerosToKeep = fmt.fracZeroCount;
            if (zerosToKeep < fracDigits) {
                while (static_cast<int>(fracStr.size()) > zerosToKeep &&
                       fracStr.back() == '0') {
                    fracStr.pop_back();
                }
            }
        }
    }

    // Assemble result.
    std::string result;
    if (value < 0.0) {
        result.push_back('-');
    }
    result.append(intStr);
    if (!fracStr.empty()) {
        result.push_back(decimalSep);
        result.append(fracStr);
    } else if (fmt.hasDecimal && fracDigits == 0) {
        result.push_back(decimalSep);
    }
    if (fmt.isPercent) {
        result.push_back('%');
    }

    return result;
}

// Returns true if the character is a numeric format placeholder or modifier
// (i.e. a character that participates in .NET-style numeric formatting rather
// than being a literal character to output as-is).
bool IsNumericFormatChar(char c) noexcept
{
    return c == '0' || c == '#' || c == '.' || c == ',' ||
           c == '%' || c == 'E' || c == 'e' || c == '+' || c == '-';
}

// Strips .NET-style format-string literal quotes from a piece of text.
// Quoted content is kept but the quote delimiters themselves are removed.
std::string StripLiteralQuotes(std::string_view text)
{
    std::string result;
    result.reserve(text.size());
    bool inQuote = false;
    for (char c : text) {
        if (c == '"') {
            inQuote = !inQuote;
            continue;
        }
        result.push_back(c);
    }
    return result;
}

// Extracts leading and trailing literal (non-format) characters from a
// sanitized numeric section.  For example, "(#,##0.00)" yields prefix "("
// and suffix ")".  The corePattern output receives only the format
// placeholder portion.
void ExtractLiteralWrapping(std::string_view section,
                            std::string& prefix, std::string& suffix,
                            std::string& corePattern)
{
    prefix.clear();
    suffix.clear();
    corePattern.clear();

    if (section.empty()) {
        return;
    }

    // Find first format character.
    size_t firstFormat = section.size();
    for (size_t i = 0; i < section.size(); ++i) {
        if (IsNumericFormatChar(section[i])) {
            firstFormat = i;
            break;
        }
    }

    // Find last format character.
    size_t lastFormat = 0;
    bool foundLast = false;
    for (size_t i = section.size(); i > 0; --i) {
        if (IsNumericFormatChar(section[i - 1])) {
            lastFormat = i - 1;
            foundLast = true;
            break;
        }
    }

    if (!foundLast || firstFormat > lastFormat) {
        // No format characters found; treat entire section as literal.
        prefix = std::string(section);
        return;
    }

    prefix = std::string(section.substr(0, firstFormat));
    suffix = std::string(section.substr(lastFormat + 1));
    corePattern = std::string(section.substr(firstFormat, lastFormat - firstFormat + 1));
}

}  // anonymous namespace

// ===========================================================================
// FormatStringValue
// ===========================================================================

std::string DisplayTextFormatter::FormatStringValue(const CellValue& value)
{
    if (value.IsEmpty()) {
        return std::string();
    }

    if (value.IsString()) {
        return value.AsString();
    }

    if (value.IsBool()) {
        return value.AsBool() ? "TRUE" : "FALSE";
    }

    if (value.IsDateTime()) {
        return FormatRawDateTimeValue(value.AsDateTime());
    }

    if (value.IsInteger()) {
        return std::to_string(value.AsInteger());
    }

    if (value.IsDouble()) {
        return FormatDoubleInvariant(value.AsDouble());
    }

    return std::string();
}

// ===========================================================================
// FormatDisplayValue
// ===========================================================================

std::string DisplayTextFormatter::FormatDisplayValue(
    const CellValue& value, const Core::StyleValue& style,
    const std::locale& workbookCulture)
{
    if (value.IsEmpty()) {
        return std::string();
    }

    if (value.IsString()) {
        return FormatTextValue(value.AsString(), style);
    }

    if (value.IsBool()) {
        return value.AsBool() ? "TRUE" : "FALSE";
    }

    if (value.IsDateTime()) {
        return FormatDateTimeValue(value.AsDateTime(), style, workbookCulture);
    }

    if (DisplayTextFormatterSupport::IsNumericValue(value)) {
        return FormatNumericValue(value, style, workbookCulture);
    }

    return FormatStringValue(value);
}

// ===========================================================================
// FormatTextValue  (private)
// ===========================================================================

std::string DisplayTextFormatter::FormatTextValue(
    std::string_view value, const Core::StyleValue& style)
{
    std::string formatCode = NumberFormat::ResolveFormatCode(
        style.GetNumberFormat().GetNumber(),
        style.GetNumberFormat().GetCustom());

    if (IsWhitespaceOrEmpty(formatCode) ||
        EqualsOrdinal(formatCode, "General")) {
        return std::string(value);
    }

    // Excel text formats can have a dedicated fourth section; only that
    // section shapes the displayed text without mutating the underlying value.
    auto sections = DisplayTextFormatterSupport::ParseSections(formatCode);
    auto textSection =
        DisplayTextFormatterSupport::SelectTextSection(sections);
    if (!textSection.has_value()) {
        return std::string(value);
    }

    std::string pattern = DisplayTextFormatterSupport::StripDirectiveBrackets(
        textSection->GetRaw(), false);
    std::string formatted = DisplayTextFormatterSupport::ExpandSectionPattern(
        pattern, value, true);
    if (formatted.empty()) {
        return std::string(value);
    }

    return formatted;
}

// ===========================================================================
// FormatNumericValue  (private)
// ===========================================================================

std::string DisplayTextFormatter::FormatNumericValue(
    const CellValue& value, const Core::StyleValue& style,
    const std::locale& workbookCulture)
{
    std::string formatCode = NumberFormat::ResolveFormatCode(
        style.GetNumberFormat().GetNumber(),
        style.GetNumberFormat().GetCustom());

    if (IsWhitespaceOrEmpty(formatCode) ||
        EqualsOrdinal(formatCode, "General")) {
        return FormatStringValue(value);
    }

    // Numeric formats can split positive, negative, zero, and text behavior
    // into separate sections, so the display path first selects the matching
    // section.
    auto sections = DisplayTextFormatterSupport::ParseSections(formatCode);
    if (sections.empty()) {
        return FormatStringValue(value);
    }

    double numericValue = DisplayTextFormatterSupport::ConvertToDouble(value);
    bool useAbsoluteValue = false;
    auto selectedSection = DisplayTextFormatterSupport::SelectNumericSection(
        sections, numericValue, useAbsoluteValue);
    if (!selectedSection.has_value() ||
        IsWhitespaceOrEmpty(selectedSection->GetRaw())) {
        return FormatStringValue(value);
    }

    std::string fractionResult;
    // Fraction placeholders are not directly compatible with .NET numeric
    // format strings, so they are handled separately before standard
    // formatting.
    if (TryFormatFraction(numericValue, selectedSection->GetRaw(),
                          useAbsoluteValue, fractionResult)) {
        return fractionResult;
    }

    std::locale sectionCulture;
    std::string localizedSection =
        DisplayTextLocaleSupport::ApplyLocaleDirectives(
            selectedSection->GetRaw(), workbookCulture, sectionCulture);
    std::string sanitizedSection =
        DisplayTextFormatterSupport::SanitizeNumericSection(localizedSection);
    if (IsWhitespaceOrEmpty(sanitizedSection)) {
        return FormatStringValue(value);
    }

    if (!DisplayTextFormatterSupport::ContainsNumericPlaceholder(
            sanitizedSection)) {
        std::string literal =
            DisplayTextFormatterSupport::ExpandSectionPattern(
                sanitizedSection, std::string_view(), false);
        if (!literal.empty()) {
            return literal;
        }
        return FormatStringValue(value);
    }

    try {
        double formattedValue = numericValue;
        if (useAbsoluteValue && numericValue < 0) {
            formattedValue = std::fabs(numericValue);
        }

        // In Excel, literal characters such as parentheses around the
        // numeric pattern must appear in the output.  .NET's
        // IFormattable.ToString handles them natively, but our custom
        // formatter only understands format placeholders.  Strip
        // leading/trailing literal characters, format the core pattern,
        // and re-wrap.
        std::string prefix;
        std::string suffix;
        std::string corePattern;
        ExtractLiteralWrapping(sanitizedSection, prefix, suffix,
                               corePattern);

        if (corePattern.empty()) {
            return StripLiteralQuotes(prefix) + StripLiteralQuotes(suffix);
        }

        std::string formatted =
            FormatWithNumericPattern(formattedValue, corePattern,
                                     sectionCulture);
        return StripLiteralQuotes(prefix) + formatted +
               StripLiteralQuotes(suffix);
    } catch (const std::exception&) {
        return FormatStringValue(value);
    }
}

// ===========================================================================
// FormatDateTimeValue  (private)
// ===========================================================================

std::string DisplayTextFormatter::FormatDateTimeValue(
    const DateTime& value, const Core::StyleValue& style,
    const std::locale& workbookCulture)
{
    std::string formatCode = NumberFormat::ResolveFormatCode(
        style.GetNumberFormat().GetNumber(),
        style.GetNumberFormat().GetCustom());

    if (IsWhitespaceOrEmpty(formatCode) ||
        EqualsOrdinal(formatCode, "General")) {
        return FormatRawDateTimeValue(value);
    }

    // Date/time display uses the first section that still carries date tokens
    // after locale and directive cleanup.
    auto sections = DisplayTextFormatterSupport::ParseSections(formatCode);
    auto section =
        DisplayTextFormatterSupport::SelectDateTimeSection(sections);
    if (!section.has_value() || IsWhitespaceOrEmpty(section->GetRaw())) {
        return FormatRawDateTimeValue(value);
    }

    std::locale sectionCulture;
    std::string localizedSection =
        DisplayTextLocaleSupport::ApplyLocaleDirectives(
            section->GetRaw(), workbookCulture, sectionCulture);
    std::string sectionFormat =
        DisplayTextFormatterSupport::StripDirectiveBrackets(
            localizedSection, true);

    // Elapsed-time formats are duration-style output, not calendar rendering.
    if (DisplayTextDateFormatSupport::ContainsElapsedTimeToken(sectionFormat)) {
        return DisplayTextDateFormatSupport::FormatElapsedTimeValue(
            GetTimeOfDay(value), sectionFormat, sectionCulture);
    }

    if (IsWhitespaceOrEmpty(sectionFormat)) {
        return FormatRawDateTimeValue(value);
    }

    try {
        return DisplayTextDateFormatSupport::FormatDateTimeValue(
            value, sectionFormat, sectionCulture);
    } catch (const std::exception&) {
        return FormatRawDateTimeValue(value);
    }
}

// ===========================================================================
// FormatRawDateTimeValue  (private)
// ===========================================================================

std::string DisplayTextFormatter::FormatRawDateTimeValue(const DateTime& value)
{
    if (IsDateOnly(value)) {
        // M/d/yyyy (no leading zeros).
        std::string result;
        AppendInt(value.GetMonth(), result);
        result.push_back('/');
        AppendInt(value.GetDay(), result);
        result.push_back('/');
        AppendInt(value.GetYear(), result);
        return result;
    }

    // M/d/yyyy H:mm (no leading zeros on month, day, hour; 2-digit minute).
    std::string result;
    AppendInt(value.GetMonth(), result);
    result.push_back('/');
    AppendInt(value.GetDay(), result);
    result.push_back('/');
    AppendInt(value.GetYear(), result);
    result.push_back(' ');
    AppendInt(value.GetHour(), result);
    result.push_back(':');
    AppendIntPadded(value.GetMinute(), 2, result);
    return result;
}

// ===========================================================================
// TryFormatFraction  (private)
// ===========================================================================

bool DisplayTextFormatter::TryFormatFraction(double numericValue,
                                             std::string_view section,
                                             bool useAbsoluteValue,
                                             std::string& result)
{
    result.clear();
    std::string sanitizedSection =
        DisplayTextFormatterSupport::SanitizeNumericSection(section);
    if (sanitizedSection.find('/') == std::string::npos) {
        return false;
    }

    auto slashIndex = sanitizedSection.find('/');
    if (slashIndex == 0) {
        return false;
    }

    int denominatorDigits = 0;
    for (size_t index = slashIndex + 1; index < sanitizedSection.size();
         ++index) {
        char character = sanitizedSection[index];
        if (character == '#' || character == '0') {
            denominatorDigits++;
            continue;
        }
        if (std::isspace(static_cast<unsigned char>(character)) != 0) {
            continue;
        }
        break;
    }

    if (denominatorDigits <= 0) {
        return false;
    }

    double absoluteValue = std::fabs(numericValue);
    auto wholePart = static_cast<long>(std::floor(absoluteValue));
    double fractionalPart = absoluteValue - static_cast<double>(wholePart);
    if (fractionalPart < 1E-12) {
        result = FormatWholeFractionResult(wholePart, useAbsoluteValue,
                                           numericValue);
        return true;
    }

    int maxDenominator = 1;
    for (int index = 0; index < denominatorDigits; ++index) {
        maxDenominator *= 10;
    }

    // Match Excel's visible fraction behavior by finding the closest rational
    // value that fits within the placeholder width from the format section.
    maxDenominator -= 1;
    int bestNumerator = 0;
    int bestDenominator = 1;
    double bestError = std::numeric_limits<double>::max();

    for (int denominator = 1; denominator <= maxDenominator; ++denominator) {
        int numerator = static_cast<int>(std::round(
            fractionalPart * static_cast<double>(denominator)));
        if (numerator == 0) {
            continue;
        }
        if (numerator > denominator) {
            numerator = denominator;
        }

        double candidate = static_cast<double>(numerator) /
                           static_cast<double>(denominator);
        double error = std::fabs(fractionalPart - candidate);
        if (error < bestError) {
            bestError = error;
            bestNumerator = numerator;
            bestDenominator = denominator;
        }
    }

    if (bestNumerator == 0) {
        result = FormatWholeFractionResult(wholePart, useAbsoluteValue,
                                           numericValue);
        return true;
    }

    int gcd = GreatestCommonDivisor(bestNumerator, bestDenominator);
    bestNumerator /= gcd;
    bestDenominator /= gcd;

    if (bestNumerator == bestDenominator) {
        wholePart++;
        bestNumerator = 0;
    }

    std::string prefix;
    if (!useAbsoluteValue && numericValue < 0) {
        prefix = "-";
    }

    if (bestNumerator == 0) {
        result = prefix + std::to_string(wholePart);
        return true;
    }

    if (wholePart == 0) {
        result = prefix + std::to_string(bestNumerator) + "/" +
                 std::to_string(bestDenominator);
        return true;
    }

    result = prefix + std::to_string(wholePart) + " " +
             std::to_string(bestNumerator) + "/" +
             std::to_string(bestDenominator);
    return true;
}

// ===========================================================================
// FormatWholeFractionResult  (private)
// ===========================================================================

std::string DisplayTextFormatter::FormatWholeFractionResult(
    long wholePart, bool useAbsoluteValue, double numericValue)
{
    if (!useAbsoluteValue && numericValue < 0) {
        return "-" + std::to_string(wholePart);
    }
    return std::to_string(wholePart);
}

// ===========================================================================
// GreatestCommonDivisor  (private)
// ===========================================================================

int DisplayTextFormatter::GreatestCommonDivisor(int left, int right)
{
    int first = std::abs(left);
    int second = std::abs(right);
    while (second != 0) {
        int remainder = first % second;
        first = second;
        second = remainder;
    }
    if (first == 0) {
        return 1;
    }
    return first;
}

// ===========================================================================
// FormatWithNumericPattern  (private)
// ===========================================================================

std::string DisplayTextFormatter::FormatWithNumericPattern(
    double value, std::string_view pattern, const std::locale& culture)
{
    ParsedNumericFormat fmt = ParseNumericPattern(pattern);
    return ApplyNumericFormat(value, fmt, culture);
}

}}  // namespace Aspose::Cells_FOSS
