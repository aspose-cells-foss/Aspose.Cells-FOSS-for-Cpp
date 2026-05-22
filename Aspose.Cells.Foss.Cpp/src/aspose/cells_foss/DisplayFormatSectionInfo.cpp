#include "aspose/cells_foss/DisplayFormatSectionInfo.h"

namespace Aspose {
namespace Cells_FOSS {

DisplayFormatSectionInfo::DisplayFormatSectionInfo() = default;

const std::string& DisplayFormatSectionInfo::GetRaw() const noexcept
{
    return _raw;
}

void DisplayFormatSectionInfo::SetRaw(std::string_view value)
{
    _raw = value;
}

bool DisplayFormatSectionInfo::GetHasCondition() const noexcept
{
    return _hasCondition;
}

void DisplayFormatSectionInfo::SetHasCondition(bool value) noexcept
{
    _hasCondition = value;
}

const std::string& DisplayFormatSectionInfo::GetConditionOperator() const noexcept
{
    return _conditionOperator;
}

void DisplayFormatSectionInfo::SetConditionOperator(std::string_view value)
{
    _conditionOperator = value;
}

double DisplayFormatSectionInfo::GetConditionValue() const noexcept
{
    return _conditionValue;
}

void DisplayFormatSectionInfo::SetConditionValue(double value) noexcept
{
    _conditionValue = value;
}

}}  // namespace Aspose::Cells_FOSS
