#pragma once
#include <optional>
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents auto filter dynamic filter model.
/// </summary>
class AutoFilterDynamicFilterModel {
public:
    AutoFilterDynamicFilterModel() noexcept = default;

    /// <summary>
    /// Gets or sets a value indicating whether enabled.
    /// </summary>
    bool GetEnabled() const noexcept { return _enabled; }
    void SetEnabled(bool value) noexcept { _enabled = value; }

    /// <summary>
    /// Gets or sets the type.
    /// </summary>
    const std::string& GetType() const noexcept { return _type; }
    void SetType(const std::string& value) { _type = value; }
    void SetType(std::string&& value) noexcept { _type = std::move(value); }

    /// <summary>
    /// Gets or sets the value.
    /// </summary>
    std::optional<double> GetValue() const noexcept { return _value; }
    void SetValue(std::optional<double> value) noexcept { _value = value; }

    /// <summary>
    /// Gets or sets the max value.
    /// </summary>
    std::optional<double> GetMaxValue() const noexcept { return _maxValue; }
    void SetMaxValue(std::optional<double> value) noexcept { _maxValue = value; }

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear() noexcept;

private:
    bool _enabled = false;
    std::string _type;
    std::optional<double> _value;
    std::optional<double> _maxValue;
};

}}}  // namespace Aspose::Cells_FOSS::Core
