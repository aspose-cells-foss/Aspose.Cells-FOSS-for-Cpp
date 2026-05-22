#include "aspose/cells_foss/XlsxWorkbookHyperlinks.h"
#include "aspose/cells_foss/InvalidFileFormatException.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"

#include <algorithm>
#include <cstdio>
#include <string>
#include <utility>

namespace Aspose {
namespace Cells_FOSS {

using Core::CellAddress;
using Core::HyperlinkModel;
using Core::MergeRegion;
using Core::WorksheetModel;

// ---------------------------------------------------------------------------
// Static namespace aliases
// ---------------------------------------------------------------------------

static const XNamespace& MainNs = XlsxWorkbookSerializerCommon::MainNs;
static const XNamespace& RelationshipNs = XlsxWorkbookSerializerCommon::RelationshipNs;
static const XNamespace& PackageRelationshipNs = XlsxWorkbookSerializerCommon::PackageRelationshipNs;

static const std::string HyperlinkRelationshipType =
    "http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink";

// ---------------------------------------------------------------------------
// BuildHyperlinksElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookHyperlinks::BuildHyperlinksElement(
    const WorksheetModel& worksheet)
{
    auto orderedHyperlinks = GetOrderedHyperlinks(worksheet.GetHyperlinks());
    if (orderedHyperlinks.empty()) {
        return XmlElement();
    }

    int relationshipId = 1;
    auto hyperlinksElement = XmlElement::MakeElement(MainNs + "hyperlinks");
    bool hasChildren = false;
    for (std::size_t index = 0; index < orderedHyperlinks.size(); ++index) {
        const auto& hyperlink = orderedHyperlinks[index];
        if (!ShouldPersistHyperlink(hyperlink)) {
            continue;
        }

        auto element = XmlElement::MakeElement(
            MainNs + "hyperlink",
            std::vector<std::pair<std::string, std::string>>{
                {"ref", ToRangeReference(hyperlink)}},
            std::vector<XmlElement>{});

        if (!hyperlink.GetAddress().empty()) {
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%d", relationshipId);
            element.SetAttribute(RelationshipNs + "id",
                                 std::string("rId") + buf);
            relationshipId++;
        }

        if (!hyperlink.GetSubAddress().empty()) {
            element.SetAttribute("location", hyperlink.GetSubAddress());
        }

        if (!hyperlink.GetScreenTip().empty()) {
            element.SetAttribute("tooltip", hyperlink.GetScreenTip());
        }

        if (!hyperlink.GetTextToDisplay().empty()) {
            element.SetAttribute("display", hyperlink.GetTextToDisplay());
        }

        hyperlinksElement.AddChild(std::move(element));
        hasChildren = true;
    }

    // Replicate C# check: if the element has no children, return null.
    if (!hasChildren) {
        return XmlElement();
    }

    return hyperlinksElement;
}

// ---------------------------------------------------------------------------
// BuildWorksheetHyperlinkRelationships
// ---------------------------------------------------------------------------

XmlDocument XlsxWorkbookHyperlinks::BuildWorksheetHyperlinkRelationships(
    const WorksheetModel& worksheet)
{
    auto orderedHyperlinks = GetOrderedHyperlinks(worksheet.GetHyperlinks());
    auto relationships = XmlElement::MakeElement(
        PackageRelationshipNs + "Relationships");
    int relationshipId = 1;
    bool hasChildren = false;

    for (std::size_t index = 0; index < orderedHyperlinks.size(); ++index) {
        const auto& hyperlink = orderedHyperlinks[index];
        if (hyperlink.GetAddress().empty()) {
            continue;
        }

        char buf[32];
        std::snprintf(buf, sizeof(buf), "%d", relationshipId);

        auto rel = XmlElement::MakeElement(
            PackageRelationshipNs + "Relationship",
            std::vector<std::pair<std::string, std::string>>{
                {"Id", std::string("rId") + buf},
                {"Type", HyperlinkRelationshipType},
                {"Target", hyperlink.GetAddress()},
                {"TargetMode", "External"}},
            std::vector<XmlElement>{});
        relationships.AddChild(std::move(rel));
        relationshipId++;
        hasChildren = true;
    }

    // Replicate C# check: if no child elements, return null document.
    if (!hasChildren) {
        return XmlDocument();
    }

    return XmlDocument::Build(relationships);
}

// ---------------------------------------------------------------------------
// LoadWorksheetHyperlinkTargets
// ---------------------------------------------------------------------------

std::unordered_map<std::string, std::string>
XlsxWorkbookHyperlinks::LoadWorksheetHyperlinkTargets(
    const ZipArchive& archive,
    std::string_view worksheetUri)
{
    auto relationshipsEntry =
        XlsxWorkbookArchiveHelpers::GetEntry(archive,
            GetWorksheetRelationshipsUri(worksheetUri));
    if (relationshipsEntry.IsNull()) {
        return {};
    }

    auto document = XlsxWorkbookArchiveHelpers::LoadDocument(relationshipsEntry);
    std::unordered_map<std::string, std::string> targets;

    const auto& root = document.GetRoot();
    if (root.IsNull()) {
        return targets;
    }

    const auto relationships =
        root.GetElements(PackageRelationshipNs + "Relationship");
    for (const auto& relationship : relationships) {
        const auto idAttr = relationship.GetAttribute("Id");
        const auto typeAttr = relationship.GetAttribute("Type");
        const auto targetAttr = relationship.GetAttribute("Target");

        const std::string id = idAttr.GetValue();
        const std::string type = typeAttr.GetValue();
        const std::string target = targetAttr.GetValue();

        if (id.empty() || target.empty() ||
            type != HyperlinkRelationshipType) {
            continue;
        }

        targets[id] = target;
    }

    return targets;
}

// ---------------------------------------------------------------------------
// LoadHyperlinks
// ---------------------------------------------------------------------------

void XlsxWorkbookHyperlinks::LoadHyperlinks(
    WorksheetModel& worksheetModel,
    const XmlElement& worksheetRoot,
    const std::unordered_map<std::string, std::string>& hyperlinkTargets,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    worksheetModel.GetHyperlinks().clear();

    const auto hyperlinksElement =
        worksheetRoot.GetElement(MainNs + "hyperlinks");
    if (hyperlinksElement.IsNull()) {
        return;
    }

    const auto hyperlinkElements =
        hyperlinksElement.GetElements(MainNs + "hyperlink");
    for (const auto& hyperlinkElement : hyperlinkElements) {
        const std::string reference =
            hyperlinkElement.GetAttribute("ref").GetValue();

        HyperlinkModel candidate;
        if (!TryParseHyperlinkReference(reference, candidate)) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The hyperlink reference '" + reference + "' is invalid.");
            }

            LoadIssue issue("HL-L001", DiagnosticSeverity::LossyRecoverable,
                "A hyperlink with an invalid reference was dropped during load.",
                true, true);
            issue.SetSheetName(sheetName);
            issue.SetCellRef(reference);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            continue;
        }

