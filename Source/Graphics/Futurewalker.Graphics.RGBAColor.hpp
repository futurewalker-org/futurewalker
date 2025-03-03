// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.RGBAColorType.hpp"
#include "Futurewalker.Graphics.RGBColor.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief RGBA color value.
///
/// @tparam ColorTag Color tag type
/// @tparam AlphaTag Alpha tag type
///
template <class ColorTag, class AlphaTag>
class RGBAColorT
{
    using ColorChannelType = ChannelT<ColorTag>;
    using AlphaChannelType = ChannelT<AlphaTag>;
    using RGBColorType = RGBColorT<ColorTag>;

public:
    static constexpr auto MakeFromU8(const UInt8 r, const UInt8 g, const UInt8 b, const UInt8 a) -> RGBAColorT;
    static constexpr auto MakeFromU16(const UInt16 r, const UInt16 g, const UInt16 b, const UInt16 a) -> RGBAColorT;
    static constexpr auto MakeFromU32(const UInt32 r, const UInt32 g, const UInt32 b, const UInt32 a) -> RGBAColorT;

public:
    constexpr RGBAColorT() = default;
    constexpr RGBAColorT(const ColorChannelType& r, const ColorChannelType& g, const ColorChannelType& b, const AlphaChannelType& a);
    constexpr RGBAColorT(const RGBColorType& rgb, const AlphaChannelType& a);

    constexpr auto GetRGBColor() const -> RGBColorType;
    constexpr auto SetRGBColor(const RGBColorType& rgb) -> void;

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

    constexpr auto GetAlpha() const -> AlphaChannelType;
    constexpr auto GetAlphaU8() const -> UInt8;
    constexpr auto GetAlphaU16() const -> UInt16;
    constexpr auto GetAlphaU32() const -> UInt32;
    constexpr auto GetAlphaF64() const -> Float64;

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

    constexpr auto SetAlphaU8(const UInt8& b) -> void;
    constexpr auto SetAlphaU16(const UInt16& b) -> void;
    constexpr auto SetAlphaU32(const UInt32& b) -> void;
    constexpr auto SetAlphaF64(const Float64& b) -> void;

    friend inline constexpr bool operator==(const RGBAColorT&, const RGBAColorT&) = default;
    friend inline constexpr bool operator!=(const RGBAColorT&, const RGBAColorT&) = default;

private:
    ColorChannelType _r;
    ColorChannelType _g;
    ColorChannelType _b;
    AlphaChannelType _a;
};

///
/// @brief Get RGBColor.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetRGBColor() const -> RGBColorType
{
    return RGBColorType(_r, _g, _b);
}

///
/// @brief Set RGBColor.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetRGBColor(const RGBColorType& rgb) -> void
{
    _r = rgb.GetRed();
    _g = rgb.GetGreen();
    _b = rgb.GetBlue();
}

///
/// @brief Set UInt8 channels.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::MakeFromU8(const UInt8 r, const UInt8 g, const UInt8 b, const UInt8 a) -> RGBAColorT
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
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::MakeFromU16(const UInt16 r, const UInt16 g, const UInt16 b, const UInt16 a) -> RGBAColorT
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
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::MakeFromU32(const UInt32 r, const UInt32 g, const UInt32 b, const UInt32 a) -> RGBAColorT
{
    auto color = RGBAColorT();
    color.SetRedU32(r);
    color.SetGreenU32(g);
    color.SetBlueU32(b);
    color.SetAlphaU32(a);
    return color;
}

///
/// @brief Construct from channels.
///
template <class ColorTag, class AlphaTag>
constexpr RGBAColorT<ColorTag, AlphaTag>::RGBAColorT(const ColorChannelType& r, const ColorChannelType& g, const ColorChannelType& b, const AlphaChannelType& a)
  : _r {r}
  , _g {g}
  , _b {b}
  , _a {a}
{
}

///
/// @brief Construct from RGBColor and AlphaChannel.
///
template <class ColorTag, class AlphaTag>
constexpr RGBAColorT<ColorTag, AlphaTag>::RGBAColorT(const RGBColorType& rgb, const AlphaChannelType& a)
  : _r {rgb.GetRed()}
  , _g {rgb.GetGreen()}
  , _b {rgb.GetBlue()}
  , _a {a}
{
}

