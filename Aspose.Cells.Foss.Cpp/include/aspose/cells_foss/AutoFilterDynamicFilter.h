#pragma once

#include "aspose/cells_foss/core/AutoFilterDynamicFilterModel.h"

#include <optional>
#include <string>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents auto filter dynamic filter.
/// </summary>
class AutoFilterDynamicFilter final {
public:
    /// <summary>
    /// Initializes a new instance of the AutoFilterDynamicFilter class.
    /// </summary>
    /// <param name="model">The auto filter dynamic filter model.</param>
    explicit AutoFilterDynamicFilter(Core::AutoFilterDynamicFilterModel& model);

    /// <summary>
    /// Gets or sets a value indicating whether enabled.
    /// </summary>
    bool GetEnabled() const noexcept;
    void SetEnabled(bool value);

    /// <summary>
    /// Gets or sets the type.
    /// </summary>
    std::string GetType() const;
    void SetType(const std::string& value);

    /// <summary>
    /// Gets or sets the value.
    /// </summary>
    std::optional<double> GetValue() const noexcept;
    void SetValue(std::optional<double> value);

    /// <summary>
    /// Gets or sets the max value.
    /// </summary>
    std::optional<double> GetMaxValue() const noexcept;
    void SetMaxValue(std::optional<double> value);

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear();

private:
    Core::AutoFilterDynamicFilterModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