        if (ContainsOverlappingHyperlink(
                worksheetModel.GetHyperlinks(), candidate)) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The hyperlink reference '" + reference +
                    "' overlaps an existing hyperlink.");
            }

            LoadIssue issue("HL-L003", DiagnosticSeverity::LossyRecoverable,
                "Overlapping hyperlink ranges were normalized during load.",
                true, true);
            issue.SetSheetName(sheetName);
            issue.SetCellRef(reference);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            continue;
        }

        candidate.SetSubAddress(
            NormalizeAttributeValue(
                hyperlinkElement.GetAttribute("location").GetValue()));
        candidate.SetScreenTip(
            NormalizeAttributeValue(
                hyperlinkElement.GetAttribute("tooltip").GetValue()));
        candidate.SetTextToDisplay(
            NormalizeAttributeValue(
                hyperlinkElement.GetAttribute("display").GetValue()));

        const std::string relationshipId =
            hyperlinkElement.GetAttribute("id").GetValue();
        if (!relationshipId.empty()) {
            auto it = hyperlinkTargets.find(relationshipId);
            if (it != hyperlinkTargets.end()) {
                candidate.SetAddress(NormalizeAttributeValue(it->second));
            } else if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The hyperlink relationship '" + relationshipId +
                    "' could not be resolved.");
            } else {
                LoadIssue issue("HL-L002",
                    DiagnosticSeverity::LossyRecoverable,
                    "A hyperlink relationship was missing and the external "
                    "address was cleared.",
                    true, true);
                issue.SetSheetName(sheetName);
                issue.SetCellRef(reference);
                XlsxWorkbookArchiveHelpers::AddIssue(
                    diagnostics, options, issue);
            }
        }

        if (candidate.GetAddress().empty() &&
            candidate.GetSubAddress().empty()) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "The hyperlink reference '" + reference +
                    "' does not define an address or sub-address.");
            }

            LoadIssue issue("HL-L004", DiagnosticSeverity::LossyRecoverable,
                "A hyperlink without an address or sub-address was dropped "
                "during load.",
                true, true);
            issue.SetSheetName(sheetName);
            issue.SetCellRef(reference);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            continue;
        }

        worksheetModel.GetHyperlinks().push_back(std::move(candidate));
    }
}

// ---------------------------------------------------------------------------
// GetOrderedHyperlinks
// ---------------------------------------------------------------------------

std::vector<HyperlinkModel> XlsxWorkbookHyperlinks::GetOrderedHyperlinks(
    const std::vector<HyperlinkModel>& hyperlinks)
{
    std::vector<HyperlinkModel> ordered;
    ordered.reserve(hyperlinks.size());
    for (std::size_t index = 0; index < hyperlinks.size(); ++index) {
        ordered.push_back(hyperlinks[index]);
    }

    std::sort(ordered.begin(), ordered.end(),
              [](const HyperlinkModel& left, const HyperlinkModel& right) {
                  return CompareHyperlinks(left, right) < 0;
              });

    return ordered;
}

// ---------------------------------------------------------------------------
// CompareHyperlinks
// ---------------------------------------------------------------------------

