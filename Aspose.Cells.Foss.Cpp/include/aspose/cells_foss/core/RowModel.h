#pragma once
#include <optional>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents row model.
/// </summary>
class RowModel final {
public:
    /// <summary>
    /// Initializes a new instance of the RowModel class.
    /// </summary>
    RowModel();

    /// <summary>
    /// Gets or sets the height.
    /// </summary>
    std::optional<double> GetHeight() const noexcept { return _height; }
    void SetHeight(std::optional<double> value) noexcept { _height = value; }

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
    std::optional<double> _height;
    bool _hidden = false;
    std::optional<int> _styleIndex;
};

}}}  // namespace Aspose::Cells_FOSS::Core
