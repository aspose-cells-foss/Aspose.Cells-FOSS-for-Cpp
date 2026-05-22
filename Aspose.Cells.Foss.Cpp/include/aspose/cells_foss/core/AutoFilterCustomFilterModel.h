#pragma once
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents auto filter custom filter model.
/// </summary>
class AutoFilterCustomFilterModel {
public:
    AutoFilterCustomFilterModel() noexcept = default;

    /// <summary>
    /// Gets or sets the operator.
    /// </summary>
    const std::string& GetOperator() const noexcept { return _op; }
    void SetOperator(const std::string& value) { _op = value; }
    void SetOperator(std::string&& value) noexcept { _op = std::move(value); }

    /// <summary>
    /// Gets or sets the value.
    /// </summary>
    const std::string& GetValue() const noexcept { return _value; }
    void SetValue(const std::string& value) { _value = value; }
    void SetValue(std::string&& value) noexcept { _value = std::move(value); }

private:
    std::string _op;
    std::string _value;
};

}}}  // namespace Aspose::Cells_FOSS::Core
