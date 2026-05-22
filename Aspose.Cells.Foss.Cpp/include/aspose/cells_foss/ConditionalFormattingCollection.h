#pragma once
#include <vector>

#include "aspose/cells_foss/core/ConditionalFormattingModel.h"

namespace Aspose {
namespace Cells_FOSS {

class FormatConditionCollection;

/// <summary>
/// Represents a collection of conditional formatting objects.
/// </summary>
class ConditionalFormattingCollection final {
public:
    /// <summary>
    /// Initializes a new instance of the ConditionalFormattingCollection class.
    /// </summary>
    /// <param name="collections">The collection of conditional formatting models.</param>
    explicit ConditionalFormattingCollection(
        std::vector<Core::ConditionalFormattingModel>& collections);

    /// <summary>
    /// Gets the number of items.
    /// </summary>
    /// <returns>The number of conditional formatting objects.</returns>
    int GetCount() const noexcept;

    /// <summary>
    /// Gets the element at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    /// <returns>The format condition collection at the specified index.</returns>
    FormatConditionCollection operator[](int index) const;

    /// <summary>
    /// Adds the specified item.
    /// </summary>
    /// <returns>The zero-based index of the added item.</returns>
    int Add();

    /// <summary>
    /// Removes the specified item.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    void RemoveAt(int index);

    /// <summary>
    /// Removes the specified item.
    /// </summary>
    /// <param name="startRow">The start row.</param>
    /// <param name="startColumn">The start column.</param>
    /// <param name="totalRows">The total number of rows.</param>
    /// <param name="totalColumns">The total number of columns.</param>
    void RemoveArea(int startRow, int startColumn, int totalRows, int totalColumns);

    /// <summary>
    /// Gets the next priority for the specified collections.
    /// </summary>
    /// <param name="collections">The collection of conditional formatting models.</param>
    /// <returns>The next priority value.</returns>
    static int GetNextPriority(
        const std::vector<Core::ConditionalFormattingModel>& collections);

private:
    std::vector<Core::ConditionalFormattingModel>& _collections;
};

}}  // namespace Aspose::Cells_FOSS
