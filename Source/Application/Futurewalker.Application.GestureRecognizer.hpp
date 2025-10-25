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

    virtual ~GestureRecognizer() = 0;

    struct Delegate
    {
        Function<void(PointerId)> capturePointer;
        Function<void(PointerId)> releasePointer;
    };

    ///
    /// @brief Start gesture recognition.
    ///
    /// @param delegate Delegate for pointer capture and release.
    ///
    virtual auto Start(Delegate const& delegate) -> void = 0;

    ///
    /// @brief Stop gesture recognition.
    ///
    virtual auto Stop() -> void = 0;

    ///
    /// @brief Process pointer event and recognize gesture.
    ///
    /// @param event Pointer event to recognize.
    /// @param area Detection area.
    ///
    /// @return True if pointer event was consumed by this recognizer.
    ///
    virtual auto Recognize(Event<PointerEvent> const& event, Rect<Dp> const& area) -> Bool = 0;

    auto SendEvent(Event<>& event) -> Async<Bool>;
    auto SendEventDetached(Event<>& event) -> Bool;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

private:
    Shared<EventReceiver> _eventReceiver;
};
}
}
