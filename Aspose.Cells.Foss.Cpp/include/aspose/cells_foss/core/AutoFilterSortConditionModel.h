#pragma once
#include <optional>
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents auto filter sort condition model.
/// </summary>
class AutoFilterSortConditionModel {
public:
    AutoFilterSortConditionModel() noexcept = default;

    /// <summary>
    /// Gets or sets the ref.
    /// </summary>
    const std::string& GetRef() const noexcept { return _ref; }
    void SetRef(const std::string& value) { _ref = value; }
    void SetRef(std::string&& value) noexcept { _ref = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether descending.
    /// </summary>
    bool GetDescending() const noexcept { return _descending; }
    void SetDescending(bool value) noexcept { _descending = value; }

    /// <summary>
    /// Gets or sets the sort by.
    /// </summary>
    const std::string& GetSortBy() const noexcept { return _sortBy; }
    void SetSortBy(const std::string& value) { _sortBy = value; }
    void SetSortBy(std::string&& value) noexcept { _sortBy = std::move(value); }

    /// <summary>
    /// Gets or sets the custom list.
    /// </summary>
    const std::string& GetCustomList() const noexcept { return _customList; }
    void SetCustomList(const std::string& value) { _customList = value; }
    void SetCustomList(std::string&& value) noexcept { _customList = std::move(value); }

    /// <summary>
    /// Gets or sets the differential style id.
    /// </summary>
    std::optional<int> GetDifferentialStyleId() const noexcept { return _differentialStyleId; }
    void SetDifferentialStyleId(std::optional<int> value) noexcept { _differentialStyleId = value; }

    /// <summary>
    /// Gets or sets the icon set.
    /// </summary>
    const std::string& GetIconSet() const noexcept { return _iconSet; }
    void SetIconSet(const std::string& value) { _iconSet = value; }
    void SetIconSet(std::string&& value) noexcept { _iconSet = std::move(value); }

    /// <summary>
    /// Gets or sets the icon id.
    /// </summary>
    std::optional<int> GetIconId() const noexcept { return _iconId; }
    void SetIconId(std::optional<int> value) noexcept { _iconId = value; }

private:
    std::string _ref;
    bool _descending = false;
    std::string _sortBy;
    std::string _customList;
    std::optional<int> _differentialStyleId;
    std::string _iconSet;
    std::optional<int> _iconId;
};

}}}  // namespace Aspose::Cells_FOSS::Core
