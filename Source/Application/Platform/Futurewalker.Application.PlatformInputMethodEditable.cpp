// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformInputMethodEditable.hpp"

namespace FW_DETAIL_NS
{
PlatformInputMethodEditable::PlatformInputMethodEditable(PassKey<PlatformInputMethodEditable>, Delegate const& delegate)
  : _delegate {delegate}
{
}

PlatformInputMethodEditable::~PlatformInputMethodEditable() = default;

auto PlatformInputMethodEditable::SendInputEvent(Event<>& event) -> Async<Bool>
{
    if (_delegate.sendInputevent)
    {
        co_return co_await _delegate.sendInputevent(event);
    }
    co_return false;
}

auto PlatformInputMethodEditable::SendInputEventDetached(Event<>& event) -> Bool
{
    auto const self = GetSelf();
    auto const e = Shared<Event<>>::Make(event);
    auto const r = Shared<Bool>::Make(false);
    AsyncFunction::SpawnFn([=] -> Lazy<void> { *r = co_await self->SendInputEvent(*e); });
    if (*r)
    {
        event = *e;
        return true;
    }
    return false;
}
}
