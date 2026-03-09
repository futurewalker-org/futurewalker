// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TextEditEvent.hpp"

namespace FW_DETAIL_NS
{
auto TextEditEvent::FocusChanged::IsFocused() const -> Bool
{
    return _focused;
}

auto TextEditEvent::FocusChanged::SetFocused(Bool focused) -> void
{
    _focused = focused;
}
}
