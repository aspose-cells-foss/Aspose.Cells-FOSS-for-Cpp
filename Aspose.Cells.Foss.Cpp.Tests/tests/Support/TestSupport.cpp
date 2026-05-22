#include "Support/TestSupport.h"

#include "aspose/cells_foss/CellValueType.h"
#include "aspose/cells_foss/Cells.h"
#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/WorkbookSettings.h"
#include "aspose/cells_foss/Worksheet.h"
#include "aspose/cells_foss/WorksheetCollection.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <cctype>
#include <fstream>
#include <stdexcept>

namespace Aspose::Cells_FOSS::Tests {
namespace {

std::filesystem::path ResolveRepositoryRoot()
{
    auto directory = std::filesystem::current_path();
    while (!directory.empty()) {
        if (std::filesystem::exists(directory / "Aspose.Cells.FOSS.Cpp.Porter.sln")
            || std::filesystem::exists(directory / "Aspose.Cells_FOSS.sln")
            || std::filesystem::exists(directory / "PLAN.md")) {
            return directory;
        }

        const auto parent = directory.parent_path();
        if (parent == directory) {
            break;
        }
        directory = parent;
    }

    return std::filesystem::current_path();
}

}  // namespace

TempDir::TempDir(std::string_view suiteName)
{
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    auto name = suiteName.empty() ? CurrentTestName() : SanitizePathPart(std::string(suiteName));
    _path = TestOutputRoot() / (name + "_" + std::to_string(stamp));
    std::filesystem::create_directories(_path);
}

const std::filesystem::path& TempDir::Root() const noexcept
{
    return _path;
}

std::filesystem::path TempDir::Path(std::string_view name) const
{
    return _path / std::string(name);
}

std::filesystem::path TestOutputRoot()
{
#ifdef ASPOSE_CELLS_FOSS_TEST_OUTPUT_DIR
    auto root = std::filesystem::path(ASPOSE_CELLS_FOSS_TEST_OUTPUT_DIR);
#else
    auto root = std::filesystem::path("workspace") / "temp";
#endif
    if (root.is_relative()) {
        root = std::filesystem::current_path() / root;
    }
    std::filesystem::create_directories(root);
    return root;
}

std::filesystem::path ResolveRepositoryFile(std::string_view firstSegment,
                                            std::string_view secondSegment)
{
    const auto root = ResolveRepositoryRoot();
    std::vector<std::filesystem::path> candidates = {
        root / std::string(firstSegment) / std::string(secondSegment),
        root / "workspace" / "aspose-cells-foss-for-net" / std::string(firstSegment) / std::string(secondSegment),
        root / "workspace" / "Aspose.Cells.Foss.Cpp.Tests" / std::string(firstSegment) / std::string(secondSegment),
    };

    for (const auto& candidate : candidates) {
        if (std::filesystem::exists(candidate)) {
            return candidate;
        }
    }

    throw std::runtime_error("Could not locate repository file: "
        + std::string(firstSegment) + "/" + std::string(secondSegment));
}

std::vector<std::uint8_t> ReadAllBytes(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for read: " + path.string());
    }

    return std::vector<std::uint8_t>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>());
}

void WriteAllBytes(const std::filesystem::path& path, const std::vector<std::uint8_t>& bytes)
{
    std::filesystem::create_directories(path.parent_path());
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for write: " + path.string());
    }

    file.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
}

std::string BytesToString(const std::vector<std::uint8_t>& bytes)
{
    return std::string(bytes.begin(), bytes.end());
}

std::vector<std::uint8_t> StringToBytes(std::string_view text)
{
    return std::vector<std::uint8_t>(text.begin(), text.end());
}

bool Contains(std::string_view text, std::string_view expected)
{
    return text.find(expected) != std::string_view::npos;
}

std::string CurrentTestName()
{
    const auto* info = ::testing::UnitTest::GetInstance()->current_test_info();
    const std::string suite = info == nullptr ? "unknown_suite" : info->test_suite_name();
    const std::string name = info == nullptr ? "unknown_test" : info->name();
    return SanitizePathPart(suite + "_" + name);
}

