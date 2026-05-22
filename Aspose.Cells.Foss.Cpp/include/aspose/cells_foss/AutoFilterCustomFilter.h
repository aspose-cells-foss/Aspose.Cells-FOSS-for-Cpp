#pragma once

#include "aspose/cells_foss/FilterOperatorType.h"
#include "aspose/cells_foss/core/AutoFilterCustomFilterModel.h"

#include <string>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents auto filter custom filter.
/// </summary>
class AutoFilterCustomFilter final {
public:
    /// <summary>
    /// Initializes a new instance of the AutoFilterCustomFilter class.
    /// </summary>
    /// <param name="model">The auto filter custom filter model.</param>
    explicit AutoFilterCustomFilter(Core::AutoFilterCustomFilterModel& model);

    /// <summary>
    /// Gets or sets the operator.
    /// </summary>
    FilterOperatorType GetOperator() const;
    void SetOperator(FilterOperatorType value);

    /// <summary>
    /// Gets or sets the value.
    /// </summary>
    const std::string& GetValue() const noexcept;
    void SetValue(const std::string& value);

private:
    Core::AutoFilterCustomFilterModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
