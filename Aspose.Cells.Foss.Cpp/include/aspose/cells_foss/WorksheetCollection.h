#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "aspose/cells_foss/Worksheet.h"

namespace Aspose {
namespace Cells_FOSS {

namespace Core {
class WorkbookModel;
}

class Workbook;

/// <summary>
/// Encapsulates the workbook's worksheets and active-sheet state.
/// </summary>
class WorksheetCollection final {
public:
    /// <summary>
    /// Initializes a new instance of the WorksheetCollection class.
    /// </summary>
    /// <param name="workbook">The parent workbook.</param>
    /// <param name="model">The underlying workbook model.</param>
    WorksheetCollection(Workbook& workbook, Core::WorkbookModel& model);

    ~WorksheetCollection();

    WorksheetCollection(const WorksheetCollection&) = delete;
    WorksheetCollection& operator=(const WorksheetCollection&) = delete;

    /// <summary>
    /// Gets the worksheet at the specified zero-based index.
    /// </summary>
    Worksheet& operator[](int index);

    /// <summary>
    /// Gets the worksheet with the specified name using case-insensitive lookup.
    /// </summary>
    Worksheet& operator[](std::string_view name);

    /// <summary>
    /// Gets the total number of worksheets in the workbook.
    /// </summary>
    int GetCount() const;

    /// <summary>
    /// Gets or sets the zero-based index of the active worksheet.
    /// </summary>
    int GetActiveSheetIndex() const;
    void SetActiveSheetIndex(int value);

    /// <summary>
    /// Gets or sets the name of the active worksheet.
    /// </summary>
    std::string GetActiveSheetName() const;
    void SetActiveSheetName(std::string_view value);

    /// <summary>
    /// Adds a worksheet using the next generated default name.
    /// </summary>
    /// <returns>The zero-based index of the added worksheet.</returns>
    int Add();

    /// <summary>
    /// Adds a worksheet with the specified name and returns its index.
    /// </summary>
    /// <param name="sheetName">The name of the new worksheet.</param>
    /// <returns>The zero-based index of the added worksheet.</returns>
    int Add(std::string_view sheetName);

    /// <summary>
    /// Removes the worksheet at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based index of the worksheet to remove.</param>
    void RemoveAt(int index);

    /// <summary>
    /// Removes the worksheet with the specified name.
    /// </summary>
    /// <param name="sheetName">The name of the worksheet to remove.</param>
    void RemoveAt(std::string_view sheetName);

    /// <summary>
    /// Returns an iterator to the beginning of the worksheet collection.
    /// </summary>
    std::vector<std::unique_ptr<Worksheet>>::const_iterator begin() const;

    /// <summary>
    /// Returns an iterator to the end of the worksheet collection.
    /// </summary>
    std::vector<std::unique_ptr<Worksheet>>::const_iterator end() const;

private:
    Workbook& _workbook;
    Core::WorkbookModel& _model;
    std::vector<std::unique_ptr<Worksheet>> _worksheets;

    void RebuildWrappers();
    int IndexOf(std::string_view name) const;
    std::string GenerateDefaultSheetName() const;
};

}}  // namespace Aspose::Cells_FOSS
