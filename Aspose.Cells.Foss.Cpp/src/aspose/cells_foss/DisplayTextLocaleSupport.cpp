#include "aspose/cells_foss/DisplayTextLocaleSupport.h"

#include <cctype>
#include <string>

namespace Aspose {
namespace Cells_FOSS {

// ---------------------------------------------------------------------------
// Public
// ---------------------------------------------------------------------------

std::string DisplayTextLocaleSupport::ApplyLocaleDirectives(
    std::string_view section,
    const std::locale& defaultCulture,
    std::locale& sectionCulture)
{
    sectionCulture = defaultCulture;
    std::string builder;
    builder.reserve(section.size() + 16);
    bool inQuote = false;

    for (std::size_t index = 0; index < section.size(); ++index) {
        char character = section[index];

        if (character == '"') {
            builder.push_back(character);
            inQuote = !inQuote;
            continue;
        }

        if (!inQuote && character == '[') {
            auto endPos = section.find(']', index + 1);
            if (endPos != std::string_view::npos && endPos > index) {
                std::string_view token = section.substr(index + 1,
                                                         endPos - index - 1);
                std::string replacement;
                std::locale resolvedCulture;
                if (TryResolveLocaleDirective(token, sectionCulture,
                                              replacement, resolvedCulture)) {
                    builder.append(replacement);
                    sectionCulture = resolvedCulture;
                    index = endPos;
                    continue;
                }
            }
        }

        builder.push_back(character);
    }

    return builder;
}

// ---------------------------------------------------------------------------
// Private
// ---------------------------------------------------------------------------

// Returns the 2-letter lowercase language code from a locale, or "en".
static std::string GetLocaleLanguage(const std::locale& culture)
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

bool DisplayTextLocaleSupport::TryResolveLocaleDirective(
    std::string_view token,
    const std::locale& defaultCulture,
    std::string& replacement,
    std::locale& resolvedCulture)
{
    replacement.clear();
    resolvedCulture = defaultCulture;

    // Token must be non-blank and start with '$'.
    if (token.empty()) {
        return false;
    }

    // Check for whitespace-only token.
    bool allWhitespace = true;
    for (char ch : token) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            allWhitespace = false;
            break;
        }
    }
    if (allWhitespace || token[0] != '$') {
        return false;
    }

    // Find the last '-' separator.
    auto dashPos = token.find_last_of('-');
    if (dashPos == std::string_view::npos || dashPos == 0 ||
        dashPos >= token.size() - 1) {
        return false;
    }

    std::string_view symbol = token.substr(1, dashPos - 1);
    std::string_view localeCode = token.substr(dashPos + 1);
    resolvedCulture = ResolveCulture(localeCode, defaultCulture);

    // F800: long date pattern (culture-specific, matching .NET behaviour)
    if (localeCode.size() == 4 &&
        (localeCode[0] == 'F' || localeCode[0] == 'f') &&
        localeCode[1] == '8' && localeCode[2] == '0' && localeCode[3] == '0') {
        std::string lang = GetLocaleLanguage(defaultCulture);
        if (lang == "fr") {
            // .NET fr-FR DateTimeFormat.LongDatePattern
            replacement = "dddd d MMMM yyyy";
        } else if (lang == "de") {
            // .NET de-DE DateTimeFormat.LongDatePattern
            replacement = "dddd, d. MMMM yyyy";
        } else {
            // Default: en-US
            replacement = "dddd, MMMM d, yyyy";
        }
        resolvedCulture = defaultCulture;
        return true;
    }

    // F400: long time pattern (culture-specific, matching .NET behaviour)
    if (localeCode.size() == 4 &&
        (localeCode[0] == 'F' || localeCode[0] == 'f') &&
        localeCode[1] == '4' && localeCode[2] == '0' && localeCode[3] == '0') {
        std::string lang = GetLocaleLanguage(defaultCulture);
        if (lang == "fr" || lang == "de") {
            // .NET fr-FR and de-DE DateTimeFormat.LongTimePattern
            replacement = "HH:mm:ss";
        } else {
            // Default: en-US
            replacement = "h:mm:ss tt";
        }
        resolvedCulture = defaultCulture;
        return true;
    }

    if (!symbol.empty()) {
        replacement = QuoteLiteral(symbol);
    }

    return true;
}

std::locale DisplayTextLocaleSupport::ResolveCulture(
    std::string_view localeCode,
    const std::locale& defaultCulture)
{
    if (localeCode.empty()) {
        return defaultCulture;
    }

    // All-whitespace check.
    bool allWhitespace = true;
    for (char ch : localeCode) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            allWhitespace = false;
            break;
        }
    }
    if (allWhitespace) {
        return defaultCulture;
    }

    // Parse hex LCID (e.g. "0409" -> 1033).
    // .NET uses NumberStyles.HexNumber which accepts optional leading
    // whitespace and sign; we replicate just the unsigned hex parse.
    std::string lcidStr(localeCode);
    std::size_t pos = 0;
    unsigned long lcid = 0;
    try {
        lcid = std::stoul(lcidStr, &pos, 16);
    } catch (...) {
        return defaultCulture;
    }

    // Verify that all characters were consumed (ignoring trailing
    // whitespace, which .NET's int.TryParse with HexNumber allows).
    while (pos < lcidStr.size() &&
           std::isspace(static_cast<unsigned char>(lcidStr[pos]))) {
        ++pos;
    }
    if (pos < lcidStr.size()) {
        return defaultCulture;
    }

    // Map well-known LCIDs to std::locale names.
    // The most common Excel LCID is 0x0409 (en-US).
    switch (lcid) {
    case 0x0409: // en-US
        return std::locale("en_US.UTF-8");
    case 0x0809: // en-GB
        return std::locale("en_GB.UTF-8");
    case 0x0407: // de-DE
        return std::locale("de_DE.UTF-8");
    case 0x040C: // fr-FR
        return std::locale("fr_FR.UTF-8");
    case 0x0411: // ja-JP
        return std::locale("ja_JP.UTF-8");
    case 0x0804: // zh-CN
        return std::locale("zh_CN.UTF-8");
    case 0x0404: // zh-TW
        return std::locale("zh_TW.UTF-8");
    case 0x0416: // pt-BR
        return std::locale("pt_BR.UTF-8");
    case 0x0C0A: // es-ES
        return std::locale("es_ES.UTF-8");
    case 0x0410: // it-IT
        return std::locale("it_IT.UTF-8");
    case 0x0413: // nl-NL
        return std::locale("nl_NL.UTF-8");
    case 0x0419: // ru-RU
        return std::locale("ru_RU.UTF-8");
    case 0x0415: // pl-PL
        return std::locale("pl_PL.UTF-8");
    default:
        // If the locale name is not in our table, return the default.
        return defaultCulture;
    }
}

std::string DisplayTextLocaleSupport::QuoteLiteral(std::string_view value)
{
    std::string builder;
    builder.reserve(value.size() + 2);
    builder.push_back('"');
    for (char ch : value) {
        if (ch == '"') {
            builder.push_back('"');
        }
        builder.push_back(ch);
    }
    builder.push_back('"');
    return builder;
}

}}  // namespace Aspose::Cells_FOSS
