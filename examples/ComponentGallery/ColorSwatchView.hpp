#pragma once

#include <Futurewalker.Application.View.hpp>

namespace Futurewalker::ComponentGallery
{
class ColorSwatchView : public View
{
public:
    static auto Make() -> Shared<ColorSwatchView>;

    ColorSwatchView(PassKey<View> key);

private:
    auto Initialize() -> void override;
};
}
