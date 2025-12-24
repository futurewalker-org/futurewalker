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
class CornerRadius
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
        return CornerRadius(radius._tl + offset, radius._tt + offset, radius._bt + offset, radius._bl + offset);
    }

public:
    constexpr CornerRadius() = default;
    constexpr CornerRadius(CornerRadius const&) = default;

    ///
    /// @brief Construct from radius 
    ///
    /// @param tl Top left radius
    /// @param tt Top trailing radius
    /// @param bt Bottom trailing radius
    /// @param bl Bottom leading radius
    ///
    constexpr CornerRadius(Dp const tl, Dp const tt, Dp const bt, Dp const bl)
      : _tl {tl}
      , _tt {tt}
      , _bt {bt}
      , _bl {bl}
    {
        _tl = Dp::Max(_tl, 0);
        _tt = Dp::Max(_tt, 0);
        _bt = Dp::Max(_bt, 0);
        _bl = Dp::Max(_bl, 0);
    }

    constexpr auto operator=(CornerRadius const&) -> CornerRadius& = default;
    constexpr auto operator==(CornerRadius const&) const -> bool = default;

    ///
    /// @brief 
    ///
    constexpr auto GetTopLeading() const -> Dp
    {
        return _tl;
    }

    ///
    /// @brief 
    ///
    /// @param tl 
    ///
    constexpr auto SetTopLeading(Dp const tl) -> void
    {
        _tl = tl;
    }

    ///
    /// @brief 
    ///
    constexpr auto GetTopTrailing() const -> Dp
    {
        return _tt;
    }

    ///
    /// @brief 
    ///
    /// @param tt 
    ///
    constexpr auto SetTopTrailing(Dp const tt) -> void
    {
        _tt = tt;
    }

    ///
    /// @brief 
    ///
    constexpr auto GetBottomLeading() const -> Dp
    {
        return _bl;
    }

    ///
    /// @brief 
    ///
    /// @param bl 
    ///
    constexpr auto SetBottomLeading(Dp const bl) -> void
    {
        _bl = bl;
    }

    ///
    /// @brief 
    ///
    constexpr auto GetBottomTrailing() const -> Dp
    {
        return _bt;
    }

    ///
    /// @brief 
    ///
    /// @param bt 
    ///
    constexpr auto SetBottomTrailing(Dp const bt) -> void
    {
        _bt = bt;
    }

    ///
    /// @brief Get top left radius.
    ///
    /// @param[in] direction Layout direction.
    ///
    constexpr auto GetTopLeft(LayoutDirection const& direction) const -> Dp
    {
        return direction == LayoutDirection::RightToLeft ? _tt : _tl;
    }

    ///
    /// @brief Get top right radius.
    ///
    /// @param[in] direction Layout direction.
    ///
    constexpr auto GetTopRight(LayoutDirection const& direction) const -> Dp
    {
        return direction == LayoutDirection::RightToLeft ? _tl : _tt;
    }

    ///
    /// @brief Get top left radius.
    ///
    /// @param[in] direction Layout direction.
    ///
    constexpr auto GetBottomLeft(LayoutDirection const& direction) const -> Dp
    {
        return direction == LayoutDirection::RightToLeft ? _bt : _bl;
    }

    ///
    /// @brief Get top right radius.
    ///
    /// @param[in] direction Layout direction.
    ///
    constexpr auto GetBottomRight(LayoutDirection const& direction) const -> Dp
    {
        return direction == LayoutDirection::RightToLeft ? _bl : _bt;
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

private:
    Dp _tl = 0;
    Dp _tt = 0;
    Dp _bt = 0;
    Dp _bl = 0;
};
}
}
