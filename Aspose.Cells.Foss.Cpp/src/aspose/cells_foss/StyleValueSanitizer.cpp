#include "aspose/cells_foss/StyleValueSanitizer.h"

namespace Aspose {
namespace Cells_FOSS {

int StyleValueSanitizer::NormalizeIndentLevel(std::optional<int> value)
{
    if (!value.has_value() || *value < 0 || *value > 250)
    {
        return 0;
    }

    return *value;
}

int StyleValueSanitizer::NormalizeTextRotation(std::optional<int> value)
{
    if (!value.has_value())
    {
        return 0;
    }

    if (*value == 255)
    {
        return 255;
    }

    if (*value < 0 || *value > 180)
    {
        return 0;
    }

    return *value;
}

int StyleValueSanitizer::NormalizeReadingOrder(std::optional<int> value)
{
    if (!value.has_value() || *value < 0 || *value > 2)
    {
        return 0;
    }

    return *value;
}

}}  // namespace Aspose::Cells_FOSS
