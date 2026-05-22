#pragma once

#include "aspose/cells_foss/core/AlignmentValue.h"
#include "aspose/cells_foss/core/ProtectionValue.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents cell format value.
/// </summary>
class CellFormatValue final {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="CellFormatValue"/> class.
    /// </summary>
    CellFormatValue() noexcept = default;

    /// <summary>
    /// Gets or sets the num fmt id.
    /// </summary>
    int GetNumFmtId() const noexcept { return _numFmtId; }
    void SetNumFmtId(int value) noexcept { _numFmtId = value; }

    /// <summary>
    /// Gets or sets the font id.
    /// </summary>
    int GetFontId() const noexcept { return _fontId; }
    void SetFontId(int value) noexcept { _fontId = value; }

    /// <summary>
    /// Gets or sets the fill id.
    /// </summary>
    int GetFillId() const noexcept { return _fillId; }
    void SetFillId(int value) noexcept { _fillId = value; }

    /// <summary>
    /// Gets or sets the border id.
    /// </summary>
    int GetBorderId() const noexcept { return _borderId; }
    void SetBorderId(int value) noexcept { _borderId = value; }

    /// <summary>
    /// Performs alignment value.
    /// </summary>
    /// <returns>The alignment value.</returns>
    const Core::AlignmentValue& GetAlignment() const noexcept { return _alignment; }
    void SetAlignment(const Core::AlignmentValue& value) { _alignment = value; }

    /// <summary>
    /// Performs protection value.
    /// </summary>
    /// <returns>The protection value.</returns>
    const Core::ProtectionValue& GetProtection() const noexcept { return _protection; }
    void SetProtection(const Core::ProtectionValue& value) { _protection = value; }

private:
    int _numFmtId = 0;
    int _fontId = 0;
    int _fillId = 0;
    int _borderId = 0;
    Core::AlignmentValue _alignment;
    Core::ProtectionValue _protection;
};

}}  // namespace Aspose::Cells_FOSS
