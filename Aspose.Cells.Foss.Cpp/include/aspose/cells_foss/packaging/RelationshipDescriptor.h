#pragma once

#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

/// Represents relationship descriptor.
class RelationshipDescriptor final {
public:
    /// Constructs a RelationshipDescriptor with default values.
    RelationshipDescriptor();

    /// Gets or sets the id.
    const std::string& GetId() const noexcept;
    void SetId(const std::string& value);
    void SetId(std::string&& value) noexcept;

    /// Gets or sets the type.
    const std::string& GetType() const noexcept;
    void SetType(const std::string& value);
    void SetType(std::string&& value) noexcept;

    /// Gets or sets the target.
    const std::string& GetTarget() const noexcept;
    void SetTarget(const std::string& value);
    void SetTarget(std::string&& value) noexcept;

    /// Gets or sets a value indicating whether external.
    bool GetIsExternal() const noexcept;
    void SetIsExternal(bool value) noexcept;

private:
    std::string _id;
    std::string _type;
    std::string _target;
    bool _isExternal;
};

}}}  // namespace Aspose::Cells_FOSS::Packaging
