#pragma once

namespace Aspose {
namespace Cells_FOSS {
namespace Packaging {

/// Provides packaging conventions operations.
class PackagingConventions final {
public:
    PackagingConventions() = delete;

    /// Gets the content types part.
    static constexpr const char* ContentTypesPart = "/[Content_Types].xml";

    /// Gets the root relationships part.
    static constexpr const char* RootRelationshipsPart = "/_rels/.rels";

    /// Gets the workbook part.
    static constexpr const char* WorkbookPart = "/xl/workbook.xml";

    /// Gets the workbook relationships part.
    static constexpr const char* WorkbookRelationshipsPart = "/xl/_rels/workbook.xml.rels";

    /// Gets the office document relationship.
    static constexpr const char* OfficeDocumentRelationship =
        "http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument";

    /// Gets the worksheet relationship.
    static constexpr const char* WorksheetRelationship =
        "http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet";

    /// Gets the styles relationship.
    static constexpr const char* StylesRelationship =
        "http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles";

    /// Gets the shared strings relationship.
    static constexpr const char* SharedStringsRelationship =
        "http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings";
};

}}}  // namespace Aspose::Cells_FOSS::Packaging
