#pragma once
#include <optional>
#include <string>

#include "aspose/cells_foss/DateTime.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents core document properties model.
/// </summary>
class CoreDocumentPropertiesModel final {
public:
    /// <summary>
    /// Initializes a new instance of the CoreDocumentPropertiesModel class.
    /// </summary>
    CoreDocumentPropertiesModel();

    /// <summary>
    /// Gets or sets the title.
    /// </summary>
    const std::string& GetTitle() const noexcept { return _title; }
    void SetTitle(std::string value) { _title = std::move(value); }

    /// <summary>
    /// Gets or sets the subject.
    /// </summary>
    const std::string& GetSubject() const noexcept { return _subject; }
    void SetSubject(std::string value) { _subject = std::move(value); }

    /// <summary>
    /// Gets or sets the creator.
    /// </summary>
    const std::string& GetCreator() const noexcept { return _creator; }
    void SetCreator(std::string value) { _creator = std::move(value); }

    /// <summary>
    /// Gets or sets the keywords.
    /// </summary>
    const std::string& GetKeywords() const noexcept { return _keywords; }
    void SetKeywords(std::string value) { _keywords = std::move(value); }

    /// <summary>
    /// Gets or sets the description.
    /// </summary>
    const std::string& GetDescription() const noexcept { return _description; }
    void SetDescription(std::string value) { _description = std::move(value); }

    /// <summary>
    /// Gets or sets the last modified by.
    /// </summary>
    const std::string& GetLastModifiedBy() const noexcept { return _lastModifiedBy; }
    void SetLastModifiedBy(std::string value) { _lastModifiedBy = std::move(value); }

    /// <summary>
    /// Gets or sets the revision.
    /// </summary>
    const std::string& GetRevision() const noexcept { return _revision; }
    void SetRevision(std::string value) { _revision = std::move(value); }

    /// <summary>
    /// Gets or sets the category.
    /// </summary>
    const std::string& GetCategory() const noexcept { return _category; }
    void SetCategory(std::string value) { _category = std::move(value); }

    /// <summary>
    /// Gets or sets the content status.
    /// </summary>
    const std::string& GetContentStatus() const noexcept { return _contentStatus; }
    void SetContentStatus(std::string value) { _contentStatus = std::move(value); }

    /// <summary>
    /// Gets or sets the created.
    /// </summary>
    std::optional<DateTime> GetCreated() const noexcept { return _created; }
    void SetCreated(std::optional<DateTime> value) noexcept { _created = value; }

    /// <summary>
    /// Gets or sets the modified.
    /// </summary>
    std::optional<DateTime> GetModified() const noexcept { return _modified; }
    void SetModified(std::optional<DateTime> value) noexcept { _modified = value; }

    /// <summary>
    /// Copies values from the specified source.
    /// </summary>
    /// <param name="source">The source.</param>
    void CopyFrom(const CoreDocumentPropertiesModel& source);

    /// <summary>
    /// Performs has stored state.
    /// </summary>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool HasStoredState() const;

private:
    std::string _title;
    std::string _subject;
    std::string _creator;
    std::string _keywords;
    std::string _description;
    std::string _lastModifiedBy;
    std::string _revision;
    std::string _category;
    std::string _contentStatus;
    std::optional<DateTime> _created;
    std::optional<DateTime> _modified;
};

}}}  // namespace Aspose::Cells_FOSS::Core
