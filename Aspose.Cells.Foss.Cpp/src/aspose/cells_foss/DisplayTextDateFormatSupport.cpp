#include "aspose/cells_foss/DisplayTextDateFormatSupport.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <locale>
#include <string>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Locale-aware name lookup helpers
// ---------------------------------------------------------------------------

std::string DisplayTextDateFormatSupport::GetLocaleLanguage(
    const std::locale& culture)
{
    std::string name = culture.name();
    if (name.size() >= 2 &&
        std::isalpha(static_cast<unsigned char>(name[0])) &&
        std::isalpha(static_cast<unsigned char>(name[1]))) {
        std::string lang;
        lang.push_back(static_cast<char>(
            std::tolower(static_cast<unsigned char>(name[0]))));
        lang.push_back(static_cast<char>(
            std::tolower(static_cast<unsigned char>(name[1]))));
        return lang;
    }
    return "en";
}

std::string DisplayTextDateFormatSupport::GetLocaleDayName(
    const std::locale& culture, int dayOfWeek, bool abbreviated)
{
    std::string lang = GetLocaleLanguage(culture);

    if (lang == "fr") {
        static const char* const full[] = {
            "dimanche", "lundi", "mardi", "mercredi",
            "jeudi", "vendredi", "samedi"};
        static const char* const abbr[] = {
            "dim.", "lun.", "mar.", "mer.",
            "jeu.", "ven.", "sam."};
        if (dayOfWeek >= 0 && dayOfWeek < 7) {
            return abbreviated ? abbr[dayOfWeek] : full[dayOfWeek];
        }
    } else if (lang == "de") {
        static const char* const full[] = {
            "Sonntag", "Montag", "Dienstag", "Mittwoch",
            "Donnerstag", "Freitag", "Samstag"};
        static const char* const abbr[] = {
            "So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};
        if (dayOfWeek >= 0 && dayOfWeek < 7) {
            return abbreviated ? abbr[dayOfWeek] : full[dayOfWeek];
        }
    }

    // Default: English.
    static const char* const full[] = {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"};
    static const char* const abbr[] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    if (dayOfWeek >= 0 && dayOfWeek < 7) {
        return abbreviated ? abbr[dayOfWeek] : full[dayOfWeek];
    }
    return std::string();
}

std::string DisplayTextDateFormatSupport::GetLocaleMonthName(
    const std::locale& culture, int month, bool abbreviated)
{
    if (month < 1 || month > 12) {
        return std::string();
    }

    std::string lang = GetLocaleLanguage(culture);

    if (lang == "fr") {
        // French month names with UTF-8 encoded accented characters.
        static const char* const full[] = {
            "", "janvier", "f\xC3\xA9vrier", "mars", "avril", "mai", "juin",
            "juillet", "ao\xC3\xBBt", "septembre", "octobre",
            "novembre", "d\xC3\xA9""cembre"};
        static const char* const abbr[] = {
            "", "janv.", "f\xC3\xA9vr.", "mars", "avr.", "mai", "juin",
            "juil.", "ao\xC3\xBBt", "sept.", "oct.", "nov.", "d\xC3\xA9""c."};
        return abbreviated ? abbr[month] : full[month];
    } else if (lang == "de") {
        // German month names with UTF-8 encoded accented characters.
        static const char* const full[] = {
            "", "Januar", "Februar", "M\xC3\xA4rz", "April", "Mai", "Juni",
            "Juli", "August", "September", "Oktober",
            "November", "Dezember"};
        static const char* const abbr[] = {
            "", "Jan", "Feb", "M\xC3\xA4r", "Apr", "Mai", "Jun",
            "Jul", "Aug", "Sep", "Okt", "Nov", "Dez"};
        return abbreviated ? abbr[month] : full[month];
    }

    // Default: English.
    static const char* const full[] = {
        "", "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"};
    static const char* const abbr[] = {
        "", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    return abbreviated ? abbr[month] : full[month];
}

// ---------------------------------------------------------------------------
// Number formatting helpers
// ---------------------------------------------------------------------------

void DisplayTextDateFormatSupport::AppendInt(int value, std::string& out)
{
    out.append(std::to_string(value));
}

void DisplayTextDateFormatSupport::AppendIntPadded(int value, int minWidth,
                                                   std::string& out)
{
    if (value < 0) {
        out.push_back('-');
        value = -value;
    }
    std::string num = std::to_string(value);
    for (int i = static_cast<int>(num.size()); i < minWidth; ++i) {
        out.push_back('0');
    }
    out.append(num);
}

// ---------------------------------------------------------------------------
// Private DateTime formatting helpers
// ---------------------------------------------------------------------------

void DisplayTextDateFormatSupport::AppendYear(const DateTime& value, int count,
                                              std::string& out)
{
    int year = value.GetYear();
    if (count <= 1) {
        AppendInt(year % 100, out);
        return;
    }
    if (count == 2) {
        AppendIntPadded(year % 100, 2, out);
        return;
    }
    AppendIntPadded(year, count, out);
}

void DisplayTextDateFormatSupport::AppendDay(const DateTime& value,
                                             const std::locale& culture,
                                             int count, std::string& out)
{
    int day = value.GetDay();
    if (count == 1) {
        AppendInt(day, out);
        return;
    }
    if (count == 2) {
        AppendIntPadded(day, 2, out);
        return;
    }

    // Sakamoto's method for day-of-week (0=Sunday .. 6=Saturday).
    static const int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    int y = value.GetYear();
    int m = value.GetMonth();
    if (m < 3) {
        --y;
    }
    int dow = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + day) % 7;

    if (count == 3) {
        out.append(GetLocaleDayName(culture, dow, true));
    } else {
        out.append(GetLocaleDayName(culture, dow, false));
    }
}

void DisplayTextDateFormatSupport::AppendMonth(const DateTime& value,
                                               const std::locale& culture,
                                               int count, std::string& out)
{
    int month = value.GetMonth();
    if (count == 1) {
        AppendInt(month, out);
        return;
    }
    if (count == 2) {
        AppendIntPadded(month, 2, out);
        return;
    }

    if (count == 3) {
        out.append(GetLocaleMonthName(culture, month, true));
        return;
    }

    std::string monthName = GetLocaleMonthName(culture, month, false);
    if (count == 4) {
        out.append(monthName);
        return;
    }

    // count >= 5: first letter of month name
    if (!monthName.empty()) {
        out.push_back(monthName[0]);
    } else {
        AppendInt(month, out);
    }
}

void DisplayTextDateFormatSupport::AppendHour(const DateTime& value,
                                              int count, bool hasAmPm,
                                              std::string& out)
{
    int hour = value.GetHour();
    if (hasAmPm) {
        hour %= 12;
        if (hour == 0) {
            hour = 12;
        }
    }
    if (count == 1) {
        AppendInt(hour, out);
    } else {
        AppendIntPadded(hour, 2, out);
    }
}

void DisplayTextDateFormatSupport::AppendMinute(const DateTime& value,
                                                int count, std::string& out)
{
    int minute = value.GetMinute();
    if (count == 1) {
        AppendInt(minute, out);
    } else {
        AppendIntPadded(minute, 2, out);
    }
}

void DisplayTextDateFormatSupport::AppendSecond(const DateTime& value,
                                                int count, std::string& out)
{
    int second = value.GetSecond();
    if (count == 1) {
        AppendInt(second, out);
    } else {
        AppendIntPadded(second, 2, out);
    }
}

void DisplayTextDateFormatSupport::AppendFractionDigits(int milliseconds,
                                                        int zeroCount,
                                                        std::string& out)
{
    char digits[3];
    digits[0] = static_cast<char>('0' + (milliseconds / 100) % 10);
    digits[1] = static_cast<char>('0' + (milliseconds / 10) % 10);
    digits[2] = static_cast<char>('0' + milliseconds % 10);

    if (zeroCount <= 0) {
        return;
    }
    if (zeroCount == 1) {
        out.push_back(digits[0]);
        return;
    }
    if (zeroCount == 2) {
        out.push_back(digits[0]);
        out.push_back(digits[1]);
        return;
    }
    out.append(digits, 3);
    for (int i = 3; i < zeroCount; ++i) {
        out.push_back('0');
    }
}

bool DisplayTextDateFormatSupport::TryAppendFractionalSeconds(
    const DateTime& value, std::string_view formatCode,
    const std::locale& culture, int& index, std::string& out)
{
    if (index + 1 >= static_cast<int>(formatCode.size()) ||
        formatCode[static_cast<size_t>(index + 1)] != '0') {
        return false;
    }

    char previous = FindNeighborToken(formatCode, index - 1, -1);
    if (previous != 's' && previous != 'S') {
        return false;
    }

    int zeroCount = CountRepeated(formatCode, index + 1, '0');
    // Use the locale's decimal separator.
    char decimalSep = '.';
    try {
        decimalSep = std::use_facet<std::numpunct<char>>(culture).decimal_point();
    } catch (...) {
        // Fallback to '.'.
    }
    out.push_back(decimalSep);
    // 1 tick = 100ns; 1ms = 10,000 ticks.
    int millisecond = static_cast<int>((value.GetTicks() / 10000LL) % 1000);
    if (millisecond < 0) {
        millisecond = -millisecond;
    }
    AppendFractionDigits(millisecond, zeroCount, out);
    index += zeroCount;
    return true;
}

std::string DisplayTextDateFormatSupport::GetAmPmDesignator(
    const DateTime& value, const std::locale& /*culture*/, bool abbreviated)
{
    int hour = value.GetHour();
    std::string designator = (hour < 12) ? "AM" : "PM";
    if (!abbreviated) {
        return designator;
    }
    return designator.substr(0, 1);
}

// ---------------------------------------------------------------------------
// Public utility methods
// ---------------------------------------------------------------------------

bool DisplayTextDateFormatSupport::MatchesToken(std::string_view formatCode,
                                                int startIndex,
                                                std::string_view token)
{
    if (startIndex < 0) {
        return false;
    }
    auto usStart = static_cast<size_t>(startIndex);
    if (usStart + token.size() > formatCode.size()) {
        return false;
    }
    for (size_t i = 0; i < token.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(formatCode[usStart + i])) !=
            std::tolower(static_cast<unsigned char>(token[i]))) {
            return false;
        }
    }
    return true;
}

