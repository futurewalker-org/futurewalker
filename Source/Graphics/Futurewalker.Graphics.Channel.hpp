// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.ChannelType.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Value type of color channels.
///
/// @tparam Tag color space tag type.
///
template <class Tag>
class ChannelT
{
public:
    using ValueType = Float64;

    static constexpr auto MakeFromU8(const UInt8& v) noexcept -> ChannelT;
    static constexpr auto MakeFromU16(const UInt16& v) noexcept -> ChannelT;
    static constexpr auto MakeFromU32(const UInt32& v) noexcept -> ChannelT;

public:
    constexpr ChannelT() = default;

    template <class U>
    constexpr explicit(!Concepts::NarrowConvertibleTo<U, ValueType>) ChannelT(const U& v) noexcept;

    constexpr auto GetValue() const noexcept -> Float64;
    constexpr auto SetValue(const Float64& v) noexcept -> void;

    constexpr auto GetU8() const noexcept -> UInt8;
    constexpr auto GetU16() const noexcept -> UInt16;
    constexpr auto GetU32() const noexcept -> UInt32;
    constexpr auto GetF32() const noexcept -> Float32;
    constexpr auto GetF64() const noexcept -> Float64;

    constexpr auto SetU8(const UInt8& v) noexcept -> void;
    constexpr auto SetU16(const UInt16& v) noexcept -> void;
    constexpr auto SetU32(const UInt32& v) noexcept -> void;
    constexpr auto SetF32(const Float32& v) noexcept -> void;
    constexpr auto SetF64(const Float64& v) noexcept -> void;

    auto operator==(const ChannelT&) const -> bool = default;;
    auto operator<=>(const ChannelT&) const = default;

private:
    ValueType _v = 0;
};

///
/// @brief Make channel from UInt8 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::MakeFromU8(const UInt8& v) noexcept -> ChannelT
{
    auto c = ChannelT();
    c.SetU8(v);
    return c;
}

///
/// @brief Make channel from UInt16 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::MakeFromU16(const UInt16& v) noexcept -> ChannelT
{
    auto c = ChannelT();
    c.SetU16(v);
    return c;
}

///
/// @brief Make channel from UInt32 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::MakeFromU32(const UInt32& v) noexcept -> ChannelT
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
constexpr ChannelT<Tag>::ChannelT(const U& v) noexcept
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
constexpr auto ChannelT<Tag>::SetValue(const Float64& v) noexcept -> void
{
    _v = v;
}

///
/// @brief Get UInt8 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::GetU8() const noexcept -> UInt8
{
    const auto max = std::numeric_limits<UInt8::ValueType>::max();
    const auto pad = 0.5 / max;
    return UInt8(Float64::Floor((Float64::Clamp(_v, 0.0, 1.0) + pad) * max));
}

///
/// @brief Get UInt16 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::GetU16() const noexcept -> UInt16
{
    const auto max = std::numeric_limits<UInt16::ValueType>::max();
    const auto pad = 0.5 / max;
    return UInt16(Float64::Floor((Float64::Clamp(_v, 0.0, 1.0) + pad) * max));
}

///
/// @brief Get UInt32 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::GetU32() const noexcept -> UInt32
{
    const auto max = std::numeric_limits<UInt32::ValueType>::max();
    const auto pad = 0.5 / max;
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
constexpr auto ChannelT<Tag>::SetU8(const UInt8& v) noexcept -> void
{
    const auto max = std::numeric_limits<UInt8::ValueType>::max();
    _v = v / Float64(max);
}

///
/// @brief Set UInt16 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::SetU16(const UInt16& v) noexcept -> void
{
    const auto max = std::numeric_limits<UInt16::ValueType>::max();
    _v = v / Float64(max);
}

///
/// @brief Set UIn32 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::SetU32(const UInt32& v) noexcept -> void
{
    const auto max = std::numeric_limits<UInt32::ValueType>::max();
    _v = v / Float64(max);
}

///
/// @brief Set F32 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::SetF32(const Float32& v) noexcept -> void
{
    _v = v;
}

///
/// @brief Set F64 value.
///
template <class Tag>
constexpr auto ChannelT<Tag>::SetF64(const Float64& v) noexcept -> void
{
    _v = v;
}
}
}
