// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TapGestureEventType.hpp"
#include "Futurewalker.Application.GestureEvent.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Tap gesture event.
///
class TapGestureEvent : public GestureEvent
{
public:
    class Begin;
    class Cancel;
    class Tap;
};

class TapGestureEvent::Begin final : public TapGestureEvent
{
};

class TapGestureEvent::Cancel final : public TapGestureEvent
{
};

class TapGestureEvent::Tap final : public TapGestureEvent
{
public:
    auto GetTapCount() const -> SInt32
    {
        return _tapCount;
    }

    auto SetTapCount(SInt32 const count) -> void
    {
        _tapCount = count;
    }

private:
    SInt32 _tapCount = 0;
};
}
}
