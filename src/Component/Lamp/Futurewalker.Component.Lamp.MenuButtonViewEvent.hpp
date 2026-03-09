// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class MenuButtonViewEvent : public ViewEvent::Notify
{
public:
    class Down;
    class Up;
    class Enter;
    class Leave;
};

class MenuButtonViewEvent::Down final : public MenuButtonViewEvent
{
};

class MenuButtonViewEvent::Up final : public MenuButtonViewEvent
{
};

class MenuButtonViewEvent::Enter final : public MenuButtonViewEvent
{
};

class MenuButtonViewEvent::Leave final : public MenuButtonViewEvent
{
};
}
}
