// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.DisplayListType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Container of recorded drawing commands.
///
class DisplayList : NonCopyable
{
public:
    virtual ~DisplayList() = 0;
    virtual auto GetBounds() const -> Rect<Dp> = 0;
};
}
}
