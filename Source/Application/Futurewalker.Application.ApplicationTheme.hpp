// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ApplicationThemeType.hpp"
#include "Futurewalker.Application.ThemeType.hpp"
#include "Futurewalker.Application.PlatformApplicationThemeType.hpp"
#include "Futurewalker.Application.PlatformApplicationThemeContextType.hpp"
#include "Futurewalker.Application.ApplicationContextType.hpp"

#include "Futurewalker.Event.EventType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ApplicationTheme : NonCopyable
{
public:
    explicit ApplicationTheme(Shared<PlatformApplicationThemeContext> const& platformContext, Shared<ApplicationContext> const& applicationContext);

    auto GetBrightness() -> ApplicationThemeBrightness;
    auto SetBrightness(ApplicationThemeBrightness const brightness) -> void;

    auto PushTheme(ThemeBrightness const brightness, Shared<Theme> const& theme) -> void;

private:
    auto ApplyTheme(Shared<Theme> const& theme) -> void;
    auto ApplyThemes() -> void;
    auto UpdateCurrentBrightness() -> void;
    auto HandlePlatformThemeEvent(Event<>& event) -> Async<Bool>;

private:
    Shared<PlatformApplicationThemeContext> _platformContext;
    Shared<PlatformApplicationTheme> _platformObject;
    Shared<ApplicationContext> _applicationContext;
    ThemeBrightness _currentBrightness = ThemeBrightness::Light;
    ApplicationThemeBrightness _brightness = ApplicationThemeBrightness::kSystem;
    std::vector<Shared<Theme>> _darkThemes;
    std::vector<Shared<Theme>> _lightThemes;
};

template <>
struct Locator::Resolver<ApplicationTheme>
{
    using Interface = ApplicationTheme;
    static auto Resolve() -> Shared<ApplicationTheme>;
};
}
}
