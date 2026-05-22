#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "aspose/cells_foss/core/AutoFilterModel.h"
#include "aspose/cells_foss/core/CellAddress.h"
#include "aspose/cells_foss/core/CellRecord.h"
#include "aspose/cells_foss/core/ColorValue.h"
#include "aspose/cells_foss/core/ColumnRangeModel.h"
#include "aspose/cells_foss/core/ConditionalFormattingModel.h"
#include "aspose/cells_foss/core/HyperlinkModel.h"
#include "aspose/cells_foss/core/MergeRegion.h"
#include "aspose/cells_foss/core/PageSetupModel.h"
#include "aspose/cells_foss/core/RowModel.h"
#include "aspose/cells_foss/core/SheetVisibility.h"
#include "aspose/cells_foss/core/ValidationModel.h"
#include "aspose/cells_foss/core/WorksheetProtectionModel.h"
#include "aspose/cells_foss/core/WorksheetViewModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents worksheet model.
/// </summary>
class WorksheetModel final {
public:
    /// <summary>
    /// Initializes a new instance of the WorksheetModel class.
    /// </summary>
    /// <param name="name">The name.</param>
    explicit WorksheetModel(std::string_view name);

    /// <summary>
    /// Gets or sets the name.
    /// </summary>
    const std::string& GetName() const noexcept { return _name; }
    void SetName(std::string value) { _name = std::move(value); }

    /// <summary>
    /// Gets or sets the visibility.
    /// </summary>
    SheetVisibility GetVisibility() const noexcept { return _visibility; }
    void SetVisibility(SheetVisibility value) noexcept { _visibility = value; }

    /// <summary>
    /// Gets the cells.
    /// </summary>
    const std::unordered_map<CellAddress, CellRecord>& GetCells() const noexcept { return _cells; }
    std::unordered_map<CellAddress, CellRecord>& GetCells() noexcept { return _cells; }

    /// <summary>
    /// Gets the rows.
    /// </summary>
    const std::unordered_map<int, RowModel>& GetRows() const noexcept { return _rows; }
    std::unordered_map<int, RowModel>& GetRows() noexcept { return _rows; }

    /// <summary>
    /// Gets the columns.
    /// </summary>
    const std::vector<ColumnRangeModel>& GetColumns() const noexcept { return _columns; }
    std::vector<ColumnRangeModel>& GetColumns() noexcept { return _columns; }

    /// <summary>
    /// Gets the merge regions.
    /// </summary>
    const std::vector<MergeRegion>& GetMergeRegions() const noexcept { return _mergeRegions; }
    std::vector<MergeRegion>& GetMergeRegions() noexcept { return _mergeRegions; }

    /// <summary>
    /// Gets the hyperlinks.
    /// </summary>
    const std::vector<HyperlinkModel>& GetHyperlinks() const noexcept { return _hyperlinks; }
    std::vector<HyperlinkModel>& GetHyperlinks() noexcept { return _hyperlinks; }

    /// <summary>
    /// Gets the validations.
    /// </summary>
    const std::vector<ValidationModel>& GetValidations() const noexcept { return _validations; }
    std::vector<ValidationModel>& GetValidations() noexcept { return _validations; }

    /// <summary>
    /// Gets the conditional formattings.
    /// </summary>
    const std::vector<ConditionalFormattingModel>& GetConditionalFormattings() const noexcept { return _conditionalFormattings; }
    std::vector<ConditionalFormattingModel>& GetConditionalFormattings() noexcept { return _conditionalFormattings; }

    /// <summary>
    /// Gets the page setup.
    /// </summary>
    const PageSetupModel& GetPageSetup() const noexcept { return _pageSetup; }
    PageSetupModel& GetPageSetup() noexcept { return _pageSetup; }

    /// <summary>
    /// Gets the view.
    /// </summary>
    const WorksheetViewModel& GetView() const noexcept { return _view; }
    WorksheetViewModel& GetView() noexcept { return _view; }

    /// <summary>
    /// Gets the protection.
    /// </summary>
    const WorksheetProtectionModel& GetProtection() const noexcept { return _protection; }
    WorksheetProtectionModel& GetProtection() noexcept { return _protection; }

    /// <summary>
    /// Gets the auto filter.
    /// </summary>
    const AutoFilterModel& GetAutoFilter() const noexcept { return _autoFilter; }
    AutoFilterModel& GetAutoFilter() noexcept { return _autoFilter; }

    /// <summary>
    /// Gets or sets the tab color.
    /// </summary>
    const std::optional<ColorValue>& GetTabColor() const noexcept { return _tabColor; }
    void SetTabColor(std::optional<ColorValue> value) noexcept { _tabColor = value; }

private:
    std::string _name;
    SheetVisibility _visibility = SheetVisibility::Visible;
    std::unordered_map<CellAddress, CellRecord> _cells;
    std::unordered_map<int, RowModel> _rows;
    std::vector<ColumnRangeModel> _columns;
    std::vector<MergeRegion> _mergeRegions;
    std::vector<HyperlinkModel> _hyperlinks;
    std::vector<ValidationModel> _validations;
    std::vector<ConditionalFormattingModel> _conditionalFormattings;
    PageSetupModel _pageSetup;
    WorksheetViewModel _view;
    WorksheetProtectionModel _protection;
    AutoFilterModel _autoFilter;
    std::optional<ColorValue> _tabColor;
};

}}}  // namespace Aspose::Cells_FOSS::Core
