#pragma once
#include "SaveFormat.h"

namespace Aspose {
namespace Cells_FOSS {

/// Specifies how a workbook should be saved.
class SaveOptions {
public:
    /// Constructs a SaveOptions with default values.
    SaveOptions();

    /// Gets or sets the output file format.
    SaveFormat GetSaveFormat() const noexcept;
    void SetSaveFormat(SaveFormat value) noexcept;

    /// Gets or sets whether shared strings should be used for string cells.
    bool GetUseSharedStrings() const noexcept;
    void SetUseSharedStrings(bool value) noexcept;

    /// Gets or sets whether the workbook should be validated before save.
    bool GetValidateBeforeSave() const noexcept;
    void SetValidateBeforeSave(bool value) noexcept;

    /// Gets or sets whether equivalent styles should be compacted during save.
    bool GetCompactStyles() const noexcept;
    void SetCompactStyles(bool value) noexcept;

    /// Gets or sets whether recovery metadata should be preserved in the saved workbook.
    bool GetPreserveRecoveryMetadata() const noexcept;
    void SetPreserveRecoveryMetadata(bool value) noexcept;

private:
    SaveFormat _saveFormat;
    bool _useSharedStrings;
    bool _validateBeforeSave;
    bool _compactStyles;
    bool _preserveRecoveryMetadata;
};

}}  // namespace Aspose::Cells_FOSS
