// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TapGestureRecognizer.hpp"
#include "Futurewalker.Application.TapGestureEvent.hpp" 
#include "Futurewalker.Application.PointerEvent.hpp"

#include "Futurewalker.Event.EventWaiter.hpp"

#include "Futurewalker.Core.MonotonicClock.hpp"

namespace FW_DETAIL_NS
{
namespace
{
struct RecognizerEvent final : EventParameter
{
    Event<PointerEvent> pointerEvent;
    Rect<Dp> area;
    Shared<Bool> result;
};
}

///
/// @brief Set allowed buttons for tap gesture recognition.
///
auto TapGestureRecognizer::SetAllowedButtons(PointerButtonFlags const buttons) -> void
{
    _allowedButtons = buttons;
}

///
/// @brief Set allowed actions for tap gesture recognition.
///
auto TapGestureRecognizer::SetAllowedActions(TapGestureActionFlags const actions) -> void
{
    _allowedActions = actions;
}

///
/// @brief Start gesture recognition.
///
/// @param delegate 
///
auto TapGestureRecognizer::Start(Delegate const& delegate) -> void
{
    if (!_taskHandle)
    {
        _delegate = delegate;
        _taskHandle = AsyncFunction::SpawnFn(std::bind(&TapGestureRecognizer::InternalRecognize, this));
    }
}

///
/// @brief Stop gesture recognition.
///
auto TapGestureRecognizer::Stop() -> void
{
    if (_taskHandle)
    {
        _taskHandle->Cancel();
        _taskHandle.Reset();
        _delegate = {};

        auto dummyEvent = Event<>();
        GetEventReceiver().SendEventDetached(dummyEvent);
    }
}

///
/// @brief Recognize tap gesture.
///
/// @param event
/// @param area
///
auto TapGestureRecognizer::Recognize(const Event<PointerEvent>& event, const Rect<Dp>& area) -> Bool
{
    if (_taskHandle)
    {
        auto recognizerEvent = Event<RecognizerEvent>();
        recognizerEvent->pointerEvent = event;
        recognizerEvent->area = area;
        recognizerEvent->result = Shared<Bool>::Make(false);
        GetEventReceiver().SendEventDetached(recognizerEvent);
        return *recognizerEvent->result;
    }
    return false;
}

///
/// @brief Recognize first down event.
///
auto TapGestureRecognizer::InternalRecognizeFirstDown() -> Async<Bool>
{
    while (true)
    {
        auto event = co_await EventWaiter(*this).Wait<RecognizerEvent>();
        auto const& pointerEvent = event->pointerEvent;
        auto const& area = event->area;
        auto const& result = event->result;

        const auto pointerId = pointerEvent->GetPointerId();
        const auto pos = pointerEvent->GetPosition();

        if (pointerEvent.Is<PointerEvent::Motion::Down>())
        {
            *result = true;

            if (Rect<Dp>::Intersect(area, pos))
            {
                if (_delegate.capturePointer)
                {
                    _delegate.capturePointer(pointerId);
                }
                auto gestureEvent = Event<>(Event<TapGestureEvent::Down>());
                SendEventDetached(gestureEvent);
            }
            co_return true;
        }
        else if (pointerEvent.Is<PointerEvent::Motion::Cancel>())
        {
            *result = true;

            if (_delegate.releasePointer)
            {
                _delegate.releasePointer(pointerId);
            }
            co_return false;
        }
    }
}

///
/// @brief Recognize first up event.
///
auto TapGestureRecognizer::InternalRecognizeFirstUp() -> Async<Bool>
{
    while (true)
    {
        auto const event = co_await EventWaiter(*this).Wait<RecognizerEvent>();

        auto const& pointerEvent = event->pointerEvent;
        auto const& area = event->area;
        auto const& result = event->result;

        const auto pointerId = pointerEvent->GetPointerId();
        const auto pos = pointerEvent->GetPosition();

        if (pointerEvent.Is<PointerEvent::Motion::Up>())
        {
            *result = true;

            if (_delegate.releasePointer)
            {
                _delegate.releasePointer(pointerId);
            }

            auto gestureEvent = Event<>(Event<TapGestureEvent::Up>());
            SendEventDetached(gestureEvent);

            if (Rect<Dp>::Intersect(area, pos))
            {
                auto tapEventParameter = Event<TapGestureEvent::Tap>();
                tapEventParameter->SetTapCount(1);
                auto sendingEvent = Event<>(tapEventParameter);
                SendEventDetached(sendingEvent);
            }
            co_return true;
        }
        else if (event->pointerEvent.Is<PointerEvent::Motion::Cancel>())
        {
            *result = true;

            if (_delegate.releasePointer)
            {
                _delegate.releasePointer(pointerId);
            }
            auto gestureEvent = Event<>(Event<TapGestureEvent::Cancel>());
            SendEventDetached(gestureEvent);
            co_return false;
        }
    }
}

///
/// @brief Recognize tap gesture.
///
auto TapGestureRecognizer::InternalRecognize() -> Async<void>
{
    while (true)
    {
        if (co_await InternalRecognizeFirstDown())
        {
            co_await InternalRecognizeFirstUp();
        }
    }
}
}
