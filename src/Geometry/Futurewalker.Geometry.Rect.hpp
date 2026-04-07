// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Geometry.RectType.hpp"
#include "Futurewalker.Geometry.Point.hpp"
#include "Futurewalker.Geometry.Size.hpp"
#include "Futurewalker.Geometry.Vector.hpp"

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
struct Rect2<Float<T, Tag>>
{
public:
    ///
    /// @brief Value type
    ///
    using ValueType = Float<T, Tag>;

    ///
    /// @brief Construct rectangle from position and size.
    ///
    /// @param position Position of rectangle
    /// @param size Size of rectangle
    ///
    [[nodiscard]]
    static inline constexpr auto Make(Point2<ValueType> const& position, Size2<ValueType> const& size) noexcept -> Rect2
    {
        return Rect2(position.x, position.y, position.x + size.width, position.y + size.height);
    }

    ///
    /// @brief Infinite sized rectangle.
    ///
    [[nodiscard]]
    static inline constexpr auto Infinite() noexcept -> Rect2
    {
        auto const inf = ValueType::Infinity();
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
    static inline constexpr auto Sort(Rect2 const& rect) noexcept -> Rect2
    {
        auto const [x0, x1] = ValueType::MinMax(rect.x0, rect.x1);
        auto const [y0, y1] = ValueType::MinMax(rect.y0, rect.y1);
        return Rect2(x0, y0, x1, y1);
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
    static inline constexpr auto Inflate(Rect2 const& rect, ValueType const& dx, ValueType const& dy) noexcept -> Rect2
    {
        return Rect2(rect.x0 - dx, rect.y0 - dy, rect.x1 + dx, rect.y1 + dy);
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
    static inline constexpr auto Offset(Rect2 const& rect, Vector2<ValueType> const& offset) noexcept -> Rect2
    {
        return Rect2(rect.x0 + offset.x, rect.y0 + offset.y, rect.x1 + offset.x, rect.y1 + offset.y);
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
    static inline constexpr auto Intersect(Rect2 const& lhs, Rect2 const& rhs) noexcept -> Rect2
    {
        if (lhs.IsEmpty() || rhs.IsEmpty())
        {
            return Rect2();
        }
        return Rect2(ValueType::Max(lhs.x0, rhs.x0), ValueType::Max(lhs.y0, rhs.y0), ValueType::Min(lhs.x1, rhs.x1), ValueType::Min(lhs.y1, rhs.y1));
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
    static inline constexpr auto Intersect(Rect2 const& lhs, Point<ValueType> const& rhs) noexcept -> Bool
    {
        if (lhs.IsEmpty())
        {
            return false;
        }

        if ((lhs.x0 <= rhs.x && rhs.x < lhs.x1))
        {
            if ((lhs.y0 <= rhs.y && rhs.y < lhs.y1))
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
    static inline constexpr auto Union(Rect2 const& lhs, Rect2 const& rhs) noexcept -> Rect2
    {
        if (lhs.IsEmpty())
        {
            return rhs;
        }
        else if (rhs.IsEmpty())
        {
            return lhs;
        }
        return Rect2(ValueType::Min(lhs.x0, rhs.x0), ValueType::Min(lhs.y0, rhs.y0), ValueType::Max(lhs.x1, rhs.x1), ValueType::Max(lhs.y1, rhs.y1));
    }

public:
    ///
    /// @brief Get width.
    ///
    inline constexpr auto GetWidth() const noexcept -> ValueType
    {
        return x1 - x0;
    }

    ///
    /// @brief Get height.
    ///
    inline constexpr auto GetHeight() const noexcept -> ValueType
    {
        return y1 - y0;
    }

    ///
    /// @brief Get size.
    ///
    inline constexpr auto GetSize() const noexcept -> Size2<ValueType>
    {
        return Size2<ValueType>(GetWidth(), GetHeight());
    }

    ///
    /// @brief Set size.
    ///
    inline constexpr auto SetSize(Size2<ValueType> const& size) noexcept -> void
    {
        x1 = x0 + size.width;
        y1 = y0 + size.height;
    }

    ///
    /// @brief Get position.
    ///
    inline constexpr auto GetPosition() const noexcept -> Point2<ValueType>
    {
        return GetCorner<0, 0>();
    }

    ///
    /// @brief Set position.
    ///
    inline constexpr auto SetPosition(Point2<ValueType> const& position) noexcept -> void
    {
        SetCorner<0, 0>(position);
    }

    ///
    /// @brief Get corner point.
    ///
    /// @tparam X X coordinate index
    /// @tparam Y Y coordinate index
    ///
    /// @return Corner point
    ///
    template <size_t X, size_t Y>
    inline constexpr auto GetCorner() const noexcept -> Point2<ValueType>
    {
        static_assert(X < 2 && Y < 2);
        return Point2<ValueType>(X == 0 ? x0 : x1, Y == 0 ? y0 : y1);
    }

    ///
    /// @brief Set corner point.
    ///
    /// @tparam X X coordinate index
    /// @tparam Y Y coordinate index
    ///
    /// @param corner Corner point
    ///
    template <size_t X, size_t Y>
    inline constexpr auto SetCorner(Point2<ValueType> const& corner) noexcept -> void
    {
        static_assert(X < 2 && Y < 2);
        (X == 0 ? x0 : x1) = corner.x;
        (Y == 0 ? y0 : y1) = corner.y;
    }

    ///
    /// @brief Return true if the rectangle is empty.
    ///
    inline constexpr auto IsEmpty() const noexcept -> Bool
    {
        return !((x1 - x0) > ValueType(0) && (y1 - y0) > ValueType(0));
    }

    ///
    /// @brief Returns true if width is finite.
    ///
    inline constexpr auto IsWidthFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(x0) && ValueType::IsFinite(x1);
    }

    ///
    /// @brief Returns true if height is finite.
    ///
    inline constexpr auto IsHeightFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(y0) && ValueType::IsFinite(y1);
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
        return x0 <= x1 && y0 <= y1;
    }

    friend inline constexpr bool operator==(Rect2 const&, Rect2 const&) = default;
    friend inline constexpr bool operator!=(Rect2 const&, Rect2 const&) = default;

    ValueType x0 = static_cast<T>(0);
    ValueType y0 = static_cast<T>(0);
    ValueType x1 = static_cast<T>(0);
    ValueType y1 = static_cast<T>(0);
};
}
}
