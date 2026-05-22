#include "aspose/cells_foss/Hyperlink.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/core/CellAddress.h"

#include <algorithm>
#include <cctype>

namespace Aspose {
namespace Cells_FOSS {

namespace {

bool IsNullOrWhiteSpace(const std::string& s)
{
    if (s.empty()) return true;
    return std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
    });
}

std::string Trim(const std::string& s)
{
    const auto start = s.find_first_not_of(" \t\n\r\v\f");
    if (start == std::string::npos) return {};
    const auto end = s.find_last_not_of(" \t\n\r\v\f");
    return s.substr(start, end - start + 1);
}

bool StartsWith(const std::string& s, const std::string& prefix, bool ignoreCase)
{
    if (s.size() < prefix.size()) return false;
    for (size_t i = 0; i < prefix.size(); ++i) {
        if (ignoreCase) {
            if (std::tolower(static_cast<unsigned char>(s[i])) !=
                std::tolower(static_cast<unsigned char>(prefix[i])))
                return false;
        } else {
            if (s[i] != prefix[i]) return false;
        }
    }
    return true;
}

std::string NormalizeText(const std::string& value)
{
    return value.empty() ? std::string{} : value;
}

}  // anonymous namespace

Hyperlink::Hyperlink(std::vector<Core::HyperlinkModel>& owner, int index)
    : _owner(owner)
    , _index(index)
{
}

Core::HyperlinkModel& Hyperlink::Model()
{
    return _owner[static_cast<std::size_t>(_index)];
}

const Core::HyperlinkModel& Hyperlink::Model() const
{
    return _owner[static_cast<std::size_t>(_index)];
}

std::string Hyperlink::GetArea() const
{
    const auto& m = Model();
    const auto first = Core::CellAddress(m.GetFirstRow(), m.GetFirstColumn()).ToString();
    if (m.GetTotalRows() == 1 && m.GetTotalColumns() == 1)
    {
        return first;
    }

    const auto last = Core::CellAddress(
        m.GetFirstRow() + m.GetTotalRows() - 1,
        m.GetFirstColumn() + m.GetTotalColumns() - 1).ToString();
    return first + ":" + last;
}

std::string Hyperlink::GetAddress() const
{
    const auto& m = Model();
    const auto& address = m.GetAddress();
    if (!address.empty())
    {
        return address;
    }

    return m.GetSubAddress();
}

void Hyperlink::SetAddress(const std::string& value)
{
    auto& m = Model();
    if (IsNullOrWhiteSpace(value))
    {
        m.SetAddress({});
        m.SetSubAddress({});
        return;
    }

    const auto normalized = Trim(value);
    if (StartsWith(normalized, "#", false))
    {
        m.SetAddress({});
        m.SetSubAddress(normalized.substr(1));
        return;
    }

    if (normalized.find('!') != std::string::npos)
    {
        m.SetAddress({});
        m.SetSubAddress(normalized);
        return;
    }

    m.SetAddress(normalized);
    m.SetSubAddress({});
}

TargetModeType Hyperlink::GetLinkType() const
{
    const auto& m = Model();
    if (!m.GetSubAddress().empty())
    {
        return TargetModeType::CellReference;
    }

    const auto& address = m.GetAddress();
    if (address.empty())
    {
        return TargetModeType::External;
    }

    if (StartsWith(address, "mailto:", true))
    {
        return TargetModeType::Email;
    }

    if (StartsWith(address, "\\", false) ||
        (address.find(":\\") != std::string::npos && address.find(":\\") > 0))
    {
        return TargetModeType::FilePath;
    }

    return TargetModeType::External;
}

std::string Hyperlink::GetScreenTip() const
{
    return Model().GetScreenTip();
}

void Hyperlink::SetScreenTip(const std::string& value)
{
    Model().SetScreenTip(NormalizeText(value));
}

std::string Hyperlink::GetTextToDisplay() const
{
    return Model().GetTextToDisplay();
}

void Hyperlink::SetTextToDisplay(const std::string& value)
{
    Model().SetTextToDisplay(NormalizeText(value));
}

void Hyperlink::Delete()
{
    if (_index >= 0 && _index < static_cast<int>(_owner.size()))
    {
        _owner.erase(_owner.begin() + _index);
    }
}

}}  // namespace Aspose::Cells_FOSS
