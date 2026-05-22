#pragma once

#include "aspose/cells_foss/ConditionalFormattingCollection.h"
#include "aspose/cells_foss/DefinedNameCollection.h"
#include "aspose/cells_foss/DocumentProperties.h"
#include "aspose/cells_foss/FormatConditionCollection.h"
#include "aspose/cells_foss/SaveFormat.h"
#include "aspose/cells_foss/SaveOptions.h"
#include "aspose/cells_foss/Style.h"
#include "aspose/cells_foss/Validation.h"
#include "aspose/cells_foss/ValidationCollection.h"
#include "aspose/cells_foss/Workbook.h"
#include "aspose/cells_foss/WorkbookProperties.h"
#include "aspose/cells_foss/WorkbookSettings.h"
#include "aspose/cells_foss/WorksheetCollection.h"

#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

namespace Aspose::Cells_FOSS::Tests {

class WorkbookHandle final {
public:
    WorkbookHandle();
    explicit WorkbookHandle(std::unique_ptr<Workbook> workbook);

    WorkbookHandle(const WorkbookHandle&) = delete;
    WorkbookHandle& operator=(const WorkbookHandle&) = delete;
    WorkbookHandle(WorkbookHandle&&) noexcept = default;
    WorkbookHandle& operator=(WorkbookHandle&&) noexcept = default;

    Workbook& Get() noexcept;
    const Workbook& Get() const noexcept;
    operator Workbook&() noexcept;
    operator const Workbook&() const noexcept;

    WorksheetCollection& GetWorksheets();
    const WorksheetCollection& GetWorksheets() const;
    WorkbookSettings& GetSettings();
    const WorkbookSettings& GetSettings() const;
    WorkbookProperties& GetProperties();
    const WorkbookProperties& GetProperties() const;
    DocumentProperties& GetDocumentProperties();
    const DocumentProperties& GetDocumentProperties() const;
    DefinedNameCollection& GetDefinedNames();
    const DefinedNameCollection& GetDefinedNames() const;

    void Save(std::string_view fileName) const;
    void Save(std::string_view fileName, SaveFormat format) const;
    void Save(std::string_view fileName, const SaveOptions& options) const;
    void Save(std::vector<std::uint8_t>& stream, SaveFormat format) const;
    void Save(std::vector<std::uint8_t>& stream, const SaveOptions& options) const;

private:
    std::unique_ptr<Workbook> _workbook;
};

WorkbookHandle CreateMixedCellWorkbook(bool useDate1904 = false);

WorkbookHandle CreateWorksheetSettingsWorkbook();
void AssertWorksheetSettings(Workbook& workbook);
void AssertWorksheetSettingsScenarioHasVisibleSheet(Workbook& workbook);

WorkbookHandle CreatePageSetupWorkbook();
void AssertPageSetup(Workbook& workbook);

WorkbookHandle CreateHyperlinkWorkbook();
void AssertHyperlinks(Workbook& workbook);

WorkbookHandle CreateStyledWorkbook();
void ApplyPrimaryStyle(Style& style);
void ApplyCustomNumberStyle(Style& style);
void ApplyDateStyle(Style& style);
void ApplyVerticalTextStyle(Style& style);
void AssertPrimaryStyle(const Style& style);
void AssertCustomNumberStyle(const Style& style);
void AssertDateStyle(const Style& style);
void AssertVerticalTextStyle(const Style& style);
void AssertStyleShowcase(Workbook& workbook);

WorkbookHandle CreateValidationWorkbook();
void AssertValidations(Workbook& workbook);
Validation GetValidationByArea(ValidationCollection& validations, const std::string& expectedArea);

WorkbookHandle CreateConditionalFormattingWorkbook();
WorkbookHandle CreateAdvancedConditionalFormattingWorkbook();
void AssertConditionalFormattings(Workbook& workbook);
void AssertAdvancedConditionalFormattings(Workbook& workbook);
FormatConditionCollection GetCollectionByArea(ConditionalFormattingCollection& collections,
                                             const std::string& expectedArea);

WorkbookHandle CreateDefinedNamesWorkbook();
void AssertDefinedNames(Workbook& workbook);

WorkbookHandle CreateAutoFilterWorkbook();
void AssertAutoFilter(Workbook& workbook);

WorkbookHandle CreateWorkbookMetadataWorkbook();
void AssertWorkbookMetadata(Workbook& workbook);
void AssertDocumentPropertiesAreDefault(Workbook& workbook);

}  // namespace Aspose::Cells_FOSS::Tests
