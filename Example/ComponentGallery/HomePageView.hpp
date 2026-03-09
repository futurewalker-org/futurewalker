#pragma once

#include <Futurewalker.Application.View.hpp>

namespace Futurewalker::ComponentGallery
{
class HomePageView : public View
{
public:
    static auto Make() -> Shared<HomePageView>;

    HomePageView(PassKey<View> key);

private:
    auto Initialize() -> void override;
};
}
