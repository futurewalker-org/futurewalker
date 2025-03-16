// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.CornerRadiusType.hpp"

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
    /// @param radius Uniform radius
    ///
    static constexpr auto MakeUniform(Dp const radius) noexcept -> CornerRadius
    {
        return CornerRadius(radius, radius, radius, radius);
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

private:
    Dp _tl = 0;
    Dp _tt = 0;
    Dp _bt = 0;
    Dp _bl = 0;
};
}
}
