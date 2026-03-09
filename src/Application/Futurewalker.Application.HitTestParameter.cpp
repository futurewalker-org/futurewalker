// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.HitTestParameter.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto HitTestParameter::GetPosition() const -> Point<Dp> const&
{
    return _position;
}

///
/// @brief
///
/// @param position
///
auto HitTestParameter::SetPosition(Point<Dp> const& position) -> void
{
    _position = position;
}
}
