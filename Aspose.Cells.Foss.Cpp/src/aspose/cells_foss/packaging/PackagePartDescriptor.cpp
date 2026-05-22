#include "aspose/cells_foss/packaging/PackagePartDescriptor.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

PackagePartDescriptor::PackagePartDescriptor()
    : _partUri()
    , _contentType()
    , _category()
{
}

// ---------------------------------------------------------------------------
// PartUri
// ---------------------------------------------------------------------------

const std::string& PackagePartDescriptor::GetPartUri() const noexcept
{
    return _partUri;
}

void PackagePartDescriptor::SetPartUri(const std::string& value)
{
    _partUri = value;
}

void PackagePartDescriptor::SetPartUri(std::string&& value) noexcept
{
    _partUri = std::move(value);
}

// ---------------------------------------------------------------------------
// ContentType
// ---------------------------------------------------------------------------

const std::string& PackagePartDescriptor::GetContentType() const noexcept
{
    return _contentType;
}

void PackagePartDescriptor::SetContentType(const std::string& value)
{
    _contentType = value;
}

void PackagePartDescriptor::SetContentType(std::string&& value) noexcept
{
    _contentType = std::move(value);
}

// ---------------------------------------------------------------------------
// Category
// ---------------------------------------------------------------------------

const std::string& PackagePartDescriptor::GetCategory() const noexcept
{
    return _category;
}

void PackagePartDescriptor::SetCategory(const std::string& value)
{
    _category = value;
}

void PackagePartDescriptor::SetCategory(std::string&& value) noexcept
{
    _category = std::move(value);
}

}}}  // namespace Aspose::Cells_FOSS::Packaging
