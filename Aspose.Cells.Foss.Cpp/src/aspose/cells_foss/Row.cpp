#include "aspose/cells_foss/Row.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/Worksheet.h"
#include "aspose/cells_foss/core/RowModel.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <functional>
#include <optional>
#include <unordered_map>

namespace Aspose {
namespace Cells_FOSS {

namespace {

std::optional<std::reference_wrapper<const Core::RowModel>>
TryGetModel(const Core::WorksheetModel& model, int index)
{
    const auto& rows = model.GetRows();
    auto it = rows.find(index);
    if (it != rows.end()) {
        return std::cref(it->second);
    }
    return std::nullopt;
}

Core::RowModel& GetOrCreateModel(Core::WorksheetModel& model, int index)
{
    auto& rows = model.GetRows();
    auto it = rows.find(index);
    if (it != rows.end()) {
        return it->second;
    }
    auto [inserted, ok] = rows.emplace(index, Core::RowModel{});
    (void)ok;
    return inserted->second;
}

void Normalize(Core::WorksheetModel& model, int index)
{
    auto& rows = model.GetRows();
    auto it = rows.find(index);
    if (it != rows.end()
        && !it->second.GetHeight().has_value()
        && !it->second.GetHidden()
        && !it->second.GetStyleIndex().has_value())
    {
        rows.erase(it);
    }
}

}  // namespace

Row::Row(Worksheet& worksheet, int index)
    : _worksheet(worksheet)
    , _index(index)
{
}

std::optional<double> Row::GetHeight() const
{
    const auto& model = _worksheet.GetModel();
    auto row = TryGetModel(model, _index);
    if (row.has_value()) {
        return row->get().GetHeight();
    }
    return std::nullopt;
}

void Row::SetHeight(std::optional<double> value)
{
    if (value.has_value() && value.value() <= 0.0) {
        throw CellsException("Row height must be positive.");
    }

    auto& model = _worksheet.GetModel();
    auto& row = GetOrCreateModel(model, _index);
    row.SetHeight(value);
    Normalize(model, _index);
}

bool Row::GetIsHidden() const
{
    const auto& model = _worksheet.GetModel();
    auto row = TryGetModel(model, _index);
    if (row.has_value()) {
        return row->get().GetHidden();
    }
    return false;
}

void Row::SetIsHidden(bool value)
{
    auto& model = _worksheet.GetModel();
    auto& row = GetOrCreateModel(model, _index);
    row.SetHidden(value);
    Normalize(model, _index);
}

}}  // namespace Aspose::Cells_FOSS
