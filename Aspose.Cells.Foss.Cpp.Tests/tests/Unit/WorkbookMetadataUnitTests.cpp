#include <gtest/gtest.h>

#include "Support/ScenarioFactories.h"

using namespace Aspose::Cells_FOSS::Tests;

TEST(WorkbookMetadataUnitTests, workbook_metadata_apis_mutate_expected_settings)
{
    auto workbook = CreateWorkbookMetadataWorkbook();
    AssertWorkbookMetadata(workbook);
}
