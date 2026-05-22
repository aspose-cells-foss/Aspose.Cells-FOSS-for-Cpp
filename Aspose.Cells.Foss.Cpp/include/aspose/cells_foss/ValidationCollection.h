#pragma once

#include <cstddef>
#include <functional>
#include <optional>
#include <vector>

#include "aspose/cells_foss/CellArea.h"
#include "aspose/cells_foss/Validation.h"
#include "aspose/cells_foss/core/ValidationModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents a collection of validation objects.
/// </summary>
class ValidationCollection final {
public:
    /// <summary>
    /// Initializes a new instance of the ValidationCollection class.
    /// </summary>
    /// <param name="validations">The underlying validation models.</param>
    explicit ValidationCollection(std::vector<Core::ValidationModel>& validations);

    /// <summary>
    /// Gets the number of items.
    /// </summary>
    int GetCount() const noexcept;

    /// <summary>
    /// Gets the element at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    /// <returns>The validation at the given index.</returns>
    Validation operator[](int index);

    /// <summary>
    /// Adds the specified item.
    /// </summary>
    /// <param name="area">The area.</param>
    /// <returns>The zero-based index of the added item.</returns>
    int Add(const CellArea& area);

    /// <summary>
    /// Gets the validation in cell.
    /// </summary>
    /// <param name="row">The zero-based row index.</param>
    /// <param name="column">The zero-based column index.</param>
    /// <returns>The validation, or empty if no validation is found.</returns>
    std::optional<Validation> GetValidationInCell(int row, int column);

    /// <summary>
    /// Removes the specified item.
    /// </summary>
    /// <param name="row">The zero-based row index.</param>
    /// <param name="column">The zero-based column index.</param>
    void RemoveACell(int row, int column);

    /// <summary>
    /// Removes the specified item.
    /// </summary>
    /// <param name="cellArea">The cell area.</param>
    void RemoveArea(const CellArea& cellArea);

    /// <summary>
    /// Adds the specified area to the validation.
    /// </summary>
    /// <param name="owner">The owner collection of validation models.</param>
    /// <param name="validation">The validation model.</param>
    /// <param name="area">The cell area.</param>
    static void AddAreaToValidation(std::vector<Core::ValidationModel>& owner,
                                    Core::ValidationModel& validation,
                                    const CellArea& area);

    /// <summary>
    /// Removes the specified area from the validation.
    /// </summary>
    /// <param name="owner">The owner collection of validation models.</param>
    /// <param name="validation">The validation model.</param>
    /// <param name="area">The cell area.</param>
    static void RemoveAreaFromValidation(std::vector<Core::ValidationModel>& owner,
                                         Core::ValidationModel& validation,
                                         const CellArea& area);

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
    std::vector<Core::ValidationModel>& _validations;

    static void ValidateArea(const CellArea& area);
    static void EnsureNoOverlap(std::vector<Core::ValidationModel>& validations,
                                std::optional<std::reference_wrapper<const Core::ValidationModel>> currentValidation,
                                const CellArea& candidate);
    static void ReplaceAreas(Core::ValidationModel& validation,
                             const std::vector<CellArea>& areas);
    static std::vector<CellArea> SubtractAreas(const std::vector<CellArea>& sourceAreas,
                                               const CellArea& removal);
    static void SubtractArea(const CellArea& source, const CellArea& removal,
                             std::vector<CellArea>& output);
    static void AddIfNonEmpty(std::vector<CellArea>& areas,
                              int firstRow, int firstColumn, int lastRow, int lastColumn);
    static bool Contains(const CellArea& area, int row, int column);
};

}}  // namespace Aspose::Cells_FOSS
