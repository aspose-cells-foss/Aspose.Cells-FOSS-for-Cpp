#include "aspose/cells_foss/StylesheetLoadContext.h"

namespace Aspose {
namespace Cells_FOSS {

StylesheetLoadContext::StylesheetLoadContext()
    : _defaultCellStyle(Core::StyleValue::Default().Clone()),
      _cellFormats{Core::StyleValue::Default().Clone()}
{
}

}}  // namespace Aspose::Cells_FOSS
