#pragma once

#include <string>
#include <vector>

#include "aspose/cells_foss/CellArea.h"
#include "aspose/cells_foss/OperatorType.h"
#include "aspose/cells_foss/ValidationAlertType.h"
#include "aspose/cells_foss/ValidationType.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents validation model.
/// </summary>
class ValidationModel final {
public:
    /// <summary>
    /// Initializes a new instance of the ValidationModel class.
    /// </summary>
    ValidationModel();

    /// <summary>
    /// Gets the areas.
    /// </summary>
    const std::vector<CellArea>& GetAreas() const noexcept { return _areas; }
    std::vector<CellArea>& GetAreas() noexcept { return _areas; }

    /// <summary>
    /// Gets or sets the type.
    /// </summary>
    ValidationType GetType() const noexcept { return _type; }
    void SetType(ValidationType value) noexcept { _type = value; }

    /// <summary>
    /// Gets or sets the alert style.
    /// </summary>
    ValidationAlertType GetAlertStyle() const noexcept { return _alertStyle; }
    void SetAlertStyle(ValidationAlertType value) noexcept { _alertStyle = value; }

    /// <summary>
    /// Gets or sets the operator.
    /// </summary>
    OperatorType GetOperator() const noexcept { return _operator; }
    void SetOperator(OperatorType value) noexcept { _operator = value; }

    /// <summary>
    /// Gets or sets the formula1.
    /// </summary>
    const std::string& GetFormula1() const noexcept { return _formula1; }
    void SetFormula1(std::string value) { _formula1 = std::move(value); }

    /// <summary>
    /// Gets or sets the formula2.
    /// </summary>
    const std::string& GetFormula2() const noexcept { return _formula2; }
    void SetFormula2(std::string value) { _formula2 = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether ignore blank.
    /// </summary>
    bool GetIgnoreBlank() const noexcept { return _ignoreBlank; }
    void SetIgnoreBlank(bool value) noexcept { _ignoreBlank = value; }

    /// <summary>
    /// Gets or sets a value indicating whether in cell drop down.
    /// </summary>
    bool GetInCellDropDown() const noexcept { return _inCellDropDown; }
    void SetInCellDropDown(bool value) noexcept { _inCellDropDown = value; }

    /// <summary>
    /// Gets or sets the input title.
    /// </summary>
    const std::string& GetInputTitle() const noexcept { return _inputTitle; }
    void SetInputTitle(std::string value) { _inputTitle = std::move(value); }

    /// <summary>
    /// Gets or sets the input message.
    /// </summary>
    const std::string& GetInputMessage() const noexcept { return _inputMessage; }
    void SetInputMessage(std::string value) { _inputMessage = std::move(value); }

    /// <summary>
    /// Gets or sets the error title.
    /// </summary>
    const std::string& GetErrorTitle() const noexcept { return _errorTitle; }
    void SetErrorTitle(std::string value) { _errorTitle = std::move(value); }

    /// <summary>
    /// Gets or sets the error message.
    /// </summary>
    const std::string& GetErrorMessage() const noexcept { return _errorMessage; }
    void SetErrorMessage(std::string value) { _errorMessage = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether show input.
    /// </summary>
    bool GetShowInput() const noexcept { return _showInput; }
    void SetShowInput(bool value) noexcept { _showInput = value; }

    /// <summary>
    /// Gets or sets a value indicating whether show error.
    /// </summary>
    bool GetShowError() const noexcept { return _showError; }
    void SetShowError(bool value) noexcept { _showError = value; }

private:
    std::vector<CellArea> _areas;
    ValidationType _type = ValidationType::AnyValue;
    ValidationAlertType _alertStyle = ValidationAlertType::Stop;
    OperatorType _operator = OperatorType::None;
    std::string _formula1;
    std::string _formula2;
    bool _ignoreBlank = false;
    bool _inCellDropDown = true;
    std::string _inputTitle;
    std::string _inputMessage;
    std::string _errorTitle;
    std::string _errorMessage;
    bool _showInput = false;
    bool _showError = false;
};

}}}  // namespace Aspose::Cells_FOSS::Core
