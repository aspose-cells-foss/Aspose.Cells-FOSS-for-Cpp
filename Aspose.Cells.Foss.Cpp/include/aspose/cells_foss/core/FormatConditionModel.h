#pragma once
#include <string>

#include "aspose/cells_foss/core/ColorValue.h"
#include "aspose/cells_foss/core/StyleValue.h"
#include "aspose/cells_foss/FormatConditionType.h"
#include "aspose/cells_foss/OperatorType.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents format condition model.
/// </summary>
class FormatConditionModel final {
public:
    /// <summary>
    /// Initializes a new instance of the FormatConditionModel class.
    /// </summary>
    FormatConditionModel();

    /// <summary>
    /// Gets or sets the type.
    /// </summary>
    FormatConditionType GetType() const noexcept { return _type; }
    void SetType(FormatConditionType value) noexcept { _type = value; }

    /// <summary>
    /// Gets or sets the operator.
    /// </summary>
    OperatorType GetOperator() const noexcept { return _operator; }
    void SetOperator(OperatorType value) noexcept { _operator = value; }

    /// <summary>
    /// Gets or sets the formula1.
    /// </summary>
    const std::string& GetFormula1() const noexcept { return _formula1; }
    void SetFormula1(const std::string& value) { _formula1 = value; }
    void SetFormula1(std::string&& value) noexcept { _formula1 = std::move(value); }

    /// <summary>
    /// Gets or sets the formula2.
    /// </summary>
    const std::string& GetFormula2() const noexcept { return _formula2; }
    void SetFormula2(const std::string& value) { _formula2 = value; }
    void SetFormula2(std::string&& value) noexcept { _formula2 = std::move(value); }

