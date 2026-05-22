#pragma once

#include "aspose/cells_foss/FilterOperatorType.h"
#include "aspose/cells_foss/core/FilterColumnModel.h"

#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

/// Internal helper methods for auto-filter operations.
class AutoFilterSupport {
public:
    AutoFilterSupport() = delete;

    /// Normalizes an optional range string, returning empty if blank.
    /// Throws CellsException if non-blank but not a valid range.
    static std::string NormalizeOptionalRange(std::string_view value,
                                              std::string_view parameterName);

    /// Normalizes a required range string, throwing if blank or invalid.
    static std::string NormalizeRequiredRange(std::string_view value, std::string_view parameterName);

    /// Attempts to normalize a range string. Returns true on success.
    static bool TryNormalizeRange(std::string_view value, std::string& normalized);

    /// Normalizes optional text by trimming; returns empty if blank.
    static std::string NormalizeOptionalText(std::string_view value);

    /// Validates that a text value is non-null. Throws ArgumentNullException equivalent
    /// if the value is null. Returns the value as a string.
    static std::string NormalizeText(std::string_view value,
                                     std::string_view parameterName);

    /// Parses an operator string, defaulting to Equal on blank input.
    static FilterOperatorType ParseOperatorOrDefault(std::string_view value);

    /// Parses an operator string, returning false on unrecognized values.
    static bool TryParseOperator(std::string_view value, FilterOperatorType& operatorType);

    /// Converts a FilterOperatorType to its XML attribute name, or null/empty for Equal.
    static std::string ToOperatorName(FilterOperatorType operatorType);

    /// Compares two filter columns by column index.
    static int CompareFilterColumns(const Core::FilterColumnModel& left,
                                    const Core::FilterColumnModel& right);
};

}}  // namespace Aspose::Cells_FOSS
