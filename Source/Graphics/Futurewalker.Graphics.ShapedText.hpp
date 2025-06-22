// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.ShapedTextType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Shaped text.
///
class ShapedText : NonCopyable
{
public:
    virtual ~ShapedText() = 0;
    virtual auto GetBounds() const -> Rect<Dp> = 0;
};
}
}
