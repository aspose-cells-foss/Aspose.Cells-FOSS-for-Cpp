#include "aspose/cells_foss/packaging/PackageModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

PackageModel::PackageModel()
    : _parts()
    , _relationships()
    , _unsupportedParts()
{
}

// ---------------------------------------------------------------------------
// Parts
// ---------------------------------------------------------------------------

const std::vector<PackagePartDescriptor>& PackageModel::GetParts() const noexcept
{
    return _parts;
}

std::vector<PackagePartDescriptor>& PackageModel::GetParts() noexcept
{
    return _parts;
}

// ---------------------------------------------------------------------------
// Relationships
// ---------------------------------------------------------------------------

const std::vector<RelationshipDescriptor>& PackageModel::GetRelationships() const noexcept
{
    return _relationships;
}

std::vector<RelationshipDescriptor>& PackageModel::GetRelationships() noexcept
{
    return _relationships;
}

// ---------------------------------------------------------------------------
// UnsupportedParts
// ---------------------------------------------------------------------------

const UnsupportedPartsMap& PackageModel::GetUnsupportedParts() const noexcept
{
    return _unsupportedParts;
}

UnsupportedPartsMap& PackageModel::GetUnsupportedParts() noexcept
{
    return _unsupportedParts;
}

}}}  // namespace Aspose::Cells_FOSS::Packaging
