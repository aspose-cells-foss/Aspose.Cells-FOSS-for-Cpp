#include "aspose/cells_foss/core/WorkbookModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

WorkbookModel::WorkbookModel()
    : _worksheets{ WorksheetModel(std::string_view("Sheet1")) },
      _defaultStyle(StyleValue::Default()),
      _activeSheetIndex(0)
{
}

}}}  // namespace Aspose::Cells_FOSS::Core
