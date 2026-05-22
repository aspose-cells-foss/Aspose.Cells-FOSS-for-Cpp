#pragma once

#include "aspose/cells_foss/CellValue.h"
#include "aspose/cells_foss/CellValueType.h"
#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/core/CellAddress.h"
#include "aspose/cells_foss/core/CellRecord.h"
#include "aspose/cells_foss/core/CellValueKind.h"

#include <functional>
#include <optional>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

class Style;
class Worksheet;

/// <summary>
/// Represents a single worksheet cell and exposes value, formula, and style operations.
/// </summary>
class Cell {
public:
    /// <summary>
    /// Constructs a cell for the given worksheet at the specified address.
    /// </summary>
    Cell(Worksheet& worksheet, const Core::CellAddress& address);

    /// <summary>
    /// Gets the row index of the cell.
    /// </summary>
    int GetRow() const noexcept;

    /// <summary>
    /// Gets the column index of the cell.
    /// </summary>
    int GetColumn() const noexcept;

    /// <summary>
    /// Gets or sets the logical cell value.
    /// </summary>
    CellValue GetValue() const;
    void SetValue(const CellValue& value);

    /// <summary>
    /// Gets a stable string representation of the cell value without applying style-based display formatting.
    /// </summary>
    std::string GetStringValue() const;

    /// <summary>
    /// Gets the display text generated from the cell value, style, and workbook culture.
    /// </summary>
    std::string GetDisplayStringValue() const;

    /// <summary>
    /// Gets or sets the cell formula.
    /// </summary>
    std::string GetFormula() const;
    void SetFormula(std::string_view value);

    /// <summary>
    /// Gets the current logical cell value type.
    /// </summary>
    CellValueType GetType() const;

    /// <summary>
    /// Sets the cell value to a string.
    /// </summary>
    void PutValue(const char* value);
    void PutValue(std::string_view value);

    /// <summary>
    /// Sets the cell value to an integer.
    /// </summary>
    void PutValue(int value);

    /// <summary>
    /// Sets the cell value to a floating-point number.
    /// </summary>
    void PutValue(double value);

    /// <summary>
    /// Sets the cell value to a boolean.
    /// </summary>
    void PutValue(bool value);

    /// <summary>
    /// Sets the cell value to a DateTime.
    /// </summary>
    void PutValue(const DateTime& value);

    /// <summary>
    /// Gets a detached copy of the cell style.
    /// </summary>
    Style GetStyle() const;

    /// <summary>
    /// Replaces the cell style with the supplied style object.
    /// </summary>
    void SetStyle(const Style& style);

private:
    Worksheet& _worksheet;
    Core::CellAddress _address;

    std::optional<std::reference_wrapper<Core::CellRecord>> TryGetRecord();
    std::optional<std::reference_wrapper<const Core::CellRecord>> TryGetRecord() const;
    Core::CellRecord& GetOrCreateRecord();
    void SetScalar(const CellValue& value, Core::CellValueKind kind);
    void ClearValue();

    static std::string NormalizeFormula(std::string_view value);
};

}}  // namespace Aspose::Cells_FOSS
