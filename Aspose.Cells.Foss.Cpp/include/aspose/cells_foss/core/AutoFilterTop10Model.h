#pragma once
#include <optional>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents auto filter top10 model.
/// </summary>
class AutoFilterTop10Model {
public:
    AutoFilterTop10Model() noexcept = default;

    /// <summary>
    /// Gets or sets a value indicating whether enabled.
    /// </summary>
    bool GetEnabled() const noexcept { return _enabled; }
    void SetEnabled(bool value) noexcept { _enabled = value; }

    /// <summary>
    /// Gets or sets a value indicating whether top.
    /// </summary>
    bool GetTop() const noexcept { return _top; }
    void SetTop(bool value) noexcept { _top = value; }

    /// <summary>
    /// Gets or sets a value indicating whether percent.
    /// </summary>
    bool GetPercent() const noexcept { return _percent; }
    void SetPercent(bool value) noexcept { _percent = value; }

    /// <summary>
    /// Gets or sets the value.
    /// </summary>
    std::optional<double> GetValue() const noexcept { return _value; }
    void SetValue(std::optional<double> value) noexcept { _value = value; }

    /// <summary>
    /// Gets or sets the filter value.
    /// </summary>
    std::optional<double> GetFilterValue() const noexcept { return _filterValue; }
    void SetFilterValue(std::optional<double> value) noexcept { _filterValue = value; }

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear() noexcept;

private:
    bool _enabled = false;
    bool _top = true;
    bool _percent = false;
    std::optional<double> _value;
    std::optional<double> _filterValue;
};

}}}  // namespace Aspose::Cells_FOSS::Core
