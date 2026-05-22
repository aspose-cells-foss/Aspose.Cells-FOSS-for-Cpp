#pragma once
#include <optional>
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents defined name model.
/// </summary>
class DefinedNameModel final {
public:
    /// <summary>
    /// Initializes a new instance of the DefinedNameModel class.
    /// </summary>
    DefinedNameModel();

    /// <summary>
    /// Gets or sets the name.
    /// </summary>
    const std::string& GetName() const noexcept { return _name; }
    void SetName(std::string value) { _name = std::move(value); }

    /// <summary>
    /// Gets or sets the formula.
    /// </summary>
    const std::string& GetFormula() const noexcept { return _formula; }
    void SetFormula(std::string value) { _formula = std::move(value); }

    /// <summary>
    /// Gets or sets the local sheet index.
    /// </summary>
    std::optional<int> GetLocalSheetIndex() const noexcept { return _localSheetIndex; }
    void SetLocalSheetIndex(std::optional<int> value) noexcept { _localSheetIndex = value; }

    /// <summary>
    /// Gets or sets a value indicating whether hidden.
    /// </summary>
    bool GetHidden() const noexcept { return _hidden; }
    void SetHidden(bool value) noexcept { _hidden = value; }

    /// <summary>
    /// Gets or sets the comment.
    /// </summary>
    const std::string& GetComment() const noexcept { return _comment; }
    void SetComment(std::string value) { _comment = std::move(value); }

private:
    std::string _name;
    std::string _formula;
    std::optional<int> _localSheetIndex;
    bool _hidden = false;
    std::string _comment;
};

}}}  // namespace Aspose::Cells_FOSS::Core
