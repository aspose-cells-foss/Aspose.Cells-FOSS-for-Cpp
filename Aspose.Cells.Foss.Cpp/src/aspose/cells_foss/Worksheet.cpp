#include "aspose/cells_foss/Worksheet.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/Color.h"
#include "aspose/cells_foss/VisibilityType.h"
#include "aspose/cells_foss/Workbook.h"
#include "aspose/cells_foss/core/ColorValue.h"
#include "aspose/cells_foss/core/SheetVisibility.h"
#include "aspose/cells_foss/core/WorksheetModel.h"
#include "aspose/cells_foss/core/WorksheetViewModel.h"
#include "aspose/cells_foss/core/WorksheetProtectionModel.h"

#include <cctype>
#include <optional>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

namespace {

bool IsNullOrWhiteSpace(std::string_view s)
{
    for (char c : s) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

}  // namespace

Worksheet::Worksheet(Workbook& workbook, Core::WorksheetModel& model)
    : _workbook(workbook)
    , _model(model)
    , _cells(*this)
    , _hyperlinks(model.GetHyperlinks())
    , _validations(model.GetValidations())
    , _conditionalFormattings(model.GetConditionalFormattings())
    , _pageSetup(model.GetPageSetup())
    , _protection(model.GetProtection())
    , _autoFilter(model.GetAutoFilter())
{
}

Worksheet::~Worksheet() = default;

// -----------------------------------------------------------------------
// Internal accessors
// -----------------------------------------------------------------------

Core::WorksheetModel& Worksheet::GetModel()
{
    return _model;
}

const Core::WorksheetModel& Worksheet::GetModel() const
{
    return _model;
}

Workbook& Worksheet::GetWorkbook()
{
    return _workbook;
}

const Workbook& Worksheet::GetWorkbook() const
{
    return _workbook;
}

// -----------------------------------------------------------------------
// Name
// -----------------------------------------------------------------------

std::string Worksheet::GetName() const
{
    return _model.GetName();
}

void Worksheet::SetName(std::string_view value)
{
    if (IsNullOrWhiteSpace(value)) {
        throw CellsException("Worksheet name must be non-empty.");
    }

    _workbook.EnsureUniqueSheetName(value, std::cref(_model));
    _model.SetName(std::string(value));
}

// -----------------------------------------------------------------------
// VisibilityType
// -----------------------------------------------------------------------

VisibilityType Worksheet::GetVisibilityType() const
{
    switch (_model.GetVisibility()) {
    case Core::SheetVisibility::Hidden:
        return VisibilityType::Hidden;
    case Core::SheetVisibility::VeryHidden:
        return VisibilityType::VeryHidden;
    default:
        return VisibilityType::Visible;
    }
}

void Worksheet::SetVisibilityType(VisibilityType value)
{
    switch (value) {
    case VisibilityType::Hidden:
        _model.SetVisibility(Core::SheetVisibility::Hidden);
        break;
    case VisibilityType::VeryHidden:
        _model.SetVisibility(Core::SheetVisibility::VeryHidden);
        break;
    default:
        _model.SetVisibility(Core::SheetVisibility::Visible);
        break;
    }
}

// -----------------------------------------------------------------------
// TabColor
// -----------------------------------------------------------------------

Color Worksheet::GetTabColor() const
{
    if (_model.GetTabColor().has_value()) {
        return Color::FromCore(_model.GetTabColor().value());
    }

    return Color::Empty();
}

void Worksheet::SetTabColor(const Color& value)
{
    if (value == Color::Empty()) {
        _model.SetTabColor(std::nullopt);
        return;
    }

    _model.SetTabColor(value.ToCore());
}

// -----------------------------------------------------------------------
// ShowGridlines
// -----------------------------------------------------------------------

bool Worksheet::GetShowGridlines() const
{
    return _model.GetView().GetShowGridLines();
}

void Worksheet::SetShowGridlines(bool value)
{
    _model.GetView().SetShowGridLines(value);
}

// -----------------------------------------------------------------------
// ShowRowColumnHeaders
// -----------------------------------------------------------------------

bool Worksheet::GetShowRowColumnHeaders() const
{
    return _model.GetView().GetShowRowColumnHeaders();
}

void Worksheet::SetShowRowColumnHeaders(bool value)
{
    _model.GetView().SetShowRowColumnHeaders(value);
}

// -----------------------------------------------------------------------
// ShowZeros
// -----------------------------------------------------------------------

bool Worksheet::GetShowZeros() const
{
    return _model.GetView().GetShowZeros();
}

void Worksheet::SetShowZeros(bool value)
{
    _model.GetView().SetShowZeros(value);
}

// -----------------------------------------------------------------------
// RightToLeft
// -----------------------------------------------------------------------

bool Worksheet::GetRightToLeft() const
{
    return _model.GetView().GetRightToLeft();
}

void Worksheet::SetRightToLeft(bool value)
{
    _model.GetView().SetRightToLeft(value);
}

// -----------------------------------------------------------------------
// Zoom
// -----------------------------------------------------------------------

int Worksheet::GetZoom() const
{
    return _model.GetView().GetZoomScale();
}

void Worksheet::SetZoom(int value)
{
    if (value < 10 || value > 400) {
        throw CellsException("Zoom must be between 10 and 400.");
    }

    _model.GetView().SetZoomScale(value);
}

// -----------------------------------------------------------------------
// Cells
// -----------------------------------------------------------------------

Cells& Worksheet::GetCells()
{
    return _cells;
}

const Cells& Worksheet::GetCells() const
{
    return _cells;
}

// -----------------------------------------------------------------------
// Hyperlinks
// -----------------------------------------------------------------------

HyperlinkCollection& Worksheet::GetHyperlinks()
{
    return _hyperlinks;
}

const HyperlinkCollection& Worksheet::GetHyperlinks() const
{
    return _hyperlinks;
}

// -----------------------------------------------------------------------
// Validations
// -----------------------------------------------------------------------

ValidationCollection& Worksheet::GetValidations()
{
    return _validations;
}

const ValidationCollection& Worksheet::GetValidations() const
{
    return _validations;
}

// -----------------------------------------------------------------------
// ConditionalFormattings
// -----------------------------------------------------------------------

ConditionalFormattingCollection& Worksheet::GetConditionalFormattings()
{
    return _conditionalFormattings;
}

const ConditionalFormattingCollection& Worksheet::GetConditionalFormattings() const
{
    return _conditionalFormattings;
}

// -----------------------------------------------------------------------
// PageSetup
// -----------------------------------------------------------------------

PageSetup& Worksheet::GetPageSetup()
{
    return _pageSetup;
}

const PageSetup& Worksheet::GetPageSetup() const
{
    return _pageSetup;
}

// -----------------------------------------------------------------------
// Protection
// -----------------------------------------------------------------------

WorksheetProtection& Worksheet::GetProtection()
{
    return _protection;
}

const WorksheetProtection& Worksheet::GetProtection() const
{
    return _protection;
}

// -----------------------------------------------------------------------
// AutoFilter
// -----------------------------------------------------------------------

AutoFilter& Worksheet::GetAutoFilter()
{
    return _autoFilter;
}

const AutoFilter& Worksheet::GetAutoFilter() const
{
    return _autoFilter;
}

// -----------------------------------------------------------------------
// Protect / Unprotect
// -----------------------------------------------------------------------

void Worksheet::Protect()
{
    _model.GetProtection().SetIsProtected(true);
}

void Worksheet::Unprotect()
{
    _protection.Reset();
}

}}  // namespace Aspose::Cells_FOSS
