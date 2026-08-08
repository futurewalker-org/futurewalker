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
struct Vector3<Float<T, Tag>>
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
    static inline constexpr auto Min(Vector3 const& lhs, Vector3 const& rhs) -> Vector3
    {
        return Vector3(ValueType::Min(lhs.x, rhs.x), ValueType::Min(lhs.y, rhs.y), ValueType::Min(lhs.z, rhs.z));
    }

    ///
    /// @brief Get maximum components.
    ///
    /// @param lhs Vector to compare
    /// @param rhs Vector to compare
    ///
    /// @return Vector of maximum components
    ///
    static inline constexpr auto Max(Vector3 const& lhs, Vector3 const& rhs) -> Vector3
    {
        return Vector3(ValueType::Max(lhs.x, rhs.x), ValueType::Max(lhs.y, rhs.y), ValueType::Max(lhs.z, rhs.z));
    }

    ///
    /// @brief Round components.
    ///
    /// @param offset Vector to round
    ///
    /// @return Rounded offset
    ///
    static inline constexpr auto Round(Vector3 const& offset) -> Vector3
    {
        return Vector3(ValueType::Round(offset.x), ValueType::Round(offset.y), ValueType::Round(offset.z));
    }

    ///
    /// @brief Floor components.
    ///
    /// @param offset Vector to floor
    ///
    /// @return Floored offset
    ///
    static inline constexpr auto Floor(Vector3 const& offset) -> Vector3
    {
        return Vector3(ValueType::Floor(offset.x), ValueType::Floor(offset.y), ValueType::Floor(offset.z));
    }

    ///
    /// @brief Ceil components.
    ///
    /// @param offset Vector to ceil
    ///
    /// @return Ceiled offset
    ///
    static inline constexpr auto Ceil(Vector3 const& offset) -> Vector3
    {
        return Vector3(ValueType::Ceil(offset.x), ValueType::Ceil(offset.y), ValueType::Ceil(offset.z));
    }

    ///
    /// @brief Returns true if the offset is finite.
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(x) && ValueType::IsFinite(y) && ValueType::IsFinite(z);
    }

    ///
    /// @brief operator==
    ///
    friend inline constexpr bool operator==(Vector3 const& lhs, Vector3 const& rhs) noexcept = default;

    ///
    /// @brief operator!=
    ///
    friend inline constexpr bool operator!=(Vector3 const& lhs, Vector3 const& rhs) noexcept = default;

    ///
    /// @brief
    ///
    inline constexpr auto operator+=(Vector3 const& offset) noexcept -> Vector3&
    {
        x += offset.x;
        y += offset.y;
        z += offset.z;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-=(Vector3 const& offset) noexcept -> Vector3&
    {
        x -= offset.x;
        y -= offset.y;
        z -= offset.z;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-() const noexcept -> Vector3
    {
        return Vector3(-x, -y, -z);
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator+() const noexcept -> Vector3
    {
        return Vector3(+x, +y, +z);
    }

    friend inline constexpr auto operator+(Vector3 const& l, Vector3 const& r) noexcept
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    friend inline constexpr auto operator-(Vector3 const& l, Vector3 const& r) noexcept
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
        return U<ValueType>(x, y, z);
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
};
}
}
