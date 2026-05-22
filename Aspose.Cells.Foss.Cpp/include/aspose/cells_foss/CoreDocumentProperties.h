#pragma once
#include <optional>
#include <string>

#include "aspose/cells_foss/DateTime.h"
#include "aspose/cells_foss/core/CoreDocumentPropertiesModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents core document properties.
/// </summary>
class CoreDocumentProperties final {
public:
    /// <summary>
    /// Initializes a new instance of the CoreDocumentProperties class.
    /// </summary>
    /// <param name="model">The core document properties model.</param>
    explicit CoreDocumentProperties(Core::CoreDocumentPropertiesModel& model);

    /// <summary>
    /// Gets or sets the title.
    /// </summary>
    const std::string& GetTitle() const noexcept { return _model.GetTitle(); }
    void SetTitle(std::string value) { _model.SetTitle(std::move(value)); }

    /// <summary>
    /// Gets or sets the subject.
    /// </summary>
    const std::string& GetSubject() const noexcept { return _model.GetSubject(); }
    void SetSubject(std::string value) { _model.SetSubject(std::move(value)); }

    /// <summary>
    /// Gets or sets the creator.
    /// </summary>
    const std::string& GetCreator() const noexcept { return _model.GetCreator(); }
    void SetCreator(std::string value) { _model.SetCreator(std::move(value)); }

    /// <summary>
    /// Gets or sets the keywords.
    /// </summary>
    const std::string& GetKeywords() const noexcept { return _model.GetKeywords(); }
    void SetKeywords(std::string value) { _model.SetKeywords(std::move(value)); }

    /// <summary>
    /// Gets or sets the description.
    /// </summary>
    const std::string& GetDescription() const noexcept { return _model.GetDescription(); }
    void SetDescription(std::string value) { _model.SetDescription(std::move(value)); }

    /// <summary>
    /// Gets or sets the last modified by.
    /// </summary>
    const std::string& GetLastModifiedBy() const noexcept { return _model.GetLastModifiedBy(); }
    void SetLastModifiedBy(std::string value) { _model.SetLastModifiedBy(std::move(value)); }

    /// <summary>
    /// Gets or sets the revision.
    /// </summary>
    const std::string& GetRevision() const noexcept { return _model.GetRevision(); }
    void SetRevision(std::string value) { _model.SetRevision(std::move(value)); }

    /// <summary>
    /// Gets or sets the category.
    /// </summary>
    const std::string& GetCategory() const noexcept { return _model.GetCategory(); }
    void SetCategory(std::string value) { _model.SetCategory(std::move(value)); }

    /// <summary>
    /// Gets or sets the content status.
    /// </summary>
    const std::string& GetContentStatus() const noexcept { return _model.GetContentStatus(); }
    void SetContentStatus(std::string value) { _model.SetContentStatus(std::move(value)); }

    /// <summary>
    /// Gets or sets the created.
    /// </summary>
    std::optional<DateTime> GetCreated() const noexcept { return _model.GetCreated(); }
    void SetCreated(std::optional<DateTime> value) noexcept { _model.SetCreated(value); }

    /// <summary>
    /// Gets or sets the modified.
    /// </summary>
    std::optional<DateTime> GetModified() const noexcept { return _model.GetModified(); }
    void SetModified(std::optional<DateTime> value) noexcept { _model.SetModified(value); }

    Core::CoreDocumentPropertiesModel& GetModel() noexcept { return _model; }
    const Core::CoreDocumentPropertiesModel& GetModel() const noexcept { return _model; }

private:
    Core::CoreDocumentPropertiesModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
