#include "aspose/cells_foss/CalculationProperties.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/WorkbookPropertySupport.h"
#include "aspose/cells_foss/core/CalculationPropertiesModel.h"

#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

CalculationProperties::CalculationProperties(
    Core::CalculationPropertiesModel& model)
    : _model(model)
{
}

// ---------------------------------------------------------------------------
// CalculationId
// ---------------------------------------------------------------------------

std::optional<int> CalculationProperties::GetCalculationId() const noexcept
{
    return _model.GetCalculationId();
}

void CalculationProperties::SetCalculationId(std::optional<int> value)
{
    if (value.has_value() && value.value() < 0) {
        throw CellsException("CalculationId must be non-negative.");
    }
    _model.SetCalculationId(value);
}

// ---------------------------------------------------------------------------
// CalculationMode
// ---------------------------------------------------------------------------

std::string CalculationProperties::GetCalculationMode() const
{
    const auto& mode = _model.GetCalculationMode();
    return mode.empty() ? std::string("auto") : mode;
}

void CalculationProperties::SetCalculationMode(std::string_view value)
{
    _model.SetCalculationMode(WorkbookPropertySupport::NormalizeCalculationMode(value));
}

// ---------------------------------------------------------------------------
// FullCalculationOnLoad
// ---------------------------------------------------------------------------

bool CalculationProperties::GetFullCalculationOnLoad() const noexcept
{
    return _model.GetFullCalculationOnLoad();
}

void CalculationProperties::SetFullCalculationOnLoad(bool value) noexcept
{
    _model.SetFullCalculationOnLoad(value);
}

// ---------------------------------------------------------------------------
// ReferenceMode
// ---------------------------------------------------------------------------

std::string CalculationProperties::GetReferenceMode() const
{
    const auto& mode = _model.GetReferenceMode();
    return mode.empty() ? std::string("A1") : mode;
}

void CalculationProperties::SetReferenceMode(std::string_view value)
{
    _model.SetReferenceMode(WorkbookPropertySupport::NormalizeReferenceMode(value));
}

// ---------------------------------------------------------------------------
// Iterate
// ---------------------------------------------------------------------------

bool CalculationProperties::GetIterate() const noexcept
{
    return _model.GetIterate();
}

void CalculationProperties::SetIterate(bool value) noexcept
{
    _model.SetIterate(value);
}

// ---------------------------------------------------------------------------
// IterateCount
// ---------------------------------------------------------------------------

int CalculationProperties::GetIterateCount() const noexcept
{
    return _model.GetIterateCount().value_or(100);
}

void CalculationProperties::SetIterateCount(int value)
{
    if (value < 0) {
        throw CellsException("IterateCount must be non-negative.");
    }
    _model.SetIterateCount(value);
}

// ---------------------------------------------------------------------------
// IterateDelta
// ---------------------------------------------------------------------------

double CalculationProperties::GetIterateDelta() const noexcept
{
    return _model.GetIterateDelta().value_or(0.001);
}

void CalculationProperties::SetIterateDelta(double value)
{
    if (value < 0.0) {
        throw CellsException("IterateDelta must be non-negative.");
    }
    _model.SetIterateDelta(value);
}

// ---------------------------------------------------------------------------
// FullPrecision
// ---------------------------------------------------------------------------

bool CalculationProperties::GetFullPrecision() const noexcept
{
    return !_model.GetFullPrecision().has_value() ||
           _model.GetFullPrecision().value();
}

void CalculationProperties::SetFullPrecision(bool value) noexcept
{
    _model.SetFullPrecision(value);
}

// ---------------------------------------------------------------------------
// CalculationCompleted
// ---------------------------------------------------------------------------

bool CalculationProperties::GetCalculationCompleted() const noexcept
{
    return !_model.GetCalculationCompleted().has_value() ||
           _model.GetCalculationCompleted().value();
}

void CalculationProperties::SetCalculationCompleted(bool value) noexcept
{
    _model.SetCalculationCompleted(value);
}

// ---------------------------------------------------------------------------
// CalculationOnSave
// ---------------------------------------------------------------------------

bool CalculationProperties::GetCalculationOnSave() const noexcept
{
    return !_model.GetCalculationOnSave().has_value() ||
           _model.GetCalculationOnSave().value();
}

void CalculationProperties::SetCalculationOnSave(bool value) noexcept
{
    _model.SetCalculationOnSave(value);
}

// ---------------------------------------------------------------------------
// ConcurrentCalculation
// ---------------------------------------------------------------------------

bool CalculationProperties::GetConcurrentCalculation() const noexcept
{
    return !_model.GetConcurrentCalculation().has_value() ||
           _model.GetConcurrentCalculation().value();
}

void CalculationProperties::SetConcurrentCalculation(bool value) noexcept
{
    _model.SetConcurrentCalculation(value);
}

// ---------------------------------------------------------------------------
// ForceFullCalculation
// ---------------------------------------------------------------------------

bool CalculationProperties::GetForceFullCalculation() const noexcept
{
    return _model.GetForceFullCalculation();
}

void CalculationProperties::SetForceFullCalculation(bool value) noexcept
{
    _model.SetForceFullCalculation(value);
}

}}  // namespace Aspose::Cells_FOSS
