// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformApplicationThemeContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationThemeWin.hpp"
#include "Futurewalker.Application.MainThread.hpp"

namespace FW_DETAIL_NS
{
namespace
{
// https://learn.microsoft.com/en-us/windows/apps/desktop/modernize/ui/apply-windows-themes
auto IsColorLight(winrt::Windows::UI::Color const& color)
{
    return (((5 * color.G) + (2 * color.R) + color.B) > (8 * 128));
}
}

auto PlatformApplicationThemeContextWin::Make() -> Shared<PlatformApplicationThemeContextWin>
{
    return PlatformApplicationThemeContext::MakeDerived<PlatformApplicationThemeContextWin>();
}

PlatformApplicationThemeContextWin::PlatformApplicationThemeContextWin(PassKey<PlatformApplicationThemeContext> key)
  : PlatformApplicationThemeContext(key)
{
}

auto PlatformApplicationThemeContextWin::GetSystemBrightness() const -> ThemeBrightness
{
    return _systemBrightness;
}

auto PlatformApplicationThemeContextWin::MakeApplicationTheme(PlatformApplicationTheme::Delegate const& delegate) -> Shared<PlatformApplicationTheme>
{
    auto theme = PlatformApplicationThemeWin::Make(delegate, GetSelf());
    _applicationThemes.push_back(theme);
    return theme;
}

void PlatformApplicationThemeContextWin::Initialize()
{
    auto weakSelf = Weak(GetSelf());
    _uiSettings = winrt::Windows::UI::ViewManagement::UISettings();
    _colorValuesChangedRevoker = _uiSettings.ColorValuesChanged(winrt::auto_revoke, [weakSelf](auto&&, auto&&) -> winrt::fire_and_forget {
        co_await MainThread::Schedule();
        if (auto const self = weakSelf.Lock())
        {
            self->UpdateSystemBrightness();
        }
    });
    _systemBrightness = GetCurrentSystemBrightness();
}

auto PlatformApplicationThemeContextWin::GetCurrentSystemBrightness() -> ThemeBrightness
{
    auto const foreground = _uiSettings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
    auto const darkMode = IsColorLight(foreground);
    return darkMode ? ThemeBrightness::Dark : ThemeBrightness::Light;
}

auto PlatformApplicationThemeContextWin::UpdateSystemBrightness() -> void
{
    auto const newBrightness = GetCurrentSystemBrightness();
    if (_systemBrightness != newBrightness)
    {
        _systemBrightness = newBrightness;

        for (auto const& weakTheme : _applicationThemes)
        {
            if (auto const theme = weakTheme.Lock())
            {
                theme->NotifyBrightnessChanged();
            }
        }
    }
}

auto Locator::Resolver<PlatformApplicationThemeContextWin>::Resolve() -> Shared<PlatformApplicationThemeContextWin>
{
    return PlatformApplicationThemeContextWin::Make();
}
}
