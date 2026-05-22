#pragma once

#include <optional>
#include <string_view>
#include <vector>

#include "aspose/cells_foss/Hyperlink.h"
#include "aspose/cells_foss/core/HyperlinkModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Encapsulates the hyperlinks defined for a worksheet.
/// </summary>
class HyperlinkCollection final {
public:
    /// <summary>
    /// Initializes a new instance of the HyperlinkCollection class.
    /// </summary>
    /// <param name="hyperlinks">The underlying hyperlink models.</param>
    explicit HyperlinkCollection(std::vector<Core::HyperlinkModel>& hyperlinks);

    /// <summary>
    /// Gets the number of hyperlinks in the worksheet.
    /// </summary>
    int GetCount() const noexcept;

    /// <summary>
    /// Gets the hyperlink at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    /// <returns>The hyperlink at the given index.</returns>
    Hyperlink operator[](int index);

    /// <summary>
    /// Adds a hyperlink anchored at a cell or rectangular range specified by zero-based coordinates.
    /// </summary>
    /// <param name="firstRow">The zero-based first row index.</param>
    /// <param name="firstColumn">The zero-based first column index.</param>
    /// <param name="totalRows">The total number of rows.</param>
    /// <param name="totalColumns">The total number of columns.</param>
    /// <param name="address">The hyperlink address.</param>
    /// <returns>The zero-based index of the added hyperlink.</returns>
    int Add(int firstRow, int firstColumn, int totalRows, int totalColumns, std::string_view address);

    /// <summary>
    /// Adds a hyperlink anchored at a cell or rectangular range specified by its top-left A1 reference.
    /// </summary>
    /// <param name="cellName">The top-left cell reference in A1 notation.</param>
    /// <param name="totalRows">The total number of rows.</param>
    /// <param name="totalColumns">The total number of columns.</param>
    /// <param name="address">The hyperlink address.</param>
    /// <returns>The zero-based index of the added hyperlink.</returns>
    int Add(std::string_view cellName, int totalRows, int totalColumns, std::string_view address);

    /// <summary>
    /// Adds a hyperlink over the specified A1 range and optional display text metadata.
    /// </summary>
    /// <param name="startCellName">The start cell reference in A1 notation.</param>
    /// <param name="endCellName">The end cell reference in A1 notation.</param>
    /// <param name="address">The hyperlink address.</param>
    /// <param name="textToDisplay">The display text, or empty for none.</param>
    /// <param name="screenTip">The screen tip text, or empty for none.</param>
    /// <returns>The zero-based index of the added hyperlink.</returns>
    int Add(std::string_view startCellName, std::string_view endCellName,
            std::string_view address, std::string_view textToDisplay,
            std::string_view screenTip);

    /// <summary>
    /// Removes the hyperlink at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based index of the hyperlink to remove.</param>
    void RemoveAt(int index);

private:
    std::vector<Core::HyperlinkModel>& _hyperlinks;

    int AddInternal(std::string_view cellName, int totalRows, int totalColumns,
                    std::string_view address, std::optional<int> firstRowOverride,
                    std::optional<int> firstColumnOverride);

    static void AssignAddress(Core::HyperlinkModel& model, std::string_view address);
    static bool Overlaps(const Core::HyperlinkModel& left, const Core::HyperlinkModel& right);
    static std::string NormalizeText(std::string_view value);
};

}}  // namespace Aspose::Cells_FOSS
