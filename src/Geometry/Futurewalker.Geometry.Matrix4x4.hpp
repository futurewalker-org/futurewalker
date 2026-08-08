// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Geometry.MatrixType.hpp"
#include "Futurewalker.Geometry.Matrix3x3.hpp"
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
/// @brief A specialization of Matrix4x4 for floating-point values.
///
/// @tparam T Floating-point type
/// @tparam Tag Tag type
///
template <Concepts::FloatingPoint T, class Tag>
struct Matrix4x4<Float<T, Tag>>
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
    static inline constexpr auto MakeIdentity() noexcept -> Matrix4x4
    {
        return Matrix4x4(
            static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    ///
    /// @brief Make a translation matrix.
    ///
    /// @param offset Amount to translate by.
    ///
    /// @return Matrix that translates by `offset`.
    ///
    static inline constexpr auto MakeTranslation(Vector3<ValueType> const& offset) noexcept -> Matrix4x4
    {
        return Matrix4x4(
            static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), offset.x,
            static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), offset.y,
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), offset.z,
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    ///
    /// @brief Make a scaling matrix.
    ///
    /// @param sx Scale factor along the x-axis.
    /// @param sy Scale factor along the y-axis.
    /// @param sz Scale factor along the z-axis.
    ///
    /// @return Matrix that scales by `sx`, `sy` and `sz`.
    ///
    static inline constexpr auto MakeScale(ValueType const& sx, ValueType const& sy, ValueType const& sz) noexcept -> Matrix4x4
    {
        return Matrix4x4(
            sx, static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), sy, static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), sz, static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    ///
    /// @brief Make a uniform scaling matrix.
    ///
    /// @param s Scale factor applied to all axes.
    ///
    /// @return Matrix that scales uniformly by `s`.
    ///
    static inline constexpr auto MakeScale(ValueType const& s) noexcept -> Matrix4x4
    {
        return MakeScale(s, s, s);
    }

    ///
    /// @brief Make a rotation matrix.
    ///
    /// @param axis Axis to rotate around. Does not need to be normalized.
    /// @param radians Angle to rotate around `axis`, in radians.
    ///
    /// @return Matrix that rotates by `radians` around `axis`.
    ///
    static inline auto MakeRotation(Vector3<ValueType> const& axis, Radian const& radians) noexcept -> Matrix4x4
    {
        auto const length = ValueType::Sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
        auto const nx = axis.x / length;
        auto const ny = axis.y / length;
        auto const nz = axis.z / length;

        auto const c = static_cast<T>(std::cos(static_cast<T>(radians)));
        auto const s = static_cast<T>(std::sin(static_cast<T>(radians)));
        auto const t = static_cast<T>(1) - c;

        return Matrix4x4(
            t * nx * nx + c, t * nx * ny - s * nz, t * nx * nz + s * ny, static_cast<T>(0),
            t * nx * ny + s * nz, t * ny * ny + c, t * ny * nz - s * nx, static_cast<T>(0),
            t * nx * nz - s * ny, t * ny * nz + s * nx, t * nz * nz + c, static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    ///
    /// @brief Transpose a matrix.
    ///
    /// @param m Matrix to transpose.
    ///
    /// @return Transposed matrix.
    ///
    static inline constexpr auto Transpose(Matrix4x4 const& m) noexcept -> Matrix4x4
    {
        return Matrix4x4(
            m.m00, m.m10, m.m20, m.m30,
            m.m01, m.m11, m.m21, m.m31,
            m.m02, m.m12, m.m22, m.m32,
            m.m03, m.m13, m.m23, m.m33);
    }

    ///
    /// @brief Compute the determinant of a matrix.
    ///
    /// @param m Matrix to compute the determinant of.
    ///
    /// @return Determinant of `m`.
    ///
    static inline constexpr auto Determinant(Matrix4x4 const& m) noexcept -> ValueType
    {
        auto const minor0 = Matrix3x3<ValueType>(
            m.m11, m.m12, m.m13,
            m.m21, m.m22, m.m23,
            m.m31, m.m32, m.m33);
        auto const minor1 = Matrix3x3<ValueType>(
            m.m10, m.m12, m.m13,
            m.m20, m.m22, m.m23,
            m.m30, m.m32, m.m33);
        auto const minor2 = Matrix3x3<ValueType>(
            m.m10, m.m11, m.m13,
            m.m20, m.m21, m.m23,
            m.m30, m.m31, m.m33);
        auto const minor3 = Matrix3x3<ValueType>(
            m.m10, m.m11, m.m12,
            m.m20, m.m21, m.m22,
            m.m30, m.m31, m.m32);

        return m.m00 * Matrix3x3<ValueType>::Determinant(minor0)
             - m.m01 * Matrix3x3<ValueType>::Determinant(minor1)
             + m.m02 * Matrix3x3<ValueType>::Determinant(minor2)
             - m.m03 * Matrix3x3<ValueType>::Determinant(minor3);
    }

    friend inline constexpr bool operator==(Matrix4x4 const& l, Matrix4x4 const& r) = default;
    friend inline constexpr bool operator!=(Matrix4x4 const& l, Matrix4x4 const& r) = default;

    ///
    /// @brief
    ///
    inline constexpr auto operator+=(Matrix4x4 const& rhs) noexcept -> Matrix4x4&
    {
        m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02; m03 += rhs.m03;
        m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13;
        m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23;
        m30 += rhs.m30; m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-=(Matrix4x4 const& rhs) noexcept -> Matrix4x4&
    {
        m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02; m03 -= rhs.m03;
        m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13;
        m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23;
        m30 -= rhs.m30; m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33;
        return *this;
    }

    ///
    /// @brief Compose with another matrix.
    ///
    /// @param rhs Matrix to compose with.
    ///
    /// @return Reference to this matrix, composed with `rhs`.
    ///
    inline constexpr auto operator*=(Matrix4x4 const& rhs) noexcept -> Matrix4x4&
    {
        auto const lhs = *this;
        m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30;
        m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21 + lhs.m03 * rhs.m31;
        m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22 + lhs.m03 * rhs.m32;
        m03 = lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23 + lhs.m03 * rhs.m33;
        m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20 + lhs.m13 * rhs.m30;
        m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31;
        m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32;
        m13 = lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33;
        m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20 + lhs.m23 * rhs.m30;
        m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31;
        m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32;
        m23 = lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33;
        m30 = lhs.m30 * rhs.m00 + lhs.m31 * rhs.m10 + lhs.m32 * rhs.m20 + lhs.m33 * rhs.m30;
        m31 = lhs.m30 * rhs.m01 + lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31;
        m32 = lhs.m30 * rhs.m02 + lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32;
        m33 = lhs.m30 * rhs.m03 + lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-() const noexcept -> Matrix4x4
    {
        return Matrix4x4(
            -m00, -m01, -m02, -m03,
            -m10, -m11, -m12, -m13,
            -m20, -m21, -m22, -m23,
            -m30, -m31, -m32, -m33);
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator+() const noexcept -> Matrix4x4
    {
        return Matrix4x4(
            +m00, +m01, +m02, +m03,
            +m10, +m11, +m12, +m13,
            +m20, +m21, +m22, +m23,
            +m30, +m31, +m32, +m33);
    }

    friend inline constexpr auto operator+(Matrix4x4 const& l, Matrix4x4 const& r) noexcept
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    friend inline constexpr auto operator-(Matrix4x4 const& l, Matrix4x4 const& r) noexcept
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
    friend inline constexpr auto operator*(Matrix4x4 const& m, Vector4<ValueType> const& v) noexcept -> Vector4<ValueType>
    {
        return Vector4<ValueType>(
            m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w,
            m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w,
            m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w,
            m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w);
    }

    ///
    /// @brief Matrix multiplication (composition).
    ///
    /// @param l Left-hand matrix.
    /// @param r Right-hand matrix.
    ///
    /// @return Product of `l` and `r`.
    ///
    friend inline constexpr auto operator*(Matrix4x4 const& l, Matrix4x4 const& r) noexcept -> Matrix4x4
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
    ValueType m10 = static_cast<T>(0);
    ValueType m11 = static_cast<T>(0);
    ValueType m12 = static_cast<T>(0);
    ValueType m13 = static_cast<T>(0);
    ValueType m20 = static_cast<T>(0);
    ValueType m21 = static_cast<T>(0);
    ValueType m22 = static_cast<T>(0);
    ValueType m23 = static_cast<T>(0);
    ValueType m30 = static_cast<T>(0);
    ValueType m31 = static_cast<T>(0);
    ValueType m32 = static_cast<T>(0);
    ValueType m33 = static_cast<T>(0);
};
}
}
