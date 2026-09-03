// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.ThemeType.hpp>

namespace Futurewalker::Gallery
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
 