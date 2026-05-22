#pragma once
#include <optional>

namespace Aspose {
namespace Cells_FOSS {

class Worksheet;

/// <summary>
/// Represents row.
/// </summary>
class Row final {
public:
    /// <summary>
    /// Constructs a Row associated with the given worksheet and row index.
    /// </summary>
    /// <param name="worksheet">The parent worksheet.</param>
    /// <param name="index">The zero-based row index.</param>
    Row(Worksheet& worksheet, int index);

    /// <summary>
    /// Gets or sets the height.
    /// </summary>
    std::optional<double> GetHeight() const;
    void SetHeight(std::optional<double> value);

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
