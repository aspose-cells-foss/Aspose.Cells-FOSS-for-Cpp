#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Represents an XML namespace, used to construct qualified element names.
class XNamespace {
public:
    XNamespace() = default;
    explicit XNamespace(std::string_view uri);

    /// Creates a qualified name in Clark notation: {uri}localname.
    std::string operator+(std::string_view localName) const;

    const std::string& GetUri() const noexcept { return _uri; }

    /// The XML namespace (http://www.w3.org/XML/1998/namespace).
    static const XNamespace Xml;

private:
    std::string _uri;
};

/// Internal node representation shared between XML types.
struct XmlNodeData {
    std::string qualifiedName;
    struct Attr {
        std::string localName;
        std::string value;
    };
    std::vector<Attr> attributes;
    std::vector<std::shared_ptr<XmlNodeData>> children;
    bool isTextNode = false;
    std::string text;

    bool IsTextNode() const noexcept { return isTextNode; }
};

/// Lightweight handle to an XML attribute. Default-constructed instances are null.
class XmlAttribute {
public:
    XmlAttribute();

    bool IsNull() const noexcept;
    const std::string& GetValue() const;

    // Factory used by the parser.
    static XmlAttribute MakeAttribute(std::string value);

private:
    std::shared_ptr<std::string> _value;
};

/// Lightweight handle to an XML element. Default-constructed instances are null.
class XmlElement {
public:
    XmlElement();
    ~XmlElement();
    XmlElement(const XmlElement&);
    XmlElement& operator=(const XmlElement&);
    XmlElement(XmlElement&&) noexcept;
    XmlElement& operator=(XmlElement&&) noexcept;

    bool IsNull() const noexcept;

    /// Gets the qualified name in Clark notation {uri}localname.
    const std::string& GetName() const;

    /// Gets the concatenated text of all descendant text nodes.
    std::string GetValue() const;

    /// Returns true if this node represents a text node rather than an element.
    bool IsTextNode() const noexcept;

    /// Gets all attributes as local-name/value pairs.
    std::vector<std::pair<std::string, std::string>> GetAttributes() const;

    /// Gets all direct child nodes (elements and text nodes).
    std::vector<XmlElement> GetChildNodes() const;

    /// Gets an attribute by local name. Returns a null XmlAttribute if not found.
    XmlAttribute GetAttribute(std::string_view localName) const;

    /// Gets all direct child elements with the given qualified name.
    std::vector<XmlElement> GetElements(std::string_view qualifiedName) const;

    /// Gets the first direct child element with the given qualified name.
    XmlElement GetElement(std::string_view qualifiedName) const;

    /// Gets all descendant elements including self, in document order.
    std::vector<XmlElement> GetDescendantsAndSelf() const;

    // Factory used by the parser.
    static XmlElement MakeElement(std::shared_ptr<XmlNodeData> node);

    /// Creates a new element with the given qualified name and no attributes or children.
    static XmlElement MakeElement(std::string qualifiedName);

    /// Creates a new element with the given qualified name, attributes, and child elements.
    static XmlElement MakeElement(
        std::string qualifiedName,
        std::vector<std::pair<std::string, std::string>> attributes,
        std::vector<XmlElement> children);

    /// Adds a child element to this element.
    void AddChild(const XmlElement& child);

    /// Adds a child element by moving.
    void AddChild(XmlElement&& child);

    /// Sets an attribute value, adding it if absent or updating if present.
    void SetAttribute(std::string_view localName, std::string value);

private:
    std::shared_ptr<XmlNodeData> _node;

    explicit XmlElement(std::shared_ptr<XmlNodeData> node);

    void CollectDescendants(std::vector<XmlElement>& out) const;
    std::string CollectText() const;
};

}}  // namespace Aspose::Cells_FOSS
