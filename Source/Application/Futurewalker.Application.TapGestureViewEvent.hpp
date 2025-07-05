// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TapGestureViewEventType.hpp"
#include "Futurewalker.Application.TapGestureEventType.hpp" 
#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TapGestureViewEvent : public ViewEvent::Notify
{
public:
    class Down;
    class Up;
    class Cancel;
    class Tap;
};

class TapGestureViewEvent::Down final : public TapGestureViewEvent
{
};

class TapGestureViewEvent::Up final : public TapGestureViewEvent
{
};

class TapGestureViewEvent::Cancel final : public TapGestureViewEvent
{
};

class TapGestureViewEvent::Tap final : public TapGestureViewEvent
{
};
}
}
