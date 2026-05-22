#include "aspose/cells_foss/packaging/RelationshipDescriptor.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

RelationshipDescriptor::RelationshipDescriptor()
    : _id()
    , _type()
    , _target()
    , _isExternal(false)
{
}

// ---------------------------------------------------------------------------
// Id
// ---------------------------------------------------------------------------

const std::string& RelationshipDescriptor::GetId() const noexcept
{
    return _id;
}

void RelationshipDescriptor::SetId(const std::string& value)
{
    _id = value;
}

void RelationshipDescriptor::SetId(std::string&& value) noexcept
{
    _id = std::move(value);
}

// ---------------------------------------------------------------------------
// Type
// ---------------------------------------------------------------------------

const std::string& RelationshipDescriptor::GetType() const noexcept
{
    return _type;
}

void RelationshipDescriptor::SetType(const std::string& value)
{
    _type = value;
}

void RelationshipDescriptor::SetType(std::string&& value) noexcept
{
    _type = std::move(value);
}

// ---------------------------------------------------------------------------
// Target
// ---------------------------------------------------------------------------

const std::string& RelationshipDescriptor::GetTarget() const noexcept
{
    return _target;
}

void RelationshipDescriptor::SetTarget(const std::string& value)
{
    _target = value;
}

void RelationshipDescriptor::SetTarget(std::string&& value) noexcept
{
    _target = std::move(value);
}

// ---------------------------------------------------------------------------
// IsExternal
// ---------------------------------------------------------------------------

bool RelationshipDescriptor::GetIsExternal() const noexcept
{
    return _isExternal;
}

void RelationshipDescriptor::SetIsExternal(bool value) noexcept
{
    _isExternal = value;
}

}}}  // namespace Aspose::Cells_FOSS::Packaging
