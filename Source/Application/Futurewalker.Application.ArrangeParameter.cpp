// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ArrangeParameter.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto ArrangeParameter::GetOldFrameRect() const -> Rect<Dp>
{
    return _oldFrameRect;
}

///
/// @brief
///
/// @param frameRect
///
auto ArrangeParameter::SetOldFrameRect(Rect<Dp> const& frameRect) -> void
{
    _oldFrameRect = frameRect;
}

///
/// @brief
///
auto ArrangeParameter::GetNewFrameRect() const -> Rect<Dp>
{
    return _newFrameRect;
}

///
/// @brief
///
/// @param frameRect
///
auto ArrangeParameter::SetNewFrameRect(Rect<Dp> const& frameRect) -> void
{
    _newFrameRect = frameRect;
}
}
