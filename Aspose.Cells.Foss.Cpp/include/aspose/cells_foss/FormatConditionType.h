#pragma once

namespace Aspose {
namespace Cells_FOSS {

/// Specifies format condition type.
enum class FormatConditionType
{
    /// Cell value.
    CellValue = 0,
    /// Expression.
    Expression = 1,
    /// Contains text.
    ContainsText = 2,
    /// Not contains text.
    NotContainsText = 3,
    /// Begins with.
    BeginsWith = 4,
    /// Ends with.
    EndsWith = 5,
    /// Time period.
    TimePeriod = 6,
    /// Duplicate values.
    DuplicateValues = 7,
    /// Unique values.
    UniqueValues = 8,
    /// Top 10.
    Top10 = 9,
    /// Bottom 10.
    Bottom10 = 10,
    /// Above average.
    AboveAverage = 11,
    /// Below average.
    BelowAverage = 12,
    /// Color scale.
    ColorScale = 13,
    /// Data bar.
    DataBar = 14,
    /// Icon set.
    IconSet = 15,
};

}}  // namespace Aspose::Cells_FOSS
