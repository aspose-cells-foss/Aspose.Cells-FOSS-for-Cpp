#include "aspose/cells_foss/Column.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/Worksheet.h"
#include "aspose/cells_foss/core/ColumnRangeModel.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

namespace {

constexpr int NotFound = -1;

int FindModel(Core::WorksheetModel& model, int index)
{
    auto& columns = model.GetColumns();
    for (int i = static_cast<int>(columns.size()) - 1; i >= 0; --i) {
        auto& entry = columns[static_cast<std::size_t>(i)];
        if (entry.GetMinColumnIndex() <= index && entry.GetMaxColumnIndex() >= index) {
            return i;
        }
    }
    return NotFound;
}

int FindModel(const Core::WorksheetModel& model, int index)
{
    const auto& columns = model.GetColumns();
    for (int i = static_cast<int>(columns.size()) - 1; i >= 0; --i) {
        const auto& entry = columns[static_cast<std::size_t>(i)];
        if (entry.GetMinColumnIndex() <= index && entry.GetMaxColumnIndex() >= index) {
            return i;
        }
    }
    return NotFound;
}

int GetOrCreateModel(Core::WorksheetModel& model, int index)
{
    int existing = FindModel(model, index);
    if (existing != NotFound) {
        auto& entry = model.GetColumns()[static_cast<std::size_t>(existing)];
        if (entry.GetMinColumnIndex() == index && entry.GetMaxColumnIndex() == index) {
            return existing;
        }
    }

    auto& columns = model.GetColumns();
    columns.emplace_back();
    auto& created = columns.back();
    created.SetMinColumnIndex(index);
    created.SetMaxColumnIndex(index);
    return static_cast<int>(columns.size()) - 1;
}

void Normalize(Core::WorksheetModel& model, int entryIndex)
{
    auto& entry = model.GetColumns()[static_cast<std::size_t>(entryIndex)];
    if (!entry.GetWidth().has_value() && !entry.GetHidden() && !entry.GetStyleIndex().has_value()) {
        model.GetColumns().erase(
            model.GetColumns().begin() + entryIndex);
    }
}

}  // namespace

Column::Column(Worksheet& worksheet, int index)
    : _worksheet(worksheet)
    , _index(index)
{
}

std::optional<double> Column::GetWidth() const
{
    const auto& model = _worksheet.GetModel();
    int idx = FindModel(model, _index);
    if (idx != NotFound) {
        return model.GetColumns()[static_cast<std::size_t>(idx)].GetWidth();
    }
    return std::nullopt;
}

void Column::SetWidth(std::optional<double> value)
{
    if (value.has_value() && value.value() <= 0.0) {
        throw CellsException("Column width must be positive.");
    }

    auto& model = _worksheet.GetModel();
    int idx = GetOrCreateModel(model, _index);
    model.GetColumns()[static_cast<std::size_t>(idx)].SetWidth(value);
    Normalize(model, idx);
}

bool Column::GetIsHidden() const
{
    const auto& model = _worksheet.GetModel();
    int idx = FindModel(model, _index);
    if (idx != NotFound) {
        return model.GetColumns()[static_cast<std::size_t>(idx)].GetHidden();
    }
    return false;
}

void Column::SetIsHidden(bool value)
{
    auto& model = _worksheet.GetModel();
    int idx = GetOrCreateModel(model, _index);
    model.GetColumns()[static_cast<std::size_t>(idx)].SetHidden(value);
    Normalize(model, idx);
}

}}  // namespace Aspose::Cells_FOSS