///
/// @brief Get red channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetRed() const -> ColorChannelType
{
    return _r;
}

///
/// @brief Get red channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetRedU8() const -> UInt8
{
    return _r.GetU8();
}

///
/// @brief Get red channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetRedU16() const -> UInt16
{
    return _r.GetU16();
}

///
/// @brief Get red channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetRedU32() const -> UInt32
{
    return _r.GetU32();
}

///
/// @brief Get red channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetRedF64() const -> Float64
{
    return _r.GetF64();
}

///
/// @brief Get green channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetGreen() const -> ColorChannelType
{
    return _g; 
}

///
/// @brief Get green channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetGreenU8() const -> UInt8
{
    return _g.GetU8();
}

///
/// @brief Get green channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetGreenU16() const -> UInt16
{
    return _g.GetU16();
}

///
/// @brief Get green channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetGreenU32() const -> UInt32
{
    return _g.GetU32();
}

///
/// @brief Get green channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetGreenF64() const -> Float64
{
    return _g.GetF64();
}

///
/// @brief Get blue channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetBlue() const -> ColorChannelType
{
    return _b;
}

///
/// @brief Get blue channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetBlueU8() const -> UInt8
{
    return _b.GetU8();
}

///
/// @brief Get blue channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetBlueU16() const -> UInt16
{
    return _b.GetU16();
}

///
/// @brief Get blue channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetBlueU32() const -> UInt32
{
    return _b.GetU32();
}

///
/// @brief Get blue channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetBlueF64() const -> Float64
{
    return _b.GetF64();
}

///
/// @brief Get alpha channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetAlpha() const -> AlphaChannelType
{
    return _a;
}

///
/// @brief Get alpha channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetAlphaU8() const -> UInt8
{
    return _a.GetU8();
}

///
/// @brief Get alpha channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetAlphaU16() const -> UInt16
{
    return _a.GetU16();
}

///
/// @brief Get alpha channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetAlphaU32() const -> UInt32
{
    return _a.GetU32();
}

///
/// @brief Get alpha channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::GetAlphaF64() const -> Float64
{
    return _a.GetF64();
}

///
/// @brief Set red channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetRed(const ColorChannelType& r) -> void
{
    _r = r;
}

///
/// @brief Set red channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetRedU8(const UInt8& r) -> void
{
    _r.SetU8(r);
}

///
/// @brief Set red channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetRedU16(const UInt16& r) -> void
{
    _r.SetU16(r);
}

///
/// @brief Set red channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetRedU32(const UInt32& r) -> void
{
    _r.SetU32(r);
}

///
/// @brief Set red channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetRedF64(const Float64& r) -> void
{
    _r.SetF64(r);
}

///
/// @brief Set green channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetGreenU8(const UInt8& g) -> void
{
    _g.SetU8(g);
}

///
/// @brief Set green channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetGreenU16(const UInt16& g) -> void
{
    _g.SetU16(g);
}

///
/// @brief Set green channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetGreenU32(const UInt32& g) -> void
{
    _g.SetU32(g);
}

///
/// @brief Set green channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetGreenF64(const Float64& g) -> void
{
    _g.SetF64(g);
}

///
/// @brief Set blue channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetBlueU8(const UInt8& b) -> void
{
    _b.SetU8(b);
}

///
/// @brief Set blue channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetBlueU16(const UInt16& b) -> void
{
    _b.SetU16(b);
}

///
/// @brief Set blue channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetBlueU32(const UInt32& b) -> void
{
    _b.SetU32(b);
}

///
/// @brief Set blue channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetBlueF64(const Float64& b) -> void
{
    _b.SetF64(b);
}

///
/// @brief Set alpha channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetAlphaU8(const UInt8& b) -> void
{
    _a.SetU8(b);
}

///
/// @brief Set alpha channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetAlphaU16(const UInt16& b) -> void
{
    _a.SetU16(b);
}

///
/// @brief Set alpha channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetAlphaU32(const UInt32& b) -> void
{
    _a.SetU32(b);
}

///
/// @brief Set alpha channel.
///
template <class ColorTag, class AlphaTag>
constexpr auto RGBAColorT<ColorTag, AlphaTag>::SetAlphaF64(const Float64& b) -> void
{
    _a.SetF64(b);
}
}
}
