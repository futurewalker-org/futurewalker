// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Geometry.RoundRectType.hpp"
#include "Futurewalker.Geometry.Rect.hpp"
#include "Futurewalker.Geometry.Radius.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A specialization of RoundRect2 for Float values. 
///
/// @tparam T Floating point type.
/// @tparam Tag Tag type.
///
template <Concepts::FloatingPoint T, class Tag>
class RoundRect2<Float<T, Tag>>
{
public:
    ///
    /// @brief 
    ///
    using ValueType = Float<T, Tag>;

    ///
    /// @brief
    ///
    using RectType = Rect2<ValueType>;

    ///
    /// @brief
    ///
    using RadiusType = Radius2<ValueType>;

public:
    ///
    /// @brief Default constructor.
    ///
    /// Initializes both Width and Height values to zero.
    ///
    inline constexpr RoundRect2() = default;

    ///
    /// @brief Copy constructor.
    ///
    inline constexpr RoundRect2(RoundRect2 const&) = default;

    ///
    /// @brief Construct rounded rectangle.
    ///
    /// @param rect Rectangle.
    /// @param tl Top left corner radius.
    /// @param tr Top right corner radius.
    /// @param br Bottom right corner radius.
    /// @param bl Bottom left corner radius.
    ///
    inline constexpr RoundRect2(RectType const& rect, RadiusType const& tl, RadiusType const& tr, RadiusType const& br, RadiusType const& bl)
      : _rect {rect}
      , _tl {tl}
      , _tr {tr}
      , _br {br}
      , _bl {bl}
    {
    }

    ///
    /// @brief Copy assignment operator.
    ///
    inline constexpr auto operator=(RoundRect2 const&) -> RoundRect2& = default;

    ///
    /// @brief 
    ///
    inline constexpr auto GetRect() const noexcept -> RectType
    {
        return _rect;
    }

    ///
    /// @brief
    ///
    /// @param rect
    ///
    inline constexpr auto SetRect(RectType const& rect) noexcept -> void
    {
        _rect = rect;
    }

    ///
    /// @brief 
    ///
    inline constexpr auto GetTopLeftRadius() const noexcept -> RadiusType
    {
        return _tl;
    }

    ///
    /// @brief 
    ///
    /// @param tl 
    ///
    inline constexpr auto SetTopLeftRadius(RadiusType const& tl) noexcept -> void
    {
        _tl = tl;
    }

    ///
    /// @brief 
    ///
    inline constexpr auto GetTopRightRadius() const noexcept -> RadiusType
    {
        return _tr;
    }

    ///
    /// @brief 
    ///
    /// @param tr 
    ///
    inline constexpr auto SetTopRightRadius(RadiusType const& tr) noexcept -> void
    {
        _tr = tr;
    }

    ///
    /// @brief 
    ///
    inline constexpr auto GetBottomRightRadius() const noexcept -> RadiusType
    {
        return _br;
    }

    ///
    /// @brief 
    ///
    /// @param br 
    ///
    inline constexpr auto SetBottomRightRadius(RadiusType const& br) noexcept -> void
    {
        _br = br;
    }

    ///
    /// @brief 
    ///
    inline constexpr auto GetBottomLeftRadius() const noexcept -> RadiusType
    {
        return _bl;
    }

    ///
    /// @brief 
    ///
    /// @param bl 
    ///
    inline constexpr auto SetBottomLeftRadius(RadiusType const& bl) noexcept -> void
    {
        _bl = bl;
    }

    friend inline constexpr bool operator==(RoundRect2 const&, RoundRect2 const&) = default;
    friend inline constexpr bool operator!=(RoundRect2 const&, RoundRect2 const&) = default;

private:
    RectType _rect;
    RadiusType _tl;
    RadiusType _tr;
    RadiusType _br;
    RadiusType _bl;
};
}
}
