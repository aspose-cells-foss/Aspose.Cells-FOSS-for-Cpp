#include "aspose/cells_foss/XlsxDocumentProperties.h"
#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/DiagnosticSeverity.h"
#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadIssue.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/XmlDocument.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"
#include "aspose/cells_foss/core/CoreDocumentPropertiesModel.h"
#include "aspose/cells_foss/core/DocumentPropertiesModel.h"
#include "aspose/cells_foss/core/ExtendedDocumentPropertiesModel.h"
#include "aspose/cells_foss/core/WorkbookModel.h"

#include <cctype>
#include <cstdio>
#include <exception>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Local type aliases
// ---------------------------------------------------------------------------

using Attrs = std::vector<std::pair<std::string, std::string>>;
using Children = std::vector<XmlElement>;

// ---------------------------------------------------------------------------
// Static namespace/relationship references
// ---------------------------------------------------------------------------

static const auto& PackageRelationshipNs =
    XlsxWorkbookSerializerCommon::PackageRelationshipNs;
static const auto& CorePropertiesRelationshipType =
    XlsxWorkbookSerializerCommon::CorePropertiesRelationshipType;
static const auto& ExtendedPropertiesRelationshipType =
    XlsxWorkbookSerializerCommon::ExtendedPropertiesRelationshipType;

// ---------------------------------------------------------------------------
// XML namespace constants
// ---------------------------------------------------------------------------

namespace {

const XNamespace CorePropertiesNs(
    "http://schemas.openxmlformats.org/package/2006/metadata/core-properties");
const XNamespace DublinCoreNs("http://purl.org/dc/elements/1.1/");
const XNamespace DublinCoreTermsNs("http://purl.org/dc/terms/");
const XNamespace XsiNs("http://www.w3.org/2001/XMLSchema-instance");
const XNamespace ExtendedPropertiesNs(
    "http://schemas.openxmlformats.org/officeDocument/2006/extended-properties");

// ---------------------------------------------------------------------------
// Anonymous helpers
// ---------------------------------------------------------------------------

XmlElement MakeTextNode(std::string text)
{
    auto node = std::make_shared<XmlNodeData>();
    node->isTextNode = true;
    node->text = std::move(text);
    return XmlElement::MakeElement(std::move(node));
}

XmlElement ElemText(const std::string& qualifiedName, std::string text)
{
    auto elem = XmlElement::MakeElement(std::string(qualifiedName));
    elem.AddChild(MakeTextNode(std::move(text)));
    return elem;
}

bool EqualsIgnoreCase(std::string_view left, std::string_view right)
{
    if (left.size() != right.size()) {
        return false;
    }
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(left[i])) !=
            std::tolower(static_cast<unsigned char>(right[i]))) {
            return false;
        }
    }
    return true;
}

std::string TrimEdge(std::string_view value)
{
    auto start = value.find_first_not_of(" \t\n\r");
    if (start == std::string_view::npos) {
        return std::string();
    }
    auto end = value.find_last_not_of(" \t\n\r");
    return std::string(value.substr(start, end - start + 1));
}

std::string ExtractLocalName(const std::string& qualifiedName)
{
    auto pos = qualifiedName.find('}');
    if (pos != std::string::npos && pos + 1 < qualifiedName.size()) {
        return qualifiedName.substr(pos + 1);
    }
    return qualifiedName;
}

std::string DateTimeToString(const DateTime& dt)
{
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d",
                  dt.GetYear(), dt.GetMonth(), dt.GetDay(),
                  dt.GetHour(), dt.GetMinute(), dt.GetSecond());
    return std::string(buf);
}

std::optional<DateTime> ParseIsoDateTime(std::string_view s)
{
    // Minimum valid: "YYYY-MM-DDTHH:MM:SS" = 19 chars
    if (s.size() < 19) {
        return std::nullopt;
    }
    if (s[4] != '-' || s[7] != '-' || s[10] != 'T' ||
        s[13] != ':' || s[16] != ':') {
        return std::nullopt;
    }

    auto parseTwoDigits = [](std::string_view sv, int& out) -> bool {
        if (sv.size() != 2) {
            return false;
        }
        if (sv[0] < '0' || sv[0] > '9' || sv[1] < '0' || sv[1] > '9') {
            return false;
        }
        out = (sv[0] - '0') * 10 + (sv[1] - '0');
        return true;
    };

    auto parseFourDigits = [](std::string_view sv, int& out) -> bool {
        if (sv.size() != 4) {
            return false;
        }
        out = 0;
        for (char c : sv) {
            if (c < '0' || c > '9') {
                return false;
            }
            out = out * 10 + (c - '0');
        }
        return true;
    };

    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;

    if (!parseFourDigits(s.substr(0, 4), year) ||
        !parseTwoDigits(s.substr(5, 2), month) ||
        !parseTwoDigits(s.substr(8, 2), day) ||
        !parseTwoDigits(s.substr(11, 2), hour) ||
        !parseTwoDigits(s.substr(14, 2), minute) ||
        !parseTwoDigits(s.substr(17, 2), second)) {
        return std::nullopt;
    }

    return DateTime(year, month, day, hour, minute, second);
}

