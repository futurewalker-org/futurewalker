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

auto PlatformApplicationTheme::SendThemeEvent(Event<>& event) -> Bool
{
    if (_delegate.sendThemeEvent)
    {
        return _delegate.sendThemeEvent(event);
    }
    return false;
}
}
