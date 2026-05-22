#pragma once
#include <string>

#include "aspose/cells_foss/core/ExtendedDocumentPropertiesModel.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents extended document properties.
/// </summary>
class ExtendedDocumentProperties final {
public:
    /// <summary>
    /// Initializes a new instance of the ExtendedDocumentProperties class.
    /// </summary>
    /// <param name="model">The extended document properties model.</param>
    explicit ExtendedDocumentProperties(Core::ExtendedDocumentPropertiesModel& model);

    /// <summary>
    /// Gets or sets the application.
    /// </summary>
    const std::string& GetApplication() const noexcept { return _model.GetApplication(); }
    void SetApplication(std::string value) { _model.SetApplication(std::move(value)); }

    /// <summary>
    /// Gets or sets the app version.
    /// </summary>
    const std::string& GetAppVersion() const noexcept { return _model.GetAppVersion(); }
    void SetAppVersion(std::string value) { _model.SetAppVersion(std::move(value)); }

    /// <summary>
    /// Gets or sets the company.
    /// </summary>
    const std::string& GetCompany() const noexcept { return _model.GetCompany(); }
    void SetCompany(std::string value) { _model.SetCompany(std::move(value)); }

    /// <summary>
    /// Gets or sets the manager.
    /// </summary>
    const std::string& GetManager() const noexcept { return _model.GetManager(); }
    void SetManager(std::string value) { _model.SetManager(std::move(value)); }

    /// <summary>
    /// Gets or sets the doc security.
    /// </summary>
    int GetDocSecurity() const noexcept { return _model.GetDocSecurity().value_or(0); }
    void SetDocSecurity(int value);

    /// <summary>
    /// Gets or sets the hyperlink base.
    /// </summary>
    const std::string& GetHyperlinkBase() const noexcept { return _model.GetHyperlinkBase(); }
    void SetHyperlinkBase(std::string value) { _model.SetHyperlinkBase(std::move(value)); }

    /// <summary>
    /// Gets or sets a value indicating whether scale crop.
    /// </summary>
    bool GetScaleCrop() const noexcept { return _model.GetScaleCrop().value_or(false); }
    void SetScaleCrop(bool value) noexcept { _model.SetScaleCrop(value); }

    /// <summary>
    /// Gets or sets a value indicating whether links up to date.
    /// </summary>
    bool GetLinksUpToDate() const noexcept { return _model.GetLinksUpToDate().value_or(false); }
    void SetLinksUpToDate(bool value) noexcept { _model.SetLinksUpToDate(value); }

    /// <summary>
    /// Gets or sets a value indicating whether shared doc.
    /// </summary>
    bool GetSharedDoc() const noexcept { return _model.GetSharedDoc().value_or(false); }
    void SetSharedDoc(bool value) noexcept { _model.SetSharedDoc(value); }

    Core::ExtendedDocumentPropertiesModel& GetModel() noexcept { return _model; }
    const Core::ExtendedDocumentPropertiesModel& GetModel() const noexcept { return _model; }

private:
    Core::ExtendedDocumentPropertiesModel& _model;
};

}}  // namespace Aspose::Cells_FOSS
