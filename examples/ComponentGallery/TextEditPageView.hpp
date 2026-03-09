#pragma once

#include <Futurewalker.Application.View.hpp>

namespace Futurewalker::ComponentGallery
{
class TextEditPageView : public View
{
public:
    static auto Make() -> Shared<TextEditPageView>;

    TextEditPageView(PassKey<View> key);

private:
    auto Initialize() -> void override;
};
}