std::optional<int> ParseInt(std::string_view s)
{
    if (s.empty()) {
        return std::nullopt;
    }

    std::size_t i = 0;
    bool negative = false;
    if (s[0] == '-') {
        negative = true;
        i = 1;
    } else if (s[0] == '+') {
        i = 1;
    }

    if (i >= s.size() || s[i] < '0' || s[i] > '9') {
        return std::nullopt;
    }

    long long result = 0;
    for (; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') {
            return std::nullopt;
        }
        result = result * 10 + (s[i] - '0');
        if (result > 0x7FFFFFFF) {
            return std::nullopt;
        }
    }

    return negative ? static_cast<int>(-result) : static_cast<int>(result);
}

}  // anonymous namespace

// ---------------------------------------------------------------------------
// BuildCorePropertiesDocument
// ---------------------------------------------------------------------------

XmlDocument XlsxDocumentProperties::BuildCorePropertiesDocument(
    const Core::WorkbookModel& model)
{
    if (!model.GetDocumentProperties().GetCore().HasStoredState()) {
        return XmlDocument();
    }

    const auto& core = model.GetDocumentProperties().GetCore();
    auto root = XmlElement::MakeElement(
        CorePropertiesNs + "coreProperties",
        Attrs{{"xmlns:cp", CorePropertiesNs.GetUri()},
              {"xmlns:dc", DublinCoreNs.GetUri()},
              {"xmlns:dcterms", DublinCoreTermsNs.GetUri()},
              {"xmlns:xsi", XsiNs.GetUri()}},
        Children{});

    AddStringElement(root, DublinCoreNs + "title", core.GetTitle());
    AddStringElement(root, DublinCoreNs + "subject", core.GetSubject());
    AddStringElement(root, DublinCoreNs + "creator", core.GetCreator());
    AddStringElement(root, CorePropertiesNs + "keywords", core.GetKeywords());
    AddStringElement(root, DublinCoreNs + "description",
                    core.GetDescription());
    AddStringElement(root, CorePropertiesNs + "lastModifiedBy",
                    core.GetLastModifiedBy());
    AddStringElement(root, CorePropertiesNs + "revision", core.GetRevision());
    AddStringElement(root, CorePropertiesNs + "category", core.GetCategory());
    AddStringElement(root, CorePropertiesNs + "contentStatus",
                    core.GetContentStatus());
    AddDateElement(root, DublinCoreTermsNs + "created", core.GetCreated());
    AddDateElement(root, DublinCoreTermsNs + "modified", core.GetModified());

    return XmlDocument::Build(root);
}

// ---------------------------------------------------------------------------
// BuildExtendedPropertiesDocument
// ---------------------------------------------------------------------------

XmlDocument XlsxDocumentProperties::BuildExtendedPropertiesDocument(
    const Core::WorkbookModel& model)
{
    if (!model.GetDocumentProperties().GetExtended().HasStoredState()) {
        return XmlDocument();
    }

    const auto& extended = model.GetDocumentProperties().GetExtended();
    auto root = XmlElement::MakeElement(
        ExtendedPropertiesNs + "Properties",
        Attrs{{"xmlns:vt",
               "http://schemas.openxmlformats.org/officeDocument/2006/"
               "docPropsVTypes"}},
        Children{});

    AddStringElement(root, ExtendedPropertiesNs + "Application",
                    extended.GetApplication());
    AddStringElement(root, ExtendedPropertiesNs + "AppVersion",
                    extended.GetAppVersion());
    AddStringElement(root, ExtendedPropertiesNs + "Company",
                    extended.GetCompany());
    AddStringElement(root, ExtendedPropertiesNs + "Manager",
                    extended.GetManager());
    AddIntElement(root, ExtendedPropertiesNs + "DocSecurity",
                  extended.GetDocSecurity());
    AddStringElement(root, ExtendedPropertiesNs + "HyperlinkBase",
                    extended.GetHyperlinkBase());
    AddBoolElement(root, ExtendedPropertiesNs + "ScaleCrop",
                   extended.GetScaleCrop());
    AddBoolElement(root, ExtendedPropertiesNs + "LinksUpToDate",
                   extended.GetLinksUpToDate());
    AddBoolElement(root, ExtendedPropertiesNs + "SharedDoc",
                   extended.GetSharedDoc());

    return XmlDocument::Build(root);
}

