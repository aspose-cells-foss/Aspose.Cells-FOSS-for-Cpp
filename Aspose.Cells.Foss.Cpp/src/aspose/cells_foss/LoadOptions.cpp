#include "aspose/cells_foss/LoadOptions.h"

namespace Aspose {
namespace Cells_FOSS {

LoadOptions::LoadOptions()
    : _loadFormat(LoadFormat::Auto)
    , _strictMode(false)
    , _tryRepairPackage(true)
    , _tryRepairXml(true)
    , _preserveUnsupportedParts(true)
    , _warningCallback(nullptr)
{
}

LoadFormat LoadOptions::GetLoadFormat() const noexcept
{
    return _loadFormat;
}

void LoadOptions::SetLoadFormat(LoadFormat value) noexcept
{
    _loadFormat = value;
}

bool LoadOptions::GetStrictMode() const noexcept
{
    return _strictMode;
}

void LoadOptions::SetStrictMode(bool value) noexcept
{
    _strictMode = value;
}

bool LoadOptions::GetTryRepairPackage() const noexcept
{
    return _tryRepairPackage;
}

void LoadOptions::SetTryRepairPackage(bool value) noexcept
{
    _tryRepairPackage = value;
}

bool LoadOptions::GetTryRepairXml() const noexcept
{
    return _tryRepairXml;
}

void LoadOptions::SetTryRepairXml(bool value) noexcept
{
    _tryRepairXml = value;
}

bool LoadOptions::GetPreserveUnsupportedParts() const noexcept
{
    return _preserveUnsupportedParts;
}

void LoadOptions::SetPreserveUnsupportedParts(bool value) noexcept
{
    _preserveUnsupportedParts = value;
}

const std::shared_ptr<IWarningCallback>& LoadOptions::GetWarningCallback() const noexcept
{
    return _warningCallback;
}

void LoadOptions::SetWarningCallback(std::shared_ptr<IWarningCallback> value) noexcept
{
    _warningCallback = std::move(value);
}

}}  // namespace Aspose::Cells_FOSS
