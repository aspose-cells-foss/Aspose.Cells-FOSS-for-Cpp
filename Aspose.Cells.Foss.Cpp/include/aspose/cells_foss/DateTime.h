#pragma once
#include <cstdint>

namespace Aspose {
namespace Cells_FOSS {

class DateTime {
public:
    DateTime() noexcept = default;
    DateTime(int year, int month, int day);
    DateTime(int year, int month, int day, int hour, int minute, int second);
    DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond);
    explicit DateTime(std::int64_t ticks) noexcept;

    std::int64_t GetTicks() const noexcept { return _ticks; }
    int GetYear() const noexcept;
    int GetMonth() const noexcept;
    int GetDay() const noexcept;
    int GetHour() const noexcept;
    int GetMinute() const noexcept;
    int GetSecond() const noexcept;

    bool Equals(const DateTime& other) const noexcept { return _ticks == other._ticks; }

    friend bool operator==(const DateTime& lhs, const DateTime& rhs) noexcept { return lhs.Equals(rhs); }
    friend bool operator!=(const DateTime& lhs, const DateTime& rhs) noexcept { return !lhs.Equals(rhs); }
    friend bool operator<(const DateTime& lhs, const DateTime& rhs) noexcept { return lhs._ticks < rhs._ticks; }
    friend bool operator<=(const DateTime& lhs, const DateTime& rhs) noexcept { return lhs._ticks <= rhs._ticks; }
    friend bool operator>(const DateTime& lhs, const DateTime& rhs) noexcept { return lhs._ticks > rhs._ticks; }
    friend bool operator>=(const DateTime& lhs, const DateTime& rhs) noexcept { return lhs._ticks >= rhs._ticks; }

private:
    std::int64_t _ticks = 0;
};

}}  // namespace Aspose::Cells_FOSS
