#pragma once

#include "aspose/cells_foss/WarningInfo.h"

namespace Aspose {
namespace Cells_FOSS {

/// Defines a callback that receives load warnings.
class IWarningCallback {
public:
    /// Destructor.
    virtual ~IWarningCallback() = default;

    /// Handles a warning raised while loading a workbook.
    /// @param warningInfo The warning details.
    virtual void Warning(const WarningInfo& warningInfo) = 0;
};

}}  // namespace Aspose::Cells_FOSS