// ---------------------------------------------------------------------------
// LoadDocumentProperties
// ---------------------------------------------------------------------------

void XlsxDocumentProperties::LoadDocumentProperties(
    const ZipArchive& archive,
    Core::WorkbookModel& model,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options)
{
    auto relationshipTargets =
        LoadRootDocumentPropertiesRelationshipTargets(archive, diagnostics,
                                                      options);

    std::string corePropertiesPartUri;
    {
        auto it = relationshipTargets.find(CorePropertiesRelationshipType);
        if (it != relationshipTargets.end() && !it->second.empty()) {
            corePropertiesPartUri = it->second;
        }
    }

    std::string extendedPropertiesPartUri;
    {
        auto it = relationshipTargets.find(ExtendedPropertiesRelationshipType);
        if (it != relationshipTargets.end() && !it->second.empty()) {
            extendedPropertiesPartUri = it->second;
        }
    }

    LoadCoreProperties(archive, model, diagnostics, options,
                       corePropertiesPartUri);
    LoadExtendedProperties(archive, model, diagnostics, options,
                           extendedPropertiesPartUri);
}

// ---------------------------------------------------------------------------
// LoadRootDocumentPropertiesRelationshipTargets (private)
// ---------------------------------------------------------------------------

std::unordered_map<std::string, std::string>
XlsxDocumentProperties::LoadRootDocumentPropertiesRelationshipTargets(
    const ZipArchive& archive,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options)
{
    std::unordered_map<std::string, std::string> relationshipTargets;
    auto entry = XlsxWorkbookArchiveHelpers::GetEntry(archive, "/_rels/.rels");
    if (entry.IsNull()) {
        return relationshipTargets;
    }

    XmlDocument document;
    try {
        document = XlsxWorkbookArchiveHelpers::LoadDocument(entry);
    } catch (const std::exception&) {
        AddDocumentPropertiesIssue(
            diagnostics, options, "/_rels/.rels",
            "Package root relationships were malformed and document "
            "properties were ignored.");
        return relationshipTargets;
    }

    auto root = document.GetRoot();
    if (root.IsNull()) {
        return relationshipTargets;
    }

    auto relationships =
        root.GetElements(PackageRelationshipNs + "Relationship");
    for (const auto& relationship : relationships) {
        auto typeAttr = relationship.GetAttribute("Type");
        auto targetAttr = relationship.GetAttribute("Target");
        auto targetModeAttr = relationship.GetAttribute("TargetMode");

        std::string type =
            typeAttr.IsNull() ? std::string() : typeAttr.GetValue();
        std::string target =
            targetAttr.IsNull() ? std::string() : targetAttr.GetValue();
        std::string targetMode =
            targetModeAttr.IsNull() ? std::string() : targetModeAttr.GetValue();

        if (type.empty() || target.empty() ||
            EqualsIgnoreCase(targetMode, "External")) {
            continue;
        }

        std::string canonicalKey;
        if (EqualsIgnoreCase(type, CorePropertiesRelationshipType)) {
            canonicalKey = CorePropertiesRelationshipType;
        } else if (EqualsIgnoreCase(type,
                                    ExtendedPropertiesRelationshipType)) {
            canonicalKey = ExtendedPropertiesRelationshipType;
        } else {
            continue;
        }

        if (relationshipTargets.find(canonicalKey) ==
            relationshipTargets.end()) {
            relationshipTargets[canonicalKey] =
                XlsxWorkbookArchiveHelpers::ResolvePartUri("/", target);
        }
    }

    return relationshipTargets;
}

// ---------------------------------------------------------------------------
// LoadCoreProperties (private)
// ---------------------------------------------------------------------------

