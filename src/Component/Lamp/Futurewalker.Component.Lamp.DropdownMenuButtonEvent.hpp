// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class DropdownMenuButtonEvent : public ViewEvent::Notify
{
public:
    class CurrentItemChanged;
};

class DropdownMenuButtonEvent::CurrentItemChanged final : public DropdownMenuButtonEvent
{
public:
    auto GetCurrentIndex() const -> SInt64;
    auto SetCurrentIndex(SInt64 const index) -> void;

private:
    SInt64 _currentIndex = -1;
};
}
}
