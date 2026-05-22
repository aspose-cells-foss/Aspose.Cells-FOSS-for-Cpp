#pragma once

#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

/// Represents package part descriptor.
class PackagePartDescriptor final {
public:
    /// Constructs a PackagePartDescriptor with default values.
    PackagePartDescriptor();

    /// Gets or sets the part uri.
    const std::string& GetPartUri() const noexcept;
    void SetPartUri(const std::string& value);
    void SetPartUri(std::string&& value) noexcept;

    /// Gets or sets the content type.
    const std::string& GetContentType() const noexcept;
    void SetContentType(const std::string& value);
    void SetContentType(std::string&& value) noexcept;

    /// Gets or sets the category.
    const std::string& GetCategory() const noexcept;
    void SetCategory(const std::string& value);
    void SetCategory(std::string&& value) noexcept;

private:
    std::string _partUri;
    std::string _contentType;
    std::string _category;
};

}}}  // namespace Aspose::Cells_FOSS::Packaging