void XlsxDocumentProperties::LoadCoreProperties(
    const ZipArchive& archive,
    Core::WorkbookModel& model,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    const std::string& partUri)
{
    if (partUri.empty()) {
        return;
    }

    auto entry = XlsxWorkbookArchiveHelpers::GetEntry(archive, partUri);
    if (entry.IsNull()) {
        return;
    }

    XmlDocument document;
    try {
        document = XlsxWorkbookArchiveHelpers::LoadDocument(entry);
    } catch (const std::exception&) {
        AddDocumentPropertiesIssue(
            diagnostics, options, partUri,
            "Core document properties part was malformed and was ignored.");
        return;
    }

    auto root = document.GetRoot();
    if (root.IsNull()) {
        return;
    }

    auto& core = model.GetDocumentProperties().GetCore();
    core.SetTitle(ReadElementValue(root, DublinCoreNs + "title"));
    core.SetSubject(ReadElementValue(root, DublinCoreNs + "subject"));
    core.SetCreator(ReadElementValue(root, DublinCoreNs + "creator"));
    core.SetKeywords(
        ReadElementValue(root, CorePropertiesNs + "keywords"));
    core.SetDescription(
        ReadElementValue(root, DublinCoreNs + "description"));
    core.SetLastModifiedBy(
        ReadElementValue(root, CorePropertiesNs + "lastModifiedBy"));
    core.SetRevision(
        ReadElementValue(root, CorePropertiesNs + "revision"));
    core.SetCategory(
        ReadElementValue(root, CorePropertiesNs + "category"));
    core.SetContentStatus(
        ReadElementValue(root, CorePropertiesNs + "contentStatus"));
    core.SetCreated(ReadDateElement(root, DublinCoreTermsNs + "created",
                                    diagnostics, options, partUri));
    core.SetModified(ReadDateElement(root, DublinCoreTermsNs + "modified",
                                     diagnostics, options, partUri));
}

// ---------------------------------------------------------------------------
// LoadExtendedProperties (private)
// ---------------------------------------------------------------------------

void XlsxDocumentProperties::LoadExtendedProperties(
    const ZipArchive& archive,
    Core::WorkbookModel& model,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    const std::string& partUri)
{
    if (partUri.empty()) {
        return;
    }

    auto entry = XlsxWorkbookArchiveHelpers::GetEntry(archive, partUri);
    if (entry.IsNull()) {
        return;
    }

    XmlDocument document;
    try {
        document = XlsxWorkbookArchiveHelpers::LoadDocument(entry);
    } catch (const std::exception&) {
        AddDocumentPropertiesIssue(
            diagnostics, options, partUri,
            "Extended document properties part was malformed and was "
            "ignored.");
        return;
    }

    auto root = document.GetRoot();
    if (root.IsNull()) {
        return;
    }

    auto& extended = model.GetDocumentProperties().GetExtended();
    extended.SetApplication(
        ReadElementValue(root, ExtendedPropertiesNs + "Application"));
    extended.SetAppVersion(
        ReadElementValue(root, ExtendedPropertiesNs + "AppVersion"));
    extended.SetCompany(
        ReadElementValue(root, ExtendedPropertiesNs + "Company"));
    extended.SetManager(
        ReadElementValue(root, ExtendedPropertiesNs + "Manager"));
    extended.SetDocSecurity(
        ReadIntElement(root, ExtendedPropertiesNs + "DocSecurity",
                       diagnostics, options, partUri));
    extended.SetHyperlinkBase(
        ReadElementValue(root, ExtendedPropertiesNs + "HyperlinkBase"));
    extended.SetScaleCrop(
        ReadBoolElement(root, ExtendedPropertiesNs + "ScaleCrop",
                        diagnostics, options, partUri));
    extended.SetLinksUpToDate(
        ReadBoolElement(root, ExtendedPropertiesNs + "LinksUpToDate",
                        diagnostics, options, partUri));
    extended.SetSharedDoc(
        ReadBoolElement(root, ExtendedPropertiesNs + "SharedDoc",
                        diagnostics, options, partUri));
}

// ---------------------------------------------------------------------------
// AddStringElement (private)
// ---------------------------------------------------------------------------

void XlsxDocumentProperties::AddStringElement(
    XmlElement& parent,
    const std::string& qualifiedName,
    const std::string& value)
{
    if (!value.empty()) {
        parent.AddChild(ElemText(qualifiedName, value));
    }
}

// ---------------------------------------------------------------------------
// AddIntElement (private)
// ---------------------------------------------------------------------------

