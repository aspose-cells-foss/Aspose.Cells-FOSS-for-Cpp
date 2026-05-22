#include "aspose/cells_foss/core/CoreDocumentPropertiesModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

CoreDocumentPropertiesModel::CoreDocumentPropertiesModel() = default;

void CoreDocumentPropertiesModel::CopyFrom(const CoreDocumentPropertiesModel& source)
{
    _title = source._title;
    _subject = source._subject;
    _creator = source._creator;
    _keywords = source._keywords;
    _description = source._description;
    _lastModifiedBy = source._lastModifiedBy;
    _revision = source._revision;
    _category = source._category;
    _contentStatus = source._contentStatus;
    _created = source._created;
    _modified = source._modified;
}

bool CoreDocumentPropertiesModel::HasStoredState() const
{
    return !_title.empty()
        || !_subject.empty()
        || !_creator.empty()
        || !_keywords.empty()
        || !_description.empty()
        || !_lastModifiedBy.empty()
        || !_revision.empty()
        || !_category.empty()
        || !_contentStatus.empty()
        || _created.has_value()
        || _modified.has_value();
}

}}}  // namespace Aspose::Cells_FOSS::Core
