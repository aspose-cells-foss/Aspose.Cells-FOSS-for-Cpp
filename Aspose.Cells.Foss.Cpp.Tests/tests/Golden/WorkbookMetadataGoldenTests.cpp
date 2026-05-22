#include <gtest/gtest.h>

#include "Support/PackageTestSupport.h"
#include "Support/ScenarioFactories.h"
#include "Support/TestSupport.h"

#include "aspose/cells_foss/DocumentProperties.h"
#include "aspose/cells_foss/Workbook.h"
#include "aspose/cells_foss/WorkbookProperties.h"
#include "aspose/cells_foss/XmlElement.h"

#include <array>

using namespace Aspose::Cells_FOSS;
using namespace Aspose::Cells_FOSS::Tests;

namespace {

void RewriteDocumentPropertiesTargets(const std::filesystem::path& packagePath,
                                      const std::string& coreTarget,
                                      const std::string& appTarget)
{
    Package::RewriteEntryText(packagePath, "[Content_Types].xml", [&](std::string content) {
        auto replaceAll = [](std::string& text, const std::string& from, const std::string& to) {
            for (auto pos = text.find(from); pos != std::string::npos; pos = text.find(from, pos + to.size())) {
                text.replace(pos, from.size(), to);
            }
        };
        replaceAll(content, "PartName=\"/docProps/core.xml\"", "PartName=\"" + coreTarget + "\"");
        replaceAll(content, "PartName=\"/docProps/app.xml\"", "PartName=\"" + appTarget + "\"");
        return content;
    });

    Package::RewriteEntryText(packagePath, "_rels/.rels", [&](std::string content) {
        auto replaceAll = [](std::string& text, const std::string& from, const std::string& to) {
            for (auto pos = text.find(from); pos != std::string::npos; pos = text.find(from, pos + to.size())) {
                text.replace(pos, from.size(), to);
            }
        };
        auto core = coreTarget;
        auto app = appTarget;
        if (!core.empty() && core.front() == '/') {
            core.erase(core.begin());
        }
        if (!app.empty() && app.front() == '/') {
            app.erase(app.begin());
        }
        replaceAll(content, "Target=\"docProps/core.xml\"", "Target=\"" + core + "\"");
        replaceAll(content, "Target=\"docProps/app.xml\"", "Target=\"" + app + "\"");
        return content;
    });
}

void RemoveDocumentPropertiesRelationships(const std::filesystem::path& packagePath)
{
    Package::RewriteEntryText(packagePath, "_rels/.rels", [](std::string content) {
        const std::array<std::string, 2> markers = {
            "http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties",
            "http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties",
        };
        for (const auto& marker : markers) {
            auto markerPos = content.find(marker);
            while (markerPos != std::string::npos) {
                auto start = content.rfind("<Relationship", markerPos);
                auto end = content.find("/>", markerPos);
                if (start == std::string::npos || end == std::string::npos) {
                    break;
                }
                content.erase(start, end + 2 - start);
                markerPos = content.find(marker);
            }
        }
        return content;
    });
}

}  // namespace

