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
struct EdgeInsets
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
        return EdgeInsets(Dp::Max(lhs.leading, rhs.leading), Dp::Max(lhs.top, rhs.top), Dp::Max(lhs.trailing, rhs.trailing), Dp::Max(lhs.bottom, rhs.bottom));
    }

    ///
    /// @brief Choose smaller insets from two EdgeInsets objects.
    ///
    static constexpr auto Min(EdgeInsets const& lhs, EdgeInsets const& rhs) -> EdgeInsets
    {
        return EdgeInsets(Dp::Min(lhs.leading, rhs.leading), Dp::Min(lhs.top, rhs.top), Dp::Min(lhs.trailing, rhs.trailing), Dp::Min(lhs.bottom, rhs.bottom));
    }

public:
    ///
    /// @brief Extract horizontal insets (leading and trailing).
    ///
    constexpr auto GetHorizontal() const -> EdgeInsets
    {
        return EdgeInsets(leading, 0.0, trailing, 0.0);
    }

    ///
    /// @brief Extract vertical insets (top and bottom).
    ///
    constexpr auto GetVertical() const -> EdgeInsets
    {
        return EdgeInsets(0.0, top, 0.0, bottom);
    }

    ///
    /// @brief Get total horizontal width.
    ///
    constexpr auto GetTotalWidth() const -> Dp
    {
        return leading + trailing;
    }

    ///
    /// @brief Get total vertical height.
    ///
    constexpr auto GetTotalHeight() const -> Dp
    {
        return top + bottom;
    }

    ///
    /// @brief
    ///
    constexpr auto operator-() const noexcept -> EdgeInsets
    {
        return EdgeInsets(-leading, -top, -trailing, -bottom);
    }

    ///
    /// @brief 
    ///
    constexpr auto operator+() const noexcept -> EdgeInsets
    {
        return EdgeInsets(+leading, +top, +trailing, +bottom);
    }

    ///
    /// @brief 
    ///
    constexpr auto operator+=(EdgeInsets const& x) noexcept -> EdgeInsets&
    {
        leading += x.leading;
        top += x.top;
        trailing += x.trailing;
        bottom += x.bottom;
        return *this;
    }

    ///
    /// @brief 
    ///
    constexpr auto operator-=(EdgeInsets const& x) noexcept -> EdgeInsets&
    {
        leading -= x.leading;
        top -= x.top;
        trailing -= x.trailing;
        bottom -= x.bottom;
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

    auto operator==(EdgeInsets const&) const -> bool = default;

    Dp leading = 0.0;
    Dp top = 0.0;
    Dp trailing = 0.0;
    Dp bottom = 0.0;
};
}
}
