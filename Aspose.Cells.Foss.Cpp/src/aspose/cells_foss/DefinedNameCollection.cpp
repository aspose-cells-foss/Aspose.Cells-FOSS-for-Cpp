#include "aspose/cells_foss/DefinedNameCollection.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/DefinedNameUtility.h"
#include "aspose/cells_foss/Workbook.h"

#include <cstddef>
#include <string>

namespace Aspose {
namespace Cells_FOSS {

DefinedNameCollection::DefinedNameCollection(Workbook& workbook)
    : _workbook(workbook)
    , _definedNames(workbook.GetDefinedNamesModel())
{
}

int DefinedNameCollection::GetCount() const noexcept
{
    return static_cast<int>(_definedNames.size());
}

DefinedName DefinedNameCollection::operator[](int index) const
{
    if (index < 0 || index >= static_cast<int>(_definedNames.size()))
    {
        throw CellsException("Defined name index was out of range.");
    }

    return DefinedName(_workbook, _definedNames, index);
}

int DefinedNameCollection::Add(std::string_view name, std::string_view formula)
{
    return Add(name, formula, std::nullopt);
}

int DefinedNameCollection::Add(std::string_view name, std::string_view formula,
                               std::optional<int> localSheetIndex)
{
    std::string normalizedName = DefinedNameUtility::NormalizeName(name);
    std::string normalizedFormula = DefinedNameUtility::NormalizeFormula(formula);
    _workbook.EnsureValidDefinedNameScope(localSheetIndex);

    Core::DefinedNameModel sentinel;
    _workbook.EnsureUniqueDefinedName(sentinel, normalizedName, localSheetIndex);

    Core::DefinedNameModel model;
    model.SetName(std::move(normalizedName));
    model.SetFormula(std::move(normalizedFormula));
    model.SetLocalSheetIndex(localSheetIndex);

    _definedNames.push_back(std::move(model));
    return static_cast<int>(_definedNames.size()) - 1;
}

void DefinedNameCollection::RemoveAt(int index)
{
    if (index < 0 || index >= static_cast<int>(_definedNames.size()))
    {
        throw CellsException("Defined name index was out of range.");
    }

    _definedNames.erase(_definedNames.begin() + index);
}

std::vector<DefinedName> DefinedNameCollection::GetEnumerator() const
{
    std::vector<DefinedName> names;
    names.reserve(_definedNames.size());
    for (std::size_t index = 0; index < _definedNames.size(); ++index)
    {
        names.emplace_back(_workbook, _definedNames, static_cast<int>(index));
    }

    return names;
}

}}  // namespace Aspose::Cells_FOSS
