// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ButtonViewEvent : public ViewEvent::Notify
{
public:
    class Down;
    class Up;
    class Enter;
    class Leave;
    class Press;
};

class ButtonViewEvent::Down : public ButtonViewEvent
{
};

class ButtonViewEvent::Up : public ButtonViewEvent
{
};

class ButtonViewEvent::Press : public ButtonViewEvent
{
};

class ButtonViewEvent::Enter : public ButtonViewEvent
{
};

class ButtonViewEvent::Leave : public ButtonViewEvent
{
};
}
}
