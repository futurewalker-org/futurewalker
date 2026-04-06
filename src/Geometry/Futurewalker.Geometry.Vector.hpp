// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Geometry.VectorType.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Vector class
///
/// @tparam T Floating point type
/// @tparam Tag Tag type
///
template <Concepts::FloatingPoint T, class Tag>
struct Vector2<Float<T, Tag>>
{
    ///
    /// @brief Value type.
    ///
    using ValueType = Float<T, Tag>;

    ///
    /// @brief Get minimum components.
    ///
    /// @param lhs Vector to compare
    /// @param rhs Vector to compare
    ///
    /// @return Vector of minimum components
    ///
    static inline constexpr auto Min(Vector2 const& lhs, Vector2 const& rhs) -> Vector2
    {
        return Vector2(ValueType::Min(lhs.x, rhs.x), ValueType::Min(lhs.y, rhs.y));
    }

    ///
    /// @brief Get maximum components.
    ///
    /// @param lhs Vector to compare
    /// @param rhs Vector to compare
    ///
    /// @return Vector of maximum components
    ///
    static inline constexpr auto Max(Vector2 const& lhs, Vector2 const& rhs) -> Vector2
    {
        return Vector2(ValueType::Max(lhs.x, rhs.x), ValueType::Max(lhs.y, rhs.y));
    }

    ///
    /// @brief Round components.
    ///
    /// @param offset Vector to round
    ///
    /// @return Rounded offset
    ///
    static inline constexpr auto Round(Vector2 const& offset) -> Vector2
    {
        return Vector2(ValueType::Round(offset.x), ValueType::Round(offset.y));
    }

    ///
    /// @brief Floor components.
    ///
    /// @param offset Vector to floor
    ///
    /// @return Floored offset
    ///
    static inline constexpr auto Floor(Vector2 const& offset) -> Vector2
    {
        return Vector2(ValueType::Floor(offset.x), ValueType::Floor(offset.y));
    }

    ///
    /// @brief Ceil components.
    ///
    /// @param offset Vector to ceil
    ///
    /// @return Ceiled offset
    ///
    static inline constexpr auto Ceil(Vector2 const& offset) -> Vector2
    {
        return Vector2(ValueType::Ceil(offset.x), ValueType::Ceil(offset.y));
    }

    ///
    /// @brief Returns true if the offset is finite.
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(x) && ValueType::IsFinite(y);
    }

    ///
    /// @brief operator==
    ///
    friend inline constexpr bool operator==(Vector2 const& lhs, Vector2 const& rhs) noexcept
    {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }

    ///
    /// @brief operator!=
    ///
    friend inline constexpr bool operator!=(Vector2 const& lhs, Vector2 const& rhs) noexcept
    {
        return (lhs.x != rhs.x) || (lhs.y != rhs.y);
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator+=(Vector2 const& offset) noexcept -> Vector2&
    {
        x += offset.x;
        y += offset.y;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-=(Vector2 const& offset) noexcept -> Vector2&
    {
        x -= offset.x;
        y -= offset.y;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-() const noexcept -> Vector2
    {
        return Vector2(-x, -y);
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator+() const noexcept -> Vector2
    {
        return Vector2(+x, +y);
    }

    friend inline constexpr auto operator+(Vector2 const& l, Vector2 const& r) noexcept
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    friend inline constexpr auto operator-(Vector2 const& l, Vector2 const& r) noexcept
    {
        auto tmp = l;
        tmp -= r;
        return tmp;
    }

    ///
    /// @brief Convert Point to other geometry types.
    ///
    template <template <class> class U>
    auto As() const -> U<ValueType>
    {
        return U<ValueType>(x, y);
    }

    ///
    /// @brief X component.
    ///
    ValueType x = static_cast<T>(0);

    ///
    /// @brief Y component.
    ///
    ValueType y = static_cast<T>(0);
};
}
}
