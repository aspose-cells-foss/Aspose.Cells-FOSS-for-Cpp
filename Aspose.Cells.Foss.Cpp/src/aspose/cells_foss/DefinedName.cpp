#include "aspose/cells_foss/DefinedName.h"
#include "aspose/cells_foss/DefinedNameUtility.h"
#include "aspose/cells_foss/Workbook.h"

namespace Aspose {
namespace Cells_FOSS {

DefinedName::DefinedName(Workbook& workbook,
                         std::vector<Core::DefinedNameModel>& models,
                         int index)
    : _workbook(workbook)
    , _models(models)
    , _index(index)
{
}

std::string DefinedName::GetName() const
{
    return GetModel().GetName();
}

void DefinedName::SetName(const std::string& value)
{
    std::string normalized = DefinedNameUtility::NormalizeName(value);
    _workbook.EnsureUniqueDefinedName(GetModel(), normalized, GetModel().GetLocalSheetIndex());
    GetModel().SetName(std::move(normalized));
}

std::string DefinedName::GetFormula() const
{
    return GetModel().GetFormula();
}

void DefinedName::SetFormula(const std::string& value)
{
    GetModel().SetFormula(DefinedNameUtility::NormalizeFormula(value));
}

std::optional<int> DefinedName::GetLocalSheetIndex() const noexcept
{
    return GetModel().GetLocalSheetIndex();
}

void DefinedName::SetLocalSheetIndex(std::optional<int> value)
{
    _workbook.EnsureValidDefinedNameScope(value);
    _workbook.EnsureUniqueDefinedName(GetModel(), GetModel().GetName(), value);
    GetModel().SetLocalSheetIndex(value);
}

bool DefinedName::GetHidden() const noexcept
{
    return GetModel().GetHidden();
}

void DefinedName::SetHidden(bool value) noexcept
{
    GetModel().SetHidden(value);
}

std::string DefinedName::GetComment() const
{
    return GetModel().GetComment();
}

void DefinedName::SetComment(const std::string& value)
{
    GetModel().SetComment(DefinedNameUtility::NormalizeComment(value));
}

}}  // namespace Aspose::Cells_FOSS
