#pragma once

#include "aspose/cells_foss/core/StyleValue.h"

#include <unordered_set>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Internal context used during stylesheet loading to accumulate cell formats,
/// differential formats, date style indexes, and the default cell style.
/// </summary>
class StylesheetLoadContext final {
public:
    /// <summary>
    /// Constructs a StylesheetLoadContext with default-initialized members.
    /// </summary>
    StylesheetLoadContext();

    /// <summary>
    /// Gets or sets the default cell style.
    /// Performs StyleValue::Default().Clone().
    /// </summary>
    const Core::StyleValue& GetDefaultCellStyle() const noexcept { return _defaultCellStyle; }
    void SetDefaultCellStyle(const Core::StyleValue& value) { _defaultCellStyle = value; }

    /// <summary>
    /// Gets the list of cell formats.
    /// Initialized with a single StyleValue::Default().Clone().
    /// </summary>
    const std::vector<Core::StyleValue>& GetCellFormats() const noexcept { return _cellFormats; }
    std::vector<Core::StyleValue>& GetCellFormats() noexcept { return _cellFormats; }

    /// <summary>
    /// Gets the list of differential formats.
    /// </summary>
    const std::vector<Core::StyleValue>& GetDifferentialFormats() const noexcept { return _differentialFormats; }
    std::vector<Core::StyleValue>& GetDifferentialFormats() noexcept { return _differentialFormats; }

    /// <summary>
    /// Gets the set of date style indexes.
    /// </summary>
    const std::unordered_set<int>& GetDateStyleIndexes() const noexcept { return _dateStyleIndexes; }
    std::unordered_set<int>& GetDateStyleIndexes() noexcept { return _dateStyleIndexes; }

private:
    Core::StyleValue _defaultCellStyle;
    std::vector<Core::StyleValue> _cellFormats;
    std::vector<Core::StyleValue> _differentialFormats;
    std::unordered_set<int> _dateStyleIndexes;
};

}}  // namespace Aspose::Cells_FOSS
