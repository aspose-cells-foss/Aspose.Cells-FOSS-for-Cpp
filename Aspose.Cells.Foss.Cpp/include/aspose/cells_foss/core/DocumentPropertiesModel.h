#pragma once

#include "aspose/cells_foss/core/CoreDocumentPropertiesModel.h"
#include "aspose/cells_foss/core/ExtendedDocumentPropertiesModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents document properties model.
/// </summary>
class DocumentPropertiesModel final {
public:
    /// <summary>
    /// Initializes a new instance of the DocumentPropertiesModel class.
    /// </summary>
    DocumentPropertiesModel();

    /// <summary>
    /// Gets the core.
    /// </summary>
    CoreDocumentPropertiesModel& GetCore() noexcept { return _core; }
    const CoreDocumentPropertiesModel& GetCore() const noexcept { return _core; }

    /// <summary>
    /// Gets the extended.
    /// </summary>
    ExtendedDocumentPropertiesModel& GetExtended() noexcept { return _extended; }
    const ExtendedDocumentPropertiesModel& GetExtended() const noexcept { return _extended; }

    /// <summary>
    /// Copies values from the specified source.
    /// </summary>
    /// <param name="source">The source.</param>
    void CopyFrom(const DocumentPropertiesModel& source);

    /// <summary>
    /// Performs has stored state.
    /// </summary>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool HasStoredState() const;

private:
    CoreDocumentPropertiesModel _core;
    ExtendedDocumentPropertiesModel _extended;
};

}}}  // namespace Aspose::Cells_FOSS::Core
