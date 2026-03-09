// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PopupEvent : public EventParameter
{
public:
    class Closed;
};

class PopupEvent::Closed final : public PopupEvent
{
};
}
}
