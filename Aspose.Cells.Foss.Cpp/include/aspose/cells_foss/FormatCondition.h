#pragma once

#include <string>
#include <vector>

#include "aspose/cells_foss/Color.h"
#include "aspose/cells_foss/FormatConditionType.h"
#include "aspose/cells_foss/OperatorType.h"
#include "aspose/cells_foss/Style.h"
#include "aspose/cells_foss/core/ConditionalFormattingModel.h"
#include "aspose/cells_foss/core/FormatConditionModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents format condition.
/// </summary>
class FormatCondition final {
public:
    /// <summary>
    /// Initializes a new instance of the FormatCondition class.
    /// </summary>
    /// <param name="owner">The owner collection of conditional formatting models.</param>
    /// <param name="collection">The conditional formatting model that owns the condition.</param>
    /// <param name="model">The format condition model.</param>
    FormatCondition(std::vector<Core::ConditionalFormattingModel>& owner,
                    Core::ConditionalFormattingModel& collection,
                    Core::FormatConditionModel& model);

    /// <summary>
    /// Gets or sets the type.
    /// </summary>
    FormatConditionType GetType() const noexcept;
    void SetType(FormatConditionType value) noexcept;

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
    /// Gets or sets the formula.
    /// </summary>
    std::string GetFormula() const;
    void SetFormula(const std::string& value);

    /// <summary>
    /// Gets or sets the time period.
    /// </summary>
    std::string GetTimePeriod() const;
    void SetTimePeriod(const std::string& value);

    /// <summary>
    /// Gets or sets a value indicating whether duplicate.
    /// </summary>
    bool GetDuplicate() const noexcept;
    void SetDuplicate(bool value) noexcept;

    /// <summary>
    /// Gets or sets a value indicating whether top.
    /// </summary>
    bool GetTop() const noexcept;
    void SetTop(bool value) noexcept;

    /// <summary>
    /// Gets or sets a value indicating whether percent.
    /// </summary>
    bool GetPercent() const noexcept;
    void SetPercent(bool value) noexcept;

    /// <summary>
    /// Gets or sets the rank.
    /// </summary>
    int GetRank() const noexcept;
    void SetRank(int value);

    /// <summary>
    /// Gets or sets a value indicating whether above.
    /// </summary>
    bool GetAbove() const noexcept;
    void SetAbove(bool value) noexcept;

    /// <summary>
    /// Gets or sets the standard deviation.
    /// </summary>
    int GetStandardDeviation() const noexcept;
    void SetStandardDeviation(int value);

    /// <summary>
    /// Gets or sets the color scale count.
    /// </summary>
    int GetColorScaleCount() const noexcept;
    void SetColorScaleCount(int value);

    /// <summary>
    /// Gets or sets the min color.
    /// </summary>
    Color GetMinColor() const noexcept;
    void SetMinColor(const Color& value) noexcept;

    /// <summary>
    /// Gets or sets the mid color.
    /// </summary>
    Color GetMidColor() const noexcept;
    void SetMidColor(const Color& value) noexcept;

    /// <summary>
    /// Gets or sets the max color.
    /// </summary>
    Color GetMaxColor() const noexcept;
    void SetMaxColor(const Color& value) noexcept;

    /// <summary>
    /// Gets or sets the bar color.
    /// </summary>
    Color GetBarColor() const noexcept;
    void SetBarColor(const Color& value) noexcept;

    /// <summary>
    /// Gets or sets the negative bar color.
    /// </summary>
    Color GetNegativeBarColor() const noexcept;
    void SetNegativeBarColor(const Color& value) noexcept;

    /// <summary>
    /// Gets or sets a value indicating whether show border.
    /// </summary>
    bool GetShowBorder() const noexcept;
    void SetShowBorder(bool value) noexcept;

    /// <summary>
    /// Gets or sets the direction.
    /// </summary>
    std::string GetDirection() const;
    void SetDirection(const std::string& value);

    /// <summary>
    /// Gets or sets the bar length.
    /// </summary>
    std::string GetBarLength() const;
    void SetBarLength(const std::string& value);

    /// <summary>
    /// Gets or sets the icon set type.
    /// </summary>
    std::string GetIconSetType() const;
    void SetIconSetType(const std::string& value);

    /// <summary>
    /// Gets or sets a value indicating whether reverse icons.
    /// </summary>
    bool GetReverseIcons() const noexcept;
    void SetReverseIcons(bool value) noexcept;

    /// <summary>
    /// Gets or sets a value indicating whether show icon only.
    /// </summary>
    bool GetShowIconOnly() const noexcept;
    void SetShowIconOnly(bool value) noexcept;

    /// <summary>
    /// Gets or sets the priority.
    /// </summary>
    int GetPriority() const noexcept;
    void SetPriority(int value);

    /// <summary>
    /// Gets or sets a value indicating whether stop if true.
    /// </summary>
    bool GetStopIfTrue() const noexcept;
    void SetStopIfTrue(bool value) noexcept;

    /// <summary>
    /// Gets or sets the style.
    /// </summary>
    Style GetStyle() const;
    void SetStyle(const Style& value);

    /// <summary>
    /// Removes the specified item.
    /// </summary>
    void Remove();

private:
    static std::string NormalizeFormula(const std::string& value);
    static std::string NormalizeText(const std::string& value);

    std::vector<Core::ConditionalFormattingModel>& _owner;
    Core::ConditionalFormattingModel& _collection;
    Core::FormatConditionModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
