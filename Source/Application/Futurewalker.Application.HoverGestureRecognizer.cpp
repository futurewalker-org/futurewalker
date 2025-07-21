// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.HoverGestureRecognizer.hpp"
#include "Futurewalker.Application.HoverGestureEvent.hpp" 
#include "Futurewalker.Application.PointerEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
/// @param delegate
/// @param event
/// @param area
///
auto HoverGestureRecognizer::Recognize(const Delegate& delegate, const Event<PointerEvent>& event, const Rect<Dp>& area) -> Bool
{
    (void)delegate;
    (void)area;

    if (event.Is<PointerEvent::Motion::Enter>())
    {
        auto hoverEvent = Event<>(Event<HoverGestureEvent::Enter>::Make());
        SendEventDetached(hoverEvent);
        return true;
    }
    else if (event.Is<PointerEvent::Motion::Leave>())
    {
        auto hoverEvent = Event<>(Event<HoverGestureEvent::Leave>::Make());
        SendEventDetached(hoverEvent);
        return true;
    }
    return false;
}
}
