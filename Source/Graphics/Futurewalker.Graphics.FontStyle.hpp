// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.FontStyleType.hpp"
#include "Futurewalker.Graphics.FontWeight.hpp"
#include "Futurewalker.Graphics.FontWidth.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class FontStyle
{
public:
    FontStyle(FontWeight weight, FontWidth width, FontSlant slant);

    auto GetSlant() const -> FontSlant;
    auto SetSlant(FontSlant const slant) -> void;

    auto GetWeight() const -> FontWeight;
    auto SetWeight(FontWeight const weight) -> void;

    auto GetWidth() const -> FontWidth;
    auto SetWidth(FontWidth const width) -> void;

private:
    FontWeight _weight = FontWeight::Normal();
    FontWidth _width = FontWidth::Normal();
    FontSlant _slant = FontSlant::Upright;
};
}
}