void XlsxDocumentProperties::AddIntElement(
    XmlElement& parent,
    const std::string& qualifiedName,
    std::optional<int> value)
{
    if (value.has_value()) {
        parent.AddChild(ElemText(qualifiedName, std::to_string(value.value())));
    }
}

// ---------------------------------------------------------------------------
// AddBoolElement (private)
// ---------------------------------------------------------------------------

void XlsxDocumentProperties::AddBoolElement(
    XmlElement& parent,
    const std::string& qualifiedName,
    std::optional<bool> value)
{
    if (value.has_value()) {
        parent.AddChild(
            ElemText(qualifiedName, value.value() ? "true" : "false"));
    }
}

// ---------------------------------------------------------------------------
// AddDateElement (private)
// ---------------------------------------------------------------------------

void XlsxDocumentProperties::AddDateElement(
    XmlElement& parent,
    const std::string& qualifiedName,
    std::optional<DateTime> value)
{
    if (!value.has_value()) {
        return;
    }

    auto element = ElemText(qualifiedName, DateTimeToString(value.value()));
    element.SetAttribute("xsi:type", "dcterms:W3CDTF");
    parent.AddChild(std::move(element));
}

// ---------------------------------------------------------------------------
// ReadElementValue (private)
// ---------------------------------------------------------------------------

std::string XlsxDocumentProperties::ReadElementValue(
    const XmlElement& parent,
    const std::string& qualifiedName)
{
    auto element = parent.GetElement(qualifiedName);
    if (element.IsNull()) {
        return std::string();
    }
    return TrimEdge(element.GetValue());
}

// ---------------------------------------------------------------------------
// ReadDateElement (private)
// ---------------------------------------------------------------------------

std::optional<DateTime> XlsxDocumentProperties::ReadDateElement(
    const XmlElement& parent,
    const std::string& qualifiedName,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    const std::string& partUri)
{
    auto element = parent.GetElement(qualifiedName);
    if (element.IsNull()) {
        return std::nullopt;
    }

    auto parsed = ParseIsoDateTime(element.GetValue());
    if (parsed.has_value()) {
        return parsed;
    }

    AddDocumentPropertiesIssue(
        diagnostics, options, partUri,
        "Document property '" + ExtractLocalName(qualifiedName) +
            "' had an invalid timestamp and was ignored.");
    return std::nullopt;
}

// ---------------------------------------------------------------------------
// ReadIntElement (private)
// ---------------------------------------------------------------------------

std::optional<int> XlsxDocumentProperties::ReadIntElement(
    const XmlElement& parent,
    const std::string& qualifiedName,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    const std::string& partUri)
{
    auto element = parent.GetElement(qualifiedName);
    if (element.IsNull()) {
        return std::nullopt;
    }

    auto parsed = ParseInt(element.GetValue());
    if (parsed.has_value() && parsed.value() >= 0) {
        return parsed;
    }

    AddDocumentPropertiesIssue(
        diagnostics, options, partUri,
        "Document property '" + ExtractLocalName(qualifiedName) +
            "' had an invalid integer value and was ignored.");
    return std::nullopt;
}

// ---------------------------------------------------------------------------
// ReadBoolElement (private)
// ---------------------------------------------------------------------------

std::optional<bool> XlsxDocumentProperties::ReadBoolElement(
    const XmlElement& parent,
    const std::string& qualifiedName,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    const std::string& partUri)
{
    auto element = parent.GetElement(qualifiedName);
    if (element.IsNull()) {
        return std::nullopt;
    }

    auto rawValue = element.GetValue();
    if (rawValue == "1" || EqualsIgnoreCase(rawValue, "true")) {
        return true;
    }

    if (rawValue == "0" || EqualsIgnoreCase(rawValue, "false")) {
        return false;
    }

    AddDocumentPropertiesIssue(
        diagnostics, options, partUri,
        "Document property '" + ExtractLocalName(qualifiedName) +
            "' had an invalid Boolean value and was ignored.");
    return std::nullopt;
}

// ---------------------------------------------------------------------------
// AddDocumentPropertiesIssue (private)
// ---------------------------------------------------------------------------

void XlsxDocumentProperties::AddDocumentPropertiesIssue(
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    const std::string& partUri,
    const std::string& message)
{
    LoadIssue issue("WB-L004", DiagnosticSeverity::Warning, message);
    issue.SetPartUri(partUri);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
}

}}  // namespace Aspose::Cells_FOSS
