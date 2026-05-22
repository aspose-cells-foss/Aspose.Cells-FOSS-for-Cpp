#include "aspose/cells_foss/Cell.h"

#include "aspose/cells_foss/DisplayTextFormatter.h"
#include "aspose/cells_foss/Style.h"
#include "aspose/cells_foss/Worksheet.h"
#include "aspose/cells_foss/Workbook.h"
#include "aspose/cells_foss/core/CellRecord.h"
#include "aspose/cells_foss/core/WorkbookModel.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <algorithm>
#include <cctype>

namespace Aspose {
namespace Cells_FOSS {

Cell::Cell(Worksheet& worksheet, const Core::CellAddress& address)
    : _worksheet(worksheet)
    , _address(address)
{
}

int Cell::GetRow() const noexcept
{
    return _address.GetRowIndex();
}

int Cell::GetColumn() const noexcept
{
    return _address.GetColumnIndex();
}

std::optional<std::reference_wrapper<Core::CellRecord>> Cell::TryGetRecord()
{
    auto& cells = _worksheet.GetModel().GetCells();
    auto it = cells.find(_address);
    if (it != cells.end())
    {
        return std::ref(it->second);
    }
    return std::nullopt;
}

std::optional<std::reference_wrapper<const Core::CellRecord>> Cell::TryGetRecord() const
{
    const auto& cells = _worksheet.GetModel().GetCells();
    auto it = cells.find(_address);
    if (it != cells.end())
    {
        return std::cref(it->second);
    }
    return std::nullopt;
}

Core::CellRecord& Cell::GetOrCreateRecord()
{
    auto& cells = _worksheet.GetModel().GetCells();
    auto it = cells.find(_address);
    if (it != cells.end())
    {
        return it->second;
    }

    Core::CellRecord record;
    record.SetStyle(_worksheet.GetWorkbook().GetModel().GetDefaultStyle().Clone());
    auto [ins, inserted] = cells.emplace(_address, std::move(record));
    (void)inserted;
    return ins->second;
}

CellValue Cell::GetValue() const
{
    auto rec = TryGetRecord();
    if (!rec.has_value())
    {
        return CellValue{};
    }
    return rec.value().get().GetValue();
}

void Cell::SetValue(const CellValue& value)
{
    if (value.IsEmpty())
    {
        ClearValue();
        return;
    }

    if (value.IsString())
    {
        PutValue(value.AsString());
        return;
    }
    if (value.IsBool())
    {
        PutValue(value.AsBool());
        return;
    }
    if (value.IsDateTime())
    {
        PutValue(value.AsDateTime());
        return;
    }
    if (value.IsInteger())
    {
        PutValue(value.AsInteger());
        return;
    }
    if (value.IsDouble())
    {
        PutValue(value.AsDouble());
        return;
    }

    // All CellValue variant states are covered above; reach here only if the
    // variant is in an unexpected state, which should not happen.
    ClearValue();
}

std::string Cell::GetStringValue() const
{
    auto rec = TryGetRecord();
    const CellValue empty;
    const CellValue& value = rec.has_value() ? rec.value().get().GetValue() : empty;
    return DisplayTextFormatter::FormatStringValue(value);
}

std::string Cell::GetDisplayStringValue() const
{
    auto rec = TryGetRecord();
    const Core::StyleValue& style = rec.has_value()
        ? rec.value().get().GetStyle()
        : _worksheet.GetWorkbook().GetModel().GetDefaultStyle();
    const CellValue empty;
    const CellValue& value = rec.has_value() ? rec.value().get().GetValue() : empty;
    return DisplayTextFormatter::FormatDisplayValue(
        value,
        style,
        _worksheet.GetWorkbook().GetModel().GetSettings().GetDisplayCulture());
}

std::string Cell::GetFormula() const
{
    auto rec = TryGetRecord();
    if (!rec.has_value() || rec.value().get().GetFormula().empty())
    {
        return std::string{};
    }

    return "=" + rec.value().get().GetFormula();
}

void Cell::SetFormula(std::string_view value)
{
    auto& record = GetOrCreateRecord();
    std::string normalized = NormalizeFormula(value);
    record.SetFormula(std::move(normalized));
    if (record.GetFormula().empty())
    {
        if (record.GetValue().IsEmpty())
        {
            record.SetKind(Core::CellValueKind::Blank);
        }

        return;
    }

    record.SetKind(Core::CellValueKind::Formula);
}

CellValueType Cell::GetType() const
{
    auto rec = TryGetRecord();
    if (!rec.has_value())
    {
        return CellValueType::Blank;
    }

    const auto& record = rec.value().get();

    if (!record.GetFormula().empty())
    {
        return CellValueType::Formula;
    }

    switch (record.GetKind())
    {
        case Core::CellValueKind::String:
            return CellValueType::String;
        case Core::CellValueKind::Number:
            return CellValueType::Number;
        case Core::CellValueKind::Boolean:
            return CellValueType::Boolean;
        case Core::CellValueKind::DateTime:
            return CellValueType::DateTime;
        case Core::CellValueKind::Formula:
            return CellValueType::Formula;
        default:
            return CellValueType::Blank;
    }
}

void Cell::PutValue(const char* value)
{
    SetScalar(CellValue(std::string(value == nullptr ? "" : value)), Core::CellValueKind::String);
}

void Cell::PutValue(std::string_view value)
{
    SetScalar(CellValue(std::string(value)), Core::CellValueKind::String);
}

void Cell::PutValue(int value)
{
    SetScalar(CellValue(value), Core::CellValueKind::Number);
}

void Cell::PutValue(double value)
{
    SetScalar(CellValue(value), Core::CellValueKind::Number);
}

void Cell::PutValue(bool value)
{
    SetScalar(CellValue(value), Core::CellValueKind::Boolean);
}

void Cell::PutValue(const DateTime& value)
{
    SetScalar(CellValue(value), Core::CellValueKind::DateTime);
}

Style Cell::GetStyle() const
{
    auto rec = TryGetRecord();
    const Core::StyleValue& coreStyle = rec.has_value()
        ? rec.value().get().GetStyle()
        : _worksheet.GetWorkbook().GetModel().GetDefaultStyle();
    return Style::FromCore(coreStyle.Clone());
}

void Cell::SetStyle(const Style& style)
{
    GetOrCreateRecord().SetStyle(style.ToCore());
}

void Cell::SetScalar(const CellValue& value, Core::CellValueKind kind)
{
    auto& record = GetOrCreateRecord();
    record.SetValue(value);
    record.SetKind(kind);
    record.SetFormula(std::string{});
}

void Cell::ClearValue()
{
    auto& record = GetOrCreateRecord();
    record.SetValue(CellValue{});
    record.SetFormula(std::string{});
    record.SetKind(Core::CellValueKind::Blank);
}

std::string Cell::NormalizeFormula(std::string_view value)
{
    if (std::all_of(value.begin(), value.end(),
                    [](unsigned char ch) { return std::isspace(ch) != 0; }))
    {
        return std::string{};
    }

    if (!value.empty() && value[0] == '=')
    {
        return std::string(value.substr(1));
    }

    return std::string(value);
}

}}  // namespace Aspose::Cells_FOSS
