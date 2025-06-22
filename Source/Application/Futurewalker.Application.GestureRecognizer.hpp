// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.GestureRecognizerType.hpp" 
#include "Futurewalker.Application.PointerEvent.hpp"

#include "Futurewalker.Event.EventReceiverType.hpp"
#include "Futurewalker.Event.Event.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Function.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Gesture recognizer class.
///
class GestureRecognizer : NonCopyable
{
public:
    GestureRecognizer();

    struct Delegate
    {
        Function<void(PointerId)> capturePointer;
        Function<void(PointerId)> releasePointer;
    };
    virtual ~GestureRecognizer() = 0;
    virtual auto PointerIntercept(Delegate const& delegate, Event<PointerEvent> const& event, Rect<Dp> const& area) -> Bool = 0;
    virtual auto Pointer(Delegate const& delegate, Event<PointerEvent> const& event, Rect<Dp> const& area) -> Bool = 0;

    auto SendEvent(Event<>& event) -> Async<Bool>;
    auto SendEventDetached(Event<>& event) -> Bool;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

private:
    Shared<EventReceiver> _eventReceiver;
};
}
}
