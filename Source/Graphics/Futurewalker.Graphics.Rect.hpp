// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.RectType.hpp"
#include "Futurewalker.Graphics.Point.hpp"
#include "Futurewalker.Graphics.Size.hpp"
#include "Futurewalker.Graphics.Offset.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A specialization of Rect2 for Float values. 
///
/// @tparam T Floating point type
/// @tparam Tag Tag type
///
template <Concepts::FloatingPoint T, class Tag>
class Rect2<Float<T, Tag>>
{
public:
    ///
    /// @brief Value type
    ///
    using ValueType = Float<T, Tag>;

    ///
    /// @brief Infinite sized rectangle.
    ///
    [[nodiscard]]
    static inline constexpr auto Infinite() noexcept -> Rect2
    {
        const auto inf = ValueType::Infinity();
        return Rect2(-inf, -inf, inf, inf);
    }

    ///
    /// @brief Sort component along axis.
    ///
    /// @param rect Rectangle to sort
    ///
    /// @return Sorted rectangle
    ///
    [[nodiscard]]
    static inline constexpr auto Sort(const Rect2& rect) noexcept -> Rect2
    {
        const auto [l, r] = ValueType::MinMax(rect._l, rect._r);
        const auto [t, b] = ValueType::MinMax(rect._t, rect._b);
        return Rect2(l, t, r, b);
    }

    ///
    /// @brief Inflate rectangle.
    ///
    /// @param rect Rectangle to inflate
    /// @param dx Amount of inflation
    /// @param dy Amount of inflation
    ///
    /// @return Inflated rectangle
    ///
    [[nodiscard]]
    static inline constexpr auto Inflate(const Rect2& rect, const ValueType& dx, const ValueType& dy) noexcept -> Rect2
    {
        return Rect2(rect._l - dx, rect._t - dy, rect._r + dx, rect._b + dy);
    }

    ///
    /// @brief Offset rectangle.
    ///
    /// @param rect Rectangle to offset.
    /// @param offset Amount of offset.
    ///
    /// @return Offsetted rectangle
    ///
    [[nodiscard]]
    static inline constexpr auto Offset(const Rect2& rect, const Offset2<ValueType>& offset) noexcept -> Rect2
    {
        const auto dx = offset.GetDeltaX();
        const auto dy = offset.GetDeltaY();
        return Rect2(rect._l + dx, rect._t + dy, rect._r + dx, rect._b + dy);
    }

    ///
    /// @brief Intersect rectangle
    ///
    /// @param lhs Rectangle to intersect
    /// @param rhs Rectangle to intersect
    ///
    /// @return Intersection of two rectangles
    ///
    [[nodiscard]]
    static inline constexpr auto Intersect(const Rect2& lhs, const Rect2& rhs) noexcept -> Rect2
    {
        if (lhs.IsEmpty() || rhs.IsEmpty())
        {
            return Rect2();
        }
        return Rect2(ValueType::Max(lhs._l, rhs._l), ValueType::Max(lhs._t, rhs._t), ValueType::Min(lhs._r, rhs._r), ValueType::Min(lhs._b, rhs._b));
    }

    ///
    /// @brief Intersect rectangle and point
    ///
    /// @param lhs Rectangle
    /// @param rhs Point
    ///
    /// @return true if the point is inside rectangle
    ///
    [[nodiscard]]
    static inline constexpr auto Intersect(const Rect2& lhs, const Point<ValueType>& rhs) noexcept -> Bool
    {
        if (lhs.IsEmpty())
        {
            return false;
        }

        if ((lhs.GetLeft() <= rhs.GetX() && rhs.GetX() < lhs.GetRight()))
        {
            if ((lhs.GetTop() <= rhs.GetY() && rhs.GetY() < lhs.GetBottom()))
            {
                return true;
            }
        }
        return false;
    }

    ///
    /// @brief Union rectangle
    ///
    /// @param lhs Rectangle to union
    /// @param rhs Rectangle to union
    ///
    /// @return Union of two rectangles
    ///
    [[nodiscard]]
    static inline constexpr auto Union(const Rect2& lhs, const Rect2& rhs) noexcept -> Rect2
    {
        if (lhs.IsEmpty())
        {
            return rhs;
        }
        else if (rhs.IsEmpty())
        {
            return lhs;
        }
        return Rect2(ValueType::Min(lhs._l, rhs._l), ValueType::Min(lhs._t, rhs._t), ValueType::Max(lhs._r, rhs._r), ValueType::Max(lhs._b, rhs._b));
    }

public:
    ///
    /// @brief Default constructor.
    ///
    /// Initializes both Width and Height values to zero.
    ///
    inline constexpr Rect2() = default;

    ///
    /// @brief Copy constructor.
    ///
    inline constexpr Rect2(const Rect2&) = default;

    ///
    /// @brief Construct rectangle from LTRB values. 
    ///
    /// @param left Left
    /// @param top Top
    /// @param right Right
    /// @param bottom Bottom
    ///
    inline constexpr Rect2(const ValueType& left, const ValueType& top, const ValueType& right, const ValueType bottom)
      : _l {left}
      , _t {top}
      , _r {right}
      , _b {bottom}
    {
    }

    ///
    /// @brief Construct rectangle from position and size.
    ///
    /// @param pos Position of rectangle
    /// @param size Size of rectangle
    ///
    inline constexpr Rect2(const Point2<ValueType>& position, const Size2<ValueType>& size)
      : _l {position.GetX()}
      , _t {position.GetY()}
      , _r {_l + size.GetWidth()}
      , _b {_t + size.GetHeight()}
    {
    }

