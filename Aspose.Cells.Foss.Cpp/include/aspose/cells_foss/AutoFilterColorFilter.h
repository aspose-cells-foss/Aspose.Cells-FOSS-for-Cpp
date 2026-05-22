#pragma once

#include <optional>

#include "aspose/cells_foss/core/AutoFilterColorFilterModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents auto filter color filter.
/// </summary>
class AutoFilterColorFilter final {
public:
    /// <summary>
    /// Initializes a new instance of the AutoFilterColorFilter class.
    /// </summary>
    /// <param name="model">The auto filter color filter model.</param>
    explicit AutoFilterColorFilter(Core::AutoFilterColorFilterModel& model);

    /// <summary>
    /// Gets or sets a value indicating whether enabled.
    /// </summary>
    bool GetEnabled() const noexcept;
    void SetEnabled(bool value);

    /// <summary>
    /// Gets or sets the differential style id.
    /// </summary>
    std::optional<int> GetDifferentialStyleId() const noexcept;
    void SetDifferentialStyleId(std::optional<int> value);

    /// <summary>
    /// Gets or sets a value indicating whether cell color.
    /// </summary>
    bool GetCellColor() const noexcept;
    void SetCellColor(bool value);

    /// <summary>
    /// Clears the current state.
    /// </summary>
    void Clear();

private:
    Core::AutoFilterColorFilterModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
