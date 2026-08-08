// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Geometry.MatrixType.hpp"
#include "Futurewalker.Geometry.Matrix4x4.hpp"
#include "Futurewalker.Geometry.Vector.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Concepts.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A specialization of Matrix5x5 for floating-point values.
///
/// @tparam T Floating-point type
/// @tparam Tag Tag type
///
template <Concepts::FloatingPoint T, class Tag>
struct Matrix5x5<Float<T, Tag>>
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
    static inline constexpr auto MakeIdentity() noexcept -> Matrix5x5
    {
        return Matrix5x5(
            static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    ///
    /// @brief Make a translation matrix.
    ///
    /// @param offset Amount to translate by.
    ///
    /// @return Matrix that translates by `offset`.
    ///
    static inline constexpr auto MakeTranslation(Vector4<ValueType> const& offset) noexcept -> Matrix5x5
    {
        return Matrix5x5(
            static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), offset.x,
            static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), offset.y,
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), offset.z,
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), offset.w,
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    ///
    /// @brief Make a scaling matrix.
    ///
    /// @param s0 Scale factor along the 1st axis.
    /// @param s1 Scale factor along the 2nd axis.
    /// @param s2 Scale factor along the 3rd axis.
    /// @param s3 Scale factor along the 4th axis.
    ///
    /// @return Matrix that scales by `s0`, `s1`, `s2` and `s3`.
    ///
    static inline constexpr auto MakeScale(ValueType const& s0, ValueType const& s1, ValueType const& s2, ValueType const& s3) noexcept -> Matrix5x5
    {
        return Matrix5x5(
            s0, static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), s1, static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), s2, static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), s3, static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    ///
    /// @brief Make a uniform scaling matrix.
    ///
    /// @param s Scale factor applied to all axes.
    ///
    /// @return Matrix that scales uniformly by `s`.
    ///
    static inline constexpr auto MakeScale(ValueType const& s) noexcept -> Matrix5x5
    {
        return MakeScale(s, s, s, s);
    }

    ///
    /// @brief Transpose a matrix.
    ///
    /// @param m Matrix to transpose.
    ///
    /// @return Transposed matrix.
    ///
    static inline constexpr auto Transpose(Matrix5x5 const& m) noexcept -> Matrix5x5
    {
        return Matrix5x5(
            m.m00, m.m10, m.m20, m.m30, m.m40,
            m.m01, m.m11, m.m21, m.m31, m.m41,
            m.m02, m.m12, m.m22, m.m32, m.m42,
            m.m03, m.m13, m.m23, m.m33, m.m43,
            m.m04, m.m14, m.m24, m.m34, m.m44);
    }

    ///
    /// @brief Compute the determinant of a matrix.
    ///
    /// @param m Matrix to compute the determinant of.
    ///
    /// @return Determinant of `m`.
    ///
    static inline constexpr auto Determinant(Matrix5x5 const& m) noexcept -> ValueType
    {
        auto const minor0 = Matrix4x4<ValueType>(
            m.m11, m.m12, m.m13, m.m14,
            m.m21, m.m22, m.m23, m.m24,
            m.m31, m.m32, m.m33, m.m34,
            m.m41, m.m42, m.m43, m.m44);
        auto const minor1 = Matrix4x4<ValueType>(
            m.m10, m.m12, m.m13, m.m14,
            m.m20, m.m22, m.m23, m.m24,
            m.m30, m.m32, m.m33, m.m34,
            m.m40, m.m42, m.m43, m.m44);
        auto const minor2 = Matrix4x4<ValueType>(
            m.m10, m.m11, m.m13, m.m14,
            m.m20, m.m21, m.m23, m.m24,
            m.m30, m.m31, m.m33, m.m34,
            m.m40, m.m41, m.m43, m.m44);
        auto const minor3 = Matrix4x4<ValueType>(
            m.m10, m.m11, m.m12, m.m14,
            m.m20, m.m21, m.m22, m.m24,
            m.m30, m.m31, m.m32, m.m34,
            m.m40, m.m41, m.m42, m.m44);
        auto const minor4 = Matrix4x4<ValueType>(
            m.m10, m.m11, m.m12, m.m13,
            m.m20, m.m21, m.m22, m.m23,
            m.m30, m.m31, m.m32, m.m33,
            m.m40, m.m41, m.m42, m.m43);

        return m.m00 * Matrix4x4<ValueType>::Determinant(minor0)
             - m.m01 * Matrix4x4<ValueType>::Determinant(minor1)
             + m.m02 * Matrix4x4<ValueType>::Determinant(minor2)
             - m.m03 * Matrix4x4<ValueType>::Determinant(minor3)
             + m.m04 * Matrix4x4<ValueType>::Determinant(minor4);
    }

    friend inline constexpr bool operator==(Matrix5x5 const& l, Matrix5x5 const& r) = default;
    friend inline constexpr bool operator!=(Matrix5x5 const& l, Matrix5x5 const& r) = default;

    ///
    /// @brief
    ///
    inline constexpr auto operator+=(Matrix5x5 const& rhs) noexcept -> Matrix5x5&
    {
        m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02; m03 += rhs.m03; m04 += rhs.m04;
        m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13; m14 += rhs.m14;
        m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23; m24 += rhs.m24;
        m30 += rhs.m30; m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33; m34 += rhs.m34;
        m40 += rhs.m40; m41 += rhs.m41; m42 += rhs.m42; m43 += rhs.m43; m44 += rhs.m44;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-=(Matrix5x5 const& rhs) noexcept -> Matrix5x5&
    {
        m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02; m03 -= rhs.m03; m04 -= rhs.m04;
        m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13; m14 -= rhs.m14;
        m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23; m24 -= rhs.m24;
        m30 -= rhs.m30; m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33; m34 -= rhs.m34;
        m40 -= rhs.m40; m41 -= rhs.m41; m42 -= rhs.m42; m43 -= rhs.m43; m44 -= rhs.m44;
        return *this;
    }

    ///
    /// @brief Compose with another matrix.
    ///
    /// @param rhs Matrix to compose with.
    ///
    /// @return Reference to this matrix, composed with `rhs`.
    ///
    inline constexpr auto operator*=(Matrix5x5 const& rhs) noexcept -> Matrix5x5&
    {
        auto const lhs = *this;
        m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30 + lhs.m04 * rhs.m40;
        m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21 + lhs.m03 * rhs.m31 + lhs.m04 * rhs.m41;
        m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22 + lhs.m03 * rhs.m32 + lhs.m04 * rhs.m42;
        m03 = lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23 + lhs.m03 * rhs.m33 + lhs.m04 * rhs.m43;
        m04 = lhs.m00 * rhs.m04 + lhs.m01 * rhs.m14 + lhs.m02 * rhs.m24 + lhs.m03 * rhs.m34 + lhs.m04 * rhs.m44;

        m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20 + lhs.m13 * rhs.m30 + lhs.m14 * rhs.m40;
        m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31 + lhs.m14 * rhs.m41;
        m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32 + lhs.m14 * rhs.m42;
        m13 = lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33 + lhs.m14 * rhs.m43;
        m14 = lhs.m10 * rhs.m04 + lhs.m11 * rhs.m14 + lhs.m12 * rhs.m24 + lhs.m13 * rhs.m34 + lhs.m14 * rhs.m44;

        m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20 + lhs.m23 * rhs.m30 + lhs.m24 * rhs.m40;
        m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31 + lhs.m24 * rhs.m41;
        m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32 + lhs.m24 * rhs.m42;
        m23 = lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33 + lhs.m24 * rhs.m43;
        m24 = lhs.m20 * rhs.m04 + lhs.m21 * rhs.m14 + lhs.m22 * rhs.m24 + lhs.m23 * rhs.m34 + lhs.m24 * rhs.m44;

        m30 = lhs.m30 * rhs.m00 + lhs.m31 * rhs.m10 + lhs.m32 * rhs.m20 + lhs.m33 * rhs.m30 + lhs.m34 * rhs.m40;
        m31 = lhs.m30 * rhs.m01 + lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31 + lhs.m34 * rhs.m41;
        m32 = lhs.m30 * rhs.m02 + lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32 + lhs.m34 * rhs.m42;
        m33 = lhs.m30 * rhs.m03 + lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33 + lhs.m34 * rhs.m43;
        m34 = lhs.m30 * rhs.m04 + lhs.m31 * rhs.m14 + lhs.m32 * rhs.m24 + lhs.m33 * rhs.m34 + lhs.m34 * rhs.m44;

        m40 = lhs.m40 * rhs.m00 + lhs.m41 * rhs.m10 + lhs.m42 * rhs.m20 + lhs.m43 * rhs.m30 + lhs.m44 * rhs.m40;
        m41 = lhs.m40 * rhs.m01 + lhs.m41 * rhs.m11 + lhs.m42 * rhs.m21 + lhs.m43 * rhs.m31 + lhs.m44 * rhs.m41;
        m42 = lhs.m40 * rhs.m02 + lhs.m41 * rhs.m12 + lhs.m42 * rhs.m22 + lhs.m43 * rhs.m32 + lhs.m44 * rhs.m42;
        m43 = lhs.m40 * rhs.m03 + lhs.m41 * rhs.m13 + lhs.m42 * rhs.m23 + lhs.m43 * rhs.m33 + lhs.m44 * rhs.m43;
        m44 = lhs.m40 * rhs.m04 + lhs.m41 * rhs.m14 + lhs.m42 * rhs.m24 + lhs.m43 * rhs.m34 + lhs.m44 * rhs.m44;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-() const noexcept -> Matrix5x5
    {
        return Matrix5x5(
            -m00, -m01, -m02, -m03, -m04,
            -m10, -m11, -m12, -m13, -m14,
            -m20, -m21, -m22, -m23, -m24,
            -m30, -m31, -m32, -m33, -m34,
            -m40, -m41, -m42, -m43, -m44);
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator+() const noexcept -> Matrix5x5
    {
        return Matrix5x5(
            +m00, +m01, +m02, +m03, +m04,
            +m10, +m11, +m12, +m13, +m14,
            +m20, +m21, +m22, +m23, +m24,
            +m30, +m31, +m32, +m33, +m34,
            +m40, +m41, +m42, +m43, +m44);
    }

    friend inline constexpr auto operator+(Matrix5x5 const& l, Matrix5x5 const& r) noexcept
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    friend inline constexpr auto operator-(Matrix5x5 const& l, Matrix5x5 const& r) noexcept
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
    friend inline constexpr auto operator*(Matrix5x5 const& m, Vector5<ValueType> const& v) noexcept -> Vector5<ValueType>
    {
        return Vector5<ValueType>(
            m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w + m.m04 * v.v,
            m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w + m.m14 * v.v,
            m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w + m.m24 * v.v,
            m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w + m.m34 * v.v,
            m.m40 * v.x + m.m41 * v.y + m.m42 * v.z + m.m43 * v.w + m.m44 * v.v);
    }

    ///
    /// @brief Matrix multiplication (composition).
    ///
    /// @param l Left-hand matrix.
    /// @param r Right-hand matrix.
    ///
    /// @return Product of `l` and `r`.
    ///
    friend inline constexpr auto operator*(Matrix5x5 const& l, Matrix5x5 const& r) noexcept -> Matrix5x5
    {
        auto tmp = l;
        tmp *= r;
        return tmp;
    }

    // Matrix elements in [row][column] order
    ValueType m00 = static_cast<T>(0);
    ValueType m01 = static_cast<T>(0);
    ValueType m02 = static_cast<T>(0);
    ValueType m03 = static_cast<T>(0);
    ValueType m04 = static_cast<T>(0);
    ValueType m10 = static_cast<T>(0);
    ValueType m11 = static_cast<T>(0);
    ValueType m12 = static_cast<T>(0);
    ValueType m13 = static_cast<T>(0);
    ValueType m14 = static_cast<T>(0);
    ValueType m20 = static_cast<T>(0);
    ValueType m21 = static_cast<T>(0);
    ValueType m22 = static_cast<T>(0);
    ValueType m23 = static_cast<T>(0);
    ValueType m24 = static_cast<T>(0);
    ValueType m30 = static_cast<T>(0);
    ValueType m31 = static_cast<T>(0);
    ValueType m32 = static_cast<T>(0);
    ValueType m33 = static_cast<T>(0);
    ValueType m34 = static_cast<T>(0);
    ValueType m40 = static_cast<T>(0);
    ValueType m41 = static_cast<T>(0);
    ValueType m42 = static_cast<T>(0);
    ValueType m43 = static_cast<T>(0);
    ValueType m44 = static_cast<T>(0);
};
}
}
