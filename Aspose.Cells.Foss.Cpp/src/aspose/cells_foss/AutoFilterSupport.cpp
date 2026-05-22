#include "aspose/cells_foss/AutoFilterSupport.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"

#include <algorithm>
#include <cctype>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// NormalizeOptionalRange
// ---------------------------------------------------------------------------

std::string AutoFilterSupport::NormalizeOptionalRange(
    std::string_view value,
    std::string_view parameterName)
{
    std::string normalized;
    if (!TryNormalizeRange(value, normalized)) {
        // In C#, IsNullOrWhiteSpace returns true for null/blank, so TryNormalizeRange
        // returns false silently. But if the value had non-whitespace characters and
        // still failed parsing, we throw.
        bool allWhitespace = true;
        for (char ch : value) {
            if (!std::isspace(static_cast<unsigned char>(ch))) {
                allWhitespace = false;
                break;
            }
        }
        if (allWhitespace) {
            return {};
        }
        throw CellsException(std::string(parameterName) +
                             " must be a valid cell or range reference.");
    }
    return normalized;
}

// ---------------------------------------------------------------------------
// NormalizeRequiredRange
// ---------------------------------------------------------------------------

std::string AutoFilterSupport::NormalizeRequiredRange(
    std::string_view value,
    std::string_view parameterName)
{
    std::string normalized;
    if (!TryNormalizeRange(value, normalized)) {
        throw CellsException(std::string(parameterName) + " must be a valid cell or range reference.");
    }
    return normalized;
}

// ---------------------------------------------------------------------------
// TryNormalizeRange
// ---------------------------------------------------------------------------

bool AutoFilterSupport::TryNormalizeRange(std::string_view value, std::string& normalized)
{
    normalized.clear();

    // Check for blank/whitespace-only input (matches C# IsNullOrWhiteSpace).
    bool allWhitespace = true;
    for (char ch : value) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            allWhitespace = false;
            break;
        }
    }
    if (allWhitespace) {
        return false;
    }

    // Trim leading and trailing whitespace only (matches C# Trim()).
    const auto start = std::find_if_not(value.begin(), value.end(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); });
    const auto end = std::find_if_not(value.rbegin(), value.rend(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); }).base();
    if (start >= end) {
        return false;
    }
    const std::string trimmed(start, end);

    Core::MergeRegion region;
    if (!XlsxWorkbookSerializerCommon::TryParseMergeReference(trimmed, region)) {
        return false;
    }

    normalized = XlsxWorkbookSerializerCommon::ToRangeReference(region);
    return true;
}

// ---------------------------------------------------------------------------
// NormalizeOptionalText
// ---------------------------------------------------------------------------

std::string AutoFilterSupport::NormalizeOptionalText(std::string_view value)
{
    // Check for blank/whitespace-only input (matches C# IsNullOrWhiteSpace).
    bool allWhitespace = true;
    for (char ch : value) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            allWhitespace = false;
            break;
        }
    }
    if (allWhitespace) {
        return {};
    }

    // Trim leading and trailing whitespace only (matches C# Trim()).
    const auto start = std::find_if_not(value.begin(), value.end(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); });
    const auto end = std::find_if_not(value.rbegin(), value.rend(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); }).base();
    return std::string(start, end);
}

// ---------------------------------------------------------------------------
// NormalizeText
// ---------------------------------------------------------------------------

std::string AutoFilterSupport::NormalizeText(
    std::string_view value,
    std::string_view parameterName)
{
    if (value.data() == nullptr) {
        throw CellsException(std::string(parameterName) +
                             " cannot be null.");
    }
    return std::string(value);
}

// ---------------------------------------------------------------------------
// ParseOperatorOrDefault
// ---------------------------------------------------------------------------

FilterOperatorType AutoFilterSupport::ParseOperatorOrDefault(std::string_view value)
{
    // Check blank (matches C# IsNullOrWhiteSpace).
    bool allWhitespace = true;
    for (char ch : value) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            allWhitespace = false;
            break;
        }
    }
    if (allWhitespace) {
        return FilterOperatorType::Equal;
    }

    // Trim.
    const auto start = std::find_if_not(value.begin(), value.end(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); });
    const auto end = std::find_if_not(value.rbegin(), value.rend(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); }).base();
    const std::string trimmed(start, end);

    if (trimmed == "lessThan") return FilterOperatorType::LessThan;
    if (trimmed == "lessThanOrEqual") return FilterOperatorType::LessOrEqual;
    if (trimmed == "notEqual") return FilterOperatorType::NotEqual;
    if (trimmed == "greaterThanOrEqual") return FilterOperatorType::GreaterOrEqual;
    if (trimmed == "greaterThan") return FilterOperatorType::GreaterThan;
    return FilterOperatorType::Equal;
}

// ---------------------------------------------------------------------------
// TryParseOperator
// ---------------------------------------------------------------------------

bool AutoFilterSupport::TryParseOperator(std::string_view value, FilterOperatorType& operatorType)
{
    operatorType = FilterOperatorType::Equal;

    // Check blank (matches C# IsNullOrWhiteSpace).
    bool allWhitespace = true;
    for (char ch : value) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            allWhitespace = false;
            break;
        }
    }
    if (allWhitespace) {
        return true;
    }

    // Trim.
    const auto start = std::find_if_not(value.begin(), value.end(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); });
    const auto end = std::find_if_not(value.rbegin(), value.rend(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); }).base();
    const std::string trimmed(start, end);

    if (trimmed == "lessThan") { operatorType = FilterOperatorType::LessThan; return true; }
    if (trimmed == "lessThanOrEqual") { operatorType = FilterOperatorType::LessOrEqual; return true; }
    if (trimmed == "notEqual") { operatorType = FilterOperatorType::NotEqual; return true; }
    if (trimmed == "greaterThanOrEqual") { operatorType = FilterOperatorType::GreaterOrEqual; return true; }
    if (trimmed == "greaterThan") { operatorType = FilterOperatorType::GreaterThan; return true; }
    if (trimmed == "equal") { operatorType = FilterOperatorType::Equal; return true; }
    return false;
}

// ---------------------------------------------------------------------------
// ToOperatorName
// ---------------------------------------------------------------------------

std::string AutoFilterSupport::ToOperatorName(FilterOperatorType operatorType)
{
    switch (operatorType) {
    case FilterOperatorType::LessThan: return "lessThan";
    case FilterOperatorType::LessOrEqual: return "lessThanOrEqual";
    case FilterOperatorType::NotEqual: return "notEqual";
    case FilterOperatorType::GreaterOrEqual: return "greaterThanOrEqual";
    case FilterOperatorType::GreaterThan: return "greaterThan";
    default: return {};
    }
}

// ---------------------------------------------------------------------------
// CompareFilterColumns
// ---------------------------------------------------------------------------

int AutoFilterSupport::CompareFilterColumns(
    const Core::FilterColumnModel& left,
    const Core::FilterColumnModel& right)
{
    const int leftIdx = left.GetColumnIndex();
    const int rightIdx = right.GetColumnIndex();
    if (leftIdx < rightIdx) return -1;
    if (leftIdx > rightIdx) return 1;
    return 0;
}

}}  // namespace Aspose::Cells_FOSS
