#pragma once

#include "aspose/cells_foss/LoadFormat.h"
#include "aspose/cells_foss/IWarningCallback.h"

#include <memory>

namespace Aspose {
namespace Cells_FOSS {

/// Specifies how a workbook should be loaded.
class LoadOptions {
public:
    /// Constructs a LoadOptions with default values.
    LoadOptions();

    /// Gets or sets the expected input format.
    LoadFormat GetLoadFormat() const noexcept;
    void SetLoadFormat(LoadFormat value) noexcept;

    /// Gets or sets whether loading should reject ambiguous repairs.
    bool GetStrictMode() const noexcept;
    void SetStrictMode(bool value) noexcept;

    /// Gets or sets whether package-level repairs are allowed during load.
    bool GetTryRepairPackage() const noexcept;
    void SetTryRepairPackage(bool value) noexcept;

    /// Gets or sets whether XML-level repairs are allowed during load.
    bool GetTryRepairXml() const noexcept;
    void SetTryRepairXml(bool value) noexcept;

    /// Gets or sets whether unsupported parts should be preserved when possible.
    bool GetPreserveUnsupportedParts() const noexcept;
    void SetPreserveUnsupportedParts(bool value) noexcept;

    /// Gets or sets a warning callback that receives recoverable-load diagnostics.
    const std::shared_ptr<IWarningCallback>& GetWarningCallback() const noexcept;
    void SetWarningCallback(std::shared_ptr<IWarningCallback> value) noexcept;

private:
    LoadFormat _loadFormat;
    bool _strictMode;
    bool _tryRepairPackage;
    bool _tryRepairXml;
    bool _preserveUnsupportedParts;
    std::shared_ptr<IWarningCallback> _warningCallback;
};

}}  // namespace Aspose::Cells_FOSS
