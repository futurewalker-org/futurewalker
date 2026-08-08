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
struct Vector5<Float<T, Tag>>
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
    static inline constexpr auto Min(Vector5 const& lhs, Vector5 const& rhs) -> Vector5
    {
        return Vector5(
            ValueType::Min(lhs.x, rhs.x), ValueType::Min(lhs.y, rhs.y), ValueType::Min(lhs.z, rhs.z),
            ValueType::Min(lhs.w, rhs.w), ValueType::Min(lhs.v, rhs.v));
    }

    ///
    /// @brief Get maximum components.
    ///
    /// @param lhs Vector to compare
    /// @param rhs Vector to compare
    ///
    /// @return Vector of maximum components
    ///
    static inline constexpr auto Max(Vector5 const& lhs, Vector5 const& rhs) -> Vector5
    {
        return Vector5(
            ValueType::Max(lhs.x, rhs.x), ValueType::Max(lhs.y, rhs.y), ValueType::Max(lhs.z, rhs.z),
            ValueType::Max(lhs.w, rhs.w), ValueType::Max(lhs.v, rhs.v));
    }

    ///
    /// @brief Round components.
    ///
    /// @param offset Vector to round
    ///
    /// @return Rounded offset
    ///
    static inline constexpr auto Round(Vector5 const& offset) -> Vector5
    {
        return Vector5(
            ValueType::Round(offset.x), ValueType::Round(offset.y), ValueType::Round(offset.z),
            ValueType::Round(offset.w), ValueType::Round(offset.v));
    }

    ///
    /// @brief Floor components.
    ///
    /// @param offset Vector to floor
    ///
    /// @return Floored offset
    ///
    static inline constexpr auto Floor(Vector5 const& offset) -> Vector5
    {
        return Vector5(
            ValueType::Floor(offset.x), ValueType::Floor(offset.y), ValueType::Floor(offset.z),
            ValueType::Floor(offset.w), ValueType::Floor(offset.v));
    }

    ///
    /// @brief Ceil components.
    ///
    /// @param offset Vector to ceil
    ///
    /// @return Ceiled offset
    ///
    static inline constexpr auto Ceil(Vector5 const& offset) -> Vector5
    {
        return Vector5(
            ValueType::Ceil(offset.x), ValueType::Ceil(offset.y), ValueType::Ceil(offset.z),
            ValueType::Ceil(offset.w), ValueType::Ceil(offset.v));
    }

    ///
    /// @brief Returns true if the offset is finite.
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(x) && ValueType::IsFinite(y) && ValueType::IsFinite(z) && ValueType::IsFinite(w) && ValueType::IsFinite(v);
    }

    ///
    /// @brief operator==
    ///
    friend inline constexpr bool operator==(Vector5 const& lhs, Vector5 const& rhs) noexcept = default;

    ///
    /// @brief operator!=
    ///
    friend inline constexpr bool operator!=(Vector5 const& lhs, Vector5 const& rhs) noexcept = default;

    ///
    /// @brief
    ///
    inline constexpr auto operator+=(Vector5 const& offset) noexcept -> Vector5&
    {
        x += offset.x;
        y += offset.y;
        z += offset.z;
        w += offset.w;
        v += offset.v;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-=(Vector5 const& offset) noexcept -> Vector5&
    {
        x -= offset.x;
        y -= offset.y;
        z -= offset.z;
        w -= offset.w;
        v -= offset.v;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-() const noexcept -> Vector5
    {
        return Vector5(-x, -y, -z, -w, -v);
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator+() const noexcept -> Vector5
    {
        return Vector5(+x, +y, +z, +w, +v);
    }

    friend inline constexpr auto operator+(Vector5 const& l, Vector5 const& r) noexcept
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    friend inline constexpr auto operator-(Vector5 const& l, Vector5 const& r) noexcept
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
        return U<ValueType>(x, y, z, w, v);
    }

    ///
    /// @brief X component.
    ///
    ValueType x = static_cast<T>(0);

    ///
    /// @brief Y component.
    ///
    ValueType y = static_cast<T>(0);

    ///
    /// @brief Z component.
    ///
    ValueType z = static_cast<T>(0);

    ///
    /// @brief W component.
    ///
    ValueType w = static_cast<T>(0);

    ///
    /// @brief V component.
    ///
    ValueType v = static_cast<T>(0);
};
}
}
