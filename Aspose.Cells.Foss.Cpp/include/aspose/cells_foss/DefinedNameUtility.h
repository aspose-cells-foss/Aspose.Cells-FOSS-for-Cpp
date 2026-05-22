#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

/// Provides normalization and validation helpers for defined names.
class DefinedNameUtility {
public:
    static constexpr const char* PrintAreaDefinedName = "_xlnm.Print_Area";
    static constexpr const char* PrintTitlesDefinedName = "_xlnm.Print_Titles";
    static constexpr const char* FilterDatabaseDefinedName = "_xlnm._FilterDatabase";

    /// Returns true if the given name matches one of the reserved built-in
    /// defined names (case-insensitive).
    static bool IsReservedName(std::string_view name);

    /// Trims and validates a defined name. Throws CellsException if the name
    /// is empty or is a reserved built-in name.
    static std::string NormalizeName(std::string_view name);

    /// Trims a formula, strips a leading '=' if present, and trims again.
    /// Throws CellsException if the result is empty.
    static std::string NormalizeFormula(std::string_view formula);

    /// Trims a comment and returns the result.
    static std::string NormalizeComment(std::string_view comment);

    /// Returns true when both scopes are absent or both are present with
    /// equal values.
    static bool SameScope(std::optional<int> left, std::optional<int> right);
};

}}  // namespace Aspose::Cells_FOSS
