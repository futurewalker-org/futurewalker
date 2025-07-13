// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformApplicationThemeContextWinType.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationThemeWinType.hpp"
#include "Futurewalker.Application.PlatformApplicationThemeContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformApplicationThemeContextWin : public PlatformApplicationThemeContext
{
public:
    static auto Make() -> Shared<PlatformApplicationThemeContextWin>;

    PlatformApplicationThemeContextWin(PassKey<PlatformApplicationThemeContext> key);

    auto MakeApplicationTheme(PlatformApplicationTheme::Delegate const& delegate) -> Shared<PlatformApplicationTheme> override;

    auto GetSystemBrightness() const -> ThemeBrightness;

protected:
    void Initialize() override;

private:
    auto GetCurrentSystemBrightness() -> ThemeBrightness;
    auto UpdateSystemBrightness() -> void;

private:
    std::vector<Weak<PlatformApplicationThemeWin>> _applicationThemes;
    winrt::Windows::UI::ViewManagement::UISettings _uiSettings;
    winrt::Windows::UI::ViewManagement::UISettings::ColorValuesChanged_revoker _colorValuesChangedRevoker;
    ThemeBrightness _systemBrightness = ThemeBrightness::Light;
};

template <>
struct Locator::Resolver<PlatformApplicationThemeContextWin>
{
    using Interface = PlatformApplicationThemeContext;
    static auto Resolve() -> Shared<PlatformApplicationThemeContextWin>;
};
}
}
