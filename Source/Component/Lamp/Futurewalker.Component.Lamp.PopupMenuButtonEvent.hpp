// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class PopupMenuButtonEvent : public ViewEvent::Notify
{
public:
    class Down;
    class Up;
    class Enter;
    class Leave;
};

class PopupMenuButtonEvent::Down final : public PopupMenuButtonEvent
{
};

class PopupMenuButtonEvent::Up final : public PopupMenuButtonEvent
{
};

class PopupMenuButtonEvent::Enter final : public PopupMenuButtonEvent
{
};

class PopupMenuButtonEvent::Leave final : public PopupMenuButtonEvent
{
};
}
}
