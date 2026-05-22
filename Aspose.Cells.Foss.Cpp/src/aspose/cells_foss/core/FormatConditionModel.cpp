#include "aspose/cells_foss/core/FormatConditionModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

FormatConditionModel::FormatConditionModel()
    : _type()
    , _operator(OperatorType::None)
    , _formula1()
    , _formula2()
    , _timePeriod()
    , _duplicate(true)
    , _top(true)
    , _percent(false)
    , _rank(0)
    , _above(true)
    , _standardDeviation(0)
    , _colorScaleCount(2)
    , _minColor()
    , _midColor()
    , _maxColor()
    , _barColor()
    , _negativeBarColor()
    , _showBorder(false)
    , _direction()
    , _barLength()
    , _iconSetType()
    , _reverseIcons(false)
    , _showIconOnly(false)
    , _priority(0)
    , _stopIfTrue(false)
    , _style(StyleValue::Default().Clone())
{
}

}}}  // namespace Aspose::Cells_FOSS::Core
