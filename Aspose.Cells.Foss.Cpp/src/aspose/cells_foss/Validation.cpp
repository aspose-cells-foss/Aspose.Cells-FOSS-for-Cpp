#include "aspose/cells_foss/Validation.h"
#include "aspose/cells_foss/ValidationCollection.h"

namespace Aspose {
namespace Cells_FOSS {

Validation::Validation(std::vector<Core::ValidationModel>& owner,
                       Core::ValidationModel& model)
    : _owner(owner)
    , _model(model)
{
}

std::vector<CellArea> Validation::GetAreas() const
{
    const auto& sourceAreas = _model.GetAreas();
    std::vector<CellArea> areas;
    areas.reserve(sourceAreas.size());
    for (std::size_t index = 0; index < sourceAreas.size(); ++index)
    {
        areas.push_back(sourceAreas[index]);
    }
    return areas;
}

ValidationType Validation::GetType() const noexcept
{
    return _model.GetType();
}

void Validation::SetType(ValidationType value) noexcept
{
    _model.SetType(value);
}

ValidationAlertType Validation::GetAlertStyle() const noexcept
{
    return _model.GetAlertStyle();
}

void Validation::SetAlertStyle(ValidationAlertType value) noexcept
{
    _model.SetAlertStyle(value);
}

OperatorType Validation::GetOperator() const noexcept
{
    return _model.GetOperator();
}

void Validation::SetOperator(OperatorType value) noexcept
{
    _model.SetOperator(value);
}

std::string Validation::GetFormula1() const
{
    return _model.GetFormula1();
}

void Validation::SetFormula1(const std::string& value)
{
    _model.SetFormula1(NormalizeFormula(value));
}

std::string Validation::GetFormula2() const
{
    return _model.GetFormula2();
}

void Validation::SetFormula2(const std::string& value)
{
    _model.SetFormula2(NormalizeFormula(value));
}

bool Validation::GetIgnoreBlank() const noexcept
{
    return _model.GetIgnoreBlank();
}

void Validation::SetIgnoreBlank(bool value) noexcept
{
    _model.SetIgnoreBlank(value);
}

bool Validation::GetInCellDropDown() const noexcept
{
    return _model.GetInCellDropDown();
}

void Validation::SetInCellDropDown(bool value) noexcept
{
    _model.SetInCellDropDown(value);
}

std::string Validation::GetInputTitle() const
{
    return _model.GetInputTitle();
}

void Validation::SetInputTitle(const std::string& value)
{
    _model.SetInputTitle(NormalizeText(value));
}

std::string Validation::GetInputMessage() const
{
    return _model.GetInputMessage();
}

void Validation::SetInputMessage(const std::string& value)
{
    _model.SetInputMessage(NormalizeText(value));
}

std::string Validation::GetErrorTitle() const
{
    return _model.GetErrorTitle();
}

void Validation::SetErrorTitle(const std::string& value)
{
    _model.SetErrorTitle(NormalizeText(value));
}

std::string Validation::GetErrorMessage() const
{
    return _model.GetErrorMessage();
}

void Validation::SetErrorMessage(const std::string& value)
{
    _model.SetErrorMessage(NormalizeText(value));
}

bool Validation::GetShowInput() const noexcept
{
    return _model.GetShowInput();
}

void Validation::SetShowInput(bool value) noexcept
{
    _model.SetShowInput(value);
}

bool Validation::GetShowError() const noexcept
{
    return _model.GetShowError();
}

void Validation::SetShowError(bool value) noexcept
{
    _model.SetShowError(value);
}

void Validation::AddArea(const CellArea& area)
{
    ValidationCollection::AddAreaToValidation(_owner, _model, area);
}

void Validation::RemoveArea(const CellArea& area)
{
    ValidationCollection::RemoveAreaFromValidation(_owner, _model, area);
}

std::string Validation::NormalizeFormula(const std::string& value)
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

std::string Validation::NormalizeText(const std::string& value)
{
    if (value.empty())
    {
        return {};
    }

    return value;
}

}}  // namespace Aspose::Cells_FOSS