std::string SanitizePathPart(std::string value)
{
    for (char& c : value) {
        const auto ch = static_cast<unsigned char>(c);
        if (!std::isalnum(ch) && c != '_' && c != '-') {
            c = '_';
        }
    }
    return value;
}

std::locale MakeLocaleStrict(std::initializer_list<std::string_view> names)
{
    for (const auto name : names) {
        try {
            return std::locale(std::string(name).c_str());
        } catch (const std::runtime_error&) {
        }
    }

    std::string message = "None of the requested locales are available:";
    for (const auto name : names) {
        message += " ";
        message += std::string(name);
    }
    throw std::runtime_error(message);
}

std::string BuildCellName(int rowIndex, int columnIndex)
{
    auto dividend = columnIndex + 1;
    std::string name;
    while (dividend > 0) {
        const auto remainder = (dividend - 1) % 26;
        name.insert(name.begin(), static_cast<char>('A' + remainder));
        dividend = (dividend - remainder - 1) / 26;
    }

    return name + std::to_string(rowIndex + 1);
}

std::string AreaKey(const std::vector<CellArea>& areas)
{
    std::string result;
    for (std::size_t index = 0; index < areas.size(); ++index) {
        const auto& area = areas[index];
        const auto startCell = BuildCellName(area.GetFirstRow(), area.GetFirstColumn());
        auto value = startCell;
        if (area.GetTotalRows() != 1 || area.GetTotalColumns() != 1) {
            value += ":";
            value += BuildCellName(area.GetFirstRow() + area.GetTotalRows() - 1,
                                   area.GetFirstColumn() + area.GetTotalColumns() - 1);
        }

        if (!result.empty()) {
            result += " ";
        }
        result += value;
    }
    return result;
}

void ExpectArea(const CellArea& area, int firstRow, int firstColumn, int totalRows, int totalColumns)
{
    EXPECT_EQ(firstRow, area.GetFirstRow());
    EXPECT_EQ(firstColumn, area.GetFirstColumn());
    EXPECT_EQ(totalRows, area.GetTotalRows());
    EXPECT_EQ(totalColumns, area.GetTotalColumns());
}

void AssertMixedWorkbook(Workbook& workbook, bool expectedDate1904)
{
    EXPECT_EQ(expectedDate1904, workbook.GetSettings().GetDate1904());
    auto& sheet = workbook.GetWorksheets()[0];
    EXPECT_EQ("Data", sheet.GetName());
    EXPECT_EQ("Hello", sheet.GetCells()["A1"].GetStringValue());
    EXPECT_EQ(123, sheet.GetCells()["B1"].GetValue().AsInteger());
    EXPECT_TRUE(sheet.GetCells()["C1"].GetValue().AsBool());
    EXPECT_DOUBLE_EQ(12.5, sheet.GetCells()["D1"].GetValue().AsDouble());
    EXPECT_NEAR(6.02214076E+23, sheet.GetCells()["E1"].GetValue().AsDouble(), 1E+10);
    EXPECT_EQ(DateTime(2024, 5, 6, 7, 8, 9), sheet.GetCells()["F1"].GetValue().AsDateTime());
    EXPECT_EQ("=B1*2", sheet.GetCells()["G1"].GetFormula());
    EXPECT_EQ(20, sheet.GetCells()["G1"].GetValue().AsInteger());
}

void AssertWorkbookDataEquals(Workbook& expected, Workbook& actual)
{
    auto& expectedSheet = expected.GetWorksheets()[0];
    auto& actualSheet = actual.GetWorksheets()[0];
    for (const auto* cellName : {"A1", "B1", "C1", "D1", "E1", "F1", "G1"}) {
        EXPECT_EQ(expectedSheet.GetCells()[cellName].GetType(), actualSheet.GetCells()[cellName].GetType()) << cellName;
        EXPECT_EQ(expectedSheet.GetCells()[cellName].GetStringValue(), actualSheet.GetCells()[cellName].GetStringValue()) << cellName;
        EXPECT_EQ(expectedSheet.GetCells()[cellName].GetFormula(), actualSheet.GetCells()[cellName].GetFormula()) << cellName;
    }
}

}  // namespace Aspose::Cells_FOSS::Tests
