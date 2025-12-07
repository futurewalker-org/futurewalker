// SPDX-License-Identifier: MPL-2.0
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

    ///
    /// @brief Choose larger insets from two EdgeInsets objects.
    ///
    static constexpr auto Max(EdgeInsets const& lhs, EdgeInsets const& rhs) -> EdgeInsets
    {
        return EdgeInsets(Dp::Max(lhs._leading, rhs._leading), Dp::Max(lhs._top, rhs._top), Dp::Max(lhs._trailing, rhs._trailing), Dp::Max(lhs._bottom, rhs._bottom));
    }

    ///
    /// @brief Choose smaller insets from two EdgeInsets objects.
    ///
    static constexpr auto Min(EdgeInsets const& lhs, EdgeInsets const& rhs) -> EdgeInsets
    {
        return EdgeInsets(Dp::Min(lhs._leading, rhs._leading), Dp::Min(lhs._top, rhs._top), Dp::Min(lhs._trailing, rhs._trailing), Dp::Min(lhs._bottom, rhs._bottom));
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

    ///
    /// @brief Extract horizontal insets (leading and trailing).
    ///
    constexpr auto GetHorizontal() const -> EdgeInsets
    {
        return EdgeInsets(_leading, 0.0, _trailing, 0.0);
    }

    ///
    /// @brief Extract vertical insets (top and bottom).
    ///
    constexpr auto GetVertical() const -> EdgeInsets
    {
        return EdgeInsets(0.0, _top, 0.0, _bottom);
    }

    ///
    /// @brief Get total horizontal width.
    ///
    constexpr auto GetTotalWidth() const -> Dp
    {
        return _leading + _trailing;
    }

    ///
    /// @brief Get total vertical height.
    ///
    constexpr auto GetTotalHeight() const -> Dp
    {
        return _top + _bottom;
    }

    ///
    /// @brief
    ///
    constexpr auto operator-() const noexcept -> EdgeInsets
    {
        return EdgeInsets(-_leading, -_top, -_trailing, -_bottom);
    }

    ///
    /// @brief 
    ///
    constexpr auto operator+() const noexcept -> EdgeInsets
    {
        return EdgeInsets(+_leading, +_top, +_trailing, +_bottom);
    }

    ///
    /// @brief 
    ///
    constexpr auto operator+=(EdgeInsets const& x) noexcept -> EdgeInsets&
    {
        _leading += x._leading;
        _top += x._top;
        _trailing += x._trailing;
        _bottom += x._bottom;
        return *this;
    }

    ///
    /// @brief 
    ///
    constexpr auto operator-=(EdgeInsets const& x) noexcept -> EdgeInsets&
    {
        _leading -= x._leading;
        _top -= x._top;
        _trailing -= x._trailing;
        _bottom -= x._bottom;
        return *this;
    }

    ///
    /// @brief 
    ///
    friend constexpr auto operator+(EdgeInsets const& l, EdgeInsets const& r) noexcept -> EdgeInsets
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    ///
    /// @brief 
    ///
    friend constexpr auto operator-(EdgeInsets const& l, EdgeInsets const& r) noexcept -> EdgeInsets
    {
        auto tmp = l;
        tmp -= r;
        return tmp;
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
