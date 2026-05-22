#pragma once

#include <cstdint>
#include <functional>

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

/// <summary>
/// Represents color value.
/// </summary>
struct ColorValue {
public:
    /// <summary>
    /// Initializes a new instance of the ColorValue struct.
    /// </summary>
    ColorValue() noexcept = default;

    /// <summary>
    /// Initializes a new instance of the ColorValue struct.
    /// </summary>
    /// <param name="a">The a.</param>
    /// <param name="r">The r.</param>
    /// <param name="g">The g.</param>
    /// <param name="b">The b.</param>
    ColorValue(std::uint8_t a, std::uint8_t r, std::uint8_t g, std::uint8_t b);

    /// <summary>
    /// Gets the a.
    /// </summary>
    std::uint8_t GetA() const noexcept { return _a; }

    /// <summary>
    /// Gets the r.
    /// </summary>
    std::uint8_t GetR() const noexcept { return _r; }

    /// <summary>
    /// Gets the g.
    /// </summary>
    std::uint8_t GetG() const noexcept { return _g; }

    /// <summary>
    /// Gets the b.
    /// </summary>
    std::uint8_t GetB() const noexcept { return _b; }

    /// <summary>
    /// Determines whether the specified value is equal to the current instance.
    /// </summary>
    /// <param name="other">The other.</param>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    bool Equals(const ColorValue& other) const noexcept;

    /// <summary>
    /// Returns a hash code for the current instance.
    /// </summary>
    /// <returns>The hash code.</returns>
    int GetHashCode() const noexcept;

    friend bool operator==(const ColorValue& lhs, const ColorValue& rhs) noexcept
    {
        return lhs.Equals(rhs);
    }

    friend bool operator!=(const ColorValue& lhs, const ColorValue& rhs) noexcept
    {
        return !lhs.Equals(rhs);
    }

private:
    std::uint8_t _a = 0;
    std::uint8_t _r = 0;
    std::uint8_t _g = 0;
    std::uint8_t _b = 0;
};

}}}  // namespace Aspose::Cells_FOSS::Core

namespace std {

template<>
struct hash<Aspose::Cells_FOSS::Core::ColorValue> {
    std::size_t operator()(const Aspose::Cells_FOSS::Core::ColorValue& cv) const noexcept
    {
        return static_cast<std::size_t>(cv.GetHashCode());
    }
};

}  // namespace std
