// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.FontStyle.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
FontStyle::FontStyle(FontWeight weight, FontWidth width, FontSlant slant)
  : _weight {weight}
  , _width {width}
  , _slant {slant}
{
}

auto FontStyle::GetSlant() const -> FontSlant
{
    return _slant;
}

auto FontStyle::SetSlant(FontSlant const slant) -> void
{
    _slant = slant;
}

auto FontStyle::GetWeight() const -> FontWeight
{
    return _weight;
}

auto FontStyle::SetWeight(FontWeight const weight) -> void
{
    _weight = weight;
}

auto FontStyle::GetWidth() const -> FontWidth
{
    return _width;
}

auto FontStyle::SetWidth(FontWidth const width) -> void
{
    _width = width;
}
}
