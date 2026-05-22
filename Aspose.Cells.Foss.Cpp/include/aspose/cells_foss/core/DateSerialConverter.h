#pragma once

#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/core/DateSystem.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Provides date serial converter operations.
/// </summary>
class DateSerialConverter {
public:
    DateSerialConverter() = delete;

    /// <summary>
    /// Converts a date serial number to a DateTime value.
    /// </summary>
    /// <param name="serial">The serial number.</param>
    /// <param name="dateSystem">The date system.</param>
    /// <returns>The date time.</returns>
    static DateTime FromSerial(double serial, DateSystem dateSystem);

    /// <summary>
    /// Converts a DateTime value to a date serial number.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <param name="dateSystem">The date system.</param>
    /// <returns>The double.</returns>
    static double ToSerial(const DateTime& value, DateSystem dateSystem);
};

}}}  // namespace Aspose::Cells_FOSS::Core
