#pragma once

#include "aspose/cells_foss/core/ColorValue.h"

#include <cstdint>

namespace Aspose {
namespace Cells_FOSS {

/// <summary>
/// Represents color.
/// </summary>
struct Color {
public:
    /// <summary>
    /// Initializes a new instance of the Color struct.
    /// </summary>
    constexpr Color() noexcept = default;

    /// <summary>
    /// Initializes a new instance of the Color struct.
    /// </summary>
    /// <param name="a">The a.</param>
    /// <param name="r">The r.</param>
    /// <param name="g">The g.</param>
    /// <param name="b">The b.</param>
    constexpr Color(std::uint8_t a, std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept
        : _a(a), _r(r), _g(g), _b(b) {}

    /// <summary>
    /// Gets the a.
    /// </summary>
    constexpr std::uint8_t GetA() const noexcept { return _a; }

    /// <summary>
    /// Gets the r.
    /// </summary>
    constexpr std::uint8_t GetR() const noexcept { return _r; }

    /// <summary>
    /// Gets the g.
    /// </summary>
    constexpr std::uint8_t GetG() const noexcept { return _g; }

    /// <summary>
    /// Gets the b.
    /// </summary>
    constexpr std::uint8_t GetB() const noexcept { return _b; }

    /// <summary>
    /// Gets the empty.
    /// </summary>
    static constexpr Color Empty() noexcept { return Color(0, 0, 0, 0); }

    /// <summary>
    /// Creates a color from ARGB components.
    /// </summary>
    /// <param name="a">The a.</param>
    /// <param name="r">The r.</param>
    /// <param name="g">The g.</param>
    /// <param name="b">The b.</param>
    /// <returns>The color.</returns>
    static constexpr Color FromArgb(int a, int r, int g, int b) noexcept
    {
        return Color(static_cast<std::uint8_t>(a),
                     static_cast<std::uint8_t>(r),
                     static_cast<std::uint8_t>(g),
                     static_cast<std::uint8_t>(b));
    }

    /// <summary>
    /// Determines whether the specified value is equal to the current instance.
    /// </summary>
    /// <param name="other">The other.</param>
    /// <returns>true if the condition is met; otherwise, false.</returns>
    constexpr bool Equals(const Color& other) const noexcept
    {
        return _a == other._a && _r == other._r && _g == other._g && _b == other._b;
    }

    /// <summary>
    /// Returns a hash code for the current instance.
    /// </summary>
    /// <returns>The hash code.</returns>
    int GetHashCode() const noexcept;

    /// <summary>
    /// Conversion to Core::ColorValue.
    /// </summary>
    Core::ColorValue ToCore() const noexcept;

    /// <summary>
    /// Creates a Color from a Core::ColorValue.
    /// </summary>
    static Color FromCore(const Core::ColorValue& value) noexcept;

    friend constexpr bool operator==(const Color& lhs, const Color& rhs) noexcept
    {
        return lhs.Equals(rhs);
    }

    friend constexpr bool operator!=(const Color& lhs, const Color& rhs) noexcept
    {
        return !lhs.Equals(rhs);
    }

private:
    std::uint8_t _a = 0;
    std::uint8_t _r = 0;
    std::uint8_t _g = 0;
    std::uint8_t _b = 0;
};

}}  // namespace Aspose::Cells_FOSS

namespace std {

template<>
struct hash<Aspose::Cells_FOSS::Color> {
    std::size_t operator()(const Aspose::Cells_FOSS::Color& c) const noexcept
    {
        return static_cast<std::size_t>(c.GetHashCode());
    }
};

}  // namespace std
