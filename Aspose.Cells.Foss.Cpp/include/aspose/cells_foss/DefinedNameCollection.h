#pragma once

#include <optional>
#include <string_view>
#include <vector>

#include "aspose/cells_foss/DefinedName.h"
#include "aspose/cells_foss/core/DefinedNameModel.h"

namespace Aspose {
namespace Cells_FOSS {

class Workbook;

/// <summary>
/// Represents a collection of defined name objects.
/// </summary>
class DefinedNameCollection final {
public:
    /// <summary>
    /// Initializes a new instance of the DefinedNameCollection class.
    /// </summary>
    /// <param name="workbook">The owner workbook.</param>
    explicit DefinedNameCollection(Workbook& workbook);

    /// <summary>
    /// Gets the number of items.
    /// </summary>
    int GetCount() const noexcept;

    /// <summary>
    /// Gets the element at the specified zero-based index.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    /// <returns>The defined name at the given index.</returns>
    DefinedName operator[](int index) const;

    /// <summary>
    /// Adds the specified item.
    /// </summary>
    /// <param name="name">The name.</param>
    /// <param name="formula">The formula.</param>
    /// <returns>The zero-based index of the added item.</returns>
    int Add(std::string_view name, std::string_view formula);

    /// <summary>
    /// Adds the specified item.
    /// </summary>
    /// <param name="name">The name.</param>
    /// <param name="formula">The formula.</param>
    /// <param name="localSheetIndex">The local sheet index.</param>
    /// <returns>The zero-based index of the added item.</returns>
    int Add(std::string_view name, std::string_view formula,
            std::optional<int> localSheetIndex);

    /// <summary>
    /// Removes the specified item.
    /// </summary>
    /// <param name="index">The zero-based index.</param>
    void RemoveAt(int index);

    /// <summary>
    /// Returns a snapshot of all defined names in the collection.
    /// </summary>
    /// <returns>A vector containing copies of all defined names.</returns>
    std::vector<DefinedName> GetEnumerator() const;

private:
    Workbook& _workbook;
    std::vector<Core::DefinedNameModel>& _definedNames;
};

}}  // namespace Aspose::Cells_FOSS
