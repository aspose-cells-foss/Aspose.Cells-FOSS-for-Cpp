#pragma once
#include "aspose/cells_foss/DiagnosticSeverity.h"

#include <optional>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

/// Represents load issue.
class LoadIssue {
public:
    /// Initializes a new instance of the LoadIssue class.
    /// @param code The code.
    /// @param severity The severity.
    /// @param message The message.
    /// @param repairApplied Whether repair was applied.
    /// @param dataLossRisk Whether there is a data loss risk.
    LoadIssue(std::string_view code,
              DiagnosticSeverity severity,
              std::string_view message,
              bool repairApplied = false,
              bool dataLossRisk = false);

    /// Gets the code.
    const std::string& GetCode() const noexcept;

    /// Gets the severity.
    DiagnosticSeverity GetSeverity() const noexcept;

    /// Gets the message.
    const std::string& GetMessage() const noexcept;

    /// Gets a value indicating whether repair was applied.
    bool GetRepairApplied() const noexcept;

    /// Gets a value indicating whether data loss risk.
    bool GetDataLossRisk() const noexcept;

    /// Gets or sets the part uri.
    const std::string& GetPartUri() const noexcept;
    void SetPartUri(std::string_view value);

    /// Gets or sets the sheet name.
    const std::string& GetSheetName() const noexcept;
    void SetSheetName(std::string_view value);

    /// Gets or sets the cell ref.
    const std::string& GetCellRef() const noexcept;
    void SetCellRef(std::string_view value);

    /// Gets or sets the row index.
    std::optional<int> GetRowIndex() const noexcept;
    void SetRowIndex(std::optional<int> value) noexcept;

private:
    std::string _code;
    DiagnosticSeverity _severity;
    std::string _message;
    bool _repairApplied;
    bool _dataLossRisk;
    std::string _partUri;
    std::string _sheetName;
    std::string _cellRef;
    std::optional<int> _rowIndex;
};

}}  // namespace Aspose::Cells_FOSS
