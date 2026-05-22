#include "aspose/cells_foss/DisplayTextFormatterSupport.h"
#include "aspose/cells_foss/DisplayTextDateFormatSupport.h"

#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Internal helpers (file-scoped)
// ---------------------------------------------------------------------------

namespace {

bool IsAllWhitespace(std::string_view s) noexcept
{
    for (char c : s) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

std::string_view TrimView(std::string_view s) noexcept
{
    while (!s.empty() &&
           std::isspace(static_cast<unsigned char>(s.front()))) {
        s.remove_prefix(1);
    }
    while (!s.empty() &&
           std::isspace(static_cast<unsigned char>(s.back()))) {
        s.remove_suffix(1);
    }
    return s;
}

bool TryParseDoubleInvariant(std::string_view str, double& result)
{
    std::string_view trimmed = TrimView(str);
    if (trimmed.empty()) {
        return false;
    }

    std::string buf(trimmed);
    char* end = nullptr;
    errno = 0;
    double val = std::strtod(buf.c_str(), &end);
    if (end == buf.c_str() || errno == ERANGE) {
        return false;
    }

    // Allow trailing whitespace only.
    while (end != buf.c_str() + buf.size()) {
        if (!std::isspace(static_cast<unsigned char>(*end))) {
            return false;
        }
        ++end;
    }

    result = val;
    return true;
}

}  // anonymous namespace

// ---------------------------------------------------------------------------
// SelectNumericSection
// ---------------------------------------------------------------------------

std::optional<DisplayFormatSectionInfo>
DisplayTextFormatterSupport::SelectNumericSection(
    const std::vector<DisplayFormatSectionInfo>& sections,
    double numericValue, bool& useAbsoluteValue)
{
    useAbsoluteValue = false;

    const auto sectionCount = static_cast<int>(sections.size());
    const int numericSectionCount = sectionCount < 3 ? sectionCount : 3;
    if (numericSectionCount == 0) {
        return std::nullopt;
    }

    bool hasCondition = false;
    int defaultSectionIndex = -1;

    for (int index = 0; index < numericSectionCount; ++index) {
        const DisplayFormatSectionInfo& section = sections[index];
        if (section.GetHasCondition()) {
            hasCondition = true;
            if (EvaluateCondition(section.GetConditionOperator(),
                                  numericValue,
                                  section.GetConditionValue())) {
                useAbsoluteValue =
                    ShouldUseAbsoluteValue(section.GetRaw(), numericValue);
                return section;
            }
        } else if (defaultSectionIndex < 0) {
            defaultSectionIndex = index;
        }
    }

    if (hasCondition && defaultSectionIndex >= 0) {
        useAbsoluteValue = ShouldUseAbsoluteValue(
            sections[defaultSectionIndex].GetRaw(), numericValue);
        return sections[defaultSectionIndex];
    }

    if (numericSectionCount == 1) {
        useAbsoluteValue =
            ShouldUseAbsoluteValue(sections[0].GetRaw(), numericValue);
        return sections[0];
    }

    if (numericSectionCount == 2) {
        if (numericValue < 0) {
            useAbsoluteValue =
                ShouldUseAbsoluteValue(sections[1].GetRaw(), numericValue);
            return sections[1];
        }
        useAbsoluteValue =
            ShouldUseAbsoluteValue(sections[0].GetRaw(), numericValue);
        return sections[0];
    }

    if (numericValue > 0) {
        useAbsoluteValue =
            ShouldUseAbsoluteValue(sections[0].GetRaw(), numericValue);
        return sections[0];
    }

    if (numericValue < 0) {
        useAbsoluteValue =
            ShouldUseAbsoluteValue(sections[1].GetRaw(), numericValue);
        return sections[1];
    }

    useAbsoluteValue = false;
    return sections[2];
}

// ---------------------------------------------------------------------------
// SelectTextSection
// ---------------------------------------------------------------------------

std::optional<DisplayFormatSectionInfo>
DisplayTextFormatterSupport::SelectTextSection(
    const std::vector<DisplayFormatSectionInfo>& sections)
{
    if (sections.size() >= 4) {
        return sections[3];
    }

    for (std::size_t index = 0; index < sections.size(); ++index) {
        std::string stripped =
            StripDirectiveBrackets(sections[index].GetRaw(), false);
        if (stripped.find('@') != std::string::npos) {
            return sections[index];
        }
    }

    return std::nullopt;
}

// ---------------------------------------------------------------------------
// SelectDateTimeSection
// ---------------------------------------------------------------------------

std::optional<DisplayFormatSectionInfo>
DisplayTextFormatterSupport::SelectDateTimeSection(
    const std::vector<DisplayFormatSectionInfo>& sections)
{
    const auto sectionCount = static_cast<int>(sections.size());
    const int dateSectionCount = sectionCount < 3 ? sectionCount : 3;
    for (int index = 0; index < dateSectionCount; ++index) {
        if (!IsAllWhitespace(sections[index].GetRaw())) {
            return sections[index];
        }
    }

    return std::nullopt;
}

// ---------------------------------------------------------------------------
// ParseSections
// ---------------------------------------------------------------------------

std::vector<DisplayFormatSectionInfo>
DisplayTextFormatterSupport::ParseSections(std::string_view formatCode)
{
    std::vector<std::string> rawSections = SplitSections(formatCode);
    std::vector<DisplayFormatSectionInfo> sections;
    sections.reserve(rawSections.size());

    for (std::size_t index = 0; index < rawSections.size(); ++index) {
        const std::string& rawSection = rawSections[index];
        DisplayFormatSectionInfo section;
        section.SetRaw(rawSection);

        std::string conditionOperator;
        double conditionValue = 0.0;
        if (TryParseSectionCondition(rawSection, conditionOperator,
                                     conditionValue)) {
            section.SetHasCondition(true);
            section.SetConditionOperator(conditionOperator);
            section.SetConditionValue(conditionValue);
        }

        sections.push_back(std::move(section));
    }

    return sections;
}

// ---------------------------------------------------------------------------
// SplitSections
// ---------------------------------------------------------------------------

std::vector<std::string>
DisplayTextFormatterSupport::SplitSections(std::string_view formatCode)
{
    std::vector<std::string> sections;
    std::string builder;
    bool inQuote = false;

    for (std::size_t index = 0; index < formatCode.size(); ++index) {
        const char character = formatCode[index];
        if (character == '"') {
            builder.push_back(character);
            inQuote = !inQuote;
            continue;
        }

        if (!inQuote) {
            if (character == '\\' && index + 1 < formatCode.size()) {
                builder.push_back(character);
                ++index;
                builder.push_back(formatCode[index]);
                continue;
            }

            if (character == ';') {
                sections.push_back(std::move(builder));
                builder.clear();
                continue;
            }
        }

        builder.push_back(character);
    }

    sections.push_back(std::move(builder));
    return sections;
}

// ---------------------------------------------------------------------------
// TryParseSectionCondition
// ---------------------------------------------------------------------------

bool DisplayTextFormatterSupport::TryParseSectionCondition(
    std::string_view section, std::string& conditionOperator,
    double& conditionValue)
{
    conditionOperator.clear();
    conditionValue = 0.0;
    bool inQuote = false;

    for (std::size_t index = 0; index < section.size(); ++index) {
        const char character = section[index];
        if (character == '"') {
            inQuote = !inQuote;
            continue;
        }

        if (inQuote || character != '[') {
            continue;
        }

        const auto endIndex = section.find(']', index + 1);
        if (endIndex == std::string_view::npos) {
            break;
        }

        std::string_view token =
            section.substr(index + 1, endIndex - index - 1);
        std::string_view trimmedToken = TrimView(token);
        if (TryParseConditionToken(trimmedToken, conditionOperator,
                                   conditionValue)) {
            return true;
        }

        index = endIndex;
    }

    return false;
}

// ---------------------------------------------------------------------------
// TryParseConditionToken
// ---------------------------------------------------------------------------

bool DisplayTextFormatterSupport::TryParseConditionToken(
    std::string_view token, std::string& conditionOperator,
    double& conditionValue)
{
    conditionOperator.clear();
    conditionValue = 0.0;
    if (IsAllWhitespace(token)) {
        return false;
    }

    static constexpr const char* operators[] = {
        ">=", "<=", "<>", ">", "<", "="};

    for (std::size_t i = 0; i < 6; ++i) {
        const char* candidate = operators[i];
        const std::size_t candidateLen = std::strlen(candidate);
        if (token.size() >= candidateLen &&
            token.substr(0, candidateLen) ==
                std::string_view(candidate, candidateLen)) {
            std::string_view numberPart =
                TrimView(token.substr(candidateLen));
            double parsedValue = 0.0;
            if (TryParseDoubleInvariant(numberPart, parsedValue)) {
                conditionOperator = candidate;
                conditionValue = parsedValue;
                return true;
            }
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
// EvaluateCondition
// ---------------------------------------------------------------------------

bool DisplayTextFormatterSupport::EvaluateCondition(
    std::string_view conditionOperator, double numericValue,
    double conditionValue)
{
    if (conditionOperator == ">") {
        return numericValue > conditionValue;
    }
    if (conditionOperator == ">=") {
        return numericValue >= conditionValue;
    }
    if (conditionOperator == "<") {
        return numericValue < conditionValue;
    }
    if (conditionOperator == "<=") {
        return numericValue <= conditionValue;
    }
    if (conditionOperator == "=") {
        return std::fabs(numericValue - conditionValue) < 1E-12;
    }
    if (conditionOperator == "<>") {
        return std::fabs(numericValue - conditionValue) >= 1E-12;
    }
    return false;
}

// ---------------------------------------------------------------------------
// ShouldUseAbsoluteValue
// ---------------------------------------------------------------------------

bool DisplayTextFormatterSupport::ShouldUseAbsoluteValue(
    std::string_view section, double numericValue)
{
    if (numericValue >= 0) {
        return false;
    }

    std::string sanitizedSection = SanitizeNumericSection(section);
    return sanitizedSection.find('-') == std::string::npos;
}

// ---------------------------------------------------------------------------
// SanitizeNumericSection
// ---------------------------------------------------------------------------

std::string DisplayTextFormatterSupport::SanitizeNumericSection(
    std::string_view section)
{
    std::string withoutDirectives = StripDirectiveBrackets(section, false);
    std::string builder;
    builder.reserve(withoutDirectives.size());
    bool inQuote = false;

    for (std::size_t index = 0; index < withoutDirectives.size(); ++index) {
        const char character = withoutDirectives[index];
        if (character == '"') {
            builder.push_back(character);
            inQuote = !inQuote;
            continue;
        }

        if (!inQuote) {
            if (character == '_') {
                ++index;
                continue;
            }

            if (character == '*') {
                ++index;
                continue;
            }

            if (character == '\\') {
                if (index + 1 < withoutDirectives.size()) {
                    ++index;
                    builder.push_back(withoutDirectives[index]);
                }
                continue;
            }

            if (character == '?') {
                builder.push_back('#');
                continue;
            }

            if (character == '[' || character == ']') {
                continue;
            }
        }

        builder.push_back(character);
    }

    // Edge-trim only (matches C# Trim()).
    std::string_view trimmed = TrimView(builder);
    return std::string(trimmed);
}

// ---------------------------------------------------------------------------
// StripDirectiveBrackets
// ---------------------------------------------------------------------------

std::string DisplayTextFormatterSupport::StripDirectiveBrackets(
    std::string_view section, bool preserveElapsedTokens)
{
    std::string builder;
    builder.reserve(section.size());
    bool inQuote = false;

    for (std::size_t index = 0; index < section.size(); ++index) {
        const char character = section[index];
        if (character == '"') {
            builder.push_back(character);
            inQuote = !inQuote;
            continue;
        }

        if (!inQuote && character == '[') {
            const auto endIndex = section.find(']', index + 1);
            if (endIndex == std::string_view::npos) {
                continue;
            }

            std::string_view token =
                section.substr(index + 1, endIndex - index - 1);
            if (preserveElapsedTokens &&
                DisplayTextDateFormatSupport::IsElapsedToken(token)) {
                builder.push_back('[');
                builder.append(token.data(), token.size());
                builder.push_back(']');
            }

            index = endIndex;
            continue;
        }

        builder.push_back(character);
    }

    return builder;
}

// ---------------------------------------------------------------------------
// ContainsNumericPlaceholder
// ---------------------------------------------------------------------------

bool DisplayTextFormatterSupport::ContainsNumericPlaceholder(
    std::string_view pattern)
{
    for (std::size_t index = 0; index < pattern.size(); ++index) {
        const char character = pattern[index];
        if (character == '0' || character == '#' || character == '?' ||
            character == '.' || character == '%' || character == 'E' ||
            character == 'e' || character == '/') {
            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
// ExpandSectionPattern
// ---------------------------------------------------------------------------

std::string DisplayTextFormatterSupport::ExpandSectionPattern(
    std::string_view pattern, std::string_view valueText,
    bool replaceTextPlaceholder)
{
    std::string builder;
    builder.reserve(pattern.size() + valueText.size());
    bool inQuote = false;

    for (std::size_t index = 0; index < pattern.size(); ++index) {
        const char character = pattern[index];
        if (character == '"') {
            inQuote = !inQuote;
            continue;
        }

        if (!inQuote) {
            if (character == '@' && replaceTextPlaceholder) {
                builder.append(valueText.data(), valueText.size());
                continue;
            }

            if (character == '_') {
                ++index;
                continue;
            }

            if (character == '*') {
                ++index;
                continue;
            }

            if (character == '\\') {
                if (index + 1 < pattern.size()) {
                    ++index;
                    builder.push_back(pattern[index]);
                }
                continue;
            }
        }

        builder.push_back(character);
    }

    return builder;
}

// ---------------------------------------------------------------------------
// IsNumericValue
// ---------------------------------------------------------------------------

bool DisplayTextFormatterSupport::IsNumericValue(const CellValue& value)
{
    return value.IsInteger() || value.IsDouble();
}

// ---------------------------------------------------------------------------
// ConvertToDouble
// ---------------------------------------------------------------------------

double DisplayTextFormatterSupport::ConvertToDouble(const CellValue& value)
{
    if (value.IsDouble()) {
        return value.AsDouble();
    }
    if (value.IsInteger()) {
        return static_cast<double>(value.AsInteger());
    }
    throw std::invalid_argument("Value is not numeric.");
}

// ---------------------------------------------------------------------------
// GetAbsoluteNumericValue
// ---------------------------------------------------------------------------

CellValue DisplayTextFormatterSupport::GetAbsoluteNumericValue(
    const CellValue& value)
{
    if (value.IsInteger()) {
        return CellValue(std::abs(value.AsInteger()));
    }
    if (value.IsDouble()) {
        return CellValue(std::fabs(value.AsDouble()));
    }
    return value;
}

}}  // namespace Aspose::Cells_FOSS
