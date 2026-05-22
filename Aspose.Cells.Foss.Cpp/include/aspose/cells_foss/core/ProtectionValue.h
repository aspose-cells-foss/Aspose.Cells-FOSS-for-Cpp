#pragma once

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents protection value.
/// </summary>
class ProtectionValue final {
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="ProtectionValue"/> class.
    /// </summary>
    ProtectionValue() = default;

    /// <summary>
    /// Gets or sets a value indicating whether locked.
    /// </summary>
    bool GetIsLocked() const noexcept { return _isLocked; }
    void SetIsLocked(bool value) noexcept { _isLocked = value; }

    /// <summary>
    /// Gets or sets a value indicating whether hidden.
    /// </summary>
    bool GetIsHidden() const noexcept { return _isHidden; }
    void SetIsHidden(bool value) noexcept { _isHidden = value; }

    /// <summary>
    /// Creates a copy of the current instance.
    /// </summary>
    /// <returns>The protection value.</returns>
    ProtectionValue Clone() const;

private:
    bool _isLocked = true;
    bool _isHidden = false;
};

}}}  // namespace Aspose::Cells_FOSS::Core
