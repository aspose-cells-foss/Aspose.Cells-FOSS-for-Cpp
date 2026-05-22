#pragma once

#include <cctype>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "aspose/cells_foss/packaging/PackagePartDescriptor.h"
#include "aspose/cells_foss/packaging/RelationshipDescriptor.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

// ---------------------------------------------------------------------------
// Case-insensitive hash and equality for std::string keys
// ---------------------------------------------------------------------------

struct CaseInsensitiveHash {
    std::size_t operator()(const std::string& key) const noexcept
    {
        std::size_t h = 0;
        for (unsigned char c : key) {
            h = h * 131u + static_cast<std::size_t>(std::tolower(c));
        }
        return h;
    }
};

struct CaseInsensitiveEqual {
    bool operator()(const std::string& lhs, const std::string& rhs) const noexcept
    {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        for (std::size_t i = 0; i < lhs.size(); ++i) {
            if (std::tolower(static_cast<unsigned char>(lhs[i])) !=
                std::tolower(static_cast<unsigned char>(rhs[i]))) {
                return false;
            }
        }
        return true;
    }
};

/// Type alias for the case-insensitive byte-array dictionary.
using UnsupportedPartsMap = std::unordered_map<
    std::string,
    std::vector<std::uint8_t>,
    CaseInsensitiveHash,
    CaseInsensitiveEqual>;

/// Represents package model.
class PackageModel final {
public:
    /// Constructs a PackageModel with empty collections.
    PackageModel();

    /// Gets the list of package part descriptors (const).
    const std::vector<PackagePartDescriptor>& GetParts() const noexcept;
    /// Gets the list of package part descriptors (mutable).
    std::vector<PackagePartDescriptor>& GetParts() noexcept;

    /// Gets the list of relationship descriptors (const).
    const std::vector<RelationshipDescriptor>& GetRelationships() const noexcept;
    /// Gets the list of relationship descriptors (mutable).
    std::vector<RelationshipDescriptor>& GetRelationships() noexcept;

    /// Gets the dictionary of unsupported parts with case-insensitive keys (const).
    const UnsupportedPartsMap& GetUnsupportedParts() const noexcept;
    /// Gets the dictionary of unsupported parts with case-insensitive keys (mutable).
    UnsupportedPartsMap& GetUnsupportedParts() noexcept;

private:
    std::vector<PackagePartDescriptor> _parts;
    std::vector<RelationshipDescriptor> _relationships;
    UnsupportedPartsMap _unsupportedParts;
};

}}}  // namespace Aspose::Cells_FOSS::Packaging
