#include "aspose/cells_foss/core/WorkbookPropertiesModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

WorkbookPropertiesModel::WorkbookPropertiesModel()
    : _codeName()
    , _showObjects()
    , _filterPrivacy(false)
    , _showBorderUnselectedTables(true)
    , _showInkAnnotation(true)
    , _backupFile(false)
    , _saveExternalLinkValues(true)
    , _updateLinks()
    , _hidePivotFieldList(false)
    , _defaultThemeVersion()
    , _protection()
    , _view()
    , _calculation()
{
}

void WorkbookPropertiesModel::CopyFrom(const WorkbookPropertiesModel& source)
{
    _codeName = source._codeName;
    _showObjects = source._showObjects;
    _filterPrivacy = source._filterPrivacy;
    _showBorderUnselectedTables = source._showBorderUnselectedTables;
    _showInkAnnotation = source._showInkAnnotation;
    _backupFile = source._backupFile;
    _saveExternalLinkValues = source._saveExternalLinkValues;
    _updateLinks = source._updateLinks;
    _hidePivotFieldList = source._hidePivotFieldList;
    _defaultThemeVersion = source._defaultThemeVersion;
    _protection.CopyFrom(source._protection);
    _view.CopyFrom(source._view);
    _calculation.CopyFrom(source._calculation);
}

bool WorkbookPropertiesModel::HasWorkbookPropertiesState() const
{
    return !_codeName.empty()
        || !_showObjects.empty()
        || _filterPrivacy
        || !_showBorderUnselectedTables
        || !_showInkAnnotation
        || _backupFile
        || !_saveExternalLinkValues
        || !_updateLinks.empty()
        || _hidePivotFieldList
        || _defaultThemeVersion.has_value();
}

}}}  // namespace Aspose::Cells_FOSS::Core
