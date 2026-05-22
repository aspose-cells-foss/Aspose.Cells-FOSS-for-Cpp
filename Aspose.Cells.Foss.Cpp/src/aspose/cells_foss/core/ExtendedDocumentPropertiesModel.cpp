#include "aspose/cells_foss/core/ExtendedDocumentPropertiesModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

ExtendedDocumentPropertiesModel::ExtendedDocumentPropertiesModel() = default;

void ExtendedDocumentPropertiesModel::CopyFrom(const ExtendedDocumentPropertiesModel& source)
{
    _application = source._application;
    _appVersion = source._appVersion;
    _company = source._company;
    _manager = source._manager;
    _docSecurity = source._docSecurity;
    _hyperlinkBase = source._hyperlinkBase;
    _scaleCrop = source._scaleCrop;
    _linksUpToDate = source._linksUpToDate;
    _sharedDoc = source._sharedDoc;
}

bool ExtendedDocumentPropertiesModel::HasStoredState() const
{
    return !_application.empty()
        || !_appVersion.empty()
        || !_company.empty()
        || !_manager.empty()
        || _docSecurity.has_value()
        || !_hyperlinkBase.empty()
        || _scaleCrop.has_value()
        || _linksUpToDate.has_value()
        || _sharedDoc.has_value();
}

}}}  // namespace Aspose::Cells_FOSS::Core
