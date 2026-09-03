// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.View.hpp>

namespace Futurewalker::Gallery
{
class WindowingPage : public View
{
public:
    static auto Make() -> Shared<WindowingPage>;

    WindowingPage(PassKey<View> key);

protected:
    auto Initialize() -> void override;
};
}
