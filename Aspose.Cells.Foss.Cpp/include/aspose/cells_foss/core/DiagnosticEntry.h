#pragma once

#include <string>

#include "aspose/cells_foss/core/DiagnosticSeverity.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents diagnostic entry.
/// </summary>
class DiagnosticEntry final {
public:
    /// <summary>
    /// Initializes a new instance of the DiagnosticEntry class.
    /// </summary>
    DiagnosticEntry();

    /// <summary>
    /// Gets or sets the code.
    /// </summary>
    const std::string& GetCode() const noexcept { return _code; }
    void SetCode(std::string value) { _code = std::move(value); }

    /// <summary>
    /// Gets or sets the severity.
    /// </summary>
    DiagnosticSeverity GetSeverity() const noexcept { return _severity; }
    void SetSeverity(DiagnosticSeverity value) noexcept { _severity = value; }

    /// <summary>
    /// Gets or sets the message.
    /// </summary>
    const std::string& GetMessage() const noexcept { return _message; }
    void SetMessage(std::string value) { _message = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether repair applied.
    /// </summary>
    bool GetRepairApplied() const noexcept { return _repairApplied; }
    void SetRepairApplied(bool value) noexcept { _repairApplied = value; }

    /// <summary>
    /// Gets or sets a value indicating whether data loss risk.
    /// </summary>
    bool GetDataLossRisk() const noexcept { return _dataLossRisk; }
    void SetDataLossRisk(bool value) noexcept { _dataLossRisk = value; }

private:
    std::string _code;
    DiagnosticSeverity _severity = DiagnosticSeverity::Warning;
    std::string _message;
    bool _repairApplied = false;
    bool _dataLossRisk = false;
};

}}}  // namespace Aspose::Cells_FOSS::Core
