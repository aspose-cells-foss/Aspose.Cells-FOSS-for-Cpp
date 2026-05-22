#pragma once

#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/WorksheetDefinedNamesState.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/core/PageSetupModel.h"
#include "aspose/cells_foss/core/WorkbookModel.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Provides static methods for building and loading page-setup XML elements.
class XlsxWorkbookPageSetup {
public:
    XlsxWorkbookPageSetup() = delete;

    /// Builds the definedNames element containing print area and print titles.
    static XmlElement BuildPageSetupDefinedNames(const Core::WorkbookModel& model);

    /// Loads worksheet defined name states (print area / print titles) from the workbook root.
    static std::unordered_map<int, WorksheetDefinedNamesState> LoadWorksheetDefinedNames(
        const XmlElement& workbookRoot,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options);

    /// Applies loaded defined names to the worksheet model.
    static void ApplyWorksheetDefinedNames(
        Core::WorksheetModel& worksheetModel,
        const WorksheetDefinedNamesState& definedNamesState);

    /// Builds the sheetPr element for fit-to-page settings.
    static XmlElement BuildSheetProperties(const Core::PageSetupModel& pageSetup);

    /// Builds the printOptions element.
    static XmlElement BuildPrintOptionsElement(const Core::PageSetupModel& pageSetup);

    /// Builds the pageMargins element.
    static XmlElement BuildPageMarginsElement(const Core::PageSetupModel& pageSetup);

    /// Builds the pageSetup element.
    static XmlElement BuildPageSetupElement(const Core::PageSetupModel& pageSetup);

    /// Builds the headerFooter element.
    static XmlElement BuildHeaderFooterElement(const Core::PageSetupModel& pageSetup);

    /// Builds the rowBreaks element.
    static XmlElement BuildRowBreaksElement(const Core::PageSetupModel& pageSetup);

    /// Builds the colBreaks element.
    static XmlElement BuildColumnBreaksElement(const Core::PageSetupModel& pageSetup);

    /// Loads page-setup data from the worksheet XML root into the model.
    static void LoadWorksheetPageSetup(
        Core::WorksheetModel& worksheetModel,
        const XmlElement& worksheetRoot,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

private:
    static constexpr int MaxSpreadsheetRow = 1048575;
    static constexpr int MaxSpreadsheetColumn = 16383;

    static XmlElement BuildBreaksElement(
        std::string_view elementName,
        const std::vector<int>& breaks,
        int maxIndex);

    static void LoadPageMargins(
        Core::PageSetupModel& pageSetup,
        const XmlElement& marginsElement,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

    static void LoadPageSetupCore(
        Core::PageSetupModel& pageSetup,
        const XmlElement& pageSetupElement,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

    static void LoadPrintOptions(
        Core::PageSetupModel& pageSetup,
        const XmlElement& printOptionsElement);

    static void LoadHeaderFooter(
        Core::PageSetupModel& pageSetup,
        const XmlElement& headerFooterElement);

    static void LoadBreaks(
        std::vector<int>& target,
        const XmlElement& breaksElement,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view axis);

    static std::string ComposeHeaderFooterText(
        const std::string& left,
        const std::string& center,
        const std::string& right);

    static void ParseHeaderFooterText(
        const std::string& value,
        std::string& left,
        std::string& center,
        std::string& right);

    static void AssignHeaderFooterSection(
        char section,
        const std::string& value,
        std::string& left,
        std::string& center,
        std::string& right);

    static bool MarginsEqual(
        const Core::PageMarginsModel& left,
        const Core::PageMarginsModel& right);

    static std::string BuildPrintTitlesDefinedNameText(
        const Core::PageSetupModel& pageSetup,
        std::string_view sheetName);

    static std::string NormalizePrintAreaList(
        std::string_view printArea,
        std::string_view sheetName);

    static std::string NormalizeLoadedPrintArea(std::string_view value);

    static void ParseLoadedPrintTitles(
        std::string_view value,
        WorksheetDefinedNamesState& state);

    static std::string NormalizeTitleRows(std::string_view value, std::string_view sheetName);
    static std::string NormalizeTitleColumns(std::string_view value, std::string_view sheetName);

    static std::vector<std::string> SplitReferenceList(std::string_view value);
    static std::string RemoveWorksheetQualifier(std::string_view value);
    static std::string QualifyReference(std::string_view sheetName, std::string_view reference);
    static std::string QuoteWorksheetName(std::string_view sheetName);
    static std::string NormalizeAreaReference(std::string_view value);
    static std::string NormalizeRowReference(std::string_view value);
    static std::string NormalizeColumnReference(std::string_view value);
    static bool LooksLikeRowRange(std::string_view value);
    static bool LooksLikeColumnRange(std::string_view value);
    static bool IsColumnName(std::string_view value);

    static Core::CellAddress ParseAbsoluteCellAddress(std::string_view value);
    static std::string ToAbsoluteCellReference(const Core::CellAddress& address);

    static double ParseMarginAttribute(
        const XmlAttribute& attribute,
        double defaultValue,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view attributeName);

    static std::optional<int> ParsePositiveIntAttribute(
        const XmlAttribute& attribute,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view attributeName);

    static std::optional<int> ParseNonNegativeIntAttribute(
        const XmlAttribute& attribute,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view attributeName);

    static std::optional<int> ParseBoundedIntAttribute(
        const XmlAttribute& attribute,
        int minimum,
        int maximum,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        std::string_view attributeName);

    static Core::PageOrientation ParseOrientation(
        std::string_view value,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

    static std::string DoubleToRoundTripString(double value);
    static std::string ReplaceAll(std::string_view input, std::string_view from, std::string_view to);
    static std::string ToUpperInvariant(std::string_view value);
};

}}  // namespace Aspose::Cells_FOSS
