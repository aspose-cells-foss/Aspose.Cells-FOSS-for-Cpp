#include "aspose/cells_foss/WorksheetCollection.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/Workbook.h"
#include "aspose/cells_foss/core/WorkbookModel.h"
#include "aspose/cells_foss/core/WorkbookViewModel.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <utility>

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

bool EqualsIgnoreCase(const std::string& left, std::string_view right)
{
    if (left.size() != right.size()) {
        return false;
    }
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(left[i])) !=
            std::tolower(static_cast<unsigned char>(right[i]))) {
            return false;
        }
    }
    return true;
}

}  // namespace

// ---------------------------------------------------------------------------
// Construction / destruction
// ---------------------------------------------------------------------------

WorksheetCollection::WorksheetCollection(Workbook& workbook,
                                         Core::WorkbookModel& model)
    : _workbook(workbook)
    , _model(model)
{
    RebuildWrappers();
}

WorksheetCollection::~WorksheetCollection() = default;

// ---------------------------------------------------------------------------
// Indexers
// ---------------------------------------------------------------------------

Worksheet& WorksheetCollection::operator[](int index)
{
    if (index < 0 || index >= static_cast<int>(_worksheets.size())) {
        throw CellsException("Worksheet index was out of range.");
    }

    return *_worksheets[static_cast<std::size_t>(index)];
}

Worksheet& WorksheetCollection::operator[](std::string_view name)
{
    const int index = IndexOf(name);
    if (index < 0) {
        throw CellsException(
            std::string("Worksheet '") + std::string(name) + "' was not found.");
    }

    return *_worksheets[static_cast<std::size_t>(index)];
}

// ---------------------------------------------------------------------------
// Count
// ---------------------------------------------------------------------------

int WorksheetCollection::GetCount() const
{
    return static_cast<int>(_model.GetWorksheets().size());
}

// ---------------------------------------------------------------------------
// ActiveSheetIndex
// ---------------------------------------------------------------------------

int WorksheetCollection::GetActiveSheetIndex() const
{
    return _model.GetActiveSheetIndex();
}

void WorksheetCollection::SetActiveSheetIndex(int value)
{
    if (value < 0 || value >= static_cast<int>(_model.GetWorksheets().size())) {
        throw CellsException(
            "ActiveSheetIndex must refer to an existing worksheet.");
    }

    _model.SetActiveSheetIndex(value);
}

// ---------------------------------------------------------------------------
// ActiveSheetName
// ---------------------------------------------------------------------------

std::string WorksheetCollection::GetActiveSheetName() const
{
    const int activeIndex = _model.GetActiveSheetIndex();
    return _model.GetWorksheets()[static_cast<std::size_t>(activeIndex)].GetName();
}

void WorksheetCollection::SetActiveSheetName(std::string_view value)
{
    const int index = IndexOf(value);
    if (index < 0) {
        throw CellsException(
            std::string("Worksheet '") + std::string(value) + "' was not found.");
    }

    SetActiveSheetIndex(index);
}

// ---------------------------------------------------------------------------
// Add
// ---------------------------------------------------------------------------

int WorksheetCollection::Add()
{
    return Add(GenerateDefaultSheetName());
}

int WorksheetCollection::Add(std::string_view sheetName)
{
    if (IsNullOrWhiteSpace(sheetName)) {
        throw CellsException("Worksheet name must be non-empty.");
    }

    _workbook.EnsureUniqueSheetName(sheetName);
    _model.GetWorksheets().emplace_back(sheetName);
    _worksheets.push_back(
        std::make_unique<Worksheet>(_workbook, _model.GetWorksheets().back()));
    return static_cast<int>(_model.GetWorksheets().size()) - 1;
}

// ---------------------------------------------------------------------------
// RemoveAt
// ---------------------------------------------------------------------------

void WorksheetCollection::RemoveAt(std::string_view sheetName)
{
    const int index = IndexOf(sheetName);
    if (index < 0) {
        throw CellsException(
            std::string("Worksheet '") + std::string(sheetName) +
            "' was not found.");
    }

    RemoveAt(index);
}

void WorksheetCollection::RemoveAt(int index)
{
    auto& worksheets = _model.GetWorksheets();

    if (static_cast<int>(worksheets.size()) == 1) {
        throw CellsException("A workbook must contain at least one worksheet.");
    }

    worksheets.erase(worksheets.begin() + index);

    int activeIndex = _model.GetActiveSheetIndex();
    if (activeIndex > index) {
        _model.SetActiveSheetIndex(activeIndex - 1);
    } else if (activeIndex >= static_cast<int>(worksheets.size())) {
        _model.SetActiveSheetIndex(static_cast<int>(worksheets.size()) - 1);
    }

    auto& view = _model.GetProperties().GetView();
    auto firstSheet = view.GetFirstSheet();
    if (firstSheet.has_value()) {
        if (static_cast<int>(worksheets.size()) == 0) {
            view.SetFirstSheet(0);
        } else if (firstSheet.value() >= static_cast<int>(worksheets.size())) {
            view.SetFirstSheet(static_cast<int>(worksheets.size()) - 1);
        }
    }

    RebuildWrappers();
}

// ---------------------------------------------------------------------------
// Iteration
// ---------------------------------------------------------------------------

std::vector<std::unique_ptr<Worksheet>>::const_iterator
WorksheetCollection::begin() const
{
    return _worksheets.begin();
}

std::vector<std::unique_ptr<Worksheet>>::const_iterator
WorksheetCollection::end() const
{
    return _worksheets.end();
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

void WorksheetCollection::RebuildWrappers()
{
    _worksheets.clear();
    auto& models = _model.GetWorksheets();
    _worksheets.reserve(models.size());
    for (auto& wsModel : models) {
        _worksheets.push_back(
            std::make_unique<Worksheet>(_workbook, wsModel));
    }
}

int WorksheetCollection::IndexOf(std::string_view name) const
{
    const auto& worksheets = _model.GetWorksheets();
    for (std::size_t i = 0; i < worksheets.size(); ++i) {
        if (EqualsIgnoreCase(worksheets[i].GetName(), name)) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

std::string WorksheetCollection::GenerateDefaultSheetName() const
{
    int suffix = 1;
    while (true) {
        std::string candidate = "Sheet" + std::to_string(suffix);
        if (IndexOf(candidate) < 0) {
            return candidate;
        }
        ++suffix;
    }
}

}}  // namespace Aspose::Cells_FOSS
