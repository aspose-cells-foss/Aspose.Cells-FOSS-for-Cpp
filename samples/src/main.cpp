#include <iostream>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

#include "aspose/cells_foss/Workbook.h"
#include "aspose/cells_foss/WorksheetCollection.h"
#include "aspose/cells_foss/Worksheet.h"
#include "aspose/cells_foss/Cell.h"
#include "aspose/cells_foss/Style.h"
#include "aspose/cells_foss/Color.h"
#include "aspose/cells_foss/Font.h"

using namespace Aspose::Cells_FOSS;

int main() {
    auto root = std::filesystem::path(ASPOSE_CELLS_FOSS_SAMPLES_OUTPUT_DIR);
    if (!std::filesystem::exists(root)) {
        std::filesystem::create_directories(root);
    }

    //Workbook workbook;
    //auto cell = workbook.GetWorksheets()[0].GetCells()["A1"];
    //cell.PutValue("Hello World");
    //std::string output = (root / "hello-world.xlsx").string();
    //workbook.Save(output);

    //std::cout << "Hello world done." << std::endl;

    Workbook workbook;
    Worksheet& sheet = workbook.GetWorksheets()[0];

    sheet.SetName("Products");
    sheet.GetCells()["A1"].PutValue("Product");
    sheet.GetCells()["B1"].PutValue("Price");
    sheet.GetCells()["A2"].PutValue("Apple");
    sheet.GetCells()["B2"].PutValue(2.99);
    sheet.GetCells()["A3"].PutValue("Orange");
    sheet.GetCells()["B3"].PutValue(1.99);
    sheet.GetCells()["B4"].SetFormula("=SUM(B2:B3)");

    Style headerStyle = sheet.GetCells()["A1"].GetStyle();
    Font font;
    font.SetBold(true);
    font.SetColor(Color::FromArgb(255, 255, 255, 255));
    headerStyle.SetFont(font);
    headerStyle.SetPattern(FillPattern::Solid);
    headerStyle.SetForegroundColor(Color::FromArgb(255, 34, 120, 212));
    sheet.GetCells()["A1"].SetStyle(headerStyle);
    sheet.GetCells()["B1"].SetStyle(headerStyle);

    workbook.Save("products.xlsx");

    return 0;
}
