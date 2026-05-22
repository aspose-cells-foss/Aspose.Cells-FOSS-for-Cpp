#include "aspose/cells_foss/XlsxWorkbookPageSetup.h"
#include "aspose/cells_foss/CellsException.h"
#include "aspose/cells_foss/DefinedNameUtility.h"
#include "aspose/cells_foss/InvalidFileFormatException.h"
#include "aspose/cells_foss/XlsxWorkbookArchiveHelpers.h"
#include "aspose/cells_foss/XlsxWorkbookSerializerCommon.h"
#include "aspose/cells_foss/core/CellAddress.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdio>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

using Core::CellAddress;
using Core::HeaderFooterModel;
using Core::PageMarginsModel;
using Core::PageOrientation;
using Core::PageSetupModel;
using Core::PrintOptionsModel;
using Core::WorkbookModel;
using Core::WorksheetModel;

static const XNamespace& MainNs = XlsxWorkbookSerializerCommon::MainNs;

// ---------------------------------------------------------------------------
// Text-node helper
// ---------------------------------------------------------------------------

static XmlElement MakeTextNode(std::string text)
{
    auto node = std::make_shared<XmlNodeData>();
    node->isTextNode = true;
    node->text = std::move(text);
    return XmlElement::MakeElement(std::move(node));
}

// ---------------------------------------------------------------------------
// String helpers
// ---------------------------------------------------------------------------

std::string XlsxWorkbookPageSetup::DoubleToRoundTripString(double value)
{
    char buf[64];
    const auto result = std::to_chars(buf, buf + sizeof(buf) - 1, value);
    std::string s;
    if (result.ec == std::errc()) {
        s.assign(buf, result.ptr);
    } else {
        std::snprintf(buf, sizeof(buf), "%.17g", value);
        s.assign(buf);
    }
    for (char& c : s) {
        if (c == 'e') {
            c = 'E';
        }
    }
    return s;
}

