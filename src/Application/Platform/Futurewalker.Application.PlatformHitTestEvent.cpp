// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformHitTestEvent.hpp"

namespace FW_DETAIL_NS
{
auto PlatformHitTestEvent::GetPosition() const -> Point<Dp>
{
    return _position;
}

auto PlatformHitTestEvent::SetPosition(Point<Dp> const& position) -> void
{
    _position = position;
}

auto PlatformHitTestEvent::GetHit() const -> Bool
{
    return _hit;
}

auto PlatformHitTestEvent::SetHit(Bool const hit) -> void
{
    _hit = hit;
}
}
