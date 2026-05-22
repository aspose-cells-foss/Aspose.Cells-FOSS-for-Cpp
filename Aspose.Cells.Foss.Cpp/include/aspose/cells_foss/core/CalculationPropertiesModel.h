#pragma once
#include <optional>
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents calculation properties model.
/// </summary>
class CalculationPropertiesModel final {
public:
    /// <summary>
    /// Initializes a new instance of the CalculationPropertiesModel class.
    /// </summary>
    CalculationPropertiesModel();

    /// <summary>
    /// Gets or sets the calculation id.
    /// </summary>
    std::optional<int> GetCalculationId() const noexcept { return _calculationId; }
    void SetCalculationId(std::optional<int> value) noexcept { _calculationId = value; }

    /// <summary>
    /// Gets or sets the calculation mode.
    /// </summary>
    const std::string& GetCalculationMode() const noexcept { return _calculationMode; }
    void SetCalculationMode(std::string value) { _calculationMode = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether full calculation on load.
    /// </summary>
    bool GetFullCalculationOnLoad() const noexcept { return _fullCalculationOnLoad; }
    void SetFullCalculationOnLoad(bool value) noexcept { _fullCalculationOnLoad = value; }

    /// <summary>
    /// Gets or sets the reference mode.
    /// </summary>
    const std::string& GetReferenceMode() const noexcept { return _referenceMode; }
    void SetReferenceMode(std::string value) { _referenceMode = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether iterate.
    /// </summary>
    bool GetIterate() const noexcept { return _iterate; }
    void SetIterate(bool value) noexcept { _iterate = value; }

    /// <summary>
    /// Gets or sets the iterate count.
    /// </summary>
    std::optional<int> GetIterateCount() const noexcept { return _iterateCount; }
    void SetIterateCount(std::optional<int> value) noexcept { _iterateCount = value; }

    /// <summary>
    /// Gets or sets the iterate delta.
    /// </summary>
    std::optional<double> GetIterateDelta() const noexcept { return _iterateDelta; }
    void SetIterateDelta(std::optional<double> value) noexcept { _iterateDelta = value; }

    /// <summary>
    /// Gets or sets a value indicating whether full precision.
    /// </summary>
    std::optional<bool> GetFullPrecision() const noexcept { return _fullPrecision; }
    void SetFullPrecision(std::optional<bool> value) noexcept { _fullPrecision = value; }

    /// <summary>
    /// Gets or sets a value indicating whether calculation completed.
    /// </summary>
    std::optional<bool> GetCalculationCompleted() const noexcept { return _calculationCompleted; }
    void SetCalculationCompleted(std::optional<bool> value) noexcept { _calculationCompleted = value; }

    /// <summary>
    /// Gets or sets a value indicating whether calculation on save.
    /// </summary>
    std::optional<bool> GetCalculationOnSave() const noexcept { return _calculationOnSave; }
    void SetCalculationOnSave(std::optional<bool> value) noexcept { _calculationOnSave = value; }

    /// <summary>
    /// Gets or sets a value indicating whether concurrent calculation.
    /// </summary>
    std::optional<bool> GetConcurrentCalculation() const noexcept { return _concurrentCalculation; }
    void SetConcurrentCalculation(std::optional<bool> value) noexcept { _concurrentCalculation = value; }

    /// <summary>
    /// Gets or sets a value indicating whether force full calculation.
    /// </summary>
    bool GetForceFullCalculation() const noexcept { return _forceFullCalculation; }
    void SetForceFullCalculation(bool value) noexcept { _forceFullCalculation = value; }

    /// <summary>
    /// Copies values from the specified source.
    /// </summary>
    /// <param name="source">The source.</param>
    void CopyFrom(const CalculationPropertiesModel& source);

    /// <summary>
    /// Performs has stored state.
    /// </summary>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool HasStoredState() const;

private:
    std::optional<int> _calculationId;
    std::string _calculationMode;
    bool _fullCalculationOnLoad = false;
    std::string _referenceMode;
    bool _iterate = false;
    std::optional<int> _iterateCount;
    std::optional<double> _iterateDelta;
    std::optional<bool> _fullPrecision;
    std::optional<bool> _calculationCompleted;
    std::optional<bool> _calculationOnSave;
    std::optional<bool> _concurrentCalculation;
    bool _forceFullCalculation = false;
};

}}}  // namespace Aspose::Cells_FOSS::Core
