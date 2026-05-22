#pragma once

#include <locale>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

/// Internal helper for parsing and applying locale directives
/// (e.g. [$-0409], [$-F800]) embedded in Excel format strings.
class DisplayTextLocaleSupport {
public:
    DisplayTextLocaleSupport() = delete;

    /// Scans the given format section for [$-XXXX] locale directives,
    /// replacing each directive with the appropriate literal text and
    /// updating sectionCulture to the resolved locale.
    ///
    /// @param section The raw format-section text.
    /// @param defaultCulture The locale to use when no directive is present.
    /// @param[out] sectionCulture Receives the locale resolved from the
    ///             directives (or defaultCulture when none are found).
    /// @return The section text with locale directives replaced.
    static std::string ApplyLocaleDirectives(std::string_view section,
                                             const std::locale& defaultCulture,
                                             std::locale& sectionCulture);

private:
    static bool TryResolveLocaleDirective(std::string_view token,
                                          const std::locale& defaultCulture,
                                          std::string& replacement,
                                          std::locale& resolvedCulture);

    static std::locale ResolveCulture(std::string_view localeCode,
                                      const std::locale& defaultCulture);

    static std::string QuoteLiteral(std::string_view value);
};

}}  // namespace Aspose::Cells_FOSS
