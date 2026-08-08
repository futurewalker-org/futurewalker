// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Geometry.MatrixType.hpp"
#include "Futurewalker.Geometry.Vector.hpp"
#include "Futurewalker.Geometry.Unit.hpp" 

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Concepts.hpp"

#include <cmath>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A specialization of Matrix3x3 for floating-point values.
///
/// @tparam T Floating-point type
/// @tparam Tag Tag type
///
template <Concepts::FloatingPoint T, class Tag>
struct Matrix3x3<Float<T, Tag>>
{
    ///
    /// @brief Value type.
    ///
    using ValueType = Float<T, Tag>;

    ///
    /// @brief Make an identity matrix.
    ///
    /// @return Identity matrix.
    ///
    static inline constexpr auto MakeIdentity() noexcept -> Matrix3x3
    {
        return Matrix3x3(
            static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    ///
    /// @brief Make a translation matrix.
    ///
    /// @param offset Amount to translate by.
    ///
    /// @return Matrix that translates by `offset`.
    ///
    static inline constexpr auto MakeTranslation(Vector2<ValueType> const& offset) noexcept -> Matrix3x3
    {
        return Matrix3x3(
            static_cast<T>(1), static_cast<T>(0), offset.x,
            static_cast<T>(0), static_cast<T>(1), offset.y,
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    ///
    /// @brief Make a scaling matrix.
    ///
    /// @param sx Scale factor along the x-axis.
    /// @param sy Scale factor along the y-axis.
    ///
    /// @return Matrix that scales by `sx` and `sy`.
    ///
    static inline constexpr auto MakeScale(ValueType const& sx, ValueType const& sy) noexcept -> Matrix3x3
    {
        return Matrix3x3(
            sx, static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), sy, static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    ///
    /// @brief Make a uniform scaling matrix.
    ///
    /// @param s Scale factor applied to both axes.
    ///
    /// @return Matrix that scales uniformly by `s`.
    ///
    static inline constexpr auto MakeScale(ValueType const& s) noexcept -> Matrix3x3
    {
        return MakeScale(s, s);
    }

    ///
    /// @brief Make a rotation matrix.
    ///
    /// @param radians Angle to rotate around the origin, in radians.
    ///
    /// @return Matrix that rotates by `radians`.
    ///
    static inline auto MakeRotation(Radian const& radians) noexcept -> Matrix3x3
    {
        auto const c = static_cast<T>(std::cos(static_cast<T>(radians)));
        auto const s = static_cast<T>(std::sin(static_cast<T>(radians)));
        return Matrix3x3(
            c, -s, static_cast<T>(0),
            s, c, static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    ///
    /// @brief Transpose a matrix.
    ///
    /// @param m Matrix to transpose.
    ///
    /// @return Transposed matrix.
    ///
    static inline constexpr auto Transpose(Matrix3x3 const& m) noexcept -> Matrix3x3
    {
        return Matrix3x3(
            m.m00, m.m10, m.m20,
            m.m01, m.m11, m.m21,
            m.m02, m.m12, m.m22);
    }

    ///
    /// @brief Compute the determinant of a matrix.
    ///
    /// @param m Matrix to compute the determinant of.
    ///
    /// @return Determinant of `m`.
    ///
    static inline constexpr auto Determinant(Matrix3x3 const& m) noexcept -> ValueType
    {
        return m.m00 * (m.m11 * m.m22 - m.m12 * m.m21)
             - m.m01 * (m.m10 * m.m22 - m.m12 * m.m20)
             + m.m02 * (m.m10 * m.m21 - m.m11 * m.m20);
    }

    friend inline constexpr bool operator==(Matrix3x3 const& l, Matrix3x3 const& r) = default;
    friend inline constexpr bool operator!=(Matrix3x3 const& l, Matrix3x3 const& r) = default;

    ///
    /// @brief
    ///
    inline constexpr auto operator+=(Matrix3x3 const& rhs) noexcept -> Matrix3x3&
    {
        m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02;
        m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12;
        m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-=(Matrix3x3 const& rhs) noexcept -> Matrix3x3&
    {
        m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02;
        m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12;
        m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22;
        return *this;
    }

    ///
    /// @brief Compose with another matrix.
    ///
    /// @param rhs Matrix to compose with.
    ///
    /// @return Reference to this matrix, composed with `rhs`.
    ///
    inline constexpr auto operator*=(Matrix3x3 const& rhs) noexcept -> Matrix3x3&
    {
        auto const lhs = *this;
        m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20;
        m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21;
        m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22;
        m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20;
        m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21;
        m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22;
        m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20;
        m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21;
        m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-() const noexcept -> Matrix3x3
    {
        return Matrix3x3(
            -m00, -m01, -m02,
            -m10, -m11, -m12,
            -m20, -m21, -m22);
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator+() const noexcept -> Matrix3x3
    {
        return Matrix3x3(
            +m00, +m01, +m02,
            +m10, +m11, +m12,
            +m20, +m21, +m22);
    }

    friend inline constexpr auto operator+(Matrix3x3 const& l, Matrix3x3 const& r) noexcept
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    friend inline constexpr auto operator-(Matrix3x3 const& l, Matrix3x3 const& r) noexcept
    {
        auto tmp = l;
        tmp -= r;
        return tmp;
    }

    ///
    /// @brief Transform a vector.
    ///
    /// @param m Matrix to transform by.
    /// @param v Vector to transform.
    ///
    /// @return Transformed vector.
    ///
    friend inline constexpr auto operator*(Matrix3x3 const& m, Vector3<ValueType> const& v) noexcept -> Vector3<ValueType>
    {
        return Vector3<ValueType>(
            m.m00 * v.x + m.m01 * v.y + m.m02 * v.z,
            m.m10 * v.x + m.m11 * v.y + m.m12 * v.z,
            m.m20 * v.x + m.m21 * v.y + m.m22 * v.z);
    }

    ///
    /// @brief Matrix multiplication (composition).
    ///
    /// @param l Left-hand matrix.
    /// @param r Right-hand matrix.
    ///
    /// @return Product of `l` and `r`.
    ///
    friend inline constexpr auto operator*(Matrix3x3 const& l, Matrix3x3 const& r) noexcept -> Matrix3x3
    {
        auto tmp = l;
        tmp *= r;
        return tmp;
    }

    // Matrix elements in [row][column] order
    ValueType m00 = static_cast<T>(0);
    ValueType m01 = static_cast<T>(0);
    ValueType m02 = static_cast<T>(0);
    ValueType m10 = static_cast<T>(0);
    ValueType m11 = static_cast<T>(0);
    ValueType m12 = static_cast<T>(0);
    ValueType m20 = static_cast<T>(0);
    ValueType m21 = static_cast<T>(0);
    ValueType m22 = static_cast<T>(0);
};
}
}
