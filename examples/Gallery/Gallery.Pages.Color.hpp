// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.ThemeViewType.hpp>
#include <Futurewalker.Application.ThemeType.hpp>

#include <Futurewalker.Core.Optional.hpp>

namespace Futurewalker::Gallery
{
class ColorPage : public View
{
public:
    static auto Make() -> Shared<ColorPage>;

    ColorPage(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Bool;

private:
    Optional<ThemeBrightness> _currentBrightness;
    Shared<ThemeView> _colorPaletteThemeView;
    Shared<ThemeView> _colorSchemeThemeView;
};
}