TEST(WorkbookMetadataGoldenTests, workbook_metadata_roundtrip_and_emit_expected_markup)
{
    TempDir temp("golden-workbook-metadata");
    const auto path = temp.Path("workbook-metadata.xlsx");
    auto workbook = CreateWorkbookMetadataWorkbook();
    workbook.Save(path.string());

    const auto workbookXml = Package::ReadEntryText(path, "xl/workbook.xml");
    const auto coreXml = Package::ReadEntryText(path, "docProps/core.xml");
    const auto appXml = Package::ReadEntryText(path, "docProps/app.xml");

    EXPECT_TRUE(Contains(workbookXml, "<workbookPr"));
    EXPECT_TRUE(Contains(workbookXml, "codeName=\"WorkbookCode\""));
    EXPECT_TRUE(Contains(workbookXml, "showObjects=\"placeholders\""));
    EXPECT_TRUE(Contains(workbookXml, "<workbookProtection"));
    EXPECT_TRUE(Contains(workbookXml, "workbookPassword=\"ABCD\""));
    EXPECT_TRUE(Contains(workbookXml, "<bookViews>"));
    EXPECT_TRUE(Contains(workbookXml, "activeTab=\"1\""));
    EXPECT_TRUE(Contains(workbookXml, "showSheetTabs=\"0\""));
    EXPECT_TRUE(Contains(workbookXml, "<calcPr"));
    EXPECT_TRUE(Contains(workbookXml, "calcMode=\"manual\""));
    EXPECT_TRUE(Contains(workbookXml, "refMode=\"R1C1\""));
    EXPECT_TRUE(Contains(coreXml, "Quarterly Summary"));
    EXPECT_TRUE(Contains(coreXml, "Automation"));
    EXPECT_TRUE(Contains(appXml, "Aspose.Cells_FOSS Tests"));
    EXPECT_TRUE(Contains(appXml, "https://example.com/base/"));

    Workbook loaded(path.string());
    AssertWorkbookMetadata(loaded);
}

TEST(WorkbookMetadataGoldenTests, extended_document_properties_do_not_inject_default_application_metadata)
{
    TempDir temp("golden-extended-document-properties-no-default-app");
    const auto path = temp.Path("document-properties-no-default-app.xlsx");
    Workbook workbook;
    workbook.GetDocumentProperties().SetCompany("Aspose Cells FOSS");
    workbook.GetDocumentProperties().SetManager("Release");
    workbook.Save(path.string());

    const auto appXml = Package::ReadEntryText(path, "docProps/app.xml");
    EXPECT_TRUE(Contains(appXml, "<Company>Aspose Cells FOSS</Company>"));
    EXPECT_TRUE(Contains(appXml, "<Manager>Release</Manager>"));
    EXPECT_FALSE(Contains(appXml, "<Application>"));
    EXPECT_FALSE(Contains(appXml, "<AppVersion>"));

    Workbook loaded(path.string());
    EXPECT_EQ("Aspose Cells FOSS", loaded.GetDocumentProperties().GetCompany());
    EXPECT_EQ("Release", loaded.GetDocumentProperties().GetManager());
    EXPECT_EQ("", loaded.GetDocumentProperties().GetExtended().GetApplication());
    EXPECT_EQ("", loaded.GetDocumentProperties().GetExtended().GetAppVersion());
}

TEST(WorkbookMetadataGoldenTests, workbook_metadata_loads_from_root_relationship_targets)
{
    TempDir temp("golden-workbook-metadata-root-relationships");
    const auto path = temp.Path("workbook-metadata-root-targets.xlsx");
    auto workbook = CreateWorkbookMetadataWorkbook();
    workbook.Save(path.string());

    Package::MoveEntry(path, "docProps/core.xml", "metadata/core-props.xml");
    Package::MoveEntry(path, "docProps/app.xml", "metadata/app-props.xml");
    RewriteDocumentPropertiesTargets(path, "/metadata/core-props.xml", "/metadata/app-props.xml");

    Workbook loaded(path.string());
    AssertWorkbookMetadata(loaded);
}

TEST(WorkbookMetadataGoldenTests, unreferenced_document_properties_parts_are_ignored)
{
    TempDir temp("golden-workbook-metadata-orphaned-docprops");
    const auto path = temp.Path("workbook-metadata-orphaned-docprops.xlsx");
    auto workbook = CreateWorkbookMetadataWorkbook();
    workbook.Save(path.string());

    RemoveDocumentPropertiesRelationships(path);

    Workbook loaded(path.string());
    EXPECT_EQ("WorkbookCode", loaded.GetProperties().GetCodeName());
    AssertDocumentPropertiesAreDefault(loaded);
}
