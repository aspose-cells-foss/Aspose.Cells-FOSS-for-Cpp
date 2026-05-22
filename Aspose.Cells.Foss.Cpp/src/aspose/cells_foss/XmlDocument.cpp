#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/XmlDocument.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// XNamespace
// ---------------------------------------------------------------------------

const XNamespace XNamespace::Xml("http://www.w3.org/XML/1998/namespace");

XNamespace::XNamespace(std::string_view uri)
    : _uri(uri)
{
}

std::string XNamespace::operator+(std::string_view localName) const
{
    std::string result;
    result.reserve(1 + _uri.size() + 1 + localName.size());
    result.push_back('{');
    result.append(_uri);
    result.push_back('}');
    result.append(localName);
    return result;
}

// ---------------------------------------------------------------------------
// XmlAttribute
// ---------------------------------------------------------------------------

XmlAttribute::XmlAttribute() = default;

XmlAttribute XmlAttribute::MakeAttribute(std::string value)
{
    XmlAttribute attr;
    attr._value = std::make_shared<std::string>(std::move(value));
    return attr;
}

bool XmlAttribute::IsNull() const noexcept
{
    return !_value;
}

const std::string& XmlAttribute::GetValue() const
{
    static const std::string empty;
    return _value ? *_value : empty;
}

// ---------------------------------------------------------------------------
// XmlElement
// ---------------------------------------------------------------------------

XmlElement::XmlElement() = default;
XmlElement::~XmlElement() = default;
XmlElement::XmlElement(const XmlElement&) = default;
XmlElement& XmlElement::operator=(const XmlElement&) = default;
XmlElement::XmlElement(XmlElement&&) noexcept = default;
XmlElement& XmlElement::operator=(XmlElement&&) noexcept = default;

XmlElement XmlElement::MakeElement(std::shared_ptr<XmlNodeData> node)
{
    return XmlElement(std::move(node));
}

XmlElement::XmlElement(std::shared_ptr<XmlNodeData> node)
    : _node(std::move(node))
{
}

bool XmlElement::IsNull() const noexcept
{
    return !_node;
}

const std::string& XmlElement::GetName() const
{
    static const std::string empty;
    return _node ? _node->qualifiedName : empty;
}

std::string XmlElement::GetValue() const
{
    if (!_node) {
        return {};
    }
    return CollectText();
}

std::string XmlElement::CollectText() const
{
    if (_node->isTextNode) {
        return _node->text;
    }
    std::string result;
    for (const auto& child : _node->children) {
        if (child->isTextNode) {
            result.append(child->text);
        } else {
            XmlElement childElem(child);
            result.append(childElem.CollectText());
        }
    }
    return result;
}

bool XmlElement::IsTextNode() const noexcept
{
    return _node && _node->isTextNode;
}

std::vector<std::pair<std::string, std::string>> XmlElement::GetAttributes() const
{
    std::vector<std::pair<std::string, std::string>> result;
    if (!_node) {
        return result;
    }
    result.reserve(_node->attributes.size());
    for (const auto& attr : _node->attributes) {
        result.emplace_back(attr.localName, attr.value);
    }
    return result;
}

std::vector<XmlElement> XmlElement::GetChildNodes() const
{
    std::vector<XmlElement> result;
    if (!_node) {
        return result;
    }
    result.reserve(_node->children.size());
    for (const auto& child : _node->children) {
        result.push_back(XmlElement(child));
    }
    return result;
}

XmlAttribute XmlElement::GetAttribute(std::string_view localName) const
{
    if (!_node) {
        return XmlAttribute();
    }
    for (const auto& attr : _node->attributes) {
        if (attr.localName == localName) {
            return XmlAttribute::MakeAttribute(attr.value);
        }
    }
    return XmlAttribute();
}

std::vector<XmlElement> XmlElement::GetElements(std::string_view qualifiedName) const
{
    std::vector<XmlElement> result;
    if (!_node) {
        return result;
    }
    for (const auto& child : _node->children) {
        if (!child->isTextNode && child->qualifiedName == qualifiedName) {
            result.push_back(XmlElement(child));
        }
    }
    return result;
}

XmlElement XmlElement::GetElement(std::string_view qualifiedName) const
{
    if (!_node) {
        return XmlElement();
    }
    for (const auto& child : _node->children) {
        if (!child->isTextNode && child->qualifiedName == qualifiedName) {
            return XmlElement(child);
        }
    }
    return XmlElement();
}

