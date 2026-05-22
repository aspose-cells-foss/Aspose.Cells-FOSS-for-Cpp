#include "aspose/cells_foss/StylesheetSaveContext.h"
#include "aspose/cells_foss/XlsxWorkbookStyles.h"

namespace Aspose {
namespace Cells_FOSS {

StylesheetSaveContext::StylesheetSaveContext(
    XmlDocument document,
    std::unordered_map<std::string, int> styleIndices,
    std::unordered_map<std::string, int> differentialStyleIndices,
    int differentialFormatCount,
    bool hasStyles)
    : _document(std::move(document))
    , _styleIndices(std::move(styleIndices))
    , _differentialStyleIndices(std::move(differentialStyleIndices))
    , _differentialFormatCount(differentialFormatCount)
    , _hasStyles(hasStyles)
{
}

int StylesheetSaveContext::GetStyleIndex(const Core::CellRecord& record) const
{
    const Core::StyleValue style = XlsxWorkbookStyles::GetStyleForSerialization(record);
    const std::string key = XlsxWorkbookStyles::GetStyleKey(style);
    const auto it = _styleIndices.find(key);
    return it != _styleIndices.end() ? it->second : 0;
}

std::optional<int> StylesheetSaveContext::GetDifferentialStyleIndex(
    const Core::FormatConditionModel& condition) const
{
    if (XlsxWorkbookStyles::StylesEqual(condition.GetStyle(), Core::StyleValue::Default()))
    {
        return std::nullopt;
    }

    const std::string key = XlsxWorkbookStyles::GetStyleKey(condition.GetStyle());
    const auto it = _differentialStyleIndices.find(key);
    if (it != _differentialStyleIndices.end())
    {
        return it->second;
    }
    return std::nullopt;
}

}}  // namespace Aspose::Cells_FOSS
