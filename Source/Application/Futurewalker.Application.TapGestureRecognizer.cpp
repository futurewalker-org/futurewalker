// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TapGestureRecognizer.hpp"
#include "Futurewalker.Application.TapGestureEvent.hpp" 
#include "Futurewalker.Application.PointerEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
/// @param event
/// @param area
///
auto TapGestureRecognizer::Recognize(const Delegate& delegate, const Event<PointerEvent>& event, const Rect<Dp>& area) -> Bool
{
    const auto pointerId = event->GetPointerId();
    const auto pos = event->GetPosition();

    if (event.Is<PointerEvent::Motion::Down>())
    {
        if (Rect<Dp>::Intersect(area, pos))
        {
            if (delegate.capturePointer)
            {
                delegate.capturePointer(pointerId);
            }
            SetPressed(true, false);
        }
        return true;
    }
    else if (event.Is<PointerEvent::Motion::Up>())
    {
        if (_pressed)
        {
            if (delegate.releasePointer)
            {
                delegate.releasePointer(pointerId);
            }

            SetPressed(false, false);

            if (Rect<Dp>::Intersect(area, pos))
            {
                auto gestureEvent = Event<>(Event<TapGestureEvent::Tap>::Make());
                SendEventDetached(gestureEvent);
            }
        }
        return true;
    }
    else if (event.Is<PointerEvent::Motion::Cancel>())
    {
        if (delegate.releasePointer)
        {
            delegate.releasePointer(pointerId);
        }
        SetPressed(false, true);
        return true;
    }
    return false;
}

///
/// @brief
///
/// @param pressed
/// @param cancel
///
auto TapGestureRecognizer::SetPressed(Bool const pressed, Bool const cancel) -> void
{
    if (_pressed != pressed)
    {
        _pressed = pressed;

        if (_pressed)
        {
            auto event = Event<>(Event<TapGestureEvent::Down>::Make());
            SendEventDetached(event);
        }
        else
        {
            if (cancel)
            {
                auto event = Event<>(Event<TapGestureEvent::Cancel>::Make());
                SendEventDetached(event);
            }
            else
            {
                auto event = Event<>(Event<TapGestureEvent::Up>::Make());
                SendEventDetached(event);
            }
        }
    }
}
}
