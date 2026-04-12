// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.CornerRadiusType.hpp"
#include "Futurewalker.Application.LayoutDirection.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Corner radius.
///
struct CornerRadius
{
public:
    ///
    /// @brief Make uniform corner radius.
    ///
    /// @param[in] radius Uniform radius
    ///
    static constexpr auto MakeUniform(Dp const radius) noexcept -> CornerRadius
    {
        return CornerRadius(radius, radius, radius, radius);
    }

    ///
    /// @brief Make largest possible corner radius.
    ///
    static constexpr auto MakeLargest() noexcept -> CornerRadius
    {
        return MakeUniform(Dp::Infinity());
    }

    ///
    /// @brief Offset corner radius by a given value.
    ///
    /// @param[in] radius The original corner radius
    /// @param[in] offset The offset value to add
    ///
    static constexpr auto Offset(CornerRadius const& radius, Dp const offset) noexcept -> CornerRadius
    {
        return CornerRadius(radius.topLeading + offset, radius.topTrailing + offset, radius.bottomTrailing + offset, radius.bottomLeading + offset);
    }

public:
    constexpr CornerRadius() = default;
    constexpr CornerRadius(CornerRadius const&) = default;

    ///
    /// @brief Construct from radius 
    ///
    /// @param tl Top leading radius
    /// @param tt Top trailing radius
    /// @param bt Bottom trailing radius
    /// @param bl Bottom leading radius
    ///
    constexpr CornerRadius(Dp const tl, Dp const tt, Dp const bt, Dp const bl)
      : topLeading {Dp::Max(tl, 0)}
      , topTrailing {Dp::Max(tt, 0)}
      , bottomTrailing {Dp::Max(bt, 0)}
      , bottomLeading {Dp::Max(bl, 0)}
    {
    }

    constexpr auto operator=(CornerRadius const&) -> CornerRadius& = default;
    constexpr auto operator==(CornerRadius const&) const -> bool = default;

    ///
    /// @brief Get top left radius.
    ///
    /// @param[in] direction Layout direction.
    ///
    constexpr auto GetTopLeft(LayoutDirection const& direction) const -> Dp
    {
        return direction == LayoutDirection::RightToLeft ? topTrailing : topLeading;
    }

    ///
    /// @brief Get top right radius.
    ///
    /// @param[in] direction Layout direction.
    ///
    constexpr auto GetTopRight(LayoutDirection const& direction) const -> Dp
    {
        return direction == LayoutDirection::RightToLeft ? topLeading : topTrailing;
    }

    ///
    /// @brief Get bottom left radius.
    ///
    /// @param[in] direction Layout direction.
    ///
    constexpr auto GetBottomLeft(LayoutDirection const& direction) const -> Dp
    {
        return direction == LayoutDirection::RightToLeft ? bottomTrailing : bottomLeading;
    }

    ///
    /// @brief Get bottom right radius.
    ///
    /// @param[in] direction Layout direction.
    ///
    constexpr auto GetBottomRight(LayoutDirection const& direction) const -> Dp
    {
        return direction == LayoutDirection::RightToLeft ? bottomLeading : bottomTrailing;
    }

    ///
    /// @brief Get round rectangle.
    ///
    /// @param[in] rect Rectangle to create round rectangle from.
    /// @param[in] direction Layout direction.
    ///
    /// @note If the corner radius is too large to fit in the rectangle, it will be clamped to half of the rectangle's width or height.
    ///
    constexpr auto GetRoundRect(Rect<Dp> const& rect, LayoutDirection const& direction) const -> RoundRect<Dp>
    {
        auto const maxRadius = Dp::Min(rect.GetWidth(), rect.GetHeight()) / 2;
        auto const tl = Dp::Min(GetTopLeft(direction), maxRadius);
        auto const tr = Dp::Min(GetTopRight(direction), maxRadius);
        auto const br = Dp::Min(GetBottomRight(direction), maxRadius);
        auto const bl = Dp::Min(GetBottomLeft(direction), maxRadius);
        return RoundRect<Dp>(rect, {tl, tl}, {tr, tr}, {br, br}, {bl, bl});
    }

    Dp topLeading = 0;
    Dp topTrailing = 0;
    Dp bottomTrailing = 0;
    Dp bottomLeading = 0;
};
}
}
