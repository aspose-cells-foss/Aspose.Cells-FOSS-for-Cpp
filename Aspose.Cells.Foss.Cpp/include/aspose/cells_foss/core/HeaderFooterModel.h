#pragma once
#include <string>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents header footer model.
/// </summary>
class HeaderFooterModel final {
public:
    /// <summary>
    /// Initializes a new instance of the HeaderFooterModel class.
    /// </summary>
    HeaderFooterModel();

    /// <summary>
    /// Gets or sets the left header.
    /// </summary>
    const std::string& GetLeftHeader() const noexcept { return _leftHeader; }
    void SetLeftHeader(std::string value) { _leftHeader = std::move(value); }

    /// <summary>
    /// Gets or sets the center header.
    /// </summary>
    const std::string& GetCenterHeader() const noexcept { return _centerHeader; }
    void SetCenterHeader(std::string value) { _centerHeader = std::move(value); }

    /// <summary>
    /// Gets or sets the right header.
    /// </summary>
    const std::string& GetRightHeader() const noexcept { return _rightHeader; }
    void SetRightHeader(std::string value) { _rightHeader = std::move(value); }

    /// <summary>
    /// Gets or sets the left footer.
    /// </summary>
    const std::string& GetLeftFooter() const noexcept { return _leftFooter; }
    void SetLeftFooter(std::string value) { _leftFooter = std::move(value); }

    /// <summary>
    /// Gets or sets the center footer.
    /// </summary>
    const std::string& GetCenterFooter() const noexcept { return _centerFooter; }
    void SetCenterFooter(std::string value) { _centerFooter = std::move(value); }

    /// <summary>
    /// Gets or sets the right footer.
    /// </summary>
    const std::string& GetRightFooter() const noexcept { return _rightFooter; }
    void SetRightFooter(std::string value) { _rightFooter = std::move(value); }

private:
    std::string _leftHeader;
    std::string _centerHeader;
    std::string _rightHeader;
    std::string _leftFooter;
    std::string _centerFooter;
    std::string _rightFooter;
};

}}}  // namespace Aspose::Cells_FOSS::Core
