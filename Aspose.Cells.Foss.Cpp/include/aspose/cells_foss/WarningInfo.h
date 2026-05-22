#pragma once
#include "aspose/cells_foss/DiagnosticSeverity.h"

#include <optional>
#include <string>
#include <string_view>

namespace Aspose {
namespace Cells_FOSS {

/// Represents warning info.
class WarningInfo {
public:
    /// Initializes a new instance of the WarningInfo class.
    WarningInfo();

    /// Gets or sets the code.
    const std::string& GetCode() const noexcept;
    void SetCode(std::string_view value);

    /// Gets or sets the severity.
    DiagnosticSeverity GetSeverity() const noexcept;
    void SetSeverity(DiagnosticSeverity value) noexcept;

    /// Gets or sets the message.
    const std::string& GetMessage() const noexcept;
    void SetMessage(std::string_view value);

    /// Gets or sets a value indicating whether data loss risk.
    bool GetDataLossRisk() const noexcept;
    void SetDataLossRisk(bool value) noexcept;

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
    bool _dataLossRisk;
    std::string _partUri;
    std::string _sheetName;
    std::string _cellRef;
    std::optional<int> _rowIndex;
};

}}  // namespace Aspose::Cells_FOSS
