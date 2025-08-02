// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Color.RGBAColorType.hpp"
#include "Futurewalker.Color.RGBColor.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief RGBA color value.
///
/// @tparam Tag Tag type
///
template <class Tag>
class RGBAColorT
{
    using ChannelType = ChannelT<Tag>;
    using RGBColorType = RGBColorT<Tag>;

public:
    static constexpr auto MakeFromU8(UInt8 const r, UInt8 const g, UInt8 const b, UInt8 const a) -> RGBAColorT;
    static constexpr auto MakeFromU16(UInt16 const r, UInt16 const g, UInt16 const b, UInt16 const a) -> RGBAColorT;
    static constexpr auto MakeFromU32(UInt32 const r, UInt32 const g, UInt32 const b, UInt32 const a) -> RGBAColorT;
    static constexpr auto MakeFromHex(SInt64 const hex) -> RGBAColorT;

public:
    constexpr RGBAColorT() = default;
    constexpr RGBAColorT(ChannelType const& r, ChannelType const& g, ChannelType const& b, ChannelType const& a);
    constexpr RGBAColorT(RGBColorType const& rgb, ChannelType const& a);

    constexpr auto GetRGBColor() const -> RGBColorType;
    constexpr auto SetRGBColor(RGBColorType const& rgb) -> void;

    constexpr auto GetRed() const -> ChannelType;
    constexpr auto GetRedU8() const -> UInt8;
    constexpr auto GetRedU16() const -> UInt16;
    constexpr auto GetRedU32() const -> UInt32;
    constexpr auto GetRedF32() const -> Float32;
    constexpr auto GetRedF64() const -> Float64;

    constexpr auto GetGreen() const -> ChannelType;
    constexpr auto GetGreenU8() const -> UInt8;
    constexpr auto GetGreenU16() const -> UInt16;
    constexpr auto GetGreenU32() const -> UInt32;
    constexpr auto GetGreenF32() const -> Float32;
    constexpr auto GetGreenF64() const -> Float64;

    constexpr auto GetBlue() const -> ChannelType;
    constexpr auto GetBlueU8() const -> UInt8;
    constexpr auto GetBlueU16() const -> UInt16;
    constexpr auto GetBlueU32() const -> UInt32;
    constexpr auto GetBlueF32() const -> Float32;
    constexpr auto GetBlueF64() const -> Float64;

    constexpr auto GetAlpha() const -> ChannelType;
    constexpr auto GetAlphaU8() const -> UInt8;
    constexpr auto GetAlphaU16() const -> UInt16;
    constexpr auto GetAlphaU32() const -> UInt32;
    constexpr auto GetAlphaF32() const -> Float32;
    constexpr auto GetAlphaF64() const -> Float64;

    constexpr auto SetRed(ChannelType const& r) -> void;
    constexpr auto SetRedU8(UInt8 const& r) -> void;
    constexpr auto SetRedU16(UInt16 const& r) -> void;
    constexpr auto SetRedU32(UInt32 const& r) -> void;
    constexpr auto SetRedF32(Float32 const& r) -> void;
    constexpr auto SetRedF64(Float64 const& r) -> void;

    constexpr auto SetGreen(ChannelType const& g) -> void;
    constexpr auto SetGreenU8(UInt8 const& g) -> void;
    constexpr auto SetGreenU16(UInt16 const& g) -> void;
    constexpr auto SetGreenU32(UInt32 const& g) -> void;
    constexpr auto SetGreenF32(Float32 const& g) -> void;
    constexpr auto SetGreenF64(Float64 const& g) -> void;

    constexpr auto SetBlue(ChannelType const& b) -> void;
    constexpr auto SetBlueU8(UInt8 const& b) -> void;
    constexpr auto SetBlueU16(UInt16 const& b) -> void;
    constexpr auto SetBlueU32(UInt32 const& b) -> void;
    constexpr auto SetBlueF32(Float32 const& b) -> void;
    constexpr auto SetBlueF64(Float64 const& b) -> void;

    constexpr auto SetAlpha(ChannelType const& a) -> void;
    constexpr auto SetAlphaU8(UInt8 const& a) -> void;
    constexpr auto SetAlphaU16(UInt16 const& a) -> void;
    constexpr auto SetAlphaU32(UInt32 const& a) -> void;
    constexpr auto SetAlphaF32(Float32 const& a) -> void;
    constexpr auto SetAlphaF64(Float64 const& a) -> void;

