// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.HitTestEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto HitTestEvent::GetPosition() const -> Point<Dp>
{
    return _position;
}

///
/// @brief
///
/// @param position
///
auto HitTestEvent::SetPosition(Point<Dp> const& position) -> void
{
    _position = position;
}

///
/// @brief
///
auto HitTestEvent::GetHit() const -> Bool
{
    return _hit;
}

///
/// @brief
///
/// @param hit
///
auto HitTestEvent::SetHit(Bool const hit) -> void
{
    _hit = hit;
}
}