std::string XlsxWorkbookPageSetup::ReplaceAll(
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

std::string XlsxWorkbookPageSetup::ToUpperInvariant(std::string_view value)
{
    std::string result(value);
    for (auto& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}

// ---------------------------------------------------------------------------
// BuildPageSetupDefinedNames
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookPageSetup::BuildPageSetupDefinedNames(const WorkbookModel& model)
{
    std::vector<XmlElement> definedNames;
    const auto& worksheets = model.GetWorksheets();
    for (std::size_t sheetIndex = 0; sheetIndex < worksheets.size(); ++sheetIndex) {
        const auto& worksheet = worksheets[sheetIndex];
        const auto& pageSetup = worksheet.GetPageSetup();

        auto printArea = NormalizePrintAreaList(
            pageSetup.GetPrintArea(), worksheet.GetName());
        if (!printArea.empty()) {
            auto dn = XmlElement::MakeElement(
                MainNs + "definedName",
                std::vector<std::pair<std::string, std::string>>{
                    {"name", DefinedNameUtility::PrintAreaDefinedName},
                    {"localSheetId", std::to_string(sheetIndex)}},
                std::vector<XmlElement>{});
            dn.AddChild(MakeTextNode(std::move(printArea)));
            definedNames.push_back(std::move(dn));
        }

        auto printTitles = BuildPrintTitlesDefinedNameText(
            pageSetup, worksheet.GetName());
        if (!printTitles.empty()) {
            auto dn = XmlElement::MakeElement(
                MainNs + "definedName",
                std::vector<std::pair<std::string, std::string>>{
                    {"name", DefinedNameUtility::PrintTitlesDefinedName},
                    {"localSheetId", std::to_string(sheetIndex)}},
                std::vector<XmlElement>{});
            dn.AddChild(MakeTextNode(std::move(printTitles)));
            definedNames.push_back(std::move(dn));
        }
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
// LoadWorksheetDefinedNames
// ---------------------------------------------------------------------------

std::unordered_map<int, WorksheetDefinedNamesState>
XlsxWorkbookPageSetup::LoadWorksheetDefinedNames(
    const XmlElement& workbookRoot,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options)
{
    std::unordered_map<int, WorksheetDefinedNamesState> states;

    auto definedNamesContainer = workbookRoot.GetElement(MainNs + "definedNames");
    if (definedNamesContainer.IsNull()) {
        return states;
    }

    auto definedNameElements = definedNamesContainer.GetElements(MainNs + "definedName");
    for (const auto& definedName : definedNameElements) {
        auto nameAttr = definedName.GetAttribute("name");
        std::string name = nameAttr.IsNull() ? std::string() : nameAttr.GetValue();
        // Trim edge whitespace only (matching C# .Trim())
        {
            auto start = name.find_first_not_of(" \t\n\r");
            if (start == std::string::npos) {
                name.clear();
            } else {
                auto end = name.find_last_not_of(" \t\n\r");
                name = name.substr(start, end - start + 1);
            }
        }

        if (name != DefinedNameUtility::PrintAreaDefinedName &&
            name != DefinedNameUtility::PrintTitlesDefinedName) {
            continue;
        }

        auto localSheetIdAttr = definedName.GetAttribute("localSheetId");
        auto localSheetId = XlsxWorkbookArchiveHelpers::ParseIntAttribute(localSheetIdAttr);
        if (!localSheetId.has_value() || localSheetId.value() < 0) {
            LoadIssue issue("PG-L004", DiagnosticSeverity::Warning,
                "Defined name '" + name + "' is missing a valid localSheetId and was ignored.",
                false, true);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
            continue;
        }

        auto it = states.find(localSheetId.value());
        if (it == states.end()) {
            auto result = states.emplace(localSheetId.value(), WorksheetDefinedNamesState());
            it = result.first;
        }
        auto& state = it->second;

        auto textValue = definedName.GetValue();
        // Trim edge whitespace
        {
            auto start = textValue.find_first_not_of(" \t\n\r");
            if (start == std::string::npos) {
                textValue.clear();
            } else {
                auto end = textValue.find_last_not_of(" \t\n\r");
                textValue = textValue.substr(start, end - start + 1);
            }
        }

        if (textValue.empty()) {
            continue;
        }

        try {
            if (name == DefinedNameUtility::PrintAreaDefinedName) {
                state.SetPrintArea(NormalizeLoadedPrintArea(textValue));
            } else {
                ParseLoadedPrintTitles(textValue, state);
            }
        } catch (const CellsException& exception) {
            if (options.GetStrictMode()) {
                throw InvalidFileFormatException(
                    "Defined name '" + name + "' is invalid.", exception);
            }

            LoadIssue issue("PG-L004", DiagnosticSeverity::Warning,
                "Defined name '" + name + "' is invalid and was ignored.",
                false, true);
            XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
        }
    }

    return states;
}

// ---------------------------------------------------------------------------
// ApplyWorksheetDefinedNames
// ---------------------------------------------------------------------------

void XlsxWorkbookPageSetup::ApplyWorksheetDefinedNames(
    WorksheetModel& worksheetModel,
    const WorksheetDefinedNamesState& definedNamesState)
{
    auto& pageSetup = worksheetModel.GetPageSetup();
    pageSetup.SetPrintArea(definedNamesState.GetPrintArea());
    pageSetup.SetPrintTitleRows(definedNamesState.GetPrintTitleRows());
    pageSetup.SetPrintTitleColumns(definedNamesState.GetPrintTitleColumns());
}

// ---------------------------------------------------------------------------
// BuildSheetProperties
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookPageSetup::BuildSheetProperties(const PageSetupModel& pageSetup)
{
    if (!pageSetup.GetFitToWidth().has_value() && !pageSetup.GetFitToHeight().has_value()) {
        return XmlElement();
    }

    return XmlElement::MakeElement(
        MainNs + "sheetPr",
        std::vector<std::pair<std::string, std::string>>{},
        std::vector<XmlElement>{
            XmlElement::MakeElement(
                MainNs + "pageSetUpPr",
                std::vector<std::pair<std::string, std::string>>{{"fitToPage", "1"}},
                std::vector<XmlElement>{})});
}

// ---------------------------------------------------------------------------
// BuildPrintOptionsElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookPageSetup::BuildPrintOptionsElement(const PageSetupModel& pageSetup)
{
    const auto& opts = pageSetup.GetPrintOptions();
    if (!opts.GetGridLines() && !opts.GetHeadings() &&
        !opts.GetHorizontalCentered() && !opts.GetVerticalCentered()) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(
        MainNs + "printOptions",
        std::vector<std::pair<std::string, std::string>>{},
        std::vector<XmlElement>{});

    if (opts.GetHeadings()) {
        element.SetAttribute("headings", "1");
    }
    if (opts.GetGridLines()) {
        element.SetAttribute("gridLines", "1");
        element.SetAttribute("gridLinesSet", "1");
    }
    if (opts.GetHorizontalCentered()) {
        element.SetAttribute("horizontalCentered", "1");
    }
    if (opts.GetVerticalCentered()) {
        element.SetAttribute("verticalCentered", "1");
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildPageMarginsElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookPageSetup::BuildPageMarginsElement(const PageSetupModel& pageSetup)
{
    if (MarginsEqual(pageSetup.GetMargins(), PageMarginsModel())) {
        return XmlElement();
    }

    const auto& m = pageSetup.GetMargins();
    return XmlElement::MakeElement(
        MainNs + "pageMargins",
        std::vector<std::pair<std::string, std::string>>{
            {"left", DoubleToRoundTripString(m.GetLeft())},
            {"right", DoubleToRoundTripString(m.GetRight())},
            {"top", DoubleToRoundTripString(m.GetTop())},
            {"bottom", DoubleToRoundTripString(m.GetBottom())},
            {"header", DoubleToRoundTripString(m.GetHeader())},
            {"footer", DoubleToRoundTripString(m.GetFooter())}},
        std::vector<XmlElement>{});
}

// ---------------------------------------------------------------------------
// BuildPageSetupElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookPageSetup::BuildPageSetupElement(const PageSetupModel& pageSetup)
{
    if (pageSetup.GetPaperSize() == 0
        && pageSetup.GetOrientation() == PageOrientation::Default
        && !pageSetup.GetFirstPageNumber().has_value()
        && !pageSetup.GetScale().has_value()
        && !pageSetup.GetFitToWidth().has_value()
        && !pageSetup.GetFitToHeight().has_value()) {
        return XmlElement();
    }

    std::vector<std::pair<std::string, std::string>> attrs;
    if (pageSetup.GetPaperSize() > 0) {
        attrs.emplace_back("paperSize", std::to_string(pageSetup.GetPaperSize()));
    }
    if (pageSetup.GetScale().has_value()) {
        attrs.emplace_back("scale", std::to_string(pageSetup.GetScale().value()));
    }
    if (pageSetup.GetFitToWidth().has_value()) {
        attrs.emplace_back("fitToWidth", std::to_string(pageSetup.GetFitToWidth().value()));
    }
    if (pageSetup.GetFitToHeight().has_value()) {
        attrs.emplace_back("fitToHeight", std::to_string(pageSetup.GetFitToHeight().value()));
    }
    if (pageSetup.GetFirstPageNumber().has_value()) {
        attrs.emplace_back("firstPageNumber", std::to_string(pageSetup.GetFirstPageNumber().value()));
        attrs.emplace_back("useFirstPageNumber", "1");
    }
    if (pageSetup.GetOrientation() == PageOrientation::Portrait) {
        attrs.emplace_back("orientation", "portrait");
    } else if (pageSetup.GetOrientation() == PageOrientation::Landscape) {
        attrs.emplace_back("orientation", "landscape");
    }

    return XmlElement::MakeElement(
        MainNs + "pageSetup",
        std::move(attrs),
        std::vector<XmlElement>{});
}

// ---------------------------------------------------------------------------
// BuildHeaderFooterElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookPageSetup::BuildHeaderFooterElement(const PageSetupModel& pageSetup)
{
    const auto& hf = pageSetup.GetHeaderFooter();
    const bool hasHeaderFooter =
        !hf.GetLeftHeader().empty() || !hf.GetCenterHeader().empty() || !hf.GetRightHeader().empty() ||
        !hf.GetLeftFooter().empty() || !hf.GetCenterFooter().empty() || !hf.GetRightFooter().empty();

    if (!hasHeaderFooter) {
        return XmlElement();
    }

    auto element = XmlElement::MakeElement(
        MainNs + "headerFooter",
        std::vector<std::pair<std::string, std::string>>{},
        std::vector<XmlElement>{});

    if (!hf.GetLeftHeader().empty() || !hf.GetCenterHeader().empty() || !hf.GetRightHeader().empty()) {
        auto oddHeader = XmlElement::MakeElement(
            MainNs + "oddHeader",
            std::vector<std::pair<std::string, std::string>>{},
            std::vector<XmlElement>{});
        oddHeader.AddChild(MakeTextNode(
            ComposeHeaderFooterText(hf.GetLeftHeader(), hf.GetCenterHeader(), hf.GetRightHeader())));
        element.AddChild(std::move(oddHeader));
    }

    if (!hf.GetLeftFooter().empty() || !hf.GetCenterFooter().empty() || !hf.GetRightFooter().empty()) {
        auto oddFooter = XmlElement::MakeElement(
            MainNs + "oddFooter",
            std::vector<std::pair<std::string, std::string>>{},
            std::vector<XmlElement>{});
        oddFooter.AddChild(MakeTextNode(
            ComposeHeaderFooterText(hf.GetLeftFooter(), hf.GetCenterFooter(), hf.GetRightFooter())));
        element.AddChild(std::move(oddFooter));
    }

    return element;
}

// ---------------------------------------------------------------------------
// BuildRowBreaksElement / BuildColumnBreaksElement
// ---------------------------------------------------------------------------

XmlElement XlsxWorkbookPageSetup::BuildRowBreaksElement(const PageSetupModel& pageSetup)
{
    return BuildBreaksElement("rowBreaks", pageSetup.GetHorizontalPageBreaks(), MaxSpreadsheetColumn);
}

XmlElement XlsxWorkbookPageSetup::BuildColumnBreaksElement(const PageSetupModel& pageSetup)
{
    return BuildBreaksElement("colBreaks", pageSetup.GetVerticalPageBreaks(), MaxSpreadsheetRow);
}

XmlElement XlsxWorkbookPageSetup::BuildBreaksElement(
    std::string_view elementName,
    const std::vector<int>& breaks,
    int maxIndex)
{
    std::set<int> distinct(breaks.begin(), breaks.end());
    if (distinct.empty()) {
        return XmlElement();
    }

    std::vector<XmlElement> breakElements;
    for (int value : distinct) {
        breakElements.push_back(XmlElement::MakeElement(
            MainNs + "brk",
            std::vector<std::pair<std::string, std::string>>{
                {"id", std::to_string(value)},
                {"max", std::to_string(maxIndex)},
                {"man", "1"}},
            std::vector<XmlElement>{}));
    }

    auto countStr = std::to_string(distinct.size());
    return XmlElement::MakeElement(
        MainNs + std::string(elementName),
        std::vector<std::pair<std::string, std::string>>{
            {"count", countStr},
            {"manualBreakCount", countStr}},
        std::move(breakElements));
}

// ---------------------------------------------------------------------------
// LoadWorksheetPageSetup
// ---------------------------------------------------------------------------

void XlsxWorkbookPageSetup::LoadWorksheetPageSetup(
    WorksheetModel& worksheetModel,
    const XmlElement& worksheetRoot,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    auto& pageSetup = worksheetModel.GetPageSetup();
    LoadPageMargins(pageSetup, worksheetRoot.GetElement(MainNs + "pageMargins"),
                    diagnostics, options, sheetName);
    LoadPageSetupCore(pageSetup, worksheetRoot.GetElement(MainNs + "pageSetup"),
                      diagnostics, options, sheetName);
    LoadPrintOptions(pageSetup, worksheetRoot.GetElement(MainNs + "printOptions"));
    LoadHeaderFooter(pageSetup, worksheetRoot.GetElement(MainNs + "headerFooter"));
    LoadBreaks(pageSetup.GetHorizontalPageBreaks(),
               worksheetRoot.GetElement(MainNs + "rowBreaks"),
               diagnostics, options, sheetName, "row");
    LoadBreaks(pageSetup.GetVerticalPageBreaks(),
               worksheetRoot.GetElement(MainNs + "colBreaks"),
               diagnostics, options, sheetName, "column");
}

// ---------------------------------------------------------------------------
// Private: Load helpers
// ---------------------------------------------------------------------------

void XlsxWorkbookPageSetup::LoadPageMargins(
    PageSetupModel& pageSetup,
    const XmlElement& marginsElement,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    if (marginsElement.IsNull()) {
        return;
    }

    auto& margins = pageSetup.GetMargins();
    margins.SetLeft(ParseMarginAttribute(
        marginsElement.GetAttribute("left"), margins.GetLeft(),
        diagnostics, options, sheetName, "left"));
    margins.SetRight(ParseMarginAttribute(
        marginsElement.GetAttribute("right"), margins.GetRight(),
        diagnostics, options, sheetName, "right"));
    margins.SetTop(ParseMarginAttribute(
        marginsElement.GetAttribute("top"), margins.GetTop(),
        diagnostics, options, sheetName, "top"));
    margins.SetBottom(ParseMarginAttribute(
        marginsElement.GetAttribute("bottom"), margins.GetBottom(),
        diagnostics, options, sheetName, "bottom"));
    margins.SetHeader(ParseMarginAttribute(
        marginsElement.GetAttribute("header"), margins.GetHeader(),
        diagnostics, options, sheetName, "header"));
    margins.SetFooter(ParseMarginAttribute(
        marginsElement.GetAttribute("footer"), margins.GetFooter(),
        diagnostics, options, sheetName, "footer"));
}

void XlsxWorkbookPageSetup::LoadPageSetupCore(
    PageSetupModel& pageSetup,
    const XmlElement& pageSetupElement,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    if (pageSetupElement.IsNull()) {
        return;
    }

    auto paperSize = ParsePositiveIntAttribute(
        pageSetupElement.GetAttribute("paperSize"), diagnostics, options, sheetName, "paperSize");
    pageSetup.SetPaperSize(paperSize.value_or(0));

    pageSetup.SetFirstPageNumber(ParsePositiveIntAttribute(
        pageSetupElement.GetAttribute("firstPageNumber"), diagnostics, options, sheetName, "firstPageNumber"));

    pageSetup.SetScale(ParseBoundedIntAttribute(
        pageSetupElement.GetAttribute("scale"), 10, 400, diagnostics, options, sheetName, "scale"));

    pageSetup.SetFitToWidth(ParseNonNegativeIntAttribute(
        pageSetupElement.GetAttribute("fitToWidth"), diagnostics, options, sheetName, "fitToWidth"));

    pageSetup.SetFitToHeight(ParseNonNegativeIntAttribute(
        pageSetupElement.GetAttribute("fitToHeight"), diagnostics, options, sheetName, "fitToHeight"));

    auto orientationAttr = pageSetupElement.GetAttribute("orientation");
    auto orientationValue = orientationAttr.IsNull() ? std::string() : orientationAttr.GetValue();
    pageSetup.SetOrientation(ParseOrientation(orientationValue, diagnostics, options, sheetName));
}

void XlsxWorkbookPageSetup::LoadPrintOptions(
    PageSetupModel& pageSetup,
    const XmlElement& printOptionsElement)
{
    if (printOptionsElement.IsNull()) {
        return;
    }

    auto& opts = pageSetup.GetPrintOptions();
    opts.SetHeadings(XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
        printOptionsElement.GetAttribute("headings")));
    opts.SetGridLines(XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
        printOptionsElement.GetAttribute("gridLines")));
    opts.SetHorizontalCentered(XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
        printOptionsElement.GetAttribute("horizontalCentered")));
    opts.SetVerticalCentered(XlsxWorkbookArchiveHelpers::ParseBoolAttribute(
        printOptionsElement.GetAttribute("verticalCentered")));
}

void XlsxWorkbookPageSetup::LoadHeaderFooter(
    PageSetupModel& pageSetup,
    const XmlElement& headerFooterElement)
{
    if (headerFooterElement.IsNull()) {
        return;
    }

    std::string leftHeader, centerHeader, rightHeader;
    auto oddHeaderElem = headerFooterElement.GetElement(MainNs + "oddHeader");
    ParseHeaderFooterText(oddHeaderElem.GetValue(), leftHeader, centerHeader, rightHeader);
    auto& hf = pageSetup.GetHeaderFooter();
    hf.SetLeftHeader(std::move(leftHeader));
    hf.SetCenterHeader(std::move(centerHeader));
    hf.SetRightHeader(std::move(rightHeader));

    std::string leftFooter, centerFooter, rightFooter;
    auto oddFooterElem = headerFooterElement.GetElement(MainNs + "oddFooter");
    ParseHeaderFooterText(oddFooterElem.GetValue(), leftFooter, centerFooter, rightFooter);
    hf.SetLeftFooter(std::move(leftFooter));
    hf.SetCenterFooter(std::move(centerFooter));
    hf.SetRightFooter(std::move(rightFooter));
}

void XlsxWorkbookPageSetup::LoadBreaks(
    std::vector<int>& target,
    const XmlElement& breaksElement,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view axis)
{
    target.clear();
    if (breaksElement.IsNull()) {
        return;
    }

    auto brkElements = breaksElement.GetElements(MainNs + "brk");
    for (const auto& breakElement : brkElements) {
        auto id = ParseNonNegativeIntAttribute(
            breakElement.GetAttribute("id"), diagnostics, options, sheetName,
            std::string(axis) + "Break");
        if (!id.has_value()) {
            continue;
        }

        if (std::find(target.begin(), target.end(), id.value()) == target.end()) {
            target.push_back(id.value());
        }
    }
}

// ---------------------------------------------------------------------------
// Private: Header/footer composition / parsing
// ---------------------------------------------------------------------------

std::string XlsxWorkbookPageSetup::ComposeHeaderFooterText(
    const std::string& left,
    const std::string& center,
    const std::string& right)
{
    std::string result;
    if (!left.empty()) {
        result += "&L";
        result += left;
    }
    if (!center.empty()) {
        result += "&C";
        result += center;
    }
    if (!right.empty()) {
        result += "&R";
        result += right;
    }
    return result;
}

void XlsxWorkbookPageSetup::ParseHeaderFooterText(
    const std::string& value,
    std::string& left,
    std::string& center,
    std::string& right)
{
    left.clear();
    center.clear();
    right.clear();
    if (value.empty()) {
        return;
    }

    char currentSection = 'C';
    std::string buffer;

    for (std::size_t index = 0; index < value.size(); ++index) {
        if (value[index] == '&' && index + 1 < value.size()) {
            char marker = static_cast<char>(std::toupper(
                static_cast<unsigned char>(value[index + 1])));
            if (marker == 'L' || marker == 'C' || marker == 'R') {
                AssignHeaderFooterSection(currentSection, buffer, left, center, right);
                buffer.clear();
                currentSection = marker;
                ++index;
                continue;
            }
        }
        buffer += value[index];
    }

    AssignHeaderFooterSection(currentSection, buffer, left, center, right);
}

void XlsxWorkbookPageSetup::AssignHeaderFooterSection(
    char section,
    const std::string& value,
    std::string& left,
    std::string& center,
    std::string& right)
{
    if (section == 'L') {
        left = value.empty() ? std::string() : value;
    } else if (section == 'R') {
        right = value.empty() ? std::string() : value;
    } else {
        center = value.empty() ? std::string() : value;
    }
}

// ---------------------------------------------------------------------------
// Private: MarginsEqual
// ---------------------------------------------------------------------------

bool XlsxWorkbookPageSetup::MarginsEqual(
    const PageMarginsModel& left,
    const PageMarginsModel& right)
{
    return left.GetLeft() == right.GetLeft()
        && left.GetRight() == right.GetRight()
        && left.GetTop() == right.GetTop()
        && left.GetBottom() == right.GetBottom()
        && left.GetHeader() == right.GetHeader()
        && left.GetFooter() == right.GetFooter();
}

// ---------------------------------------------------------------------------
// Private: Print titles / print area normalization
// ---------------------------------------------------------------------------

std::string XlsxWorkbookPageSetup::BuildPrintTitlesDefinedNameText(
    const PageSetupModel& pageSetup,
    std::string_view sheetName)
{
    std::vector<std::string> segments;

    auto normalizedRows = NormalizeTitleRows(pageSetup.GetPrintTitleRows(), sheetName);
    if (!normalizedRows.empty()) {
        segments.push_back(std::move(normalizedRows));
    }

    auto normalizedColumns = NormalizeTitleColumns(pageSetup.GetPrintTitleColumns(), sheetName);
    if (!normalizedColumns.empty()) {
        segments.push_back(std::move(normalizedColumns));
    }

    if (segments.empty()) {
        return std::string();
    }

    std::string result = segments[0];
    for (std::size_t i = 1; i < segments.size(); ++i) {
        result += ",";
        result += segments[i];
    }
    return result;
}

static bool IsBlankOrWhitespace(std::string_view s)
{
    for (char ch : s) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    return true;
}

std::string XlsxWorkbookPageSetup::NormalizePrintAreaList(
    std::string_view printArea,
    std::string_view sheetName)
{
    if (IsBlankOrWhitespace(printArea)) {
        return std::string();
    }

    std::vector<std::string> segments;
    for (const auto& segment : SplitReferenceList(printArea)) {
        segments.push_back(
            QualifyReference(sheetName,
                NormalizeAreaReference(RemoveWorksheetQualifier(segment))));
    }

    if (segments.empty()) {
        return std::string();
    }

    std::string result = segments[0];
    for (std::size_t i = 1; i < segments.size(); ++i) {
        result += ",";
        result += segments[i];
    }
    return result;
}

std::string XlsxWorkbookPageSetup::NormalizeLoadedPrintArea(std::string_view value)
{
    std::vector<std::string> segments;
    for (const auto& segment : SplitReferenceList(value)) {
        segments.push_back(NormalizeAreaReference(RemoveWorksheetQualifier(segment)));
    }

    if (segments.empty()) {
        return std::string();
    }

    std::string result = segments[0];
    for (std::size_t i = 1; i < segments.size(); ++i) {
        result += ",";
        result += segments[i];
    }
    return result;
}

void XlsxWorkbookPageSetup::ParseLoadedPrintTitles(
    std::string_view value,
    WorksheetDefinedNamesState& state)
{
    for (const auto& segment : SplitReferenceList(value)) {
        auto unqualified = RemoveWorksheetQualifier(segment);
        if (LooksLikeRowRange(unqualified)) {
            state.SetPrintTitleRows(NormalizeRowReference(unqualified));
        } else if (LooksLikeColumnRange(unqualified)) {
            state.SetPrintTitleColumns(NormalizeColumnReference(unqualified));
        } else {
            throw CellsException("Print title reference is invalid.");
        }
    }
}

std::string XlsxWorkbookPageSetup::NormalizeTitleRows(
    std::string_view value, std::string_view sheetName)
{
    if (IsBlankOrWhitespace(value)) {
        return std::string();
    }

    return QualifyReference(sheetName,
        NormalizeRowReference(RemoveWorksheetQualifier(value)));
}

std::string XlsxWorkbookPageSetup::NormalizeTitleColumns(
    std::string_view value, std::string_view sheetName)
{
    if (IsBlankOrWhitespace(value)) {
        return std::string();
    }

    return QualifyReference(sheetName,
        NormalizeColumnReference(RemoveWorksheetQualifier(value)));
}

// ---------------------------------------------------------------------------
// Private: Reference parsing
// ---------------------------------------------------------------------------

std::vector<std::string> XlsxWorkbookPageSetup::SplitReferenceList(std::string_view value)
{
    std::vector<std::string> parts;
    std::string buffer;
    bool inQuotes = false;

    for (std::size_t index = 0; index < value.size(); ++index) {
        char character = value[index];
        if (character == '\'') {
            buffer += character;
            if (inQuotes && index + 1 < value.size() && value[index + 1] == '\'') {
                buffer += value[index + 1];
                ++index;
                continue;
            }
            inQuotes = !inQuotes;
            continue;
        }

        if (character == ',' && !inQuotes) {
            // Trim edge whitespace
            auto start = buffer.find_first_not_of(" \t\n\r");
            if (start != std::string::npos) {
                auto end = buffer.find_last_not_of(" \t\n\r");
                parts.push_back(buffer.substr(start, end - start + 1));
            }
            buffer.clear();
            continue;
        }

        buffer += character;
    }

    // Last segment
    auto start = buffer.find_first_not_of(" \t\n\r");
    if (start != std::string::npos) {
        auto end = buffer.find_last_not_of(" \t\n\r");
        parts.push_back(buffer.substr(start, end - start + 1));
    }

    return parts;
}

std::string XlsxWorkbookPageSetup::RemoveWorksheetQualifier(std::string_view value)
{
    // Trim edge whitespace
    auto trimStart = value.find_first_not_of(" \t\n\r");
    if (trimStart == std::string_view::npos) {
        throw CellsException("Worksheet reference is invalid.");
    }
    auto trimEnd = value.find_last_not_of(" \t\n\r");
    value = value.substr(trimStart, trimEnd - trimStart + 1);

    if (value.empty()) {
        throw CellsException("Worksheet reference is invalid.");
    }

    if (value[0] == '\'') {
        std::size_t index = 1;
        while (index < value.size()) {
            if (value[index] == '\'') {
                if (index + 1 < value.size() && value[index + 1] == '\'') {
                    index += 2;
                    continue;
                }
                break;
            }
            ++index;
        }

        if (index + 1 < value.size() && value[index + 1] == '!') {
            return std::string(value.substr(index + 2));
        }
    }

    auto exclamation = value.find('!');
    return exclamation != std::string_view::npos
        ? std::string(value.substr(exclamation + 1))
        : std::string(value);
}

std::string XlsxWorkbookPageSetup::QualifyReference(
    std::string_view sheetName, std::string_view reference)
{
    return QuoteWorksheetName(sheetName) + "!" + std::string(reference);
}

std::string XlsxWorkbookPageSetup::QuoteWorksheetName(std::string_view sheetName)
{
    return "'" + ReplaceAll(sheetName, "'", "''") + "'";
}

std::string XlsxWorkbookPageSetup::NormalizeAreaReference(std::string_view value)
{
    // Split on ':'
    auto colonPos = value.find(':');
    if (colonPos == std::string_view::npos) {
        auto address = ParseAbsoluteCellAddress(value);
        return ToAbsoluteCellReference(address);
    }

    auto secondColon = value.find(':', colonPos + 1);
    if (secondColon != std::string_view::npos) {
        throw CellsException("PrintArea must be a cell or range reference.");
    }

    auto first = ParseAbsoluteCellAddress(value.substr(0, colonPos));
    auto last = ParseAbsoluteCellAddress(value.substr(colonPos + 1));
    if (last.GetRowIndex() < first.GetRowIndex() ||
        last.GetColumnIndex() < first.GetColumnIndex()) {
        throw CellsException("PrintArea range must be ordered from top-left to bottom-right.");
    }

    return ToAbsoluteCellReference(first) + ":" + ToAbsoluteCellReference(last);
}

std::string XlsxWorkbookPageSetup::NormalizeRowReference(std::string_view value)
{
    auto normalized = ReplaceAll(value, "$", "");
    // Trim edge whitespace
    {
        auto start = normalized.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) {
            normalized.clear();
        } else {
            auto end = normalized.find_last_not_of(" \t\n\r");
            normalized = normalized.substr(start, end - start + 1);
        }
    }

    auto colonPos = normalized.find(':');
    if (colonPos == std::string::npos) {
        throw CellsException("PrintTitleRows must be a row span like '1:2'.");
    }

    auto firstStr = normalized.substr(0, colonPos);
    auto lastStr = normalized.substr(colonPos + 1);

    int first = 0;
    int last = 0;
    auto firstResult = std::from_chars(firstStr.data(), firstStr.data() + firstStr.size(), first);
    auto lastResult = std::from_chars(lastStr.data(), lastStr.data() + lastStr.size(), last);
    if (firstResult.ec != std::errc() || lastResult.ec != std::errc() ||
        firstResult.ptr != firstStr.data() + firstStr.size() ||
        lastResult.ptr != lastStr.data() + lastStr.size() ||
        first <= 0 || last < first) {
        throw CellsException("PrintTitleRows must be a row span like '1:2'.");
    }

    return "$" + std::to_string(first) + ":$" + std::to_string(last);
}

std::string XlsxWorkbookPageSetup::NormalizeColumnReference(std::string_view value)
{
    auto normalized = ReplaceAll(value, "$", "");
    // Trim edge whitespace
    {
        auto start = normalized.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) {
            normalized.clear();
        } else {
            auto end = normalized.find_last_not_of(" \t\n\r");
            normalized = normalized.substr(start, end - start + 1);
        }
    }
    normalized = ToUpperInvariant(normalized);

    auto colonPos = normalized.find(':');
    if (colonPos == std::string::npos) {
        throw CellsException("PrintTitleColumns must be a column span like 'A:B'.");
    }

    auto firstPart = normalized.substr(0, colonPos);
    auto secondPart = normalized.substr(colonPos + 1);
    if (!IsColumnName(firstPart) || !IsColumnName(secondPart)) {
        throw CellsException("PrintTitleColumns must be a column span like 'A:B'.");
    }

    return "$" + firstPart + ":$" + secondPart;
}

bool XlsxWorkbookPageSetup::LooksLikeRowRange(std::string_view value)
{
    auto stripped = ReplaceAll(value, "$", "");
    auto colonPos = stripped.find(':');
    if (colonPos == std::string::npos) {
        int ignored = 0;
        auto result = std::from_chars(stripped.data(), stripped.data() + stripped.size(), ignored);
        return result.ec == std::errc() && result.ptr == stripped.data() + stripped.size();
    }

    auto firstPart = stripped.substr(0, colonPos);
    auto secondPart = stripped.substr(colonPos + 1);

    int ignored = 0;
    auto r1 = std::from_chars(firstPart.data(), firstPart.data() + firstPart.size(), ignored);
    if (r1.ec != std::errc() || r1.ptr != firstPart.data() + firstPart.size()) {
        return false;
    }
    auto r2 = std::from_chars(secondPart.data(), secondPart.data() + secondPart.size(), ignored);
    return r2.ec == std::errc() && r2.ptr == secondPart.data() + secondPart.size();
}

bool XlsxWorkbookPageSetup::LooksLikeColumnRange(std::string_view value)
{
    auto stripped = ReplaceAll(value, "$", "");
    auto colonPos = stripped.find(':');
    if (colonPos == std::string::npos) {
        return IsColumnName(stripped);
    }
    return IsColumnName(stripped.substr(0, colonPos)) &&
           IsColumnName(stripped.substr(colonPos + 1));
}

bool XlsxWorkbookPageSetup::IsColumnName(std::string_view value)
{
    if (value.empty()) {
        return false;
    }
    for (char ch : value) {
        if (!std::isalpha(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    return true;
}

CellAddress XlsxWorkbookPageSetup::ParseAbsoluteCellAddress(std::string_view value)
{
    auto normalized = ReplaceAll(value, "$", "");
    // Trim edge whitespace
    {
        auto start = normalized.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) {
            normalized.clear();
        } else {
            auto end = normalized.find_last_not_of(" \t\n\r");
            normalized = normalized.substr(start, end - start + 1);
        }
    }

    try {
        return CellAddress::Parse(normalized);
    } catch (const std::invalid_argument& exception) {
        throw CellsException(
            std::string("Cell reference '") + std::string(value) + "' is invalid.", exception);
    }
}

std::string XlsxWorkbookPageSetup::ToAbsoluteCellReference(const CellAddress& address)
{
    auto reference = address.ToString();
    std::size_t splitIndex = 0;
    while (splitIndex < reference.size() &&
           std::isalpha(static_cast<unsigned char>(reference[splitIndex]))) {
        ++splitIndex;
    }

    return "$" + reference.substr(0, splitIndex) + "$" + reference.substr(splitIndex);
}

// ---------------------------------------------------------------------------
// Private: Attribute parsing helpers
// ---------------------------------------------------------------------------

double XlsxWorkbookPageSetup::ParseMarginAttribute(
    const XmlAttribute& attribute,
    double defaultValue,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view attributeName)
{
    if (attribute.IsNull()) {
        return defaultValue;
    }

    auto value = XlsxWorkbookArchiveHelpers::ParseDoubleAttribute(attribute);
    if (value.has_value() && value.value() >= 0.0) {
        return value.value();
    }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            "The page margin attribute '" + std::string(attributeName) + "' is invalid.");
    }

    LoadIssue issue("PG-L001", DiagnosticSeverity::Warning,
        "Page margin attribute '" + std::string(attributeName) + "' is invalid and the default value was used.",
        false, true);
    issue.SetSheetName(sheetName);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return defaultValue;
}

std::optional<int> XlsxWorkbookPageSetup::ParsePositiveIntAttribute(
    const XmlAttribute& attribute,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view attributeName)
{
    if (attribute.IsNull()) {
        return std::nullopt;
    }

    auto value = XlsxWorkbookArchiveHelpers::ParseIntAttribute(attribute);
    if (value.has_value() && value.value() > 0) {
        return value.value();
    }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            "The page setup attribute '" + std::string(attributeName) + "' is invalid.");
    }

    LoadIssue issue("PG-L002", DiagnosticSeverity::Warning,
        "Page setup attribute '" + std::string(attributeName) + "' is invalid and was ignored.",
        false, true);
    issue.SetSheetName(sheetName);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return std::nullopt;
}

