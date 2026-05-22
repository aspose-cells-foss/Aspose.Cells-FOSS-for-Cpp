#pragma once
#include <string>
#include <string_view>
#include <vector>

#include "aspose/cells_foss/CellArea.h"
#include "aspose/cells_foss/FormatConditionType.h"
#include "aspose/cells_foss/OperatorType.h"
#include "aspose/cells_foss/core/ConditionalFormattingModel.h"
#include "aspose/cells_foss/core/FormatConditionModel.h"

namespace Aspose {
namespace Cells_FOSS {

class FormatCondition;

/// <summary>
/// Represents a collection of format condition objects.
/// </summary>
class FormatConditionCollection final {
public:
    /// <summary>
    /// Initializes a new instance of the FormatConditionCollection class.
    /// </summary>
    /// <param name="owner">The owner collection of conditional formatting models.</param>
    /// <param name="model">The conditional formatting model.</param>
    FormatConditionCollection(std::vector<Core::ConditionalFormattingModel>& owner,
                              Core::ConditionalFormattingModel& model);

    /// <summary>
    /// Gets the number of items.
    /// </summary>
    int GetCount() const noexcept;

    /// <summary>
    /// Gets the range count.
    /// </summary>
    int GetRangeCount() const noexcept;

    /// <summary>
    /// Gets the element at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    /// <returns>The format condition.</returns>
    FormatCondition operator[](int index);

    /// <summary>
    /// Adds the specified item.
    /// </summary>
    /// <param name="area">The area.</param>
    /// <param name="type">The type.</param>
    /// <param name="operatorType">The operator type.</param>
    /// <param name="formula1">The formula1.</param>
    /// <param name="formula2">The formula2.</param>
    /// <returns>The zero-based index of the added item.</returns>
    int Add(const CellArea& area, FormatConditionType type, OperatorType operatorType,
            std::string_view formula1, std::string_view formula2);

    /// <summary>
    /// Adds the specified condition.
    /// </summary>
    /// <param name="type">The type.</param>
    /// <returns>The zero-based index of the added item.</returns>
    int AddCondition(FormatConditionType type);

    /// <summary>
    /// Adds the specified condition.
    /// </summary>
    /// <param name="type">The type.</param>
    /// <param name="operatorType">The operator type.</param>
    /// <param name="formula1">The formula1.</param>
    /// <param name="formula2">The formula2.</param>
    /// <returns>The zero-based index of the added item.</returns>
    int AddCondition(FormatConditionType type, OperatorType operatorType,
                     std::string_view formula1, std::string_view formula2);

    /// <summary>
    /// Adds the specified area.
    /// </summary>
    /// <param name="area">The area.</param>
    void AddArea(const CellArea& area);

    /// <summary>
    /// Gets the cell area at the specified index.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    /// <returns>The cell area.</returns>
    CellArea GetCellArea(int index) const;

    /// <summary>
    /// Removes the area at the specified index.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    void RemoveArea(int index);

    /// <summary>
    /// Removes the area at the specified location.
    /// </summary>
    /// <param name="startRow">The start row.</param>
    /// <param name="startColumn">The start column.</param>
    /// <param name="totalRows">The total number of rows.</param>
    /// <param name="totalColumns">The total number of columns.</param>
    void RemoveArea(int startRow, int startColumn, int totalRows, int totalColumns);

    /// <summary>
    /// Removes the specified area.
    /// </summary>
    /// <param name="area">The cell area to remove.</param>
    void RemoveArea(const CellArea& area);

    /// <summary>
    /// Removes the condition at the specified index.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    void RemoveCondition(int index);

    /// <summary>
    /// Removes the specified condition.
    /// </summary>
    /// <param name="owner">The owner collection.</param>
    /// <param name="collection">The collection.</param>
    /// <param name="model">The model to remove.</param>
    static void RemoveCondition(std::vector<Core::ConditionalFormattingModel>& owner,
                                Core::ConditionalFormattingModel& collection,
                                Core::FormatConditionModel& model);

    /// <summary>
    /// Determines whether the specified areas overlap.
    /// </summary>
    /// <param name="left">The left area.</param>
    /// <param name="right">The right area.</param>
    /// <returns>true if the areas overlap; otherwise, false.</returns>
    static bool AreasOverlap(const CellArea& left, const CellArea& right);

    /// <summary>
    /// Sorts the specified areas.
    /// </summary>
    /// <param name="areas">The areas to sort.</param>
    static void SortAreas(std::vector<CellArea>& areas);

    /// <summary>
    /// Compares two areas for sorting.
    /// </summary>
    /// <param name="left">The left area.</param>
    /// <param name="right">The right area.</param>
    /// <returns>A negative value if left is less than right, zero if equal, positive if greater.</returns>
    static int CompareAreas(const CellArea& left, const CellArea& right);

private:
    static void InitializeDefaults(Core::FormatConditionModel& condition);
    static void RemoveCollectionIfEmpty(std::vector<Core::ConditionalFormattingModel>& owner,
                                        Core::ConditionalFormattingModel& collection);
    static void ReplaceAreas(Core::ConditionalFormattingModel& model,
                             const std::vector<CellArea>& areas);
    static std::string NormalizeFormula(std::string_view value);
    static std::string NormalizeText(std::string_view value);
    static void ValidateArea(const CellArea& area);
    static std::vector<CellArea> SubtractAreas(const std::vector<CellArea>& sourceAreas,
                                               const CellArea& removal);
    static void SubtractArea(const CellArea& source, const CellArea& removal,
                             std::vector<CellArea>& output);
    static void AddIfNonEmpty(std::vector<CellArea>& areas,
                              int firstRow, int firstColumn, int lastRow, int lastColumn);

    std::vector<Core::ConditionalFormattingModel>& _owner;
    Core::ConditionalFormattingModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
