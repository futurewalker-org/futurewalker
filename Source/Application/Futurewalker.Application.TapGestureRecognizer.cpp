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
auto TapGestureRecognizer::PointerIntercept(const Delegate& delegate, const PointerEvent& event, const Rect<Dp>& area) -> Bool
{
    (void)delegate;
    (void)event;
    (void)area;
    return false;
}

///
/// @brief 
///
/// @param event 
/// @param area 
///
auto TapGestureRecognizer::Pointer(const Delegate& delegate, const PointerEvent& event, const Rect<Dp>& area) -> Bool
{
    const auto pointerId = event.GetPointerId();
    const auto type = event.GetEventType();
    const auto pos = event.GetPosition();

    if (type == PointerEventType::Down)
    {
        if (Rect<Dp>::Intersect(area, pos))
        {
            if (delegate.capturePointer)
            {
                delegate.capturePointer(pointerId);
            }

            SetPressed(true);
        }
    }
    else if (type == PointerEventType::Up)
    {
        if (_pressed)
        {
            if (delegate.releasePointer)
            {
                delegate.releasePointer(pointerId);
            }

            SetPressed(false);

            if (Rect<Dp>::Intersect(area, pos))
            {
                auto gestureEvent = Event(TapGestureEvent(TapGestureEventType::Tap));
                SendEventDetached(gestureEvent);
            }
        }
    }
    else if (type == PointerEventType::Cancel)
    {
        if (delegate.releasePointer)
        {
            delegate.releasePointer(pointerId);
        }
        SetPressed(false);
    }
    return true;
}

///
/// @brief 
///
/// @param pressed 
///
auto TapGestureRecognizer::SetPressed(const Bool pressed) -> void
{
    if (_pressed != pressed)
    {
        _pressed = pressed;

        if (_pressed)
        {
            auto event = Event(TapGestureEvent(TapGestureEventType::Down));
            SendEventDetached(event);
        }
        else
        {
            auto event = Event(TapGestureEvent(TapGestureEventType::Up));
            SendEventDetached(event);
        }
    }
}
}
