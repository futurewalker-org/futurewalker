// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformApplicationThemeWin.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationThemeContextWin.hpp"

namespace FW_DETAIL_NS
{
auto PlatformApplicationThemeWin::Make(PlatformApplicationTheme::Delegate const& delegate, Shared<PlatformApplicationThemeContextWin> const& context) -> Shared<PlatformApplicationThemeWin>
{
    return PlatformApplicationTheme::MakeDerived<PlatformApplicationThemeWin>(delegate, context);
}

PlatformApplicationThemeWin::PlatformApplicationThemeWin(PassKey<PlatformApplicationTheme> key, Delegate const& delegate, Shared<PlatformApplicationThemeContextWin> const& context)
  : PlatformApplicationTheme(key, delegate, context)
{
}

auto PlatformApplicationThemeWin::GetSystemBrightness() const -> ThemeBrightness
{
    return GetContext().As<PlatformApplicationThemeContextWin const>()->GetSystemBrightness();
}

auto PlatformApplicationThemeWin::NotifyBrightnessChanged() -> void
{
    auto themeEvent = Event<PlatformApplicationThemeEvent::SystemBrightnessChanged>();
    SendThemeEventDetached(themeEvent);
}
}
