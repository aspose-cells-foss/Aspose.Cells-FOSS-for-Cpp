#include "aspose/cells_foss/core/ColorValue.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

ColorValue::ColorValue(std::uint8_t a, std::uint8_t r, std::uint8_t g, std::uint8_t b)
    : _a(a), _r(r), _g(g), _b(b)
{
}

bool ColorValue::Equals(const ColorValue& other) const noexcept
{
    return _a == other._a && _r == other._r && _g == other._g && _b == other._b;
}

int ColorValue::GetHashCode() const noexcept
{
    int hash = _a;
    hash = (hash * 397) ^ _r;
    hash = (hash * 397) ^ _g;
    hash = (hash * 397) ^ _b;
    return hash;
}

}}}  // namespace Aspose::Cells_FOSS::Core
