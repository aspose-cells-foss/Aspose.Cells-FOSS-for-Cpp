#include "aspose/cells_foss/core/CalculationPropertiesModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

CalculationPropertiesModel::CalculationPropertiesModel() = default;

void CalculationPropertiesModel::CopyFrom(const CalculationPropertiesModel& source)
{
    _calculationId = source._calculationId;
    _calculationMode = source._calculationMode;
    _fullCalculationOnLoad = source._fullCalculationOnLoad;
    _referenceMode = source._referenceMode;
    _iterate = source._iterate;
    _iterateCount = source._iterateCount;
    _iterateDelta = source._iterateDelta;
    _fullPrecision = source._fullPrecision;
    _calculationCompleted = source._calculationCompleted;
    _calculationOnSave = source._calculationOnSave;
    _concurrentCalculation = source._concurrentCalculation;
    _forceFullCalculation = source._forceFullCalculation;
}

bool CalculationPropertiesModel::HasStoredState() const
{
    return _calculationId.has_value()
        || !_calculationMode.empty()
        || _fullCalculationOnLoad
        || !_referenceMode.empty()
        || _iterate
        || _iterateCount.has_value()
        || _iterateDelta.has_value()
        || _fullPrecision.has_value()
        || _calculationCompleted.has_value()
        || _calculationOnSave.has_value()
        || _concurrentCalculation.has_value()
        || _forceFullCalculation;
}

}}}  // namespace Aspose::Cells_FOSS::Core
