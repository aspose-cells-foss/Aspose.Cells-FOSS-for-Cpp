#pragma once

#include "aspose/cells_foss/XmlDocument.h"
#include "aspose/cells_foss/core/CellRecord.h"
#include "aspose/cells_foss/core/FormatConditionModel.h"

#include <optional>
#include <string>
#include <unordered_map>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Internal context used during stylesheet saving to hold the built stylesheet
/// document together with style index maps and format counts.
/// </summary>
class StylesheetSaveContext final {
public:
    /// <summary>
    /// Initializes a new instance of the StylesheetSaveContext class.
    /// </summary>
    StylesheetSaveContext(
        XmlDocument document,
        std::unordered_map<std::string, int> styleIndices,
        std::unordered_map<std::string, int> differentialStyleIndices,
        int differentialFormatCount,
        bool hasStyles);

    /// <summary>
    /// Gets the built stylesheet XML document.
    /// </summary>
    const XmlDocument& GetDocument() const noexcept { return _document; }

    /// <summary>
    /// Gets the differential format count.
    /// </summary>
    int GetDifferentialFormatCount() const noexcept { return _differentialFormatCount; }

    /// <summary>
    /// Gets a value indicating whether the workbook has any non-default styles.
    /// </summary>
    bool GetHasStyles() const noexcept { return _hasStyles; }

    /// <summary>
    /// Gets the cell format index for the given cell record.
    /// Returns 0 if the style is not found.
    /// </summary>
    int GetStyleIndex(const Core::CellRecord& record) const;

    /// <summary>
    /// Gets the differential style index for the given format condition.
    /// Returns nullopt if the style is default or not found.
    /// </summary>
    std::optional<int> GetDifferentialStyleIndex(const Core::FormatConditionModel& condition) const;

private:
    XmlDocument _document;
    std::unordered_map<std::string, int> _styleIndices;
    std::unordered_map<std::string, int> _differentialStyleIndices;
    int _differentialFormatCount;
    bool _hasStyles;
};

}}  // namespace Aspose::Cells_FOSS
