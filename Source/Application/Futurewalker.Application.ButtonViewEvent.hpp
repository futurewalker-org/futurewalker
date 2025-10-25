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

class ButtonViewEvent::Down final : public ButtonViewEvent
{
};

class ButtonViewEvent::Up final : public ButtonViewEvent
{
};

class ButtonViewEvent::Press final : public ButtonViewEvent
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
