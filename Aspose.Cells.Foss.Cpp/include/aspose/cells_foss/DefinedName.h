#pragma once

#include <optional>
#include <string>
#include <vector>

#include "aspose/cells_foss/core/DefinedNameModel.h"

namespace Aspose {
namespace Cells_FOSS {

class Workbook;

/// <summary>
/// Represents defined name.
/// </summary>
class DefinedName final {
public:
    /// <summary>
    /// Initializes a new instance of the DefinedName class.
    /// </summary>
    /// <param name="workbook">The owner workbook.</param>
    /// <param name="models">The underlying defined name models vector.</param>
    /// <param name="index">The index within the models vector.</param>
    DefinedName(Workbook& workbook,
                std::vector<Core::DefinedNameModel>& models,
                int index);

    /// <summary>
    /// Gets or sets the name.
    /// </summary>
    std::string GetName() const;
    void SetName(const std::string& value);

    /// <summary>
    /// Gets or sets the formula.
    /// </summary>
    std::string GetFormula() const;
    void SetFormula(const std::string& value);

    /// <summary>
    /// Gets or sets the local sheet index.
    /// </summary>
    std::optional<int> GetLocalSheetIndex() const noexcept;
    void SetLocalSheetIndex(std::optional<int> value);

    /// <summary>
    /// Gets or sets a value indicating whether hidden.
    /// </summary>
    bool GetHidden() const noexcept;
    void SetHidden(bool value) noexcept;

    /// <summary>
    /// Gets or sets the comment.
    /// </summary>
    std::string GetComment() const;
    void SetComment(const std::string& value);

private:
    Workbook& _workbook;
    std::vector<Core::DefinedNameModel>& _models;
    int _index;

    Core::DefinedNameModel& GetModel() const { return _models[_index]; }
};

}}  // namespace Aspose::Cells_FOSS
