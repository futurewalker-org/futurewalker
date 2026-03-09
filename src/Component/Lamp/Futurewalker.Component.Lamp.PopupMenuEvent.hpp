// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class PopupMenuEvent : public EventParameter
{
public:
    class Closed;
    class Activated;
};

class PopupMenuEvent::Closed final : public PopupMenuEvent
{
};

class PopupMenuEvent::Activated final : public PopupMenuEvent
{
};
}
}
