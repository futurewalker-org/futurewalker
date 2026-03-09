// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformApplicationTheme.hpp"
#include "Futurewalker.Application.PlatformApplicationThemeContext.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
PlatformApplicationTheme::PlatformApplicationTheme(PassKey<PlatformApplicationTheme>, Delegate const& delegate, Shared<PlatformApplicationThemeContext> const& context)
  : _delegate(delegate)
  , _context(context)
{
}

PlatformApplicationTheme::~PlatformApplicationTheme() = default;

auto PlatformApplicationTheme::GetContext() -> Shared<PlatformApplicationThemeContext>
{
    return _context;
}
auto PlatformApplicationTheme::GetContext() const -> Shared<PlatformApplicationThemeContext const>
{
    return _context;
}

auto PlatformApplicationTheme::SendThemeEvent(Event<>& event) -> Async<Bool>
{
    if (_delegate.sendThemeEvent)
    {
        co_return co_await _delegate.sendThemeEvent(event);
    }
    co_return false;
}

auto PlatformApplicationTheme::SendThemeEventDetached(Event<> const& event) -> void
{
    AsyncFunction::SpawnFn([self = GetSelf(), e = event]() mutable -> Task<void> {
        try
        {
            co_await self->SendThemeEvent(e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }).Detach();
}
}
