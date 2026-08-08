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
struct Vector4<Float<T, Tag>>
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
    static inline constexpr auto Min(Vector4 const& lhs, Vector4 const& rhs) -> Vector4
    {
        return Vector4(ValueType::Min(lhs.x, rhs.x), ValueType::Min(lhs.y, rhs.y), ValueType::Min(lhs.z, rhs.z), ValueType::Min(lhs.w, rhs.w));
    }

    ///
    /// @brief Get maximum components.
    ///
    /// @param lhs Vector to compare
    /// @param rhs Vector to compare
    ///
    /// @return Vector of maximum components
    ///
    static inline constexpr auto Max(Vector4 const& lhs, Vector4 const& rhs) -> Vector4
    {
        return Vector4(ValueType::Max(lhs.x, rhs.x), ValueType::Max(lhs.y, rhs.y), ValueType::Max(lhs.z, rhs.z), ValueType::Max(lhs.w, rhs.w));
    }

    ///
    /// @brief Round components.
    ///
    /// @param offset Vector to round
    ///
    /// @return Rounded offset
    ///
    static inline constexpr auto Round(Vector4 const& offset) -> Vector4
    {
        return Vector4(ValueType::Round(offset.x), ValueType::Round(offset.y), ValueType::Round(offset.z), ValueType::Round(offset.w));
    }

    ///
    /// @brief Floor components.
    ///
    /// @param offset Vector to floor
    ///
    /// @return Floored offset
    ///
    static inline constexpr auto Floor(Vector4 const& offset) -> Vector4
    {
        return Vector4(ValueType::Floor(offset.x), ValueType::Floor(offset.y), ValueType::Floor(offset.z), ValueType::Floor(offset.w));
    }

    ///
    /// @brief Ceil components.
    ///
    /// @param offset Vector to ceil
    ///
    /// @return Ceiled offset
    ///
    static inline constexpr auto Ceil(Vector4 const& offset) -> Vector4
    {
        return Vector4(ValueType::Ceil(offset.x), ValueType::Ceil(offset.y), ValueType::Ceil(offset.z), ValueType::Ceil(offset.w));
    }

    ///
    /// @brief Returns true if the offset is finite.
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(x) && ValueType::IsFinite(y) && ValueType::IsFinite(z) && ValueType::IsFinite(w);
    }

    ///
    /// @brief operator==
    ///
    friend inline constexpr bool operator==(Vector4 const& lhs, Vector4 const& rhs) noexcept = default;

    ///
    /// @brief operator!=
    ///
    friend inline constexpr bool operator!=(Vector4 const& lhs, Vector4 const& rhs) noexcept = default;

    ///
    /// @brief
    ///
    inline constexpr auto operator+=(Vector4 const& offset) noexcept -> Vector4&
    {
        x += offset.x;
        y += offset.y;
        z += offset.z;
        w += offset.w;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-=(Vector4 const& offset) noexcept -> Vector4&
    {
        x -= offset.x;
        y -= offset.y;
        z -= offset.z;
        w -= offset.w;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-() const noexcept -> Vector4
    {
        return Vector4(-x, -y, -z, -w);
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator+() const noexcept -> Vector4
    {
        return Vector4(+x, +y, +z, +w);
    }

    friend inline constexpr auto operator+(Vector4 const& l, Vector4 const& r) noexcept
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    friend inline constexpr auto operator-(Vector4 const& l, Vector4 const& r) noexcept
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
        return U<ValueType>(x, y, z, w);
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
};
}
}
