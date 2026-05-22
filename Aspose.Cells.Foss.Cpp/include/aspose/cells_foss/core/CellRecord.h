#pragma once

#include "aspose/cells_foss/CellValue.h"
#include "aspose/cells_foss/core/CellValueKind.h"
#include "aspose/cells_foss/core/StyleValue.h"

#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents cell record.
/// </summary>
class CellRecord final {
public:
    /// <summary>
    /// Initializes a new instance of the CellRecord class.
    /// </summary>
    CellRecord() = default;

    /// <summary>
    /// Gets or sets the value.
    /// </summary>
    const CellValue& GetValue() const noexcept { return _value; }
    void SetValue(const CellValue& value) { _value = value; }

    /// <summary>
    /// Gets or sets the kind.
    /// </summary>
    CellValueKind GetKind() const noexcept { return _kind; }
    void SetKind(CellValueKind value) noexcept { _kind = value; }

    /// <summary>
    /// Gets or sets the formula.
    /// </summary>
    const std::string& GetFormula() const noexcept { return _formula; }
    void SetFormula(const std::string& value) { _formula = value; }
    void SetFormula(std::string&& value) noexcept { _formula = std::move(value); }

    /// <summary>
    /// Gets or sets the style.
    /// </summary>
    const StyleValue& GetStyle() const noexcept { return _style; }
    void SetStyle(const StyleValue& value) { _style = value; }

    /// <summary>
    /// Gets or sets a value indicating whether explicitly stored.
    /// </summary>
    bool GetIsExplicitlyStored() const noexcept { return _isExplicitlyStored; }
    void SetIsExplicitlyStored(bool value) noexcept { _isExplicitlyStored = value; }

private:
    CellValue _value;
    CellValueKind _kind = CellValueKind::Blank;
    std::string _formula;
    StyleValue _style;
    bool _isExplicitlyStored = false;
};

}}}  // namespace Aspose::Cells_FOSS::Core