    friend inline constexpr bool operator==(RGBAColorT const&, RGBAColorT const&) = default;
    friend inline constexpr bool operator!=(RGBAColorT const&, RGBAColorT const&) = default;

private:
    ChannelType _r;
    ChannelType _g;
    ChannelType _b;
    ChannelType _a;
};

///
/// @brief Get RGBColor.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetRGBColor() const -> RGBColorType
{
    return RGBColorType(_r, _g, _b);
}

///
/// @brief Set RGBColor.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetRGBColor(RGBColorType const& rgb) -> void
{
    _r = rgb.GetRed();
    _g = rgb.GetGreen();
    _b = rgb.GetBlue();
}

///
/// @brief Set UInt8 channels.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::MakeFromU8(UInt8 const r, UInt8 const g, UInt8 const b, UInt8 const a) -> RGBAColorT
{
    auto color = RGBAColorT();
    color.SetRedU8(r);
    color.SetGreenU8(g);
    color.SetBlueU8(b);
    color.SetAlphaU8(a);
    return color;
}

///
/// @brief Set UInt16 channels.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::MakeFromU16(UInt16 const r, UInt16 const g, UInt16 const b, UInt16 const a) -> RGBAColorT
{
    auto color = RGBAColorT();
    color.SetRedU16(r);
    color.SetGreenU16(g);
    color.SetBlueU16(b);
    color.SetAlphaU16(a);
    return color;
}

///
/// @brief Set UInt32 channels.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::MakeFromU32(UInt32 const r, UInt32 const g, UInt32 const b, UInt32 const a) -> RGBAColorT
{
    auto color = RGBAColorT();
    color.SetRedU32(r);
    color.SetGreenU32(g);
    color.SetBlueU32(b);
    color.SetAlphaU32(a);
    return color;
}

///
/// @brief Make from hex value.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::MakeFromHex(SInt64 const hex) -> RGBAColorT
{
    return MakeFromU8(
      UInt8((static_cast<uint32_t>(hex) & 0xFF000000) >> 24), //
      UInt8((static_cast<uint32_t>(hex) & 0x00FF0000) >> 16), //
      UInt8((static_cast<uint32_t>(hex) & 0x0000FF00) >> 8),  //
      UInt8((static_cast<uint32_t>(hex) & 0x000000FF) >> 0)); //
}

///
/// @brief Construct from channels.
///
template <class Tag>
constexpr RGBAColorT<Tag>::RGBAColorT(ChannelType const& r, ChannelType const& g, ChannelType const& b, ChannelType const& a)
  : _r {r}
  , _g {g}
  , _b {b}
  , _a {a}
{
}

///
/// @brief Construct from RGBColor and alpha channel.
///
template <class Tag>
constexpr RGBAColorT<Tag>::RGBAColorT(RGBColorType const& rgb, ChannelType const& a)
  : _r {rgb.GetRed()}
  , _g {rgb.GetGreen()}
  , _b {rgb.GetBlue()}
  , _a {a}
{
}

///
/// @brief Get red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetRed() const -> ChannelType
{
    return _r;
}

///
/// @brief Get red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetRedU8() const -> UInt8
{
    return _r.GetU8();
}

///
/// @brief Get red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetRedU16() const -> UInt16
{
    return _r.GetU16();
}

///
/// @brief Get red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetRedU32() const -> UInt32
{
    return _r.GetU32();
}

///
/// @brief Get red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetRedF32() const -> Float32
{
    return _r.GetF32();
}

///
/// @brief Get red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetRedF64() const -> Float64
{
    return _r.GetF64();
}

///
/// @brief Get green channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetGreen() const -> ChannelType
{
    return _g; 
}

///
/// @brief Get green channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetGreenU8() const -> UInt8
{
    return _g.GetU8();
}

///
/// @brief Get green channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetGreenU16() const -> UInt16
{
    return _g.GetU16();
}

///
/// @brief Get green channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetGreenU32() const -> UInt32
{
    return _g.GetU32();
}

