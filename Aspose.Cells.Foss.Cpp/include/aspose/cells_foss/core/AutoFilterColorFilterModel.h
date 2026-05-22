#pragma once
#include <optional>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents auto filter color filter model.
/// </summary>
class AutoFilterColorFilterModel {
public:
    AutoFilterColorFilterModel() noexcept = default;

    /// <summary>
    /// Gets or sets a value indicating whether enabled.
    /// </summary>
    bool GetEnabled() const noexcept { return _enabled; }
    void SetEnabled(bool value) noexcept { _enabled = value; }

    /// <summary>
    /// Gets or sets the differential style id.
    /// </summary>
    std::optional<int> GetDifferentialStyleId() const noexcept { return _differentialStyleId; }
    void SetDifferentialStyleId(std::optional<int> value) noexcept { _differentialStyleId = value; }

    /// <summary>
    /// Gets or sets a value indicating whether cell color.
    /// </summary>
    bool GetCellColor() const noexcept { return _cellColor; }
    void SetCellColor(bool value) noexcept { _cellColor = value; }

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear() noexcept;

private:
    bool _enabled = false;
    std::optional<int> _differentialStyleId;
    bool _cellColor = false;
};

}}}  // namespace Aspose::Cells_FOSS::Core