std::optional<int> XlsxWorkbookPageSetup::ParseNonNegativeIntAttribute(
    const XmlAttribute& attribute,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view attributeName)
{
    if (attribute.IsNull()) {
        return std::nullopt;
    }

    auto value = XlsxWorkbookArchiveHelpers::ParseIntAttribute(attribute);
    if (value.has_value() && value.value() >= 0) {
        return value.value();
    }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            "The page setup attribute '" + std::string(attributeName) + "' is invalid.");
    }

    LoadIssue issue("PG-L002", DiagnosticSeverity::Warning,
        "Page setup attribute '" + std::string(attributeName) + "' is invalid and was ignored.",
        false, true);
    issue.SetSheetName(sheetName);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return std::nullopt;
}

std::optional<int> XlsxWorkbookPageSetup::ParseBoundedIntAttribute(
    const XmlAttribute& attribute,
    int minimum,
    int maximum,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName,
    std::string_view attributeName)
{
    if (attribute.IsNull()) {
        return std::nullopt;
    }

    auto value = XlsxWorkbookArchiveHelpers::ParseIntAttribute(attribute);
    if (value.has_value() && value.value() >= minimum && value.value() <= maximum) {
        return value.value();
    }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException(
            "The page setup attribute '" + std::string(attributeName) + "' is invalid.");
    }

    LoadIssue issue("PG-L002", DiagnosticSeverity::Warning,
        "Page setup attribute '" + std::string(attributeName) + "' is invalid and was ignored.",
        false, true);
    issue.SetSheetName(sheetName);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return std::nullopt;
}

Core::PageOrientation XlsxWorkbookPageSetup::ParseOrientation(
    std::string_view value,
    LoadDiagnostics& diagnostics,
    const LoadOptions& options,
    std::string_view sheetName)
{
    if (IsBlankOrWhitespace(value)) {
        return PageOrientation::Default;
    }

    auto upper = ToUpperInvariant(value);
    if (upper == "PORTRAIT") {
        return PageOrientation::Portrait;
    }
    if (upper == "LANDSCAPE") {
        return PageOrientation::Landscape;
    }

    if (options.GetStrictMode()) {
        throw InvalidFileFormatException("The page setup orientation is invalid.");
    }

    LoadIssue issue("PG-L003", DiagnosticSeverity::Warning,
        "Page setup orientation is invalid and the default orientation was used.",
        false, true);
    issue.SetSheetName(sheetName);
    XlsxWorkbookArchiveHelpers::AddIssue(diagnostics, options, issue);
    return PageOrientation::Default;
}

}}  // namespace Aspose::Cells_FOSS
