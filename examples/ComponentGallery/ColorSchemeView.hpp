#pragma once

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.ThemeType.hpp>

namespace Futurewalker::ComponentGallery
{
class ColorSchemeView : public View
{
public:
    static auto Make() -> Shared<ColorSchemeView>;

    ColorSchemeView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
};
}
 