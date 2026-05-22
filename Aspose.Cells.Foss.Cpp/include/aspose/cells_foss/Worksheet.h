#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "aspose/cells_foss/AutoFilter.h"
#include "aspose/cells_foss/Cells.h"
#include "aspose/cells_foss/Color.h"
#include "aspose/cells_foss/ConditionalFormattingCollection.h"
#include "aspose/cells_foss/HyperlinkCollection.h"
#include "aspose/cells_foss/PageSetup.h"
#include "aspose/cells_foss/ValidationCollection.h"
#include "aspose/cells_foss/VisibilityType.h"
#include "aspose/cells_foss/WorksheetProtection.h"

namespace Aspose {
namespace Cells_FOSS {

namespace Core {
class WorksheetModel;
}  // namespace Core

class Workbook;

/// <summary>
/// Encapsulates a single worksheet and its supported v0.1 worksheet features.
/// </summary>
class Worksheet final {
public:
    /// <summary>
    /// Constructs a Worksheet wrapping the specified model within the given workbook.
    /// </summary>
    /// <param name="workbook">The parent workbook.</param>
    /// <param name="model">The underlying worksheet model.</param>
    Worksheet(Workbook& workbook, Core::WorksheetModel& model);

    ~Worksheet();

    Worksheet(const Worksheet&) = delete;
    Worksheet& operator=(const Worksheet&) = delete;

    // -----------------------------------------------------------------------
    // Internal accessors
    // -----------------------------------------------------------------------

    /// Gets the underlying worksheet model.
    Core::WorksheetModel& GetModel();
    const Core::WorksheetModel& GetModel() const;

    /// Gets the parent workbook.
    Workbook& GetWorkbook();
    const Workbook& GetWorkbook() const;

    // -----------------------------------------------------------------------
    // Public properties
    // -----------------------------------------------------------------------

    /// <summary>
    /// Gets or sets the worksheet name.
    /// </summary>
    std::string GetName() const;
    void SetName(std::string_view value);

    /// <summary>
    /// Gets or sets the worksheet visibility state.
    /// </summary>
    VisibilityType GetVisibilityType() const;
    void SetVisibilityType(VisibilityType value);

    /// <summary>
    /// Gets or sets the worksheet tab color.
    /// </summary>
    Color GetTabColor() const;
    void SetTabColor(const Color& value);

    /// <summary>
    /// Gets or sets whether gridlines are shown in the worksheet view.
    /// </summary>
    bool GetShowGridlines() const;
    void SetShowGridlines(bool value);

    /// <summary>
    /// Gets or sets whether row and column headers are shown in the worksheet view.
    /// </summary>
    bool GetShowRowColumnHeaders() const;
    void SetShowRowColumnHeaders(bool value);

    /// <summary>
    /// Gets or sets whether zero values are shown in the worksheet view.
    /// </summary>
    bool GetShowZeros() const;
    void SetShowZeros(bool value);

    /// <summary>
    /// Gets or sets whether the worksheet view is right-to-left.
    /// </summary>
    bool GetRightToLeft() const;
    void SetRightToLeft(bool value);

    /// <summary>
    /// Gets or sets the worksheet zoom percentage.
    /// </summary>
    int GetZoom() const;
    void SetZoom(int value);

    /// <summary>
    /// Gets the cell grid facade for the worksheet.
    /// </summary>
    Cells& GetCells();
    const Cells& GetCells() const;

    /// <summary>
    /// Gets the worksheet hyperlink collection.
    /// </summary>
    HyperlinkCollection& GetHyperlinks();
    const HyperlinkCollection& GetHyperlinks() const;

    /// <summary>
    /// Gets the worksheet data validation collection.
    /// </summary>
    ValidationCollection& GetValidations();
    const ValidationCollection& GetValidations() const;

    /// <summary>
    /// Gets the worksheet conditional formatting collection.
    /// </summary>
    ConditionalFormattingCollection& GetConditionalFormattings();
    const ConditionalFormattingCollection& GetConditionalFormattings() const;

    /// <summary>
    /// Gets page setup settings for the worksheet.
    /// </summary>
    PageSetup& GetPageSetup();
    const PageSetup& GetPageSetup() const;

    /// <summary>
    /// Gets worksheet protection settings.
    /// </summary>
    WorksheetProtection& GetProtection();
    const WorksheetProtection& GetProtection() const;

    /// <summary>
    /// Gets auto-filter settings for the worksheet.
    /// </summary>
    AutoFilter& GetAutoFilter();
    const AutoFilter& GetAutoFilter() const;

    // -----------------------------------------------------------------------
    // Public methods
    // -----------------------------------------------------------------------

    /// <summary>
    /// Marks the worksheet as protected using the current protection settings.
    /// </summary>
    void Protect();

    /// <summary>
    /// Clears worksheet protection and resets supported protection flags.
    /// </summary>
    void Unprotect();

private:
    Workbook& _workbook;
    Core::WorksheetModel& _model;
    Cells _cells;
    HyperlinkCollection _hyperlinks;
    ValidationCollection _validations;
    ConditionalFormattingCollection _conditionalFormattings;
    PageSetup _pageSetup;
    WorksheetProtection _protection;
    AutoFilter _autoFilter;
};

}}  // namespace Aspose::Cells_FOSS