std::vector<XmlElement> XmlElement::GetDescendantsAndSelf() const
{
    std::vector<XmlElement> result;
    CollectDescendants(result);
    return result;
}

void XmlElement::CollectDescendants(std::vector<XmlElement>& out) const
{
    if (!_node || _node->isTextNode) {
        return;
    }
    out.push_back(*this);
    for (const auto& child : _node->children) {
        if (!child->isTextNode) {
            XmlElement(child).CollectDescendants(out);
        }
    }
}

// ---------------------------------------------------------------------------
// XmlDocument
// ---------------------------------------------------------------------------

XmlDocument::XmlDocument() = default;

bool XmlDocument::IsNull() const noexcept
{
    return _root.IsNull();
}

const XmlElement& XmlDocument::GetRoot() const
{
    return _root;
}

// ---------------------------------------------------------------------------
// XML Parser (recursive descent)
// ---------------------------------------------------------------------------

namespace {

class XmlParser {
public:
    explicit XmlParser(const uint8_t* data, size_t length)
        : _data(reinterpret_cast<const char*>(data))
        , _len(length)
        , _pos(0)
    {
    }

    std::shared_ptr<XmlNodeData> Parse()
    {
        SkipProlog();
        SkipWhitespace();
        return ParseElement(std::string());
    }

private:
    const char* _data;
    size_t _len;
    size_t _pos;

    struct NsScope {
        std::unordered_map<std::string, std::string> prefixMap;
        std::string defaultNs;
    };

    std::vector<NsScope> _nsStack;
    std::string _currentDefaultNs;

    char Peek() const
    {
        return _pos < _len ? _data[_pos] : '\0';
    }

    char Advance()
    {
        return _pos < _len ? _data[_pos++] : '\0';
    }

    bool HasMore() const
    {
        return _pos < _len;
    }

    bool Match(const char* s) const
    {
        const size_t slen = std::strlen(s);
        if (_pos + slen > _len) {
            return false;
        }
        return std::strncmp(_data + _pos, s, slen) == 0;
    }

    void SkipWhitespace()
    {
        while (HasMore() && std::isspace(static_cast<unsigned char>(Peek()))) {
            Advance();
        }
    }

    void SkipProlog()
    {
        SkipWhitespace();
        if (Match("<?")) {
            _pos += 2;
            while (HasMore() && !Match("?>")) {
                Advance();
            }
            if (Match("?>")) {
                _pos += 2;
            }
        }
        SkipWhitespace();
        while (Match("<!--")) {
            SkipComment();
            SkipWhitespace();
        }
    }

    void SkipComment()
    {
        _pos += 4;
        while (HasMore() && !Match("-->")) {
            Advance();
        }
        if (Match("-->")) {
            _pos += 3;
        }
    }

    std::shared_ptr<XmlNodeData> ParseElement(const std::string& parentDefaultNs)
    {
        if (!HasMore() || Peek() != '<') {
            return nullptr;
        }
        Advance(); // '<'

        std::string tagName = ReadName();
        if (tagName.empty()) {
            return nullptr;
        }

        // Push namespace scope
        NsScope scope;
        scope.defaultNs = parentDefaultNs;
        _nsStack.push_back(scope);
        const std::string savedDefaultNs = _currentDefaultNs;
        _currentDefaultNs = parentDefaultNs;

        auto node = std::make_shared<XmlNodeData>();

        // Parse attributes
        while (HasMore() && Peek() != '>' && Peek() != '/') {
            SkipWhitespace();
            if (Peek() == '>' || Peek() == '/') {
                break;
            }
            ParseAttribute(node);
        }

        // Process namespace declarations from attributes
        for (const auto& attr : node->attributes) {
            if (attr.localName == "xmlns") {
                _currentDefaultNs = attr.value;
                _nsStack.back().defaultNs = attr.value;
            } else if (attr.localName.size() > 6 &&
                       attr.localName.compare(0, 6, "xmlns:") == 0) {
                std::string prefix = attr.localName.substr(6);
                _nsStack.back().prefixMap[prefix] = attr.value;
            }
        }

        // Resolve element qualified name
        node->qualifiedName = ResolveQualifiedName(tagName);

        SkipWhitespace();

        // Self-closing tag
        if (Match("/>")) {
            _pos += 2;
            _nsStack.pop_back();
            _currentDefaultNs = savedDefaultNs;
            return node;
        }

        // Expect '>'
        if (Peek() == '>') {
            Advance();
        }

        // Parse content
        ParseContent(node);

        // Expect closing tag
        if (Match("</")) {
            _pos += 2;
            ReadName(); // discard closing tag name
            SkipWhitespace();
            if (Peek() == '>') {
                Advance();
            }
        }

        _nsStack.pop_back();
        _currentDefaultNs = savedDefaultNs;
        return node;
    }

