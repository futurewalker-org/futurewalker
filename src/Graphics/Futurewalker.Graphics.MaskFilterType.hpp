// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Prelude.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
class MaskFilter;

///
/// @brief Blur style.
///
enum class BlurStyle
{
    Normal,
    Solid,
    Outer,
    Inner,
};
}
}
