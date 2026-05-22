#pragma once

#include <optional>
#include <string>

#include "aspose/cells_foss/core/AutoFilterSortConditionModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents auto filter sort condition.
/// </summary>
class AutoFilterSortCondition final {
public:
    /// <summary>
    /// Initializes a new instance of the AutoFilterSortCondition class.
    /// </summary>
    /// <param name="model">The auto filter sort condition model.</param>
    explicit AutoFilterSortCondition(Core::AutoFilterSortConditionModel& model);

    /// <summary>
    /// Gets or sets the ref.
    /// </summary>
    std::string GetRef() const;
    void SetRef(const std::string& value);

    /// <summary>
    /// Gets or sets a value indicating whether descending.
    /// </summary>
    bool GetDescending() const noexcept;
    void SetDescending(bool value) noexcept;

    /// <summary>
    /// Gets or sets the sort by.
    /// </summary>
    std::string GetSortBy() const;
    void SetSortBy(const std::string& value);

    /// <summary>
    /// Gets or sets the custom list.
    /// </summary>
    std::string GetCustomList() const;
    void SetCustomList(const std::string& value);

    /// <summary>
    /// Gets or sets the differential style id.
    /// </summary>
    std::optional<int> GetDifferentialStyleId() const noexcept;
    void SetDifferentialStyleId(std::optional<int> value);

    /// <summary>
    /// Gets or sets the icon set.
    /// </summary>
    std::string GetIconSet() const;
    void SetIconSet(const std::string& value);

    /// <summary>
    /// Gets or sets the icon id.
    /// </summary>
    std::optional<int> GetIconId() const noexcept;
    void SetIconId(std::optional<int> value);

private:
    Core::AutoFilterSortConditionModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