    void ParseAttribute(const std::shared_ptr<XmlNodeData>& node)
    {
        std::string attrName = ReadName();
        if (attrName.empty()) {
            if (HasMore()) {
                Advance();
            }
            return;
        }

        SkipWhitespace();
        std::string attrValue;
        if (Peek() == '=') {
            Advance();
            SkipWhitespace();
            attrValue = ReadAttributeValue();
        }

        XmlNodeData::Attr attr;
        if (attrName == "xmlns" || (attrName.size() > 6 && attrName.compare(0, 6, "xmlns:") == 0)) {
            attr.localName = attrName;
        } else {
            const auto colonPos = attrName.find(':');
            if (colonPos != std::string::npos) {
                attr.localName = attrName.substr(colonPos + 1);
            } else {
                attr.localName = attrName;
            }
        }
        attr.value = std::move(attrValue);
        node->attributes.push_back(std::move(attr));
    }

    void ParseContent(const std::shared_ptr<XmlNodeData>& node)
    {
        while (HasMore()) {
            if (Match("</")) {
                return;
            }
            if (Match("<!--")) {
                SkipComment();
                continue;
            }
            if (Peek() == '<') {
                const size_t saved = _pos;
                Advance();
                if (HasMore() && Peek() != '/' && Peek() != '!') {
                    _pos = saved;
                    auto child = ParseElement(_currentDefaultNs);
                    if (child) {
                        node->children.push_back(std::move(child));
                    }
                } else {
                    _pos = saved;
                    return;
                }
            } else {
                std::string text = ReadTextContent();
                if (!text.empty()) {
                    auto textNode = std::make_shared<XmlNodeData>();
                    textNode->isTextNode = true;
                    textNode->text = std::move(text);
                    node->children.push_back(std::move(textNode));
                }
            }
        }
    }

    std::string ReadName()
    {
        std::string name;
        while (HasMore()) {
            const char c = Peek();
            if (std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == ':' || c == '-' || c == '.') {
                name.push_back(c);
                Advance();
            } else {
                break;
            }
        }
        return name;
    }

    std::string ReadAttributeValue()
    {
        if (!HasMore()) {
            return {};
        }
        const char quote = Advance();
        if (quote != '"' && quote != '\'') {
            return {};
        }
        std::string value;
        while (HasMore() && Peek() != quote) {
            if (Peek() == '&') {
                value.append(ReadEntity());
            } else {
                value.push_back(Advance());
            }
        }
        if (HasMore()) {
            Advance();
        }
        return value;
    }

    std::string ReadTextContent()
    {
        std::string text;
        while (HasMore() && Peek() != '<') {
            if (Peek() == '&') {
                text.append(ReadEntity());
            } else {
                text.push_back(Advance());
            }
        }
        return text;
    }

    std::string ReadEntity()
    {
        Advance(); // '&'
        std::string entity;
        while (HasMore() && Peek() != ';') {
            entity.push_back(Advance());
        }
        if (HasMore()) {
            Advance(); // ';'
        }
        if (entity == "amp") return {"&"};
        if (entity == "lt") return {"<"};
        if (entity == "gt") return {">"};
        if (entity == "quot") return {"\""};
        if (entity == "apos") return {"'"};
        if (!entity.empty() && entity[0] == '#') {
            long codepoint = 0;
            if (entity.size() > 1 && entity[1] == 'x') {
                codepoint = std::strtol(entity.c_str() + 2, nullptr, 16);
            } else {
                codepoint = std::strtol(entity.c_str() + 1, nullptr, 10);
            }
            if (codepoint > 0 && codepoint < 128) {
                return {static_cast<char>(codepoint)};
            }
            std::string result;
            if (codepoint < 0x80) {
                result.push_back(static_cast<char>(codepoint));
            } else if (codepoint < 0x800) {
                result.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
                result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
            } else if (codepoint < 0x10000) {
                result.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
                result.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
            } else {
                result.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
                result.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
            }
            return result;
        }
        return "&" + entity + ";";
    }

