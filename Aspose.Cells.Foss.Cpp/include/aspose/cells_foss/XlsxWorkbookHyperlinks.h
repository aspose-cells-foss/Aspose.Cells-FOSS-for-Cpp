#pragma once

#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/XmlDocument.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/ZipArchive.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <string>
#include <string_view>
#include <unordered_map>

namespace Aspose {
namespace Cells_FOSS {

/// Provides static methods for building and loading worksheet hyperlink data.
class XlsxWorkbookHyperlinks {
public:
    XlsxWorkbookHyperlinks() = delete;

    /// Builds the hyperlinks XML element for a worksheet. Returns a null
    /// XmlElement when no hyperlinks exist.
    static XmlElement BuildHyperlinksElement(const Core::WorksheetModel& worksheet);

    /// Builds the worksheet hyperlink relationships XML document. Returns a
    /// null XmlDocument when no hyperlink relationships exist.
    static XmlDocument BuildWorksheetHyperlinkRelationships(const Core::WorksheetModel& worksheet);

    /// Loads hyperlink relationship targets from the worksheet .rels file.
    static std::unordered_map<std::string, std::string> LoadWorksheetHyperlinkTargets(
        const ZipArchive& archive,
        std::string_view worksheetUri);

    /// Loads hyperlinks from the worksheet XML root into the model.
    static void LoadHyperlinks(
        Core::WorksheetModel& worksheetModel,
        const XmlElement& worksheetRoot,
        const std::unordered_map<std::string, std::string>& hyperlinkTargets,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

private:
    static std::vector<Core::HyperlinkModel> GetOrderedHyperlinks(
        const std::vector<Core::HyperlinkModel>& hyperlinks);

    static int CompareHyperlinks(const Core::HyperlinkModel& left,
                                 const Core::HyperlinkModel& right);

    static bool ShouldPersistHyperlink(const Core::HyperlinkModel& hyperlink);

    static std::string ToRangeReference(const Core::HyperlinkModel& hyperlink);

    static std::string GetWorksheetRelationshipsUri(std::string_view worksheetUri);

    static bool TryParseHyperlinkReference(std::string_view reference,
                                           Core::HyperlinkModel& hyperlink);

    static bool ContainsOverlappingHyperlink(
        const std::vector<Core::HyperlinkModel>& hyperlinks,
        const Core::HyperlinkModel& candidate);

    static bool HyperlinksOverlap(const Core::HyperlinkModel& left,
                                  const Core::HyperlinkModel& right);

    static std::string NormalizeAttributeValue(std::string_view value);
};

}}  // namespace Aspose::Cells_FOSS
