// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformInputEditable.hpp"

namespace FW_DETAIL_NS
{
PlatformInputEditable::PlatformInputEditable(PassKey<PlatformInputEditable>, Delegate const& delegate)
  : _delegate {delegate}
{
}

PlatformInputEditable::~PlatformInputEditable() = default;

auto PlatformInputEditable::SendInputEvent(Event<>& event) -> Bool
{
    if (_delegate.sendInputevent)
    {
        return _delegate.sendInputevent(event);
    }
    return false;
}
}
