// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.Prelude.hpp"
#include "Futurewalker.Graphics.Channel.hpp"
#include "Futurewalker.Graphics.RGBColor.hpp"
#include "Futurewalker.Graphics.RGBAColor.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ColorSpaceTagRGB;       ///< Standard RGB color space (sRGB with gamma encoding).
class ColorSpaceTagLinearRGB; ///< Standard Linear RGB color space (sRGB without gamma encoding).
class ColorSpaceTagAlpha;     ///< Alpha channel.

using RGBChannel = ChannelT<ColorSpaceTagRGB>;
using LinearRGBChannel = ChannelT<ColorSpaceTagLinearRGB>;
using AlphaChannel = ChannelT<ColorSpaceTagAlpha>;

using RGBColor = RGBColorT<ColorSpaceTagRGB>;
using RGBAColor = RGBAColorT<ColorSpaceTagRGB, ColorSpaceTagAlpha>;

using LinearRGBColor = RGBColorT<ColorSpaceTagLinearRGB>;
using LinearRGBAColor = RGBAColorT<ColorSpaceTagLinearRGB, ColorSpaceTagAlpha>;
}
}
