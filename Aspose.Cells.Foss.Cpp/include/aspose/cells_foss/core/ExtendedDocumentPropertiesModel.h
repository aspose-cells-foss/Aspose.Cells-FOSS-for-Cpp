#pragma once
#include <optional>
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents extended document properties model.
/// </summary>
class ExtendedDocumentPropertiesModel final {
public:
    /// <summary>
    /// Initializes a new instance of the ExtendedDocumentPropertiesModel class.
    /// </summary>
    ExtendedDocumentPropertiesModel();

    /// <summary>
    /// Gets or sets the application.
    /// </summary>
    const std::string& GetApplication() const noexcept { return _application; }
    void SetApplication(std::string value) { _application = std::move(value); }

    /// <summary>
    /// Gets or sets the app version.
    /// </summary>
    const std::string& GetAppVersion() const noexcept { return _appVersion; }
    void SetAppVersion(std::string value) { _appVersion = std::move(value); }

    /// <summary>
    /// Gets or sets the company.
    /// </summary>
    const std::string& GetCompany() const noexcept { return _company; }
    void SetCompany(std::string value) { _company = std::move(value); }

    /// <summary>
    /// Gets or sets the manager.
    /// </summary>
    const std::string& GetManager() const noexcept { return _manager; }
    void SetManager(std::string value) { _manager = std::move(value); }

    /// <summary>
    /// Gets or sets the doc security.
    /// </summary>
    std::optional<int> GetDocSecurity() const noexcept { return _docSecurity; }
    void SetDocSecurity(std::optional<int> value) noexcept { _docSecurity = value; }

    /// <summary>
    /// Gets or sets the hyperlink base.
    /// </summary>
    const std::string& GetHyperlinkBase() const noexcept { return _hyperlinkBase; }
    void SetHyperlinkBase(std::string value) { _hyperlinkBase = std::move(value); }

    /// <summary>
    /// Gets or sets a value indicating whether scale crop.
    /// </summary>
    std::optional<bool> GetScaleCrop() const noexcept { return _scaleCrop; }
    void SetScaleCrop(std::optional<bool> value) noexcept { _scaleCrop = value; }

    /// <summary>
    /// Gets or sets a value indicating whether links up to date.
    /// </summary>
    std::optional<bool> GetLinksUpToDate() const noexcept { return _linksUpToDate; }
    void SetLinksUpToDate(std::optional<bool> value) noexcept { _linksUpToDate = value; }

    /// <summary>
    /// Gets or sets a value indicating whether shared doc.
    /// </summary>
    std::optional<bool> GetSharedDoc() const noexcept { return _sharedDoc; }
    void SetSharedDoc(std::optional<bool> value) noexcept { _sharedDoc = value; }

    /// <summary>
    /// Copies values from the specified source.
    /// </summary>
    /// <param name="source">The source.</param>
    void CopyFrom(const ExtendedDocumentPropertiesModel& source);

    /// <summary>
    /// Performs has stored state.
    /// </summary>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool HasStoredState() const;

private:
    std::string _application;
    std::string _appVersion;
    std::string _company;
    std::string _manager;
    std::optional<int> _docSecurity;
    std::string _hyperlinkBase;
    std::optional<bool> _scaleCrop;
    std::optional<bool> _linksUpToDate;
    std::optional<bool> _sharedDoc;
};

}}}  // namespace Aspose::Cells_FOSS::Core
