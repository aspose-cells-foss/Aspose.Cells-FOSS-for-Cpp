#pragma once

#include "aspose/cells_foss/XmlElement.h"

#include <cstdint>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Represents a parsed XML document.
class XmlDocument {
public:
    XmlDocument();

    bool IsNull() const noexcept;

    /// Parses XML from UTF-8 bytes, preserving whitespace.
    static XmlDocument Load(const std::vector<uint8_t>& data);

    /// Builds an XML document with the given root element and optional declaration.
    static XmlDocument Build(const XmlElement& root);

    /// Gets the root element.
    const XmlElement& GetRoot() const;

    /// Serializes this document to UTF-8 encoded XML bytes
    /// (XML declaration + root element).
    std::vector<uint8_t> SaveToUtf8() const;

private:
    XmlElement _root;
};

}}  // namespace Aspose::Cells_FOSS
