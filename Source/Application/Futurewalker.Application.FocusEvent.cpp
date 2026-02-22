// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.FocusEvent.hpp"

namespace FW_DETAIL_NS
{
auto FocusEvent::FocusIn::GetReason() const -> FocusReason
{
    return _reason;
}

auto FocusEvent::FocusIn::SetReason(FocusReason const reason) -> void
{
    _reason = reason;
}
}
