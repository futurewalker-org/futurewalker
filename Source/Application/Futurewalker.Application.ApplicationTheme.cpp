// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ApplicationTheme.hpp"
#include "Futurewalker.Application.ApplicationContext.hpp"
#include "Futurewalker.Application.PlatformApplicationTheme.hpp"
#include "Futurewalker.Application.PlatformApplicationThemeContext.hpp"
#include "Futurewalker.Application.Theme.hpp"
#include "Futurewalker.Application.DefaultTheme.hpp"

#include "Futurewalker.Application.ApplicationColor.hpp"
#include "Futurewalker.Application.ApplicationStyle.hpp"

namespace FW_DETAIL_NS
{
ApplicationTheme::ApplicationTheme(Shared<PlatformApplicationThemeContext> const& platformContext, Shared<ApplicationContext> const& applicationContext)
  : _platformContext(platformContext)
  , _applicationContext(applicationContext)
{
    _platformObject = _platformContext->MakeApplicationTheme({
      .sendThemeEvent = [this](Event<>& event) -> Async<Bool> { co_return co_await HandlePlatformThemeEvent(event); },
    });

    PushTheme(ThemeBrightness::Dark, Shared<DefaultTheme>::Make(ThemeBrightness::Dark));
    PushTheme(ThemeBrightness::Light, Shared<DefaultTheme>::Make(ThemeBrightness::Light));

    UpdateCurrentBrightness();
}

auto ApplicationTheme::GetBrightness() -> ApplicationThemeBrightness
{
    return _brightness;
}

auto ApplicationTheme::SetBrightness(ApplicationThemeBrightness const brightness) -> void
{
    if (_brightness != brightness)
    {
        _brightness = brightness;
        UpdateCurrentBrightness();
    }
}

auto ApplicationTheme::PushTheme(ThemeBrightness const brightness, Shared<Theme> const& theme) -> void
{
    if (!theme)
    {
        return;
    }

    if (brightness == ThemeBrightness::Dark)
    {
        _darkThemes.push_back(theme);
    }
    else
    {
        _lightThemes.push_back(theme);
    }

    if (brightness == _currentBrightness)
    {
        ApplyTheme(theme);
    }
}

auto ApplicationTheme::ApplyTheme(Shared<Theme> const& theme) -> void
{
    if (theme && _applicationContext)
    {
        theme->Apply(_applicationContext->GetAttributeNode());
    }
}

auto ApplicationTheme::ApplyThemes() -> void
{
    if (_currentBrightness == ThemeBrightness::Dark)
    {
        for (auto const& theme : _darkThemes)
        {
            ApplyTheme(theme);
        }
    }
    else
    {
        for (auto const& theme : _lightThemes)
        {
            ApplyTheme(theme);
        }
    }
}

auto ApplicationTheme::UpdateCurrentBrightness() -> void
{
    auto newBrightness = _currentBrightness;

    if (_brightness == ApplicationThemeBrightness::System)
    {
        newBrightness = _platformObject->GetSystemBrightness();
    }
    else if (_brightness == ApplicationThemeBrightness::Dark)
    {
        newBrightness = ThemeBrightness::Dark;
    }
    else if (_brightness == ApplicationThemeBrightness::Light)
    {
        newBrightness = ThemeBrightness::Light;
    }

    if (_currentBrightness != newBrightness)
    {
        _currentBrightness = newBrightness;
        ApplyThemes();
    }
}

auto ApplicationTheme::HandlePlatformThemeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PlatformApplicationThemeEvent::SystemBrightnessChanged>())
    {
        UpdateCurrentBrightness();
    }
    co_return false;
}

auto Locator::Resolver<ApplicationTheme>::Resolve() -> Shared<ApplicationTheme>
{
    auto themeContext = Locator::Resolve<PlatformApplicationThemeContext>();
    auto applicationContext = Locator::Resolve<ApplicationContext>();
    return Shared<ApplicationTheme>::Make(themeContext, applicationContext);
}
}
