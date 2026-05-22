#include "aspose/cells_foss/Color.h"

namespace Aspose {
namespace Cells_FOSS {

int Color::GetHashCode() const noexcept
{
    int hash = _a;
    hash = (hash * 397) ^ _r;
    hash = (hash * 397) ^ _g;
    hash = (hash * 397) ^ _b;
    return hash;
}

Core::ColorValue Color::ToCore() const noexcept
{
    return Core::ColorValue(_a, _r, _g, _b);
}

Color Color::FromCore(const Core::ColorValue& value) noexcept
{
    return Color(value.GetA(), value.GetR(), value.GetG(), value.GetB());
}

}}  // namespace Aspose::Cells_FOSS
