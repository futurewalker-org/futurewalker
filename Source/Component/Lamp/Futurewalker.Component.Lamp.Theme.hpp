// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.ThemeType.hpp"

#include "Futurewalker.Application.Theme.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class Theme : public ::FW_NS::Theme
{
public:
    explicit Theme(ThemeBrightness brightness);

    auto Apply(AttributeNode& target) -> void override;

private:
    auto ApplyApplicationColor(AttributeNode& target) -> void;
    auto ApplyApplicationStyle(AttributeNode& target) -> void;

private:
    ThemeBrightness _brightness = ThemeBrightness::Light;
};
}
}
