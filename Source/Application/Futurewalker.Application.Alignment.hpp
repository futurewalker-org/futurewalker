// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.AlignmentType.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Represents alignment in a rectangle.
///
/// An alignment is represented by two values, x and y, both of which range from -1.0 to 1.0.
///
/// @note The horizontal component is dependent on the layout direction.
///
class Alignment
{
public:
    // clang-format off
    static constexpr auto TopLeading() noexcept { return Alignment(-1.0, -1.0); }
    static constexpr auto TopCenter() noexcept { return Alignment(0.0, -1.0); } 
    static constexpr auto TopTrailing() noexcept { return Alignment(1.0, -1.0); }
    static constexpr auto MiddleLeading() noexcept { return Alignment(-1.0, 0.0); }
    static constexpr auto Center() noexcept { return Alignment(0.0, 0.0); }
    static constexpr auto MiddleTrailing() noexcept { return Alignment(1.0, 0.0); }
    static constexpr auto BottomLeading() noexcept { return Alignment(-1.0, 1.0); }
    static constexpr auto BottomCenter() noexcept { return Alignment(0.0, 1.0); }
    static constexpr auto BottomTrailing() noexcept { return Alignment(1.0, 1.0); }
    // clang-format on

public:
    constexpr Alignment(Float64 const x, Float64 const y) noexcept
    {
        SetX(x);
        SetY(y);
    }

    constexpr auto GetX() const noexcept -> Float64
    {
        return _x;
    }

    constexpr auto SetX(Float64 const x) noexcept -> void
    {
        if (Float64::IsFinite(x))
        {
            _x = x;
        }
    }

    constexpr auto GetY() const noexcept -> Float64
    {
        return _y;
    }

    constexpr auto SetY(Float64 const y) noexcept -> void
    {
        if (Float64::IsFinite(y))
        {
            _y = y;
        }
    }

    auto operator==(Alignment const&) const -> bool = default;
    auto operator!=(Alignment const&) const -> bool = default;

private:
    Float64 _x = 0.0;
    Float64 _y = 0.0;
};
}
}
