// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformApplicationThemeMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformApplicationThemeContextMacType.hpp"
#include "Futurewalker.Application.PlatformApplicationTheme.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformApplicationThemeMac : public PlatformApplicationTheme
{
public:
    static auto Make(PlatformApplicationTheme::Delegate const& delegate, Shared<PlatformApplicationThemeContextMac> const& context) -> Shared<PlatformApplicationThemeMac>;

    PlatformApplicationThemeMac(PassKey<PlatformApplicationTheme> key, Delegate const& delegate, Shared<PlatformApplicationThemeContextMac> const& context);

    auto GetSystemBrightness() const -> ThemeBrightness override;
    auto NotifyBrightnessChanged() -> void;
};
}
}
