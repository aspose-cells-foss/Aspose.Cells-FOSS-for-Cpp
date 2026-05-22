#pragma once

#include "aspose/cells_foss/CellValue.h"
#include "aspose/cells_foss/DisplayFormatSectionInfo.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Internal helper methods for display-text formatting of numeric, text,
/// and date/time values.
class DisplayTextFormatterSupport {
public:
    DisplayTextFormatterSupport() = delete;

    /// Selects the appropriate numeric section from a parsed format code.
    /// Returns std::nullopt when sections is empty.
    static std::optional<DisplayFormatSectionInfo> SelectNumericSection(
        const std::vector<DisplayFormatSectionInfo>& sections,
        double numericValue, bool& useAbsoluteValue);

    /// Selects the text section (fourth section, or first containing '@').
    /// Returns std::nullopt if no text section is found.
    static std::optional<DisplayFormatSectionInfo> SelectTextSection(
        const std::vector<DisplayFormatSectionInfo>& sections);

    /// Selects the first non-empty date/time section (among the first three).
    /// Returns std::nullopt if all are blank.
    static std::optional<DisplayFormatSectionInfo> SelectDateTimeSection(
        const std::vector<DisplayFormatSectionInfo>& sections);

    /// Parses a format code into section descriptors.
    static std::vector<DisplayFormatSectionInfo> ParseSections(
        std::string_view formatCode);

    /// Splits a format code on unquoted, unescaped semicolons.
    static std::vector<std::string> SplitSections(
        std::string_view formatCode);

    /// Attempts to parse a condition (operator + value) from a section string.
    static bool TryParseSectionCondition(std::string_view section,
        std::string& conditionOperator, double& conditionValue);

    /// Attempts to parse a condition token such as ">=10".
    static bool TryParseConditionToken(std::string_view token,
        std::string& conditionOperator, double& conditionValue);

    /// Evaluates a comparison between numericValue and conditionValue.
    static bool EvaluateCondition(std::string_view conditionOperator,
        double numericValue, double conditionValue);

    /// Determines whether the absolute value of a negative number should be
    /// used when formatting (i.e. the pattern has no explicit '-' sign).
    static bool ShouldUseAbsoluteValue(std::string_view section,
        double numericValue);

    /// Strips directive brackets, quote escapes, and placeholder characters
    /// from a numeric section, returning the cleaned string (edge-trimmed).
    static std::string SanitizeNumericSection(std::string_view section);

    /// Removes bracketed directive tokens from a section string.
    static std::string StripDirectiveBrackets(std::string_view section,
        bool preserveElapsedTokens);

    /// Returns true if the pattern contains any numeric placeholder character.
    static bool ContainsNumericPlaceholder(std::string_view pattern);

    /// Expands a section pattern by replacing text placeholders and removing
    /// formatting escapes.
    static std::string ExpandSectionPattern(std::string_view pattern,
        std::string_view valueText, bool replaceTextPlaceholder);

    /// Returns true if the CellValue holds a numeric type (int or double).
    static bool IsNumericValue(const CellValue& value);

    /// Converts a numeric CellValue to double.
    /// Throws std::invalid_argument if the value is not numeric.
    static double ConvertToDouble(const CellValue& value);

    /// Returns the absolute numeric value.  Non-numeric values are returned
    /// unchanged.
    static CellValue GetAbsoluteNumericValue(const CellValue& value);
};

}}  // namespace Aspose::Cells_FOSS