int DisplayTextDateFormatSupport::CountRepeated(std::string_view formatCode,
                                                int startIndex, char token)
{
    int count = 1;
    char lowerToken = static_cast<char>(
        std::tolower(static_cast<unsigned char>(token)));
    for (size_t i = static_cast<size_t>(startIndex + 1);
         i < formatCode.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(formatCode[i])) !=
            lowerToken) {
            break;
        }
        ++count;
    }
    return count;
}

char DisplayTextDateFormatSupport::FindNeighborToken(
    std::string_view formatCode, int startIndex, int direction)
{
    bool inQuote = false;
    if (direction < 0) {
        for (int i = startIndex; i >= 0; --i) {
            char c = formatCode[static_cast<size_t>(i)];
            if (c == '"') {
                inQuote = !inQuote;
                continue;
            }
            if (inQuote) {
                continue;
            }
            if (c == '\\' || c == '_' || c == '*') {
                --i;
                continue;
            }
            if (std::isspace(static_cast<unsigned char>(c)) != 0) {
                continue;
            }
            return c;
        }
    } else {
        for (size_t i = static_cast<size_t>(startIndex);
             i < formatCode.size(); ++i) {
            char c = formatCode[i];
            if (c == '"') {
                inQuote = !inQuote;
                continue;
            }
            if (inQuote) {
                continue;
            }
            if (c == '\\' || c == '_' || c == '*') {
                ++i;
                continue;
            }
            if (std::isspace(static_cast<unsigned char>(c)) != 0) {
                continue;
            }
            return c;
        }
    }
    return '\0';
}

