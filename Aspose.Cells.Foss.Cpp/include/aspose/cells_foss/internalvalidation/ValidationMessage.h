#pragma once

#include <string>

#include "aspose/cells_foss/internalvalidation/ValidationMessageSeverity.h"

namespace Aspose {
namespace Cells_FOSS {
namespace InternalValidation {

/// <summary>
/// Represents validation message.
/// </summary>
class ValidationMessage final {
public:
    /// <summary>
    /// Initializes a new instance of the ValidationMessage class.
    /// </summary>
    ValidationMessage();

    /// <summary>
    /// Gets or sets the code.
    /// </summary>
    const std::string& GetCode() const noexcept { return _code; }
    void SetCode(std::string value) { _code = std::move(value); }

    /// <summary>
    /// Gets or sets the message.
    /// </summary>
    const std::string& GetMessage() const noexcept { return _message; }
    void SetMessage(std::string value) { _message = std::move(value); }

    /// <summary>
    /// Gets or sets the severity.
    /// </summary>
    ValidationMessageSeverity GetSeverity() const noexcept { return _severity; }
    void SetSeverity(ValidationMessageSeverity value) noexcept { _severity = value; }

private:
    std::string _code;
    std::string _message;
    ValidationMessageSeverity _severity = ValidationMessageSeverity::Info;
};

}}}  // namespace Aspose::Cells_FOSS::InternalValidation
