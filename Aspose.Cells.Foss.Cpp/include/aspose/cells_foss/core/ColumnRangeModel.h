#pragma once
#include <optional>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents column range model.
/// </summary>
class ColumnRangeModel final {
public:
    /// <summary>
    /// Initializes a new instance of the ColumnRangeModel class.
    /// </summary>
    ColumnRangeModel();

    /// <summary>
    /// Gets or sets the min column index.
    /// </summary>
    int GetMinColumnIndex() const noexcept { return _minColumnIndex; }
    void SetMinColumnIndex(int value) noexcept { _minColumnIndex = value; }

    /// <summary>
    /// Gets or sets the max column index.
    /// </summary>
    int GetMaxColumnIndex() const noexcept { return _maxColumnIndex; }
    void SetMaxColumnIndex(int value) noexcept { _maxColumnIndex = value; }

    /// <summary>
    /// Gets or sets the width.
    /// </summary>
    std::optional<double> GetWidth() const noexcept { return _width; }
    void SetWidth(std::optional<double> value) noexcept { _width = value; }

    /// <summary>
    /// Gets or sets a value indicating whether hidden.
    /// </summary>
    bool GetHidden() const noexcept { return _hidden; }
    void SetHidden(bool value) noexcept { _hidden = value; }

    /// <summary>
    /// Gets or sets the style index.
    /// </summary>
    std::optional<int> GetStyleIndex() const noexcept { return _styleIndex; }
    void SetStyleIndex(std::optional<int> value) noexcept { _styleIndex = value; }

private:
    int _minColumnIndex = 0;
    int _maxColumnIndex = 0;
    std::optional<double> _width;
    bool _hidden = false;
    std::optional<int> _styleIndex;
};

}}}  // namespace Aspose::Cells_FOSS::Core
