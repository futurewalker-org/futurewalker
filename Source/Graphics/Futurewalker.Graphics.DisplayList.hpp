// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.DisplayListType.hpp"
#include "Futurewalker.Graphics.Rect.hpp"
#include "Futurewalker.Graphics.GraphicsUnits.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
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
