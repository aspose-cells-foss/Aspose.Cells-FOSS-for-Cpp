#pragma once

#include "aspose/cells_foss/DateTime.h"

#include <chrono>
#include <locale>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

/// Internal helper methods for formatting date/time display text.
class DisplayTextDateFormatSupport {
public:
    DisplayTextDateFormatSupport() = delete;

    /// Formats a DateTime value according to the given format code and locale.
    static std::string FormatDateTimeValue(const DateTime& value,
                                           std::string_view formatCode,
                                           const std::locale& culture);

    /// Formats a TimeSpan value according to the given format code and locale.
    static std::string FormatElapsedTimeValue(std::chrono::duration<double> time,
                                              std::string_view formatCode,
                                              const std::locale& culture);

    /// Returns true if the token (case-insensitive) is an elapsed-time token
    /// such as h, hh, m, mm, s, or ss.
    static bool IsElapsedToken(std::string_view token);

    /// Scans for bracketed elapsed-time tokens in the format code.
    static bool ContainsElapsedTimeToken(std::string_view formatCode);

    /// Tests whether the substring starting at startIndex matches token
    /// (case-insensitive).
    static bool MatchesToken(std::string_view formatCode, int startIndex,
                             std::string_view token);

    /// Counts consecutive occurrences of the character at startIndex.
    static int CountRepeated(std::string_view formatCode, int startIndex,
                             char token);

    /// Determines whether an 'm'/'M' sequence at the given position is in a
    /// minute context (rather than a month context).
    static bool IsMinuteContext(std::string_view formatCode, int startIndex,
                               int count);

    /// Finds the nearest non-whitespace, non-escape token character in the
    /// given direction from startIndex. Returns '\\0' if none is found.
    static char FindNeighborToken(std::string_view formatCode, int startIndex,
                                  int direction);

private:
    static void AppendYear(const DateTime& value, int count, std::string& out);
    static void AppendDay(const DateTime& value, const std::locale& culture,
                          int count, std::string& out);
    static void AppendMonth(const DateTime& value, const std::locale& culture,
                            int count, std::string& out);
    static void AppendHour(const DateTime& value, int count, bool hasAmPm,
                           std::string& out);
    static void AppendMinute(const DateTime& value, int count,
                             std::string& out);
    static void AppendSecond(const DateTime& value, int count,
                             std::string& out);
    static bool TryAppendFractionalSeconds(const DateTime& value,
                                           std::string_view formatCode,
                                           const std::locale& culture,
                                           int& index, std::string& out);
    static void AppendFractionDigits(int milliseconds, int zeroCount,
                                     std::string& out);
    static std::string GetAmPmDesignator(const DateTime& value,
                                         const std::locale& culture,
                                         bool abbreviated);

    static void AppendInt(int value, std::string& out);
    static void AppendIntPadded(int value, int minWidth, std::string& out);

    static std::string GetLocaleDayName(const std::locale& culture,
                                        int dayOfWeek, bool abbreviated);
    static std::string GetLocaleMonthName(const std::locale& culture,
                                          int month, bool abbreviated);
    static std::string GetLocaleLanguage(const std::locale& culture);
};

}}  // namespace Aspose::Cells_FOSS
