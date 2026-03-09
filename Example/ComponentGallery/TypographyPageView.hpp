#pragma once

#include <Futurewalker.Application.View.hpp>

namespace Futurewalker::ComponentGallery
{
class TypographyPageView : public View
{
public:
    static auto Make() -> Shared<TypographyPageView>;

    TypographyPageView(PassKey<View> key);

private:
    auto Initialize() -> void override;
};
}
