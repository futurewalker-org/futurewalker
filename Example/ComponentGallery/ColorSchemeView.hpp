#pragma once

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.ThemeType.hpp>

namespace Futurewalker::ComponentGallery
{
class ColorSchemeView : public View
{
public:
    static auto Make(ThemeBrightness brightness) -> Shared<ColorSchemeView>;

    ColorSchemeView(PassKey<View> key, ThemeBrightness brightness);

protected:
    auto Initialize() -> void override;
private:
    ThemeBrightness _brightness = ThemeBrightness::Light;
};
}
 