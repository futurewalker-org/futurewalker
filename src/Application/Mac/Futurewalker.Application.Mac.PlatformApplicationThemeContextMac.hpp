// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformApplicationThemeContextMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformApplicationThemeMacType.hpp"
#include "Futurewalker.Application.PlatformApplicationThemeContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformApplicationThemeContextMac : public PlatformApplicationThemeContext
{
public:
    static auto Make() -> Shared<PlatformApplicationThemeContextMac>;

    PlatformApplicationThemeContextMac(PassKey<PlatformApplicationThemeContext>);

    auto MakeApplicationTheme(PlatformApplicationTheme::Delegate const& delegate) -> Shared<PlatformApplicationTheme> override;

    auto GetSystemBrightness() const -> ThemeBrightness;
    auto GetCurrentSystemBrightness() const -> ThemeBrightness;
    auto UpdateSystemBrightness() -> void;

protected:
    auto Initialize() -> void override;

private:
    std::vector<Weak<PlatformApplicationThemeMac>> _applicationThemes;
    ThemeBrightness _systemBrightness = ThemeBrightness::Light;
    __strong NSObject* _observer;
};

template <>
struct Locator::Resolver<PlatformApplicationThemeContextMac>
{
    using Interface = PlatformApplicationThemeContext;
    static auto Resolve() -> Shared<PlatformApplicationThemeContextMac>;
};
}
}
