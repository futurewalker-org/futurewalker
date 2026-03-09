// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Color.ChannelType.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Color or alpha channel type.
///
/// @tparam Tag Color space tag type.
///
/// Channel value is represented as 64-bit floating point value to maintain precision for any quantization of its color space.
/// It does not apply any clamping so it can be used for extended color spaces which can have values outside of [0.0, 1.0].
///
template <class Tag>
class ChannelT
{
public:
    using ValueType = Float64;

    static constexpr auto MakeFromU8(UInt8 const& v) noexcept -> ChannelT;
    static constexpr auto MakeFromU16(UInt16 const& v) noexcept -> ChannelT;
    static constexpr auto MakeFromU32(UInt32 const& v) noexcept -> ChannelT;

public:
    constexpr ChannelT() = default;

    template <class U>
    constexpr explicit(!Concepts::NarrowConvertibleTo<U, ValueType>) ChannelT(U const& v) noexcept;

    constexpr auto GetValue() const noexcept -> Float64;
    constexpr auto SetValue(Float64 const& v) noexcept -> void;

    constexpr auto GetU8() const noexcept -> UInt8;
    constexpr auto GetU16() const noexcept -> UInt16;
    constexpr auto GetU32() const noexcept -> UInt32;
    constexpr auto GetF32() const noexcept -> Float32;
    constexpr auto GetF64() const noexcept -> Float64;

    constexpr auto SetU8(UInt8 const& v) noexcept -> void;
    constexpr auto SetU16(UInt16 const& v) noexcept -> void;
    constexpr auto SetU32(UInt32 const& v) noexcept -> void;
    constexpr auto SetF32(Float32 const& v) noexcept -> void;
    constexpr auto SetF64(Float64 const& v) noexcept -> void;

    auto operator==(ChannelT const&) const -> bool = default;;
    auto operator<=>(ChannelT const&) const = default;

private:
    ValueType _v = 0;
};

///
/// @brief Make channel from UInt8 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::MakeFromU8(UInt8 const& v) noexcept -> ChannelT
{
    auto c = ChannelT();
    c.SetU8(v);
    return c;
}

///
/// @brief Make channel from UInt16 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::MakeFromU16(UInt16 const& v) noexcept -> ChannelT
{
    auto c = ChannelT();
    c.SetU16(v);
    return c;
}

///
/// @brief Make channel from UInt32 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::MakeFromU32(UInt32 const& v) noexcept -> ChannelT
{
    auto c = ChannelT();
    c.SetU32(v);
    return c;
}

///
/// @brief Construct from value.
///
template <class Tag>
template <class U>
constexpr ChannelT<Tag>::ChannelT(U const& v) noexcept
  : _v {v}
{
}

///
/// @brief Get Float64 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::GetValue() const noexcept -> Float64
{
    return _v;
}

///
/// @brief Set Float64 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::SetValue(Float64 const& v) noexcept -> void
{
    _v = v;
}

///
/// @brief Get UInt8 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::GetU8() const noexcept -> UInt8
{
    auto const max = std::numeric_limits<UInt8::ValueType>::max();
    auto const pad = 0.5 / max;
    return UInt8(Float64::Floor((Float64::Clamp(_v, 0.0, 1.0) + pad) * max));
}

///
/// @brief Get UInt16 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::GetU16() const noexcept -> UInt16
{
    auto const max = std::numeric_limits<UInt16::ValueType>::max();
    auto const pad = 0.5 / max;
    return UInt16(Float64::Floor((Float64::Clamp(_v, 0.0, 1.0) + pad) * max));
}

///
/// @brief Get UInt32 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::GetU32() const noexcept -> UInt32
{
    auto const max = std::numeric_limits<UInt32::ValueType>::max();
    auto const pad = 0.5 / max;
    return UInt32(Float64::Floor((Float64::Clamp(_v, 0.0, 1.0) + pad) * max));
}

///
/// @brief Get Float32 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::GetF32() const noexcept -> Float32
{
    return Float32(_v);
}

///
/// @brief Get Float64 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::GetF64() const noexcept -> Float64
{
    return Float64(_v);
}

///
/// @brief Set UInt8 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::SetU8(UInt8 const& v) noexcept -> void
{
    auto const max = std::numeric_limits<UInt8::ValueType>::max();
    _v = static_cast<UInt8::ValueType>(v) / float64_t(max);
}

///
/// @brief Set UInt16 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::SetU16(UInt16 const& v) noexcept -> void
{
    auto const max = std::numeric_limits<UInt16::ValueType>::max();
    _v = static_cast<UInt16::ValueType>(v) / float64_t(max);
}

///
/// @brief Set UIn32 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::SetU32(UInt32 const& v) noexcept -> void
{
    auto const max = std::numeric_limits<UInt32::ValueType>::max();
    _v = static_cast<UInt32::ValueType>(v) / float64_t(max);
}

///
/// @brief Set F32 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::SetF32(Float32 const& v) noexcept -> void
{
    _v = v;
}

///
/// @brief Set F64 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::SetF64(Float64 const& v) noexcept -> void
{
    _v = v;
}
}
}
