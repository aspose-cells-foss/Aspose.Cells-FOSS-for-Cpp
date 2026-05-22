#pragma once

#include <optional>

#include "aspose/cells_foss/core/AutoFilterTop10Model.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents auto filter top10.
/// </summary>
class AutoFilterTop10 final {
public:
    /// <summary>
    /// Initializes a new instance of the AutoFilterTop10 class.
    /// </summary>
    /// <param name="model">The auto filter top10 model.</param>
    explicit AutoFilterTop10(Core::AutoFilterTop10Model& model);

    /// <summary>
    /// Gets or sets a value indicating whether enabled.
    /// </summary>
    bool GetEnabled() const noexcept;
    void SetEnabled(bool value);

    /// <summary>
    /// Gets or sets a value indicating whether top.
    /// </summary>
    bool GetTop() const noexcept;
    void SetTop(bool value);

    /// <summary>
    /// Gets or sets a value indicating whether percent.
    /// </summary>
    bool GetPercent() const noexcept;
    void SetPercent(bool value);

    /// <summary>
    /// Gets or sets the value.
    /// </summary>
    std::optional<double> GetValue() const noexcept;
    void SetValue(std::optional<double> value);

    /// <summary>
    /// Gets or sets the filter value.
    /// </summary>
    std::optional<double> GetFilterValue() const noexcept;
    void SetFilterValue(std::optional<double> value);

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear();

private:
    Core::AutoFilterTop10Model& _model;
};

}}  // namespace Aspose::Cells_FOSS