    ///
    /// @brief Copy assignment operator.
    ///
    inline constexpr auto operator=(const Rect2&) -> Rect2& = default;

    ///
    /// @brief Get left value.
    ///
    inline constexpr auto GetLeft() const noexcept -> const ValueType&
    {
        return _l;
    }

    ///
    /// @brief Set left value.
    ///
    inline constexpr void SetLeft(const ValueType& left) noexcept
    {
        _l = left;
    }

    ///
    /// @brief Get top value.
    ///
    inline constexpr auto GetTop() const noexcept -> const ValueType&
    {
        return _t;
    }

    ///
    /// @brief Set top value.
    ///
    inline constexpr void SetTop(const ValueType& top) noexcept
    {
        _t = top;
    }

    ///
    /// @brief Get right value.
    ///
    inline constexpr auto GetRight() const noexcept -> const ValueType&
    {
        return _r;
    }

    ///
    /// @brief Set right value.
    ///
    inline constexpr void SetRight(const ValueType& right) noexcept
    {
        _r = right;
    }

    ///
    /// @brief Get bottom value.
    ///
    inline constexpr auto GetBottom() const noexcept -> const ValueType&
    {
        return _b;
    }

    ///
    /// @brief Set bottom value.
    ///
    inline constexpr void SetBottom(const ValueType& bottom) noexcept
    {
        _b = bottom;
    }

    ///
    /// @brief Get width.
    ///
    inline constexpr auto GetWidth() const noexcept -> ValueType
    {
        return _r - _l;
    }

    ///
    /// @brief Get height.
    ///
    inline constexpr auto GetHeight() const noexcept -> ValueType
    {
        return _b - _t;
    }

    ///
    /// @brief Get size.
    ///
    inline constexpr auto GetSize() const noexcept -> Size2<ValueType>
    {
        return Size2<ValueType>(_r - _l, _b - _t);
    }

    ///
    /// @brief Set size.
    ///
    inline constexpr void SetSize(const Size2<ValueType>& size) noexcept
    {
        _r = _l + size.GetWidth();
        _b = _t + size.GetHeight();
    }

    ///
    /// @brief Get position.
    ///
    inline constexpr auto GetPosition() const noexcept -> Point2<ValueType>
    {
        return GetTopLeft();
    }

    ///
    /// @brief Set position.
    ///
    inline constexpr void SetPosition(const Point2<ValueType>& position) noexcept
    {
        SetTopLeft(position);
    }

    ///
    /// @brief Get top left point.
    ///
    inline constexpr auto GetTopLeft() const -> Point2<ValueType>
    {
        return Point2<ValueType>(_l, _t);
    }

    ///
    /// @brief Set top left point.
    ///
    inline constexpr void SetTopLeft(const Point2<ValueType>& topLeft) noexcept
    {
        _l = topLeft.GetX();
        _t = topLeft.GetY();
    }

    ///
    /// @brief Get top right point.
    ///
    inline constexpr auto GetTopRight() const -> Point2<ValueType>
    {
        return Point2<ValueType>(_r, _t);
    }

    ///
    /// @brief Set top right point.
    ///
    inline constexpr void SetTopRight(const Point2<ValueType>& topRight) noexcept
    {
        _r = topRight.GetX();
        _t = topRight.GetY();
    }

    ///
    /// @brief Get bottom left point.
    ///
    inline constexpr auto GetBottomLeft() const -> Point2<ValueType>
    {
        return Point2<ValueType>(_l, _b);
    }

    ///
    /// @brief Set bottom left point.
    ///
    inline constexpr void SetBottomLeft(const Point2<ValueType>& bottomLeft) noexcept
    {
        _l = bottomLeft.GetX();
        _b = bottomLeft.GetY();
    }

    ///
    /// @brief Get bottom right point.
    ///
    inline constexpr auto GetBottomRight() const -> Point2<ValueType>
    {
        return Point2<ValueType>(_r, _b);
    }

    ///
    /// @brief Set bottom right point.
    ///
    inline constexpr void SetBottomRight(const Point2<ValueType>& bottomRight) noexcept
    {
        _r = bottomRight.GetX();
        _b = bottomRight.GetY();
    }

    ///
    /// @brief Return true if the rectangle is empty.
    ///
    inline constexpr auto IsEmpty() const noexcept -> Bool
    {
        return !((_r - _l) > ValueType(0) && (_b - _t) > ValueType(0));
    }

    ///
    /// @brief Returns true if width is finite.
    ///
    inline constexpr auto IsWidthFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(_l) && ValueType::IsFinite(_r);
    }

    ///
    /// @brief Returns true if height is finite.
    ///
    inline constexpr auto IsHeightFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(_t) && ValueType::IsFinite(_b);
    }

    ///
    /// @brief Returns true if the rectangle is finite.
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return IsWidthFinite() && IsHeightFinite();
    }

    ///
    /// @brief Returns true if the rectangle is sorted.
    ///
    inline constexpr auto IsSorted() const noexcept -> Bool
    {
        return _l <= _r && _t <= _b;
    }

    friend inline constexpr bool operator==(const Rect2&, const Rect2&) = default;
    friend inline constexpr bool operator!=(const Rect2&, const Rect2&) = default;

private:
    ValueType _l = static_cast<T>(0);
    ValueType _t = static_cast<T>(0);
    ValueType _r = static_cast<T>(0);
    ValueType _b = static_cast<T>(0);
};
}
}