    std::string ResolveQualifiedName(const std::string& tagName) const
    {
        const auto colonPos = tagName.find(':');
        if (colonPos != std::string::npos) {
            const std::string prefix = tagName.substr(0, colonPos);
            const std::string localName = tagName.substr(colonPos + 1);
            for (auto it = _nsStack.rbegin(); it != _nsStack.rend(); ++it) {
                const auto found = it->prefixMap.find(prefix);
                if (found != it->prefixMap.end()) {
                    return "{" + found->second + "}" + localName;
                }
            }
            return "{}" + localName;
        }
        if (!_currentDefaultNs.empty()) {
            return "{" + _currentDefaultNs + "}" + tagName;
        }
        return "{}" + tagName;
    }
};

} // anonymous namespace

XmlDocument XmlDocument::Load(const std::vector<uint8_t>& data)
{
    if (data.empty()) {
        return XmlDocument();
    }

    XmlParser parser(data.data(), data.size());
    auto rootNode = parser.Parse();

    XmlDocument doc;
    if (rootNode) {
        doc._root = XmlElement::MakeElement(std::move(rootNode));
    }
    return doc;
}

// ---------------------------------------------------------------------------
// XmlElement programmatic construction
// ---------------------------------------------------------------------------

XmlElement XmlElement::MakeElement(std::string qualifiedName)
{
    auto node = std::make_shared<XmlNodeData>();
    node->qualifiedName = std::move(qualifiedName);
    return XmlElement(std::move(node));
}

XmlElement XmlElement::MakeElement(
    std::string qualifiedName,
    std::vector<std::pair<std::string, std::string>> attributes,
    std::vector<XmlElement> children)
{
    auto node = std::make_shared<XmlNodeData>();
    node->qualifiedName = std::move(qualifiedName);
    node->attributes.reserve(attributes.size());
    for (auto& attr : attributes) {
        XmlNodeData::Attr a;
        a.localName = std::move(attr.first);
        a.value = std::move(attr.second);
        node->attributes.push_back(std::move(a));
    }
    node->children.reserve(children.size());
    for (auto& child : children) {
        if (child._node) {
            node->children.push_back(child._node);
        }
    }
    return XmlElement(std::move(node));
}

void XmlElement::AddChild(const XmlElement& child)
{
    if (_node && child._node) {
        _node->children.push_back(child._node);
    }
}

void XmlElement::AddChild(XmlElement&& child)
{
    if (_node && child._node) {
        _node->children.push_back(std::move(child._node));
    }
}

void XmlElement::SetAttribute(std::string_view localName, std::string value)
{
    if (!_node) {
        return;
    }
    for (auto& attr : _node->attributes) {
        if (attr.localName == localName) {
            attr.value = std::move(value);
            return;
        }
    }
    XmlNodeData::Attr attr;
    attr.localName = std::string(localName);
    attr.value = std::move(value);
    _node->attributes.push_back(std::move(attr));
}

// ---------------------------------------------------------------------------
// XmlDocument::Build
// ---------------------------------------------------------------------------

XmlDocument XmlDocument::Build(const XmlElement& root)
{
    XmlDocument doc;
    doc._root = root;
    return doc;
}

// ---------------------------------------------------------------------------
// XML serialization helpers (used by XmlDocument::SaveToUtf8)
// ---------------------------------------------------------------------------

