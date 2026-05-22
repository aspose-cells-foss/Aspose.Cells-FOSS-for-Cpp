#pragma once

#include "aspose/cells_foss/LoadDiagnostics.h"
#include "aspose/cells_foss/LoadOptions.h"
#include "aspose/cells_foss/StylesheetLoadContext.h"
#include "aspose/cells_foss/StylesheetSaveContext.h"
#include "aspose/cells_foss/XmlElement.h"
#include "aspose/cells_foss/core/ColorValue.h"
#include "aspose/cells_foss/core/FormatConditionModel.h"
#include "aspose/cells_foss/core/WorksheetModel.h"

#include <string>
#include <string_view>
#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Provides static methods for building and loading conditional formatting XML elements.
class XlsxWorkbookConditionalFormatting {
public:
    XlsxWorkbookConditionalFormatting() = delete;

    /// Builds conditionalFormatting XML elements from the worksheet model.
    static std::vector<XmlElement> BuildConditionalFormattingElements(
        Core::WorksheetModel& worksheet,
        StylesheetSaveContext& stylesheet);

    /// Loads conditional formatting data from the worksheet XML root into the model.
    static void LoadConditionalFormattings(
        Core::WorksheetModel& worksheetModel,
        const XmlElement& worksheetRoot,
        StylesheetLoadContext& stylesheet,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

private:
    static XmlElement BuildConditionRule(
        const Core::ConditionalFormattingModel& formatting,
        const Core::FormatConditionModel& condition,
        int conditionIndex,
        StylesheetSaveContext& stylesheet);

    static bool TryLoadAreas(
        Core::ConditionalFormattingModel& formatting,
        const std::string& sqref,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName);

    static bool TryLoadCondition(
        Core::ConditionalFormattingModel& formatting,
        const XmlElement& ruleElement,
        StylesheetLoadContext& stylesheet,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        const std::string& cellRef);

    static bool TryParseRuleType(
        const XmlElement& ruleElement,
        const LoadOptions& options,
        LoadDiagnostics& diagnostics,
        std::string_view sheetName,
        const std::string& cellRef,
        FormatConditionType& type);

    static void ApplyDefaults(Core::FormatConditionModel& condition);

    static int ParsePriority(
        const XmlAttribute& attribute,
        const Core::ConditionalFormattingModel& formatting,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        const std::string& cellRef);

    static Core::StyleValue ResolveDifferentialStyle(
        const XmlAttribute& attribute,
        StylesheetLoadContext& stylesheet,
        LoadDiagnostics& diagnostics,
        const LoadOptions& options,
        std::string_view sheetName,
        const std::string& cellRef);

    static void LoadColorScale(
        Core::FormatConditionModel& condition,
        const XmlElement& colorScaleElement);

    static void LoadDataBar(
        Core::FormatConditionModel& condition,
        const XmlElement& dataBarElement);

    static void LoadIconSet(
        Core::FormatConditionModel& condition,
        const XmlElement& iconSetElement);

    static XmlElement BuildColorScaleElement(const Core::FormatConditionModel& condition);
    static XmlElement BuildDataBarElement(const Core::FormatConditionModel& condition);
    static XmlElement BuildIconSetElement(const Core::FormatConditionModel& condition);

    static XmlElement BuildColorElement(
        const Core::ColorValue& actual,
        const Core::ColorValue& defaultColor);

    static void AddFormulaElement(XmlElement& parent, const std::string& formula);

    static std::string BuildTextRuleFormula(
        FormatConditionType type,
        const std::string& text,
        const std::string& firstCell);

    static std::string GetAnchorCellReference(
        const Core::ConditionalFormattingModel& formatting);

    static std::vector<Core::ConditionalFormattingModel> GetOrderedConditionalFormattings(
        const std::vector<Core::ConditionalFormattingModel>& collections);

    static int CompareConditionalFormattings(
        const Core::ConditionalFormattingModel& left,
        const Core::ConditionalFormattingModel& right);

    static std::string BuildSqref(const std::vector<CellArea>& areas);
    static std::string ToAreaReference(const CellArea& area);

    static std::string ToRuleTypeName(const Core::FormatConditionModel& condition);
    static std::string ToOperatorName(OperatorType type);
    static OperatorType ParseOperatorType(std::string_view text);

    static int ParseNonNegativeIntAttribute(const XmlAttribute& attribute);
    static int GetIconCount(std::string_view iconSetType);

    static std::string NormalizeToken(std::string_view value);
    static std::string NormalizeFormulaValue(std::string_view value);
    static std::string NormalizeTextValue(std::string_view value);

    static Core::ColorValue ReadRgbColor(const XmlElement& element);
    static std::string ToArgbHex(const Core::ColorValue& color);
    static bool IsEmptyColor(const Core::ColorValue& color);
};

}}  // namespace Aspose::Cells_FOSS
