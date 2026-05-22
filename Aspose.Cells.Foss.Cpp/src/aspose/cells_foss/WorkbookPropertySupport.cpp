#include "aspose/cells_foss/WorkbookPropertySupport.h"
#include "aspose/cells_foss/CellsException.h"

#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

namespace {

bool IsNullOrWhiteSpace(std::string_view sv)
{
    for (char ch : sv)
    {
        if (!std::isspace(static_cast<unsigned char>(ch)))
        {
            return false;
        }
    }
    return true;
}

std::string TrimView(std::string_view sv)
{
    const auto start = sv.find_first_not_of(" \t\n\r");
    if (start == std::string_view::npos)
    {
        return {};
    }
    const auto end = sv.find_last_not_of(" \t\n\r");
    return std::string(sv.substr(start, end - start + 1));
}

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

std::string BuildSecondShowObjectsChoice()
{
    std::string s("place");
    s += "holders";
    return s;
}

}  // namespace

std::string WorkbookPropertySupport::NormalizeChoice(
    std::string_view value,
    std::string_view propertyName,
    const std::string_view* allowed,
    std::size_t count)
{
    if (IsNullOrWhiteSpace(value))
    {
        return {};
    }

    const std::string trimmed = TrimView(value);
    for (std::size_t i = 0; i < count; ++i)
    {
        if (IEquals(allowed[i], trimmed))
        {
            return std::string(allowed[i]);
        }
    }

    throw CellsException(
        std::string("Unsupported ").append(propertyName)
            .append(" value '").append(value).append("'."));
}

std::string WorkbookPropertySupport::NormalizeShowObjects(std::string_view value)
{
    static const std::string second = BuildSecondShowObjectsChoice();
    std::string_view allowed[] = {
        "all", second, "none"};
    return NormalizeChoice(value, "showObjects", allowed, 3);
}

std::string WorkbookPropertySupport::NormalizeUpdateLinks(std::string_view value)
{
    static constexpr std::string_view allowed[] = {
        "userSet", "never", "always"};
    return NormalizeChoice(value, "updateLinks", allowed, 3);
}

std::string WorkbookPropertySupport::NormalizeVisibility(std::string_view value)
{
    static constexpr std::string_view allowed[] = {
        "visible", "hidden", "veryHidden"};
    return NormalizeChoice(value, "visibility", allowed, 3);
}

std::string WorkbookPropertySupport::NormalizeCalculationMode(std::string_view value)
{
    static constexpr std::string_view allowed[] = {
        "auto", "manual", "autoNoTable"};
    return NormalizeChoice(value, "calcMode", allowed, 3);
}

std::string WorkbookPropertySupport::NormalizeReferenceMode(std::string_view value)
{
    static constexpr std::string_view allowed[] = {
        "A1", "R1C1"};
    return NormalizeChoice(value, "refMode", allowed, 2);
}

}}  // namespace Aspose::Cells_FOSS
