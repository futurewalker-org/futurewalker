// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformApplicationThemeWinType.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationThemeContextWinType.hpp"
#include "Futurewalker.Application.PlatformApplicationTheme.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformApplicationThemeWin : public PlatformApplicationTheme
{
public:
    static auto Make(PlatformApplicationTheme::Delegate const& delegate, Shared<PlatformApplicationThemeContextWin> const& context) -> Shared<PlatformApplicationThemeWin>;

    PlatformApplicationThemeWin(PassKey<PlatformApplicationTheme> key, Delegate const& delegate, Shared<PlatformApplicationThemeContextWin> const& context);

    auto GetSystemBrightness() const -> ThemeBrightness override;

    auto NotifyBrightnessChanged() -> void;
};
}
}