    /// <summary>
    /// Gets or sets the time period.
    /// </summary>
    const std::string& GetTimePeriod() const noexcept { return _timePeriod; }
    void SetTimePeriod(const std::string& value) { _timePeriod = value; }
    void SetTimePeriod(std::string&& value) noexcept { _timePeriod = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether duplicate.
    /// </summary>
    bool GetDuplicate() const noexcept { return _duplicate; }
    void SetDuplicate(bool value) noexcept { _duplicate = value; }

    /// <summary>
    /// Gets or sets a value indicating whether top.
    /// </summary>
    bool GetTop() const noexcept { return _top; }
    void SetTop(bool value) noexcept { _top = value; }

    /// <summary>
    /// Gets or sets a value indicating whether percent.
    /// </summary>
    bool GetPercent() const noexcept { return _percent; }
    void SetPercent(bool value) noexcept { _percent = value; }

    /// <summary>
    /// Gets or sets the rank.
    /// </summary>
    int GetRank() const noexcept { return _rank; }
    void SetRank(int value) noexcept { _rank = value; }

    /// <summary>
    /// Gets or sets a value indicating whether above.
    /// </summary>
    bool GetAbove() const noexcept { return _above; }
    void SetAbove(bool value) noexcept { _above = value; }

    /// <summary>
    /// Gets or sets the standard deviation.
    /// </summary>
    int GetStandardDeviation() const noexcept { return _standardDeviation; }
    void SetStandardDeviation(int value) noexcept { _standardDeviation = value; }

    /// <summary>
    /// Gets or sets the color scale count.
    /// </summary>
    int GetColorScaleCount() const noexcept { return _colorScaleCount; }
    void SetColorScaleCount(int value) noexcept { _colorScaleCount = value; }

    /// <summary>
    /// Gets or sets the min color.
    /// </summary>
    const ColorValue& GetMinColor() const noexcept { return _minColor; }
    void SetMinColor(const ColorValue& value) noexcept { _minColor = value; }

    /// <summary>
    /// Gets or sets the mid color.
    /// </summary>
    const ColorValue& GetMidColor() const noexcept { return _midColor; }
    void SetMidColor(const ColorValue& value) noexcept { _midColor = value; }

    /// <summary>
    /// Gets or sets the max color.
    /// </summary>
    const ColorValue& GetMaxColor() const noexcept { return _maxColor; }
    void SetMaxColor(const ColorValue& value) noexcept { _maxColor = value; }

    /// <summary>
    /// Gets or sets the bar color.
    /// </summary>
    const ColorValue& GetBarColor() const noexcept { return _barColor; }
    void SetBarColor(const ColorValue& value) noexcept { _barColor = value; }

    /// <summary>
    /// Gets or sets the negative bar color.
    /// </summary>
    const ColorValue& GetNegativeBarColor() const noexcept { return _negativeBarColor; }
    void SetNegativeBarColor(const ColorValue& value) noexcept { _negativeBarColor = value; }

    /// <summary>
    /// Gets or sets a value indicating whether show border.
    /// </summary>
    bool GetShowBorder() const noexcept { return _showBorder; }
    void SetShowBorder(bool value) noexcept { _showBorder = value; }

    /// <summary>
    /// Gets or sets the direction.
    /// </summary>
    const std::string& GetDirection() const noexcept { return _direction; }
    void SetDirection(const std::string& value) { _direction = value; }
    void SetDirection(std::string&& value) noexcept { _direction = std::move(value); }

    /// <summary>
    /// Gets or sets the bar length.
    /// </summary>
    const std::string& GetBarLength() const noexcept { return _barLength; }
    void SetBarLength(const std::string& value) { _barLength = value; }
    void SetBarLength(std::string&& value) noexcept { _barLength = std::move(value); }

    /// <summary>
    /// Gets or sets the icon set type.
    /// </summary>
    const std::string& GetIconSetType() const noexcept { return _iconSetType; }
    void SetIconSetType(const std::string& value) { _iconSetType = value; }
    void SetIconSetType(std::string&& value) noexcept { _iconSetType = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether reverse icons.
    /// </summary>
    bool GetReverseIcons() const noexcept { return _reverseIcons; }
    void SetReverseIcons(bool value) noexcept { _reverseIcons = value; }

    /// <summary>
    /// Gets or sets a value indicating whether show icon only.
    /// </summary>
    bool GetShowIconOnly() const noexcept { return _showIconOnly; }
    void SetShowIconOnly(bool value) noexcept { _showIconOnly = value; }

    /// <summary>
    /// Gets or sets the priority.
    /// </summary>
    int GetPriority() const noexcept { return _priority; }
    void SetPriority(int value) noexcept { _priority = value; }

    /// <summary>
    /// Gets or sets a value indicating whether stop if true.
    /// </summary>
    bool GetStopIfTrue() const noexcept { return _stopIfTrue; }
    void SetStopIfTrue(bool value) noexcept { _stopIfTrue = value; }

    /// <summary>
    /// Performs style value.default.clone.
    /// </summary>
    const StyleValue& GetStyle() const noexcept { return _style; }
    void SetStyle(const StyleValue& value) { _style = value; }
    void SetStyle(StyleValue&& value) noexcept { _style = std::move(value); }

private:
    FormatConditionType _type = FormatConditionType::CellValue;
    OperatorType _operator = OperatorType::None;
    std::string _formula1;
    std::string _formula2;
    std::string _timePeriod;
    bool _duplicate = true;
    bool _top = true;
    bool _percent = false;
    int _rank = 0;
    bool _above = true;
    int _standardDeviation = 0;
    int _colorScaleCount = 2;
    ColorValue _minColor;
    ColorValue _midColor;
    ColorValue _maxColor;
    ColorValue _barColor;
    ColorValue _negativeBarColor;
    bool _showBorder = false;
    std::string _direction;
    std::string _barLength;
    std::string _iconSetType;
    bool _reverseIcons = false;
    bool _showIconOnly = false;
    int _priority = 0;
    bool _stopIfTrue = false;
    StyleValue _style;
};

}}}  // namespace Aspose::Cells_FOSS::Core
