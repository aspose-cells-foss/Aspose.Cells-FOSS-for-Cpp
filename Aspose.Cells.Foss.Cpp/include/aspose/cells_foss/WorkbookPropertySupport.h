#pragma once

#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

/// Internal helpers that normalize workbook-level property strings to their
/// canonical XML attribute values, throwing CellsException on unsupported input.
class WorkbookPropertySupport {
public:
    WorkbookPropertySupport() = delete;

    /// Normalizes a showObjects property value to its canonical form.
    static std::string NormalizeShowObjects(std::string_view value);

    /// Normalizes an updateLinks property value to its canonical form.
    static std::string NormalizeUpdateLinks(std::string_view value);

    /// Normalizes a visibility property value to its canonical form.
    static std::string NormalizeVisibility(std::string_view value);

    /// Normalizes a calcMode property value to its canonical form.
    static std::string NormalizeCalculationMode(std::string_view value);

    /// Normalizes a refMode property value to its canonical form.
    static std::string NormalizeReferenceMode(std::string_view value);

private:
    static std::string NormalizeChoice(std::string_view value,
                                       std::string_view propertyName,
                                       const std::string_view* allowed,
                                       std::size_t count);
};

}}  // namespace Aspose::Cells_FOSS
