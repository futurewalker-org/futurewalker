// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ThemeViewType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.ThemeType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class ThemeView : public View
{
public:
    static auto Make() -> Shared<ThemeView>;
    static auto Make(Shared<Theme> const& theme) -> Shared<ThemeView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<ThemeView>;
    static auto MakeWithContent(Shared<Theme> const& theme, Shared<View> const& content) -> Shared<ThemeView>;

    ThemeView(PassKey<View> key);

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> const& content) -> void;

    auto GetTheme() const -> Shared<Theme>;
    auto SetTheme(Shared<Theme> const& theme) -> void;

private:
    auto UpdateTheme() -> void;

private:
    Shared<Theme> _theme;
};
}
}
