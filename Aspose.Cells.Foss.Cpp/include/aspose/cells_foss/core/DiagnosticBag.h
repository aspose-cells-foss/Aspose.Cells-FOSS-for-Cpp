#pragma once

#include <vector>

#include "aspose/cells_foss/core/DiagnosticEntry.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents diagnostic bag.
/// </summary>
class DiagnosticBag final {
public:
    /// <summary>
    /// Initializes a new instance of the DiagnosticBag class.
    /// </summary>
    DiagnosticBag();

    /// <summary>
    /// Gets the entries.
    /// </summary>
    const std::vector<DiagnosticEntry>& GetEntries() const noexcept;

    /// <summary>
    /// Adds the specified item.
    /// </summary>
    /// <param name="entry">The entry.</param>
    void Add(const DiagnosticEntry& entry);

private:
    std::vector<DiagnosticEntry> _entries;
};

}}}  // namespace Aspose::Cells_FOSS::Core
