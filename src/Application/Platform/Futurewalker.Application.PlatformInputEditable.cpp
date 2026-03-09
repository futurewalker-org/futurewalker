// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformInputEditable.hpp"

namespace FW_DETAIL_NS
{
PlatformInputEditable::PlatformInputEditable(PassKey<PlatformInputEditable>, Delegate const& delegate)
  : _delegate {delegate}
{
}

PlatformInputEditable::~PlatformInputEditable() = default;

auto PlatformInputEditable::SendInputEvent(Event<>& event) -> Async<Bool>
{
    if (_delegate.sendInputevent)
    {
        co_return co_await _delegate.sendInputevent(event);
    }
    co_return false;
}

auto PlatformInputEditable::SendInputEventDetached(Event<>& event) -> Bool
{
    auto const self = GetSelf();
    auto const e = Shared<Event<>>::Make(event);
    auto const r = Shared<Bool>::Make(false);
    AsyncFunction::SpawnFn([=] -> Lazy<void> { *r = co_await self->SendInputEvent(*e); }).Detach();
    if (*r)
    {
        event = *e;
        return true;
    }
    return false;
}
}
