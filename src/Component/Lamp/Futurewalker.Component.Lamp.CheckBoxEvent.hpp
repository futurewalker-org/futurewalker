// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class CheckBoxEvent : public ViewEvent::Notify
{
public:
    class Pressed;
};

class CheckBoxEvent::Pressed final : public CheckBoxEvent
{
};
}
}
