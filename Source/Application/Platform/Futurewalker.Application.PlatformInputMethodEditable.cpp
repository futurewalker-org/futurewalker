// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.PlatformInputMethodEditable.hpp"

namespace FW_DETAIL_NS
{
PlatformInputMethodEditable::PlatformInputMethodEditable(Delegate const& delegate)
  : _delegate {delegate}
{
}

PlatformInputMethodEditable::~PlatformInputMethodEditable() = default;

auto PlatformInputMethodEditable::SendInputEvent(Event& event) -> Async<Bool>
{
    if (_delegate.sendInputevent)
    {
        co_return co_await _delegate.sendInputevent(event);
    }
    co_return false;
}
}
