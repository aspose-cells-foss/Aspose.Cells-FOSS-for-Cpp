#pragma once

#include <string>

namespace Aspose {
namespace Cells_FOSS {

/// Represents display format section info.
class DisplayFormatSectionInfo final {
public:
    /// Initializes a new instance of the DisplayFormatSectionInfo class.
    DisplayFormatSectionInfo();

    /// Gets or sets the raw.
    const std::string& GetRaw() const noexcept;
    void SetRaw(std::string_view value);

    /// Gets or sets a value indicating whether condition.
    bool GetHasCondition() const noexcept;
    void SetHasCondition(bool value) noexcept;

    /// Gets or sets the condition operator.
    const std::string& GetConditionOperator() const noexcept;
    void SetConditionOperator(std::string_view value);

    /// Gets or sets the condition value.
    double GetConditionValue() const noexcept;
    void SetConditionValue(double value) noexcept;

private:
    std::string _raw;
    bool _hasCondition = false;
    std::string _conditionOperator;
    double _conditionValue = 0.0;
};

}}  // namespace Aspose::Cells_FOSS