namespace {

// Parses a Clark notation name {uri}localname into its parts.
// Returns (uri, localName). If no braces, uri is empty.
std::pair<std::string_view, std::string_view> ParseClarkName(
    std::string_view qualifiedName)
{
    if (!qualifiedName.empty() && qualifiedName[0] == '{') {
        const auto close = qualifiedName.find('}');
        if (close != std::string_view::npos) {
            return {qualifiedName.substr(1, close - 1),
                    qualifiedName.substr(close + 1)};
        }
    }
    return {{}, qualifiedName};
}

void EscapeAttrValue(std::string& out, std::string_view value)
{
    for (const char c : value) {
        switch (c) {
        case '&':  out += "&amp;";  break;
        case '<':  out += "&lt;";   break;
        case '>':  out += "&gt;";   break;
        case '"':  out += "&quot;"; break;
        default:   out += c;        break;
        }
    }
}

void EscapeTextContent(std::string& out, std::string_view value)
{
    for (const char c : value) {
        switch (c) {
        case '&':  out += "&amp;"; break;
        case '<':  out += "&lt;";  break;
        case '>':  out += "&gt;";  break;
        default:   out += c;       break;
        }
    }
}

// Resolves a Clark notation name to a prefixed or plain name,
// given the default namespace and a prefix map built from xmlns:* attributes.
std::string ResolveQualifiedName(
    std::string_view qualifiedName,
    const std::string& defaultNs,
    const std::unordered_map<std::string, std::string>& uriToPrefix)
{
    auto [uri, local] = ParseClarkName(qualifiedName);
    if (uri.empty() || uri == defaultNs) {
        return std::string(local);
    }
    const auto it = uriToPrefix.find(std::string(uri));
    if (it != uriToPrefix.end()) {
        return it->second + ":" + std::string(local);
    }
    return std::string(local);
}

// Recursively serializes an element to the output string.
void SerializeElement(
    std::string& out,
    const XmlElement& elem,
    const std::string& defaultNs,
    const std::unordered_map<std::string, std::string>& uriToPrefix,
    bool isRoot)
{
    const auto tag = ResolveQualifiedName(elem.GetName(), defaultNs, uriToPrefix);

    out += '<';
    out += tag;

    // For the root element, emit the default namespace declaration.
    if (isRoot) {
        auto [rootUri, rootLocal] = ParseClarkName(elem.GetName());
        (void)rootLocal;
        if (!rootUri.empty()) {
            out += " xmlns=\"";
            EscapeAttrValue(out, rootUri);
            out += '"';
        }
    }

    // Emit attributes, resolving Clark notation {uri}localname to prefixed
    // names when a namespace prefix is known.
    const auto attrs = elem.GetAttributes();
    for (const auto& [name, value] : attrs) {
        out += ' ';
        if (!name.empty() && name[0] == '{') {
            auto [attrUri, attrLocal] = ParseClarkName(name);
            const auto it = uriToPrefix.find(std::string(attrUri));
            if (it != uriToPrefix.end()) {
                out += it->second;
                out += ':';
                out += attrLocal;
            } else {
                out += attrLocal;
            }
        } else {
            out += name;
        }
        out += "=\"";
        EscapeAttrValue(out, value);
        out += '"';
    }

    // Emit children.
    const auto children = elem.GetChildNodes();
    if (children.empty()) {
        out += " />";
    } else {
        out += '>';
        for (const auto& child : children) {
            if (child.IsNull()) {
                continue;
            }
            if (child.IsTextNode()) {
                // Text node: the name is empty; text is the value.
                EscapeTextContent(out, child.GetValue());
            } else {
                SerializeElement(out, child, defaultNs, uriToPrefix, false);
            }
        }
        out += "</";
        out += tag;
        out += '>';
    }
}

// Collects namespace URI -> prefix mappings from xmlns:* attributes
// in the given element tree.
void CollectNsPrefixes(
    const XmlElement& elem,
    std::unordered_map<std::string, std::string>& uriToPrefix)
{
    const auto attrs = elem.GetAttributes();
    for (const auto& [name, value] : attrs) {
        // xmlns:prefix="uri" attributes
        if (name.size() > 6 && name.compare(0, 6, "xmlns:") == 0) {
            const std::string prefix = name.substr(6);
            uriToPrefix[value] = prefix;
        }
    }
    const auto children = elem.GetChildNodes();
    for (const auto& child : children) {
        if (!child.IsNull() && !child.IsTextNode()) {
            CollectNsPrefixes(child, uriToPrefix);
        }
    }
}

}  // anonymous namespace

// ---------------------------------------------------------------------------
// XmlDocument::SaveToUtf8
// ---------------------------------------------------------------------------

std::vector<uint8_t> XmlDocument::SaveToUtf8() const
{
    std::string out;
    out.reserve(4096);

    out += "<?xml version=\"1.0\" encoding=\"utf-8\"?>";

    if (!_root.IsNull()) {
        // Build prefix map from xmlns:* attributes.
        std::unordered_map<std::string, std::string> uriToPrefix;
        CollectNsPrefixes(_root, uriToPrefix);

        SerializeElement(out, _root, std::string(), uriToPrefix, true);
    }

    return std::vector<uint8_t>(out.begin(), out.end());
}

}}  // namespace Aspose::Cells_FOSS
