#include "aspose/cells_foss/core/SharedStringRepository.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

void SharedStringRepository::Clear()
{
    _indices.clear();
    _values.clear();
}

bool SharedStringRepository::TryGetValue(int index, std::string& value) const
{
    if (index >= 0 && index < static_cast<int>(_values.size()))
    {
        value = _values[static_cast<std::size_t>(index)];
        return true;
    }

    value.clear();
    return false;
}

int SharedStringRepository::Intern(std::string_view value)
{
    std::string key(value);
    auto it = _indices.find(key);
    if (it != _indices.end())
    {
        return it->second;
    }

    int index = static_cast<int>(_values.size());
    _values.push_back(key);
    _indices.emplace(std::move(key), index);
    return index;
}

}}}  // namespace Aspose::Cells_FOSS::Core
