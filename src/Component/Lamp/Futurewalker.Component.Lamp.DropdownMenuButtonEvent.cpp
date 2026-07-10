// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.DropdownMenuButtonEvent.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto DropdownMenuButtonEvent::CurrentItemChanged::GetCurrentIndex() const -> SInt64
{
    return _currentIndex;
}

auto DropdownMenuButtonEvent::CurrentItemChanged::SetCurrentIndex(SInt64 const index) -> void
{
    _currentIndex = index;
}
}
