#pragma once

#include <Futurewalker.Application.View.hpp>

namespace Futurewalker::ComponentGallery
{
class ButtonPageView : public View
{
public:
    static auto Make() -> Shared<ButtonPageView>;

    ButtonPageView(PassKey<View> key);

private:
    auto Initialize() -> void override;
};
}
