#include "aspose/cells_foss/XlsxWorkbookDefinedNames.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/DefinedNameUtility.h"
#include "aspose/cells_foss/InvalidFileFormatException.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookPageSetup.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"
#include "aspose/cells_foss/core/CellAddress.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <algorithm>
#include <cctype>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

using Core::CellAddress;
using Core::DefinedNameModel;
using Core::WorkbookModel;
using Core::WorksheetModel;

static const XNamespace& MainNs = XlsxWorkbookSerializerCommon::MainNs;

// ---------------------------------------------------------------------------
// Local helpers
// ---------------------------------------------------------------------------

static XmlElement MakeTextNode(std::string text)
{
    auto node = std::make_shared<XmlNodeData>();
    node->isTextNode = true;
    node->text = std::move(text);
    return XmlElement::MakeElement(std::move(node));
}

static std::string ReplaceAll(
    std::string_view input,
    std::string_view from,
    std::string_view to)
{
    if (from.empty()) {
        return std::string(input);
    }
    std::string result;
    result.reserve(input.size());
    std::string::size_type pos = 0;
    while (pos < input.size()) {
        const auto found = input.find(from, pos);
        if (found == std::string_view::npos) {
            result.append(input.data() + pos, input.size() - pos);
            break;
        }
        result.append(input.data() + pos, found - pos);
        result.append(to.data(), to.size());
        pos = found + from.size();
    }
    return result;
}

static bool EqualsIgnoreCase(const std::string& left, std::string_view right)
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

static std::string TrimEdge(std::string value)
{
    const auto start = value.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return {};
    }
    const auto end = value.find_last_not_of(" \t\n\r");
    return value.substr(start, end - start + 1);
}

// ---------------------------------------------------------------------------
// BuildDefinedNames
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookDefinedNames::BuildDefinedNames(const WorkbookModel& model)
{
    std::vector<XmlElement> definedNames;

    auto pageSetupDefinedNames = XlsxWorkbookPageSetup::BuildPageSetupDefinedNames(model);
    if (!pageSetupDefinedNames.IsNull()) {
        auto elements = pageSetupDefinedNames.GetElements(MainNs + "definedName");
        for (auto& element : elements) {
            definedNames.push_back(std::move(element));
        }
    }

    auto autoFilterDefinedNames = BuildAutoFilterDefinedNames(model);
    if (!autoFilterDefinedNames.IsNull()) {
        auto elements = autoFilterDefinedNames.GetElements(MainNs + "definedName");
        for (auto& element : elements) {
            definedNames.push_back(std::move(element));
        }
    }

    const auto& userDefinedNames = model.GetDefinedNames();
    for (std::size_t index = 0; index < userDefinedNames.size(); ++index) {
        const auto& definedName = userDefinedNames[index];

        auto element = XmlElement::MakeElement(
            MainNs + "definedName",
            std::vector<std::pair<std::string, std::string>>{
                {"name", definedName.GetName()}},
            std::vector<XmlElement>{MakeTextNode(definedName.GetFormula())});

        if (definedName.GetLocalSheetIndex().has_value()) {
            element.SetAttribute("localSheetId",
                std::to_string(definedName.GetLocalSheetIndex().value()));
        }

        if (definedName.GetHidden()) {
            element.SetAttribute("hidden", "1");
        }

        if (!definedName.GetComment().empty()) {
            element.SetAttribute("comment", definedName.GetComment());
        }

        definedNames.push_back(std::move(element));
    }

    if (definedNames.empty()) {
        return XmlElement();
    }

    return XmlElement::MakeElement(
        MainNs + "definedNames",
        std::vector<std::pair<std::string, std::string>>{},
        std::move(definedNames));
}

// ---------------------------------------------------------------------------
// LoadWorkbookDefinedNames
// ---------------------------------------------------------------------------

void XlsxWorkbookDefinedNames::LoadWorkbookDefinedNames(
    const XmlElement& workbookRoot,
    WorkbookModel& workbookModel,
    int sheetCount,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options)
{
    auto definedNamesElement = workbookRoot.GetElement(MainNs + "definedNames");
    if (definedNamesElement.IsNull()) {
        return;
    }

    auto elements = definedNamesElement.GetElements(MainNs + "definedName");
    for (const auto& element : elements) {
        std::string name = TrimEdge(element.GetAttribute("name").GetValue());
        if (DefinedNameUtility::IsReservedName(name)) {
            continue;
        }

        if (name.empty()) {
            HandleInvalidDefinedName(options, "Workbook defined name is missing a valid name.");
            AddInvalidDefinedNameIssue(diagnostics, options,
                "Workbook defined name is missing a valid name and was ignored.");
            continue;
        }

        std::optional<int> localSheetIndex;
        const auto localSheetAttribute = element.GetAttribute("localSheetId");
        if (!localSheetAttribute.IsNull()) {
            localSheetIndex = XlsxWorkbookArchiveHelpers::ParseIntAttribute(localSheetAttribute);
            if (!localSheetIndex.has_value() || localSheetIndex.value() < 0 ||
                localSheetIndex.value() >= sheetCount) {
                HandleInvalidDefinedName(options,
                    "Workbook defined name '" + name + "' has an invalid localSheetId.");
                AddInvalidDefinedNameIssue(diagnostics, options,
                    "Workbook defined name '" + name +
                    "' has an invalid localSheetId and was ignored.");
                continue;
            }
        }

        std::string formula;
        try {
            formula = DefinedNameUtility::NormalizeFormula(element.GetValue());
        } catch (const CellsException& exception) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "Workbook defined name '" + name + "' is invalid.", exception);
            }

            AddInvalidDefinedNameIssue(diagnostics, options,
                "Workbook defined name '" + name +
                "' has an invalid formula and was ignored.");
            continue;
        }

        if (ContainsDuplicate(workbookModel.GetDefinedNames(), name, localSheetIndex)) {
            HandleInvalidDefinedName(options,
                "Workbook defined name '" + name + "' is duplicated in the same scope.");
            AddInvalidDefinedNameIssue(diagnostics, options,
                "Workbook defined name '" + name +
                "' duplicates an existing scope and was ignored.");
            continue;
        }

        DefinedNameModel dn;
        dn.SetName(std::move(name));
        dn.SetFormula(std::move(formula));
        dn.SetLocalSheetIndex(localSheetIndex);
        dn.SetHidden(
            XlsxWorkbookArchiveHelpers::ParseBoolAttribute(element.GetAttribute("hidden")));
        dn.SetComment(
            DefinedNameUtility::NormalizeComment(element.GetAttribute("comment").GetValue()));
        workbookModel.GetDefinedNames().push_back(std::move(dn));
    }
}

