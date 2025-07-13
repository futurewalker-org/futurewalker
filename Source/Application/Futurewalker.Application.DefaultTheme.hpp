// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.DefaultThemeType.hpp"

#include "Futurewalker.Application.Theme.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class DefaultTheme : public Theme
{
public:
    explicit DefaultTheme(ThemeBrightness brightness);

    auto Apply(AttributeNode& target) -> void override;

private:
    auto ApplyApplicationColor(AttributeNode& target) -> void;
    auto ApplyApplicationStyle(AttributeNode& target) -> void;

private:
    ThemeBrightness _brightness = ThemeBrightness::Light;
};
}
}
