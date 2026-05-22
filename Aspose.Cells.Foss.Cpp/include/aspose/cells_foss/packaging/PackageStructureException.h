#pragma once
#include <exception>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

/// Represents an error that occurs during package structure.
class PackageStructureException : public std::exception {
public:
    /// Initializes a new instance of the PackageStructureException class.
    /// @param message The error message.
    explicit PackageStructureException(std::string_view message);

    const char* what() const noexcept override;

private:
    std::string _message;
};

}}}  // namespace Aspose::Cells_FOSS::Packaging
