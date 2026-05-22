#pragma once
#include <optional>

namespace Aspose {
namespace Cells_FOSS {

class Worksheet;

/// <summary>
/// Represents column.
/// </summary>
class Column final {
public:
    /// <summary>
    /// Constructs a Column associated with the given worksheet and column index.
    /// </summary>
    /// <param name="worksheet">The parent worksheet.</param>
    /// <param name="index">The zero-based column index.</param>
    Column(Worksheet& worksheet, int index);

    /// <summary>
    /// Gets or sets the width.
    /// </summary>
    std::optional<double> GetWidth() const;
    void SetWidth(std::optional<double> value);

    /// <summary>
    /// Gets or sets a value indicating whether hidden.
    /// </summary>
    bool GetIsHidden() const;
    void SetIsHidden(bool value);

private:
    Worksheet& _worksheet;
    int _index;
};

}}  // namespace Aspose::Cells_FOSS
