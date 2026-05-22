#pragma once

#include <string>
#include <vector>

#include "aspose/cells_foss/CellArea.h"
#include "aspose/cells_foss/OperatorType.h"
#include "aspose/cells_foss/ValidationAlertType.h"
#include "aspose/cells_foss/ValidationType.h"
#include "aspose/cells_foss/core/ValidationModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents validation.
/// </summary>
class Validation final {
public:
    /// <summary>
    /// Initializes a new instance of the Validation class.
    /// </summary>
    /// <param name="owner">The owner collection of validation models.</param>
    /// <param name="model">The validation model.</param>
    Validation(std::vector<Core::ValidationModel>& owner,
               Core::ValidationModel& model);

    /// <summary>
    /// Gets the areas.
    /// </summary>
    std::vector<CellArea> GetAreas() const;

    /// <summary>
    /// Gets or sets the type.
    /// </summary>
    ValidationType GetType() const noexcept;
    void SetType(ValidationType value) noexcept;

    /// <summary>
    /// Gets or sets the alert style.
    /// </summary>
    ValidationAlertType GetAlertStyle() const noexcept;
    void SetAlertStyle(ValidationAlertType value) noexcept;

    /// <summary>
    /// Gets or sets the operator.
    /// </summary>
    OperatorType GetOperator() const noexcept;
    void SetOperator(OperatorType value) noexcept;

    /// <summary>
    /// Gets or sets the formula1.
    /// </summary>
    std::string GetFormula1() const;
    void SetFormula1(const std::string& value);

    /// <summary>
    /// Gets or sets the formula2.
    /// </summary>
    std::string GetFormula2() const;
    void SetFormula2(const std::string& value);

    /// <summary>
    /// Gets or sets a value indicating whether ignore blank.
    /// </summary>
    bool GetIgnoreBlank() const noexcept;
    void SetIgnoreBlank(bool value) noexcept;

    /// <summary>
    /// Gets or sets a value indicating whether in cell drop down.
    /// </summary>
    bool GetInCellDropDown() const noexcept;
    void SetInCellDropDown(bool value) noexcept;

    /// <summary>
    /// Gets or sets the input title.
    /// </summary>
    std::string GetInputTitle() const;
    void SetInputTitle(const std::string& value);

    /// <summary>
    /// Gets or sets the input message.
    /// </summary>
    std::string GetInputMessage() const;
    void SetInputMessage(const std::string& value);

    /// <summary>
    /// Gets or sets the error title.
    /// </summary>
    std::string GetErrorTitle() const;
    void SetErrorTitle(const std::string& value);

    /// <summary>
    /// Gets or sets the error message.
    /// </summary>
    std::string GetErrorMessage() const;
    void SetErrorMessage(const std::string& value);

    /// <summary>
    /// Gets or sets a value indicating whether show input.
    /// </summary>
    bool GetShowInput() const noexcept;
    void SetShowInput(bool value) noexcept;

    /// <summary>
    /// Gets or sets a value indicating whether show error.
    /// </summary>
    bool GetShowError() const noexcept;
    void SetShowError(bool value) noexcept;

    /// <summary>
    /// Adds the specified item.
    /// </summary>
    /// <param name="area">The area.</param>
    void AddArea(const CellArea& area);

    /// <summary>
    /// Removes the specified item.
    /// </summary>
    /// <param name="area">The area.</param>
    void RemoveArea(const CellArea& area);

private:
    static std::string NormalizeFormula(const std::string& value);
    static std::string NormalizeText(const std::string& value);

    std::vector<Core::ValidationModel>& _owner;
    Core::ValidationModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
