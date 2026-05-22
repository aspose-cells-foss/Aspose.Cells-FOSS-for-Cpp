#include "aspose/cells_foss/core/DateSerialConverter.h"

#include <cmath>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

namespace {

constexpr std::int64_t TicksPerDay = 864000000000LL;

const DateTime Windows1900Epoch(1899, 12, 31);
const DateTime Mac1904Epoch(1904, 1, 1);

}  // namespace

double DateSerialConverter::ToSerial(const DateTime& value, DateSystem dateSystem)
{
    const DateTime& baseDate = (dateSystem == DateSystem::Mac1904) ? Mac1904Epoch : Windows1900Epoch;
    double serial = static_cast<double>(value.GetTicks() - baseDate.GetTicks()) / static_cast<double>(TicksPerDay);

    // Excel's 1900 date system preserves the historic leap-year bug, so serials
    // on or after the phantom 1900-02-29 are shifted by one day.
    if (dateSystem == DateSystem::Windows1900 && serial >= 60.0)
    {
        serial += 1.0;
    }

    return serial;
}

DateTime DateSerialConverter::FromSerial(double serial, DateSystem dateSystem)
{
    // Reverse the 1900-system compatibility adjustment before converting back
    // to a DateTime.
    if (dateSystem == DateSystem::Windows1900 && serial >= 60.0)
    {
        serial -= 1.0;
    }

    const DateTime& baseDate = (dateSystem == DateSystem::Mac1904) ? Mac1904Epoch : Windows1900Epoch;
    auto ticks = static_cast<std::int64_t>(std::llround(serial * static_cast<double>(TicksPerDay)));
    return DateTime(baseDate.GetTicks() + ticks);
}

}}}  // namespace Aspose::Cells_FOSS::Core
