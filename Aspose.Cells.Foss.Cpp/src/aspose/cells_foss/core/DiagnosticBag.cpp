#include "aspose/cells_foss/core/DiagnosticBag.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

DiagnosticBag::DiagnosticBag() = default;

const std::vector<DiagnosticEntry>& DiagnosticBag::GetEntries() const noexcept
{
    return _entries;
}

void DiagnosticBag::Add(const DiagnosticEntry& entry)
{
    _entries.push_back(entry);
}

}}}  // namespace Aspose::Cells_FOSS::Core
