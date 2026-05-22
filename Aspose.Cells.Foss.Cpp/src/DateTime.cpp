#include "aspose/cells_foss/DateTime.h"

#include <stdexcept>

namespace Aspose {
namespace Cells_FOSS {
namespace {

constexpr std::int64_t TicksPerMillisecond = 10000LL;
constexpr std::int64_t TicksPerSecond = TicksPerMillisecond * 1000LL;
constexpr std::int64_t TicksPerMinute = TicksPerSecond * 60LL;
constexpr std::int64_t TicksPerHour = TicksPerMinute * 60LL;
constexpr std::int64_t TicksPerDay = TicksPerHour * 24LL;
constexpr std::int64_t DotNetEpochToUnixEpochDays = 719162LL;

bool IsLeapYear(int year) noexcept
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

int DaysInMonth(int year, int month) noexcept
{
    static constexpr int Days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return month == 2 && IsLeapYear(year) ? 29 : Days[month - 1];
}

int DaysFromCivil(int y, unsigned m, unsigned d) noexcept
{
    y -= static_cast<int>(m <= 2u);
    const int era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153u * (m > 2u ? m - 3u : m + 9u) + 2u) / 5u + d - 1u;
    const unsigned doe = yoe * 365u + yoe / 4u - yoe / 100u + doy;
    return era * 146097 + static_cast<int>(doe) - 719468;
}

void CivilFromDays(int days, int& year, unsigned& month, unsigned& day) noexcept
{
    days += 719468;
    const int era = (days >= 0 ? days : days - 146096) / 146097;
    const unsigned doe = static_cast<unsigned>(days - era * 146097);
    const unsigned yoe = (doe - doe / 1460u + doe / 36524u - doe / 146096u) / 365u;
    const int y = static_cast<int>(yoe) + era * 400;
    const unsigned doy = doe - (365u * yoe + yoe / 4u - yoe / 100u);
    const unsigned mp = (5u * doy + 2u) / 153u;
    day = doy - (153u * mp + 2u) / 5u + 1u;
    month = mp < 10u ? mp + 3u : mp - 9u;
    year = y + static_cast<int>(month <= 2u);
}

std::int64_t DateToTicks(int year, int month, int day, int hour, int minute, int second)
{
    if (year < 1 || year > 9999 || month < 1 || month > 12 ||
        day < 1 || day > DaysInMonth(year, month) ||
        hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59)
    {
        throw std::out_of_range("DateTime component out of range");
    }

    const int daysFromUnix = DaysFromCivil(year, static_cast<unsigned>(month), static_cast<unsigned>(day));
    const auto dateTicks = (static_cast<std::int64_t>(daysFromUnix) + DotNetEpochToUnixEpochDays) * TicksPerDay;
    return dateTicks + hour * TicksPerHour + minute * TicksPerMinute + second * TicksPerSecond;
}

int DayNumber(std::int64_t ticks) noexcept
{
    return static_cast<int>(ticks / TicksPerDay - DotNetEpochToUnixEpochDays);
}

}  // namespace

DateTime::DateTime(int year, int month, int day)
    : DateTime(year, month, day, 0, 0, 0)
{
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
    : _ticks(DateToTicks(year, month, day, hour, minute, second))
{
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
    : _ticks(DateToTicks(year, month, day, hour, minute, second) + millisecond * TicksPerMillisecond)
{
}

DateTime::DateTime(std::int64_t ticks) noexcept
    : _ticks(ticks)
{
}

int DateTime::GetYear() const noexcept
{
    int year = 0;
    unsigned month = 0;
    unsigned day = 0;
    CivilFromDays(DayNumber(_ticks), year, month, day);
    return year;
}

int DateTime::GetMonth() const noexcept
{
    int year = 0;
    unsigned month = 0;
    unsigned day = 0;
    CivilFromDays(DayNumber(_ticks), year, month, day);
    return static_cast<int>(month);
}

int DateTime::GetDay() const noexcept
{
    int year = 0;
    unsigned month = 0;
    unsigned day = 0;
    CivilFromDays(DayNumber(_ticks), year, month, day);
    return static_cast<int>(day);
}

int DateTime::GetHour() const noexcept
{
    return static_cast<int>((_ticks % TicksPerDay) / TicksPerHour);
}

int DateTime::GetMinute() const noexcept
{
    return static_cast<int>((_ticks % TicksPerHour) / TicksPerMinute);
}

int DateTime::GetSecond() const noexcept
{
    return static_cast<int>((_ticks % TicksPerMinute) / TicksPerSecond);
}

}}  // namespace Aspose::Cells_FOSS