// ---------------------------------------------------------------------------
// ContainsDuplicate
// ---------------------------------------------------------------------------

bool XlsxWorkbookDefinedNames::ContainsDuplicate(
    const std::vector<DefinedNameModel>& definedNames,
    std::string_view name,
    std::optional<int> localSheetIndex)
{
    for (std::size_t index = 0; index < definedNames.size(); ++index) {
        const auto& existing = definedNames[index];
        if (!EqualsIgnoreCase(existing.GetName(), name)) {
            continue;
        }

        if (DefinedNameUtility::SameScope(existing.GetLocalSheetIndex(), localSheetIndex)) {
            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
// AddInvalidDefinedNameIssue
// ---------------------------------------------------------------------------

void XlsxWorkbookDefinedNames::AddInvalidDefinedNameIssue(
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view message)
{
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options,
        LoadIssue("WB-L002", DiagnosticSeverity::Warning, message, false, true));
}

// ---------------------------------------------------------------------------
// HandleInvalidDefinedName
// ---------------------------------------------------------------------------

void XlsxWorkbookDefinedNames::HandleInvalidDefinedName(
    const LoadOptions& options,
    std::string_view message)
{
    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(message);
    }
}

// ---------------------------------------------------------------------------
// BuildAutoFilterDefinedNames
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookDefinedNames::BuildAutoFilterDefinedNames(
    const WorkbookModel& model)
{
    std::vector<XmlElement> definedNames;
    const auto& worksheets = model.GetWorksheets();
    for (std::size_t sheetIndex = 0; sheetIndex < worksheets.size(); ++sheetIndex) {
        const auto& worksheet = worksheets[sheetIndex];
        if (worksheet.GetAutoFilter().GetRange().empty()) {
            continue;
        }

        auto element = XmlElement::MakeElement(
            MainNs + "definedName",
            std::vector<std::pair<std::string, std::string>>{
                {"name", DefinedNameUtility::FilterDatabaseDefinedName},
                {"localSheetId", std::to_string(sheetIndex)},
                {"hidden", "1"}},
            std::vector<XmlElement>{
                MakeTextNode(
                    QualifyAutoFilterRange(
                        worksheet.GetName(),
                        worksheet.GetAutoFilter().GetRange()))});
        definedNames.push_back(std::move(element));
    }

    if (definedNames.empty()) {
        return XmlElement();
    }

    return XmlElement::MakeElement(
        MainNs + "definedNames",
        std::vector<std::pair<std::string, std::string>>{},
        std::move(definedNames));
}

// ---------------------------------------------------------------------------
// QualifyAutoFilterRange
// ---------------------------------------------------------------------------

std::string XlsxWorkbookDefinedNames::QualifyAutoFilterRange(
    std::string_view sheetName,
    std::string_view range)
{
    // Split on ':'.
    const auto colonPos = range.find(':');
    if (colonPos == std::string_view::npos) {
        return QuoteWorksheetName(sheetName) + "!" +
               ToAbsoluteCellReference(range);
    }

    // Ensure there is only one colon.
    if (range.find(':', colonPos + 1) != std::string_view::npos) {
        throw CellsException("AutoFilter range is invalid.");
    }

    return QuoteWorksheetName(sheetName) + "!" +
           ToAbsoluteCellReference(range.substr(0, colonPos)) + ":" +
           ToAbsoluteCellReference(range.substr(colonPos + 1));
}

// ---------------------------------------------------------------------------
// QuoteWorksheetName
// ---------------------------------------------------------------------------

std::string XlsxWorkbookDefinedNames::QuoteWorksheetName(std::string_view sheetName)
{
    return "'" + ReplaceAll(sheetName, "'", "''") + "'";
}

// ---------------------------------------------------------------------------
// ToAbsoluteCellReference
// ---------------------------------------------------------------------------

std::string XlsxWorkbookDefinedNames::ToAbsoluteCellReference(std::string_view value)
{
    CellAddress address;
    try {
        address = CellAddress::Parse(value);
    } catch (const std::invalid_argument& exception) {
        throw CellsException("AutoFilter range is invalid.", exception);
    }

    const std::string reference = address.ToString();
    std::size_t splitIndex = 0;
    while (splitIndex < reference.size() &&
           std::isalpha(static_cast<unsigned char>(reference[splitIndex]))) {
        ++splitIndex;
    }

    return "$" + reference.substr(0, splitIndex) + "$" + reference.substr(splitIndex);
}

}}  // namespace Aspose::Cells_FOSS
