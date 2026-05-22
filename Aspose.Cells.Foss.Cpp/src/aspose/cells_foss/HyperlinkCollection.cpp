#include "aspose/cells_foss/HyperlinkCollection.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/core/CellAddress.h"

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

namespace {

bool IsNullOrWhiteSpace(std::string_view s)
{
    if (s.empty()) return true;
    return std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return c == ' ' || c == '\t' || c == '\n'
            || c == '\r' || c == '\v' || c == '\f';
    });
}

std::string Trim(std::string_view s)
{
    const auto start = s.find_first_not_of(" \t\n\r\v\f");
    if (start == std::string_view::npos) return {};
    const auto end = s.find_last_not_of(" \t\n\r\v\f");
    return std::string(s.substr(start, end - start + 1));
}

bool StartsWith(std::string_view s, std::string_view prefix)
{
    if (s.size() < prefix.size()) return false;
    return s.substr(0, prefix.size()) == prefix;
}

}  // anonymous namespace

HyperlinkCollection::HyperlinkCollection(
    std::vector<Core::HyperlinkModel>& hyperlinks)
    : _hyperlinks(hyperlinks)
{
}

int HyperlinkCollection::GetCount() const noexcept
{
    return static_cast<int>(_hyperlinks.size());
}

Hyperlink HyperlinkCollection::operator[](int index)
{
    if (index < 0 || index >= static_cast<int>(_hyperlinks.size()))
    {
        throw CellsException("Hyperlink index was out of range.");
    }

    return Hyperlink(_hyperlinks, index);
}

int HyperlinkCollection::Add(int firstRow, int firstColumn,
                             int totalRows, int totalColumns,
                             std::string_view address)
{
    if (firstRow < 0 || firstColumn < 0)
    {
        throw CellsException("Hyperlink origin indices must be non-negative.");
    }

    if (address.empty())
    {
        throw CellsException("address must not be empty.");
    }

    return AddInternal({}, totalRows, totalColumns, address,
                       firstRow, firstColumn);
}

int HyperlinkCollection::Add(std::string_view cellName,
                             int totalRows, int totalColumns,
                             std::string_view address)
{
    if (address.empty())
    {
        throw CellsException("address must not be empty.");
    }

    return AddInternal(cellName, totalRows, totalColumns, address,
                       std::nullopt, std::nullopt);
}

int HyperlinkCollection::Add(std::string_view startCellName,
                             std::string_view endCellName,
                             std::string_view address,
                             std::string_view textToDisplay,
                             std::string_view screenTip)
{
    if (startCellName.empty())
    {
        throw CellsException("startCellName must not be empty.");
    }

    if (endCellName.empty())
    {
        throw CellsException("endCellName must not be empty.");
    }

    if (address.empty())
    {
        throw CellsException("address must not be empty.");
    }

    Core::CellAddress startAddress;
    Core::CellAddress endAddress;
    try
    {
        startAddress = Core::CellAddress::Parse(startCellName);
        endAddress = Core::CellAddress::Parse(endCellName);
    }
    catch (const std::invalid_argument& exception)
    {
        throw CellsException(exception.what(), exception);
    }

    const int firstRow = (std::min)(startAddress.GetRowIndex(),
                                    endAddress.GetRowIndex());
    const int firstColumn = (std::min)(startAddress.GetColumnIndex(),
                                       endAddress.GetColumnIndex());
    const int lastRow = (std::max)(startAddress.GetRowIndex(),
                                   endAddress.GetRowIndex());
    const int lastColumn = (std::max)(startAddress.GetColumnIndex(),
                                      endAddress.GetColumnIndex());
    const int index = AddInternal({}, lastRow - firstRow + 1,
                                  lastColumn - firstColumn + 1,
                                  address, firstRow, firstColumn);
    auto& hyperlink = _hyperlinks[index];
    hyperlink.SetTextToDisplay(NormalizeText(textToDisplay));
    hyperlink.SetScreenTip(NormalizeText(screenTip));
    return index;
}

void HyperlinkCollection::RemoveAt(int index)
{
    if (index < 0 || index >= static_cast<int>(_hyperlinks.size()))
    {
        throw CellsException("Hyperlink index was out of range.");
    }

    _hyperlinks.erase(_hyperlinks.begin() + index);
}

int HyperlinkCollection::AddInternal(std::string_view cellName,
                                     int totalRows, int totalColumns,
                                     std::string_view address,
                                     std::optional<int> firstRowOverride,
                                     std::optional<int> firstColumnOverride)
{
    if (totalRows <= 0 || totalColumns <= 0)
    {
        throw CellsException("Hyperlink range dimensions must be positive.");
    }

    Core::CellAddress anchor;
    if (firstRowOverride.has_value() && firstColumnOverride.has_value())
    {
        anchor = Core::CellAddress(*firstRowOverride, *firstColumnOverride);
    }
    else
    {
        if (IsNullOrWhiteSpace(cellName))
        {
            throw CellsException(
                "Hyperlink anchor must be a valid cell reference.");
        }

        try
        {
            anchor = Core::CellAddress::Parse(cellName);
        }
        catch (const std::invalid_argument& exception)
        {
            throw CellsException(exception.what(), exception);
        }
    }

    Core::HyperlinkModel candidate;
    candidate.SetFirstRow(anchor.GetRowIndex());
    candidate.SetFirstColumn(anchor.GetColumnIndex());
    candidate.SetTotalRows(totalRows);
    candidate.SetTotalColumns(totalColumns);
    AssignAddress(candidate, address);

    for (std::size_t index = 0; index < _hyperlinks.size(); ++index)
    {
        if (Overlaps(_hyperlinks[index], candidate))
        {
            throw CellsException("Hyperlink ranges must not overlap.");
        }
    }

    _hyperlinks.push_back(std::move(candidate));
    return static_cast<int>(_hyperlinks.size()) - 1;
}

void HyperlinkCollection::AssignAddress(Core::HyperlinkModel& model,
                                        std::string_view address)
{
    if (IsNullOrWhiteSpace(address))
    {
        throw CellsException("Hyperlink address must be non-empty.");
    }

    const auto normalized = Trim(address);
    if (StartsWith(normalized, "#"))
    {
        model.SetAddress({});
        model.SetSubAddress(std::string(normalized.substr(1)));
        return;
    }

    if (normalized.find('!') != std::string_view::npos)
    {
        model.SetAddress({});
        model.SetSubAddress(std::string(normalized));
        return;
    }

    model.SetAddress(std::string(normalized));
    model.SetSubAddress({});
}

bool HyperlinkCollection::Overlaps(const Core::HyperlinkModel& left,
                                   const Core::HyperlinkModel& right)
{
    const int leftLastRow = left.GetFirstRow() + left.GetTotalRows() - 1;
    const int leftLastColumn = left.GetFirstColumn() + left.GetTotalColumns() - 1;
    const int rightLastRow = right.GetFirstRow() + right.GetTotalRows() - 1;
    const int rightLastColumn = right.GetFirstColumn() + right.GetTotalColumns() - 1;

    return left.GetFirstRow() <= rightLastRow
        && right.GetFirstRow() <= leftLastRow
        && left.GetFirstColumn() <= rightLastColumn
        && right.GetFirstColumn() <= leftLastColumn;
}

std::string HyperlinkCollection::NormalizeText(std::string_view value)
{
    if (value.empty())
    {
        return {};
    }

    return std::string(value);
}

}}  // namespace Aspose::Cells_FOSS
