// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformApplicationThemeMac.hpp"
#include "Futurewalker.Application.Mac.PlatformApplicationThemeContextMac.hpp"

namespace FW_DETAIL_NS
{
auto PlatformApplicationThemeMac::Make(PlatformApplicationTheme::Delegate const& delegate, Shared<PlatformApplicationThemeContextMac> const& context) -> Shared<PlatformApplicationThemeMac>
{
    return PlatformApplicationTheme::MakeDerived<PlatformApplicationThemeMac>(delegate, context);
}

PlatformApplicationThemeMac::PlatformApplicationThemeMac(PassKey<PlatformApplicationTheme> key, Delegate const& delegate, Shared<PlatformApplicationThemeContextMac> const& context)
  : PlatformApplicationTheme(key, delegate, context)
{
}

auto PlatformApplicationThemeMac::GetSystemBrightness() const -> ThemeBrightness
{
    return GetContext().As<PlatformApplicationThemeContextMac const>()->GetSystemBrightness();
}

auto PlatformApplicationThemeMac::NotifyBrightnessChanged() -> void
{
    auto themeEvent = Event<PlatformApplicationThemeEvent::SystemBrightnessChanged>();
    SendThemeEventDetached(themeEvent);
}
}
