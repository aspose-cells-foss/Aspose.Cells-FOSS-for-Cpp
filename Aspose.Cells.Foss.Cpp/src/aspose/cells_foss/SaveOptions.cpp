#include "aspose/cells_foss/SaveOptions.h"

namespace Aspose {
namespace Cells_FOSS {

SaveOptions::SaveOptions()
    : _saveFormat(SaveFormat::Xlsx)
    , _useSharedStrings(true)
    , _validateBeforeSave(true)
    , _compactStyles(true)
    , _preserveRecoveryMetadata(false)
{
}

SaveFormat SaveOptions::GetSaveFormat() const noexcept
{
    return _saveFormat;
}

void SaveOptions::SetSaveFormat(SaveFormat value) noexcept
{
    _saveFormat = value;
}

bool SaveOptions::GetUseSharedStrings() const noexcept
{
    return _useSharedStrings;
}

void SaveOptions::SetUseSharedStrings(bool value) noexcept
{
    _useSharedStrings = value;
}

bool SaveOptions::GetValidateBeforeSave() const noexcept
{
    return _validateBeforeSave;
}

void SaveOptions::SetValidateBeforeSave(bool value) noexcept
{
    _validateBeforeSave = value;
}

bool SaveOptions::GetCompactStyles() const noexcept
{
    return _compactStyles;
}

void SaveOptions::SetCompactStyles(bool value) noexcept
{
    _compactStyles = value;
}

bool SaveOptions::GetPreserveRecoveryMetadata() const noexcept
{
    return _preserveRecoveryMetadata;
}

void SaveOptions::SetPreserveRecoveryMetadata(bool value) noexcept
{
    _preserveRecoveryMetadata = value;
}

}}  // namespace Aspose::Cells_FOSS