bool DisplayTextDateFormatSupport::IsMinuteContext(std::string_view formatCode,
                                                   int startIndex, int count)
{
    char previous = FindNeighborToken(formatCode, startIndex - 1, -1);
    char next = FindNeighborToken(formatCode, startIndex + count, 1);
    if (previous == ':' || next == ':') {
        return true;
    }
    if (previous == 'h' || previous == 'H' ||
        previous == 's' || previous == 'S') {
        return true;
    }
    if (next == 'h' || next == 'H' ||
        next == 's' || next == 'S') {
        return true;
    }
    return false;
}

bool DisplayTextDateFormatSupport::IsElapsedToken(std::string_view token)
{
    // Trim whitespace from both ends.
    size_t start = 0;
    while (start < token.size() &&
           std::isspace(static_cast<unsigned char>(token[start])) != 0) {
        ++start;
    }
    size_t end = token.size();
    while (end > start &&
           std::isspace(static_cast<unsigned char>(token[end - 1])) != 0) {
        --end;
    }
    if (start >= end) {
        return false;
    }

    std::string normalized;
    normalized.reserve(end - start);
    for (size_t i = start; i < end; ++i) {
        normalized.push_back(static_cast<char>(
            std::tolower(static_cast<unsigned char>(token[i]))));
    }

    return normalized == "h" || normalized == "hh" ||
           normalized == "m" || normalized == "mm" ||
           normalized == "s" || normalized == "ss";
}

