#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {
class CalculationPropertiesModel;
}  // namespace Core

/// Represents calculation properties.
class CalculationProperties final {
public:
    /// Constructs a CalculationProperties wrapping the specified model.
    /// @param model The underlying calculation properties model.
    explicit CalculationProperties(Core::CalculationPropertiesModel& model);

    /// Gets or sets the calculation id.
    std::optional<int> GetCalculationId() const noexcept;
    void SetCalculationId(std::optional<int> value);

    /// Gets or sets the calculation mode.
    std::string GetCalculationMode() const;
    void SetCalculationMode(std::string_view value);

    /// Gets or sets a value indicating whether full calculation on load.
    bool GetFullCalculationOnLoad() const noexcept;
    void SetFullCalculationOnLoad(bool value) noexcept;

    /// Gets or sets the reference mode.
    std::string GetReferenceMode() const;
    void SetReferenceMode(std::string_view value);

    /// Gets or sets a value indicating whether iterate.
    bool GetIterate() const noexcept;
    void SetIterate(bool value) noexcept;

    /// Gets or sets the iterate count.
    int GetIterateCount() const noexcept;
    void SetIterateCount(int value);

    /// Gets or sets the iterate delta.
    double GetIterateDelta() const noexcept;
    void SetIterateDelta(double value);

    /// Gets or sets a value indicating whether full precision.
    bool GetFullPrecision() const noexcept;
    void SetFullPrecision(bool value) noexcept;

    /// Gets or sets a value indicating whether calculation completed.
    bool GetCalculationCompleted() const noexcept;
    void SetCalculationCompleted(bool value) noexcept;

    /// Gets or sets a value indicating whether calculation on save.
    bool GetCalculationOnSave() const noexcept;
    void SetCalculationOnSave(bool value) noexcept;

    /// Gets or sets a value indicating whether concurrent calculation.
    bool GetConcurrentCalculation() const noexcept;
    void SetConcurrentCalculation(bool value) noexcept;

    /// Gets or sets a value indicating whether force full calculation.
    bool GetForceFullCalculation() const noexcept;
    void SetForceFullCalculation(bool value) noexcept;

private:
    Core::CalculationPropertiesModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
