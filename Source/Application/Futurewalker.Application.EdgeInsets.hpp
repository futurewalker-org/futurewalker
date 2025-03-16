// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.EdgeInsetsType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Describes finite insets from boundaries.
///
class EdgeInsets
{
public:
    ///
    /// @brief Make symmetric insets.
    ///
    /// @param horizontal Horizontal insets
    /// @param vertical Vertical insets
    ///
    static constexpr auto MakeSymmetric(Dp const horizontal, Dp const vertical) noexcept -> EdgeInsets
    {
        return EdgeInsets(horizontal, vertical, horizontal, vertical);
    }

    ///
    /// @brief Make uniform insets.
    ///
    /// @param value Uniform insets
    ///
    static constexpr auto MakeUniform(Dp const value) noexcept -> EdgeInsets
    {
        return EdgeInsets(value, value, value, value);
    }

public:
    constexpr EdgeInsets() noexcept = default;
    constexpr EdgeInsets(EdgeInsets const&) = default;

    ///
    /// @brief Construct from values.
    ///
    /// @param leading Leading isnet
    /// @param top Top inset
    /// @param trailing Trailing inset
    /// @param bottom Bottom inset
    ///
    constexpr EdgeInsets(Dp const leading, Dp const top, Dp const trailing, Dp const bottom) noexcept
    {
        SetLeading(leading);
        SetTop(top);
        SetTrailing(trailing);
        SetBottom(bottom);
    }

    ///
    /// @brief 
    ///
    constexpr auto GetLeading() const -> Dp
    {
        return _leading;
    }

    ///
    /// @brief 
    ///
    /// @param leading 
    ///
    constexpr auto SetLeading(Dp const leading) -> void
    {
        _leading = leading;
    }

    ///
    /// @brief 
    ///
    constexpr auto GetTop() const -> Dp
    {
        return _top;
    }

    ///
    /// @brief 
    ///
    /// @param top 
    ///
    constexpr auto SetTop(Dp const top) -> void
    {
        _top = top;
    }

    ///
    /// @brief 
    ///
    constexpr auto GetTrailing() const -> Dp
    {
        return _trailing;
    }

    ///
    /// @brief 
    ///
    /// @param trailing 
    ///
    constexpr auto SetTrailing(Dp const trailing) -> void
    {
        _trailing = trailing;
    }

    ///
    /// @brief 
    ///
    constexpr auto GetBottom() const -> Dp
    {
        return _bottom;
    }

    ///
    /// @brief 
    ///
    /// @param bottom 
    ///
    constexpr auto SetBottom(Dp const bottom) -> void
    {
        _bottom = bottom;
    }

    auto operator=(EdgeInsets const&) -> EdgeInsets& = default;
    auto operator==(EdgeInsets const&) const -> bool = default;

private:
    Dp _leading = 0.0;
    Dp _top = 0.0;
    Dp _trailing = 0.0;
    Dp _bottom = 0.0;
};
}
}