///
/// @brief Get green channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetGreenF32() const -> Float32
{
    return _g.GetF32();
}

///
/// @brief Get green channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetGreenF64() const -> Float64
{
    return _g.GetF64();
}

///
/// @brief Get blue channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetBlue() const -> ChannelType
{
    return _b;
}

///
/// @brief Get blue channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetBlueU8() const -> UInt8
{
    return _b.GetU8();
}

///
/// @brief Get blue channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetBlueU16() const -> UInt16
{
    return _b.GetU16();
}

///
/// @brief Get blue channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetBlueU32() const -> UInt32
{
    return _b.GetU32();
}

///
/// @brief Get blue channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetBlueF32() const -> Float32
{
    return _b.GetF32();
}

///
/// @brief Get blue channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetBlueF64() const -> Float64
{
    return _b.GetF64();
}

///
/// @brief Get alpha channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetAlpha() const -> ChannelType
{
    return _a;
}

///
/// @brief Get alpha channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetAlphaU8() const -> UInt8
{
    return _a.GetU8();
}

///
/// @brief Get alpha channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetAlphaU16() const -> UInt16
{
    return _a.GetU16();
}

///
/// @brief Get alpha channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetAlphaU32() const -> UInt32
{
    return _a.GetU32();
}

///
/// @brief Get alpha channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetAlphaF32() const -> Float32
{
    return _a.GetF32();
}

///
/// @brief Get alpha channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::GetAlphaF64() const -> Float64
{
    return _a.GetF64();
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetRed(ChannelType const& r) -> void
{
    _r = r;
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetRedU8(UInt8 const& r) -> void
{
    _r.SetU8(r);
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetRedU16(UInt16 const& r) -> void
{
    _r.SetU16(r);
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetRedU32(UInt32 const& r) -> void
{
    _r.SetU32(r);
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetRedF32(Float32 const& r) -> void
{
    _r.SetF32(r);
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetRedF64(Float64 const& r) -> void
{
    _r.SetF64(r);
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetGreen(ChannelType const& g) -> void
{
    _g = g;
}

///
/// @brief Set green channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetGreenU8(UInt8 const& g) -> void
{
    _g.SetU8(g);
}

///
/// @brief Set green channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetGreenU16(UInt16 const& g) -> void
{
    _g.SetU16(g);
}

///
/// @brief Set green channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetGreenU32(UInt32 const& g) -> void
{
    _g.SetU32(g);
}

///
/// @brief Set green channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetGreenF32(Float32 const& g) -> void
{
    _g.SetF32(g);
}

///
/// @brief Set green channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetGreenF64(Float64 const& g) -> void
{
    _g.SetF64(g);
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetBlue(ChannelType const& b) -> void
{
    _b = b;
}

///
/// @brief Set blue channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetBlueU8(UInt8 const& b) -> void
{
    _b.SetU8(b);
}

///
/// @brief Set blue channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetBlueU16(UInt16 const& b) -> void
{
    _b.SetU16(b);
}

///
/// @brief Set blue channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetBlueU32(UInt32 const& b) -> void
{
    _b.SetU32(b);
}

///
/// @brief Set blue channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetBlueF32(Float32 const& b) -> void
{
    _b.SetF32(b);
}

///
/// @brief Set blue channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetBlueF64(Float64 const& b) -> void
{
    _b.SetF64(b);
}

///
/// @brief Set red channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetAlpha(ChannelType const& a) -> void
{
    _a = a;
}

///
/// @brief Set alpha channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetAlphaU8(UInt8 const& a) -> void
{
    _a.SetU8(a);
}

///
/// @brief Set alpha channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetAlphaU16(UInt16 const& a) -> void
{
    _a.SetU16(a);
}

///
/// @brief Set alpha channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetAlphaU32(UInt32 const& a) -> void
{
    _a.SetU32(a);
}

///
/// @brief Set alpha channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetAlphaF32(Float32 const& a) -> void
{
    _a.SetF32(a);
}

///
/// @brief Set alpha channel.
///
template <class Tag>
constexpr auto RGBAColorT<Tag>::SetAlphaF64(Float64 const& a) -> void
{
    _a.SetF64(a);
}
}
}
