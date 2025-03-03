// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.RGBColorType.hpp"
#include "Futurewalker.Graphics.Channel.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief RGB color value.
///
/// @tparam Tag Tag type
///
template <class ColorTag>
class RGBColorT
{
    using ColorChannelType = ChannelT<ColorTag>;

public:
    static constexpr auto MakeFromU8(const UInt8& r, const UInt8& g, const UInt8& b) -> RGBColorT;
    static constexpr auto MakeFromU16(const UInt16& r, const UInt16& g, const UInt16& b) -> RGBColorT;
    static constexpr auto MakeFromU32(const UInt32& r, const UInt32& g, const UInt32& b) -> RGBColorT;

public:
    constexpr RGBColorT() = default;
    constexpr RGBColorT(const ColorChannelType& r, const ColorChannelType& g, const ColorChannelType& b);

    constexpr auto GetRed() const -> ColorChannelType;
    constexpr auto GetRedU8() const -> UInt8;
    constexpr auto GetRedU16() const -> UInt16;
    constexpr auto GetRedU32() const -> UInt32;
    constexpr auto GetRedF64() const -> Float64;

    constexpr auto GetGreen() const -> ColorChannelType;
    constexpr auto GetGreenU8() const -> UInt8;
    constexpr auto GetGreenU16() const -> UInt16;
    constexpr auto GetGreenU32() const -> UInt32;
    constexpr auto GetGreenF64() const -> Float64;

    constexpr auto GetBlue() const -> ColorChannelType;
    constexpr auto GetBlueU8() const -> UInt8;
    constexpr auto GetBlueU16() const -> UInt16;
    constexpr auto GetBlueU32() const -> UInt32;
    constexpr auto GetBlueF64() const -> Float64;

    constexpr auto SetRed(const ColorChannelType& r) -> void;
    constexpr auto SetRedU8(const UInt8& r) -> void;
    constexpr auto SetRedU16(const UInt16& r) -> void;
    constexpr auto SetRedU32(const UInt32& r) -> void;
    constexpr auto SetRedF64(const Float64& r) -> void;

    constexpr auto SetGreenU8(const UInt8& g) -> void;
    constexpr auto SetGreenU16(const UInt16& g) -> void;
    constexpr auto SetGreenU32(const UInt32& g) -> void;
    constexpr auto SetGreenF64(const Float64& g) -> void;

    constexpr auto SetBlueU8(const UInt8& b) -> void;
    constexpr auto SetBlueU16(const UInt16& b) -> void;
    constexpr auto SetBlueU32(const UInt32& b) -> void;
    constexpr auto SetBlueF64(const Float64& b) -> void;

    friend inline constexpr bool operator==(const RGBColorT&, const RGBColorT&) = default;
    friend inline constexpr bool operator!=(const RGBColorT&, const RGBColorT&) = default;

private:
    ColorChannelType _r;
    ColorChannelType _g;
    ColorChannelType _b;
};

///
/// @brief Make from UInt8 values.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::MakeFromU8(const UInt8& r, const UInt8& g, const UInt8& b) -> RGBColorT
{
    auto color = RGBColorT();
    color.SetRedU8(r);
    color.SetGreenU8(g);
    color.SetBlueU8(b);
    return color;
}

///
/// @brief Make from UInt16 values.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::MakeFromU16(const UInt16& r, const UInt16& g, const UInt16& b) -> RGBColorT
{
    auto color = RGBColorT();
    color.SetRedU16(r);
    color.SetGreenU16(g);
    color.SetBlueU16(b);
    return color;
}

///
/// @brief Make from UInt32 values.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::MakeFromU32(const UInt32& r, const UInt32& g, const UInt32& b) -> RGBColorT
{
    auto color = RGBColorT();
    color.SetRedU32(r);
    color.SetGreenU32(g);
    color.SetBlueU32(b);
    return color;
}

///
/// @brief Construct from RGB channels.
///
template <class Tag>
constexpr RGBColorT<Tag>::RGBColorT(const ColorChannelType& r, const ColorChannelType& g, const ColorChannelType& b)
  : _r {r}
  , _g {g}
  , _b {b}
{
}

///
/// @brief Get red channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetRed() const -> ColorChannelType
{
    return _r;
}

///
/// @brief Get red channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetRedU8() const -> UInt8
{
    return _r.GetU8();
}

///
/// @brief Get red channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetRedU16() const -> UInt16
{
    return _r.GetU16();
}

///
/// @brief Get red channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetRedU32() const -> UInt32
{
    return _r.GetU32();
}

///
/// @brief Get red channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetRedF64() const -> Float64
{
    return _r.GetF64();
}

///
/// @brief Get green channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetGreen() const -> ColorChannelType
{
    return _g; 
}

///
/// @brief Get green channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetGreenU8() const -> UInt8
{
    return _g.GetU8();
}

///
/// @brief Get green channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetGreenU16() const -> UInt16
{
    return _g.GetU16();
}

///
/// @brief Get green channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetGreenU32() const -> UInt32
{
    return _g.GetU32();
}

///
/// @brief Get green channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetGreenF64() const -> Float64
{
    return _g.GetF64();
}

///
/// @brief Get blue channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetBlue() const -> ColorChannelType
{
    return _b;
}

///
/// @brief Get blue channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetBlueU8() const -> UInt8
{
    return _b.GetU8();
}

///
/// @brief Get blue channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetBlueU16() const -> UInt16
{
    return _b.GetU16();
}

///
/// @brief Get blue channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetBlueU32() const -> UInt32
{
    return _b.GetU32();
}

///
/// @brief Get blue channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::GetBlueF64() const -> Float64
{
    return _b.GetF64();
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetRed(const ColorChannelType& r) -> void
{
    _r = r;
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetRedU8(const UInt8& r) -> void
{
    _r.SetU8(r);
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetRedU16(const UInt16& r) -> void
{
    _r.SetU16(r);
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetRedU32(const UInt32& r) -> void
{
    _r.SetU32(r);
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetRedF64(const Float64& r) -> void
{
    _r.SetF64(r);
}

///
/// @brief Set green channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetGreenU8(const UInt8& g) -> void
{
    _g.SetU8(g);
}

///
/// @brief Set green channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetGreenU16(const UInt16& g) -> void
{
    _g.SetU16(g);
}

///
/// @brief Set green channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetGreenU32(const UInt32& g) -> void
{
    _g.SetU32(g);
}

///
/// @brief Set green channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetGreenF64(const Float64& g) -> void
{
    _g.SetF64(g);
}

///
/// @brief Set blue channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetBlueU8(const UInt8& b) -> void
{
    _b.SetU8(b);
}

///
/// @brief Set blue channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetBlueU16(const UInt16& b) -> void
{
    _b.SetU16(b);
}

///
/// @brief Set blue channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetBlueU32(const UInt32& b) -> void
{
    _b.SetU32(b);
}

///
/// @brief Set blue channel.
///
template <class Tag>
constexpr auto RGBColorT<Tag>::SetBlueF64(const Float64& b) -> void
{
    _b.SetF64(b);
}
}
}
