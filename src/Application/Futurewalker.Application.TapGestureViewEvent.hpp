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
    class Begin;
    class Cancel;
    class Tap;
};

///
/// @brief Notifies that a tap gesture has begun.
///
/// @note After this event, either Tap or TapCancel will be sent.
///
class TapGestureViewEvent::Begin final : public TapGestureViewEvent
{
};

///
/// @brief Notifies that a tap gesture has been canceled.
///
class TapGestureViewEvent::Cancel final : public TapGestureViewEvent
{
};

///
/// @brief Notifies that a tap gesture has been recognized.
///
class TapGestureViewEvent::Tap final : public TapGestureViewEvent
{
};
}
}
