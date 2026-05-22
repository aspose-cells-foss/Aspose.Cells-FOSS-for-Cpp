#pragma once

#include "aspose/cells_foss/CellArea.h"
#include "aspose/cells_foss/CellValue.h"
#include "aspose/cells_foss/Workbook.h"

#include <cstdint>
#include <filesystem>
#include <locale>
#include <string>
#include <string_view>
#include <vector>

namespace Aspose::Cells_FOSS::Tests {

class TempDir final {
public:
    explicit TempDir(std::string_view suiteName = {});

    const std::filesystem::path& Root() const noexcept;
    std::filesystem::path Path(std::string_view name) const;

private:
    std::filesystem::path _path;
};

std::filesystem::path TestOutputRoot();
std::filesystem::path ResolveRepositoryFile(std::string_view firstSegment,
                                            std::string_view secondSegment);

std::vector<std::uint8_t> ReadAllBytes(const std::filesystem::path& path);
void WriteAllBytes(const std::filesystem::path& path, const std::vector<std::uint8_t>& bytes);
std::string BytesToString(const std::vector<std::uint8_t>& bytes);
std::vector<std::uint8_t> StringToBytes(std::string_view text);

bool Contains(std::string_view text, std::string_view expected);
std::string CurrentTestName();
std::string SanitizePathPart(std::string value);
std::locale MakeLocaleStrict(std::initializer_list<std::string_view> names);

std::string BuildCellName(int rowIndex, int columnIndex);
std::string AreaKey(const std::vector<CellArea>& areas);
void ExpectArea(const CellArea& area, int firstRow, int firstColumn, int totalRows, int totalColumns);

void AssertMixedWorkbook(Workbook& workbook, bool expectedDate1904);
void AssertWorkbookDataEquals(Workbook& expected, Workbook& actual);

}  // namespace Aspose::Cells_FOSS::Tests
