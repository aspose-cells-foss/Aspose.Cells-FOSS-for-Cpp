#include "aspose/cells_foss/DefinedNameUtility.h"
#include "aspose/cells_foss/CellsException.h"

#include <cctype>
#include <string>

namespace Aspose {
namespace Cells_FOSS {

namespace {

bool IEquals(std::string_view a, std::string_view b)
{
    if (a.size() != b.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < a.size(); ++i)
    {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i])))
        {
            return false;
        }
    }

    return true;
}

std::string Trim(std::string_view sv)
{
    const auto start = sv.find_first_not_of(" \t\n\r");
    if (start == std::string_view::npos)
    {
        return {};
    }

    const auto end = sv.find_last_not_of(" \t\n\r");
    return std::string(sv.substr(start, end - start + 1));
}

}  // namespace

bool DefinedNameUtility::IsReservedName(std::string_view name)
{
    return IEquals(name, PrintAreaDefinedName)
        || IEquals(name, PrintTitlesDefinedName)
        || IEquals(name, FilterDatabaseDefinedName);
}

std::string DefinedNameUtility::NormalizeName(std::string_view name)
{
    std::string normalized = Trim(name);

    if (normalized.empty())
    {
        throw CellsException("Defined name must be non-empty.");
    }

    if (IsReservedName(normalized))
    {
        throw CellsException("Built-in print defined names must be managed through PageSetup.");
    }

    return normalized;
}

std::string DefinedNameUtility::NormalizeFormula(std::string_view formula)
{
    std::string normalized = Trim(formula);

    if (!normalized.empty() && normalized[0] == '=')
    {
        normalized = Trim(std::string_view(normalized).substr(1));
    }

    if (normalized.empty())
    {
        throw CellsException("Defined name formula must be non-empty.");
    }

    return normalized;
}

std::string DefinedNameUtility::NormalizeComment(std::string_view comment)
{
    return Trim(comment);
}

bool DefinedNameUtility::SameScope(std::optional<int> left, std::optional<int> right)
{
    if (!left.has_value() && !right.has_value())
    {
        return true;
    }

    return left.has_value() && right.has_value() && (*left == *right);
}

}}  // namespace Aspose::Cells_FOSS
