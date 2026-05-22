#include "aspose/cells_foss/FormatCondition.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/FormatConditionCollection.h"

namespace Aspose {
namespace Cells_FOSS {

FormatCondition::FormatCondition(
    std::vector<Core::ConditionalFormattingModel>& owner,
    Core::ConditionalFormattingModel& collection,
    Core::FormatConditionModel& model)
    : _owner(owner)
    , _collection(collection)
    , _model(model)
{
}

FormatConditionType FormatCondition::GetType() const noexcept
{
    return _model.GetType();
}

void FormatCondition::SetType(FormatConditionType value) noexcept
{
    _model.SetType(value);
}

OperatorType FormatCondition::GetOperator() const noexcept
{
    return _model.GetOperator();
}

void FormatCondition::SetOperator(OperatorType value) noexcept
{
    _model.SetOperator(value);
}

std::string FormatCondition::GetFormula1() const
{
    return _model.GetFormula1();
}

void FormatCondition::SetFormula1(const std::string& value)
{
    _model.SetFormula1(NormalizeFormula(value));
}

std::string FormatCondition::GetFormula2() const
{
    return _model.GetFormula2();
}

void FormatCondition::SetFormula2(const std::string& value)
{
    _model.SetFormula2(NormalizeFormula(value));
}

std::string FormatCondition::GetFormula() const
{
    return _model.GetFormula1();
}

void FormatCondition::SetFormula(const std::string& value)
{
    _model.SetFormula1(NormalizeFormula(value));
}

std::string FormatCondition::GetTimePeriod() const
{
    return _model.GetTimePeriod();
}

void FormatCondition::SetTimePeriod(const std::string& value)
{
    _model.SetTimePeriod(NormalizeText(value));
}

bool FormatCondition::GetDuplicate() const noexcept
{
    return _model.GetDuplicate();
}

void FormatCondition::SetDuplicate(bool value) noexcept
{
    _model.SetDuplicate(value);
}

bool FormatCondition::GetTop() const noexcept
{
    return _model.GetTop();
}

void FormatCondition::SetTop(bool value) noexcept
{
    _model.SetTop(value);
}

bool FormatCondition::GetPercent() const noexcept
{
    return _model.GetPercent();
}

void FormatCondition::SetPercent(bool value) noexcept
{
    _model.SetPercent(value);
}

int FormatCondition::GetRank() const noexcept
{
    return _model.GetRank();
}

void FormatCondition::SetRank(int value)
{
    if (value < 0)
    {
        throw CellsException("Conditional formatting rank must be zero or greater.");
    }

    _model.SetRank(value);
}

bool FormatCondition::GetAbove() const noexcept
{
    return _model.GetAbove();
}

void FormatCondition::SetAbove(bool value) noexcept
{
    _model.SetAbove(value);
}

int FormatCondition::GetStandardDeviation() const noexcept
{
    return _model.GetStandardDeviation();
}

void FormatCondition::SetStandardDeviation(int value)
{
    if (value < 0)
    {
        throw CellsException("Conditional formatting standard deviation must be zero or greater.");
    }

    _model.SetStandardDeviation(value);
}

int FormatCondition::GetColorScaleCount() const noexcept
{
    return _model.GetColorScaleCount();
}

void FormatCondition::SetColorScaleCount(int value)
{
    if (value != 2 && value != 3)
    {
        throw CellsException("ColorScaleCount must be 2 or 3.");
    }

    _model.SetColorScaleCount(value);
}

Color FormatCondition::GetMinColor() const noexcept
{
    return Color::FromCore(_model.GetMinColor());
}

void FormatCondition::SetMinColor(const Color& value) noexcept
{
    _model.SetMinColor(value.ToCore());
}

Color FormatCondition::GetMidColor() const noexcept
{
    return Color::FromCore(_model.GetMidColor());
}

void FormatCondition::SetMidColor(const Color& value) noexcept
{
    _model.SetMidColor(value.ToCore());
}

Color FormatCondition::GetMaxColor() const noexcept
{
    return Color::FromCore(_model.GetMaxColor());
}

void FormatCondition::SetMaxColor(const Color& value) noexcept
{
    _model.SetMaxColor(value.ToCore());
}

Color FormatCondition::GetBarColor() const noexcept
{
    return Color::FromCore(_model.GetBarColor());
}

void FormatCondition::SetBarColor(const Color& value) noexcept
{
    _model.SetBarColor(value.ToCore());
}

Color FormatCondition::GetNegativeBarColor() const noexcept
{
    return Color::FromCore(_model.GetNegativeBarColor());
}

void FormatCondition::SetNegativeBarColor(const Color& value) noexcept
{
    _model.SetNegativeBarColor(value.ToCore());
}

bool FormatCondition::GetShowBorder() const noexcept
{
    return _model.GetShowBorder();
}

void FormatCondition::SetShowBorder(bool value) noexcept
{
    _model.SetShowBorder(value);
}

std::string FormatCondition::GetDirection() const
{
    return _model.GetDirection();
}

void FormatCondition::SetDirection(const std::string& value)
{
    _model.SetDirection(NormalizeText(value));
}

std::string FormatCondition::GetBarLength() const
{
    return _model.GetBarLength();
}

void FormatCondition::SetBarLength(const std::string& value)
{
    _model.SetBarLength(NormalizeText(value));
}

std::string FormatCondition::GetIconSetType() const
{
    return _model.GetIconSetType();
}

void FormatCondition::SetIconSetType(const std::string& value)
{
    _model.SetIconSetType(NormalizeText(value));
}

bool FormatCondition::GetReverseIcons() const noexcept
{
    return _model.GetReverseIcons();
}

void FormatCondition::SetReverseIcons(bool value) noexcept
{
    _model.SetReverseIcons(value);
}

bool FormatCondition::GetShowIconOnly() const noexcept
{
    return _model.GetShowIconOnly();
}

void FormatCondition::SetShowIconOnly(bool value) noexcept
{
    _model.SetShowIconOnly(value);
}

int FormatCondition::GetPriority() const noexcept
{
    return _model.GetPriority();
}

void FormatCondition::SetPriority(int value)
{
    if (value <= 0)
    {
        throw CellsException("Conditional formatting priority must be greater than zero.");
    }

    _model.SetPriority(value);
}

bool FormatCondition::GetStopIfTrue() const noexcept
{
    return _model.GetStopIfTrue();
}

void FormatCondition::SetStopIfTrue(bool value) noexcept
{
    _model.SetStopIfTrue(value);
}

Style FormatCondition::GetStyle() const
{
    return Style::FromCore(_model.GetStyle()).Clone();
}

void FormatCondition::SetStyle(const Style& value)
{
    _model.SetStyle(value.ToCore());
}

void FormatCondition::Remove()
{
    FormatConditionCollection::RemoveCondition(_owner, _collection, _model);
}

std::string FormatCondition::NormalizeFormula(const std::string& value)
{
    if (value.empty())
    {
        return {};
    }

    const auto start = value.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
    {
        return {};
    }

    const auto end = value.find_last_not_of(" \t\n\r");
    const auto trimmed = value.substr(start, end - start + 1);

    if (trimmed.empty())
    {
        return {};
    }

    if (trimmed[0] == '=')
    {
        return std::string(trimmed.substr(1));
    }

    return std::string(trimmed);
}

std::string FormatCondition::NormalizeText(const std::string& value)
{
    if (value.empty())
    {
        return {};
    }

    const auto start = value.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
    {
        return {};
    }

    const auto end = value.find_last_not_of(" \t\n\r");
    return std::string(value.substr(start, end - start + 1));
}

}}  // namespace Aspose::Cells_FOSS