bool DisplayTextDateFormatSupport::ContainsElapsedTimeToken(
    std::string_view formatCode)
{
    for (size_t i = 0; i < formatCode.size(); ++i) {
        if (formatCode[i] != '[') {
            continue;
        }
        auto endPos = formatCode.find(']', i + 1);
        if (endPos == std::string_view::npos) {
            continue;
        }
        std::string_view token = formatCode.substr(i + 1, endPos - i - 1);
        if (IsElapsedToken(token)) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
// FormatDateTimeValue
// ---------------------------------------------------------------------------

std::string DisplayTextDateFormatSupport::FormatDateTimeValue(
    const DateTime& value, std::string_view formatCode,
    const std::locale& culture)
{
    std::string builder;
    builder.reserve(formatCode.size() + 16);

    // Detect AM/PM usage.
    bool hasAmPm = false;
    {
        std::string lower;
        lower.reserve(formatCode.size());
        for (char c : formatCode) {
            lower.push_back(
                static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        }
        if (lower.find("am/pm") != std::string::npos ||
            lower.find("a/p") != std::string::npos) {
            hasAmPm = true;
        }
    }

    bool inQuote = false;

    for (int index = 0; index < static_cast<int>(formatCode.size()); ++index) {
        if (MatchesToken(formatCode, index, "AM/PM")) {
            builder.append(GetAmPmDesignator(value, culture, false));
            index += 4;
            continue;
        }

        if (MatchesToken(formatCode, index, "A/P")) {
            builder.append(GetAmPmDesignator(value, culture, true));
            index += 2;
            continue;
        }

        char character = formatCode[static_cast<size_t>(index)];
        if (character == '"') {
            inQuote = !inQuote;
            continue;
        }

        if (inQuote) {
            builder.push_back(character);
            continue;
        }

        if (character == '\\') {
            if (index + 1 < static_cast<int>(formatCode.size())) {
                ++index;
                builder.push_back(formatCode[static_cast<size_t>(index)]);
            }
            continue;
        }

        if (character == '_' || character == '*') {
            ++index;
            continue;
        }

        if (character == '.' &&
            TryAppendFractionalSeconds(value, formatCode, culture, index,
                                       builder)) {
            continue;
        }

        if (character == 'y' || character == 'Y') {
            int count = CountRepeated(formatCode, index, character);
            AppendYear(value, count, builder);
            index += count - 1;
            continue;
        }

        if (character == 'd' || character == 'D') {
            int count = CountRepeated(formatCode, index, character);
            AppendDay(value, culture, count, builder);
            index += count - 1;
            continue;
        }

        if (character == 'h' || character == 'H') {
            int count = CountRepeated(formatCode, index, character);
            AppendHour(value, count, hasAmPm, builder);
            index += count - 1;
            continue;
        }

        if (character == 's' || character == 'S') {
            int count = CountRepeated(formatCode, index, character);
            AppendSecond(value, count, builder);
            index += count - 1;
            continue;
        }

        if (character == 'm' || character == 'M') {
            int count = CountRepeated(formatCode, index, character);
            if (IsMinuteContext(formatCode, index, count)) {
                AppendMinute(value, count, builder);
            } else {
                AppendMonth(value, culture, count, builder);
            }
            index += count - 1;
            continue;
        }

        builder.push_back(character);
    }

    return builder;
}

// ---------------------------------------------------------------------------
// FormatElapsedTimeValue
// ---------------------------------------------------------------------------

static int GetElapsedHours(std::chrono::duration<double> time)
{
    double totalSec = time.count();
    if (totalSec < 0.0) {
        totalSec = -totalSec;
    }
    return static_cast<int>(std::floor(totalSec / 3600.0));
}

static int GetElapsedMinutes(std::chrono::duration<double> time)
{
    double totalSec = time.count();
    if (totalSec < 0.0) {
        totalSec = -totalSec;
    }
    return static_cast<int>(std::floor(totalSec / 60.0));
}

static int GetElapsedSeconds(std::chrono::duration<double> time)
{
    double totalSec = time.count();
    if (totalSec < 0.0) {
        totalSec = -totalSec;
    }
    return static_cast<int>(std::floor(totalSec));
}

static int GetComponentHours(std::chrono::duration<double> time)
{
    double totalSec = time.count();
    if (totalSec < 0.0) {
        totalSec = -totalSec;
    }
    auto isec = static_cast<int64_t>(totalSec);
    return static_cast<int>((isec / 3600) % 24);
}

static int GetComponentMinutes(std::chrono::duration<double> time)
{
    double totalSec = time.count();
    if (totalSec < 0.0) {
        totalSec = -totalSec;
    }
    auto isec = static_cast<int64_t>(totalSec);
    return static_cast<int>((isec / 60) % 60);
}

static int GetComponentSeconds(std::chrono::duration<double> time)
{
    double totalSec = time.count();
    if (totalSec < 0.0) {
        totalSec = -totalSec;
    }
    auto isec = static_cast<int64_t>(totalSec);
    return static_cast<int>(isec % 60);
}

static int GetComponentMilliseconds(std::chrono::duration<double> time)
{
    double totalSec = time.count();
    if (totalSec < 0.0) {
        totalSec = -totalSec;
    }
    double frac = totalSec - std::floor(totalSec);
    return static_cast<int>(frac * 1000.0) % 1000;
}

std::string DisplayTextDateFormatSupport::FormatElapsedTimeValue(
    std::chrono::duration<double> time, std::string_view formatCode,
    const std::locale& culture)
{
    std::string builder;
    builder.reserve(formatCode.size() + 8);
    bool inQuote = false;

    for (int index = 0; index < static_cast<int>(formatCode.size()); ++index) {
        char character = formatCode[static_cast<size_t>(index)];

        if (character == '"') {
            inQuote = !inQuote;
            continue;
        }

        if (inQuote) {
            builder.push_back(character);
            continue;
        }

        if (character == '[') {
            auto endPos = formatCode.find(']', static_cast<size_t>(index + 1));
            if (endPos != std::string_view::npos &&
                static_cast<int>(endPos) > index) {
                std::string_view token =
                    formatCode.substr(static_cast<size_t>(index + 1),
                                      endPos - static_cast<size_t>(index) - 1);
                std::string tokenLower;
                tokenLower.reserve(token.size());
                for (char c : token) {
                    tokenLower.push_back(static_cast<char>(
                        std::tolower(static_cast<unsigned char>(c))));
                }

                if (tokenLower == "h") {
                    AppendInt(GetElapsedHours(time), builder);
                } else if (tokenLower == "hh") {
                    AppendIntPadded(GetElapsedHours(time), 2, builder);
                } else if (tokenLower == "m") {
                    AppendInt(GetElapsedMinutes(time), builder);
                } else if (tokenLower == "mm") {
                    AppendIntPadded(GetElapsedMinutes(time), 2, builder);
                } else if (tokenLower == "s") {
                    AppendInt(GetElapsedSeconds(time), builder);
                } else if (tokenLower == "ss") {
                    AppendIntPadded(GetElapsedSeconds(time), 2, builder);
                } else {
                    builder.push_back('[');
                    builder.append(tokenLower);
                    builder.push_back(']');
                }

                index = static_cast<int>(endPos);
                continue;
            }
        }

        if (character == 'h' || character == 'H') {
            int count = CountRepeated(formatCode, index, character);
            int hours = GetComponentHours(time);
            if (count == 1) {
                AppendInt(hours, builder);
            } else {
                AppendIntPadded(hours, 2, builder);
            }
            index += count - 1;
            continue;
        }

        if (character == 'm' || character == 'M') {
            int count = CountRepeated(formatCode, index, character);
            int minutes = GetComponentMinutes(time);
            if (count == 1) {
                AppendInt(minutes, builder);
            } else {
                AppendIntPadded(minutes, 2, builder);
            }
            index += count - 1;
            continue;
        }

        if (character == 's' || character == 'S') {
            int count = CountRepeated(formatCode, index, character);
            int seconds = GetComponentSeconds(time);
            if (count == 1) {
                AppendInt(seconds, builder);
            } else {
                AppendIntPadded(seconds, 2, builder);
            }
            index += count - 1;
            continue;
        }

        if (character == '.' && index + 1 < static_cast<int>(formatCode.size()) &&
            formatCode[static_cast<size_t>(index + 1)] == '0') {
            int zeroCount = CountRepeated(formatCode, index + 1, '0');
            // Use the locale's decimal separator.
            char decimalSep = '.';
            try {
                decimalSep = std::use_facet<std::numpunct<char>>(culture).decimal_point();
            } catch (...) {
                // Fallback to '.'.
            }
            builder.push_back(decimalSep);
            int ms = GetComponentMilliseconds(time);
            AppendFractionDigits(ms, zeroCount, builder);
            index += zeroCount;
            continue;
        }

        if (character == '\\') {
            if (index + 1 < static_cast<int>(formatCode.size())) {
                ++index;
                builder.push_back(formatCode[static_cast<size_t>(index)]);
            }
            continue;
        }

        if (character == '_' || character == '*') {
            ++index;
            continue;
        }

        builder.push_back(character);
    }

    return builder;
}

}}  // namespace Aspose::Cells_FOSS