int XlsxWorkbookHyperlinks::CompareHyperlinks(
    const HyperlinkModel& left,
    const HyperlinkModel& right)
{
    const int rowComparison = left.GetFirstRow() - right.GetFirstRow();
    if (rowComparison != 0) {
        return rowComparison;
    }

    const int columnComparison = left.GetFirstColumn() - right.GetFirstColumn();
    if (columnComparison != 0) {
        return columnComparison;
    }

    const int rowCountComparison = left.GetTotalRows() - right.GetTotalRows();
    if (rowCountComparison != 0) {
        return rowCountComparison;
    }

    return left.GetTotalColumns() - right.GetTotalColumns();
}

// ---------------------------------------------------------------------------
// ShouldPersistHyperlink
// ---------------------------------------------------------------------------

bool XlsxWorkbookHyperlinks::ShouldPersistHyperlink(
    const HyperlinkModel& hyperlink)
{
    return !hyperlink.GetAddress().empty() ||
           !hyperlink.GetSubAddress().empty();
}

// ---------------------------------------------------------------------------
// ToRangeReference
// ---------------------------------------------------------------------------

std::string XlsxWorkbookHyperlinks::ToRangeReference(
    const HyperlinkModel& hyperlink)
{
    const std::string first =
        CellAddress(hyperlink.GetFirstRow(),
                    hyperlink.GetFirstColumn()).ToString();
    if (hyperlink.GetTotalRows() == 1 &&
        hyperlink.GetTotalColumns() == 1) {
        return first;
    }

    const std::string last =
        CellAddress(hyperlink.GetFirstRow() + hyperlink.GetTotalRows() - 1,
                    hyperlink.GetFirstColumn() + hyperlink.GetTotalColumns() - 1)
            .ToString();
    return first + ":" + last;
}

// ---------------------------------------------------------------------------
// GetWorksheetRelationshipsUri
// ---------------------------------------------------------------------------

std::string XlsxWorkbookHyperlinks::GetWorksheetRelationshipsUri(
    std::string_view worksheetUri)
{
    // Trim leading slashes (matches C# TrimStart('/'))
    std::size_t start = 0;
    while (start < worksheetUri.size() && worksheetUri[start] == '/') {
        ++start;
    }
    const std::string normalizedUri(worksheetUri.substr(start));

    const auto slashPos = normalizedUri.rfind('/');
    const std::string directory =
        (slashPos != std::string::npos)
            ? normalizedUri.substr(0, slashPos + 1)
            : std::string();
    const std::string fileName =
        (slashPos != std::string::npos)
            ? normalizedUri.substr(slashPos + 1)
            : normalizedUri;

    return "/" + directory + "_rels/" + fileName + ".rels";
}

// ---------------------------------------------------------------------------
// TryParseHyperlinkReference
// ---------------------------------------------------------------------------

bool XlsxWorkbookHyperlinks::TryParseHyperlinkReference(
    std::string_view reference,
    HyperlinkModel& hyperlink)
{
    MergeRegion region;
    if (!XlsxWorkbookSerializerCommon::TryParseMergeReference(
            reference.empty() ? std::string_view{} : reference, region)) {
        return false;
    }

    hyperlink.SetFirstRow(region.GetFirstRow());
    hyperlink.SetFirstColumn(region.GetFirstColumn());
    hyperlink.SetTotalRows(region.GetTotalRows());
    hyperlink.SetTotalColumns(region.GetTotalColumns());
    return true;
}

// ---------------------------------------------------------------------------
// ContainsOverlappingHyperlink
// ---------------------------------------------------------------------------

bool XlsxWorkbookHyperlinks::ContainsOverlappingHyperlink(
    const std::vector<HyperlinkModel>& hyperlinks,
    const HyperlinkModel& candidate)
{
    for (std::size_t index = 0; index < hyperlinks.size(); ++index) {
        if (HyperlinksOverlap(hyperlinks[index], candidate)) {
            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
// HyperlinksOverlap
// ---------------------------------------------------------------------------

bool XlsxWorkbookHyperlinks::HyperlinksOverlap(
    const HyperlinkModel& left,
    const HyperlinkModel& right)
{
    const int leftLastRow = left.GetFirstRow() + left.GetTotalRows() - 1;
    const int leftLastColumn =
        left.GetFirstColumn() + left.GetTotalColumns() - 1;
    const int rightLastRow = right.GetFirstRow() + right.GetTotalRows() - 1;
    const int rightLastColumn =
        right.GetFirstColumn() + right.GetTotalColumns() - 1;

    return left.GetFirstRow() <= rightLastRow &&
           right.GetFirstRow() <= leftLastRow &&
           left.GetFirstColumn() <= rightLastColumn &&
           right.GetFirstColumn() <= leftLastColumn;
}

// ---------------------------------------------------------------------------
// NormalizeAttributeValue
// ---------------------------------------------------------------------------

std::string XlsxWorkbookHyperlinks::NormalizeAttributeValue(
    std::string_view value)
{
    // Matches C#: string.IsNullOrEmpty(value) ? null : value
    // In C++ returning empty string for null/empty and the value otherwise.
    return value.empty() ? std::string() : std::string(value);
}

}}  // namespace Aspose::Cells_FOSS
