#pragma once

#include <Futurewalker.Application.View.hpp>

namespace Futurewalker::ComponentGallery
{
class ColorPageView : public View
{
public:
    static auto Make() -> Shared<ColorPageView>;

    ColorPageView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
};
}
