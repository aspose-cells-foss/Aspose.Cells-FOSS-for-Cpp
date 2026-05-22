#pragma once

#include <string>
#include <vector>

#include "aspose/cells_foss/core/HyperlinkModel.h"
#include "aspose/cells_foss/TargetModeType.h"

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents hyperlink.
/// </summary>
class Hyperlink final {
public:
    /// <summary>
    /// Initializes a new instance of the Hyperlink class.
    /// </summary>
    /// <param name="owner">The owner list of hyperlink models.</param>
    /// <param name="index">The zero-based index into the owner list.</param>
    Hyperlink(std::vector<Core::HyperlinkModel>& owner, int index);

    /// <summary>
    /// Gets the area.
    /// </summary>
    std::string GetArea() const;

    /// <summary>
    /// Gets or sets the address.
    /// </summary>
    std::string GetAddress() const;
    void SetAddress(const std::string& value);

    /// <summary>
    /// Gets the link type.
    /// </summary>
    TargetModeType GetLinkType() const;

    /// <summary>
    /// Gets or sets the screen tip.
    /// </summary>
    std::string GetScreenTip() const;
    void SetScreenTip(const std::string& value);

    /// <summary>
    /// Gets or sets the text to display.
    /// </summary>
    std::string GetTextToDisplay() const;
    void SetTextToDisplay(const std::string& value);

    /// <summary>
    /// Performs delete.
    /// </summary>
    void Delete();

private:
    std::vector<Core::HyperlinkModel>& _owner;
    int _index;

    Core::HyperlinkModel& Model();
    const Core::HyperlinkModel& Model() const;
};

}}  // namespace Aspose::Cells_FOSS
