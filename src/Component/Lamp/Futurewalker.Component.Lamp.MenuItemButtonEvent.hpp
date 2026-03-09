// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class MenuItemButtonEvent : public ViewEvent::Notify
{
public:
    class Down;
    class Up;
    class Enter;
    class Leave;
};

class MenuItemButtonEvent::Down final : public MenuItemButtonEvent
{
};

class MenuItemButtonEvent::Up final : public MenuItemButtonEvent
{
};

class MenuItemButtonEvent::Enter final : public MenuItemButtonEvent
{
};

class MenuItemButtonEvent::Leave final : public MenuItemButtonEvent
{
};
}
}
