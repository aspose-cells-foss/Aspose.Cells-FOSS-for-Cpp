#pragma once
#include <string>

#include "aspose/cells_foss/CoreDocumentProperties.h"
#include "aspose/cells_foss/ExtendedDocumentProperties.h"
#include "aspose/cells_foss/core/DocumentPropertiesModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents document properties.
/// </summary>
class DocumentProperties final {
public:
    /// <summary>
    /// Initializes a new instance of the DocumentProperties class.
    /// </summary>
    /// <param name="model">The document properties model.</param>
    explicit DocumentProperties(Core::DocumentPropertiesModel& model);

    DocumentProperties(const DocumentProperties&) = delete;
    DocumentProperties& operator=(const DocumentProperties&) = delete;
    DocumentProperties(DocumentProperties&&) = delete;
    DocumentProperties& operator=(DocumentProperties&&) = delete;

    /// <summary>
    /// Gets the core.
    /// </summary>
    CoreDocumentProperties& GetCore() noexcept { return _core; }
    const CoreDocumentProperties& GetCore() const noexcept { return _core; }

    /// <summary>
    /// Gets the extended.
    /// </summary>
    ExtendedDocumentProperties& GetExtended() noexcept { return _extended; }
    const ExtendedDocumentProperties& GetExtended() const noexcept { return _extended; }

    /// <summary>
    /// Gets or sets the title.
    /// </summary>
    const std::string& GetTitle() const noexcept { return _model.GetCore().GetTitle(); }
    void SetTitle(std::string value) { _model.GetCore().SetTitle(std::move(value)); }

    /// <summary>
    /// Gets or sets the subject.
    /// </summary>
    const std::string& GetSubject() const noexcept { return _model.GetCore().GetSubject(); }
    void SetSubject(std::string value) { _model.GetCore().SetSubject(std::move(value)); }

    /// <summary>
    /// Gets or sets the author.
    /// </summary>
    const std::string& GetAuthor() const noexcept { return _model.GetCore().GetCreator(); }
    void SetAuthor(std::string value) { _model.GetCore().SetCreator(std::move(value)); }

    /// <summary>
    /// Gets or sets the keywords.
    /// </summary>
    const std::string& GetKeywords() const noexcept { return _model.GetCore().GetKeywords(); }
    void SetKeywords(std::string value) { _model.GetCore().SetKeywords(std::move(value)); }

    /// <summary>
    /// Gets or sets the comments.
    /// </summary>
    const std::string& GetComments() const noexcept { return _model.GetCore().GetDescription(); }
    void SetComments(std::string value) { _model.GetCore().SetDescription(std::move(value)); }

    /// <summary>
    /// Gets or sets the category.
    /// </summary>
    const std::string& GetCategory() const noexcept { return _model.GetCore().GetCategory(); }
    void SetCategory(std::string value) { _model.GetCore().SetCategory(std::move(value)); }

    /// <summary>
    /// Gets or sets the company.
    /// </summary>
    const std::string& GetCompany() const noexcept { return _model.GetExtended().GetCompany(); }
    void SetCompany(std::string value) { _model.GetExtended().SetCompany(std::move(value)); }

    /// <summary>
    /// Gets or sets the manager.
    /// </summary>
    const std::string& GetManager() const noexcept { return _model.GetExtended().GetManager(); }
    void SetManager(std::string value) { _model.GetExtended().SetManager(std::move(value)); }

private:
    Core::DocumentPropertiesModel& _model;
    CoreDocumentProperties _core;
    ExtendedDocumentProperties _extended;
};

}}  // namespace Aspose::Cells_FOSS
