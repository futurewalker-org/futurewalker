// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.Prelude.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
class Scene;

///
/// @brief Blend mode.
///
enum class BlendMode
{
    SrcOver,
    DstOver,
    Src,
    Dst,
};

///
/// @brief Drawing style.
///
enum class DrawStyle
{
    Fill,
    Stroke,
};
}
}
