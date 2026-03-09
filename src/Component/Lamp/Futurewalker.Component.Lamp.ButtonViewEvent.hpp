// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class ButtonViewEvent : public ViewEvent::Notify
{
public:
    class Down;
    class Cancel;
    class Press;
    class Enter;
    class Leave;
};

class ButtonViewEvent::Down final : public ButtonViewEvent
{
};

class ButtonViewEvent::Press final : public ButtonViewEvent
{
};

class ButtonViewEvent::Cancel final : public ButtonViewEvent
{
};

class ButtonViewEvent::Enter final : public ButtonViewEvent
{
};

class ButtonViewEvent::Leave final : public ButtonViewEvent
{
};
}
}
