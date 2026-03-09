// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.MenuItemViewBuilderType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class MenuItemViewBuilder
{
public:
    static auto MakeForMenuView() -> MenuItemViewBuilder;
    static auto Make(MenuItemViewBuilderFunction const& function) -> MenuItemViewBuilder;

    auto Build(MenuItem const& item) const -> Shared<View>;

private:
    explicit MenuItemViewBuilder(MenuItemViewBuilderFunction const& function);

private:
    MenuItemViewBuilderFunction _function;
};
}
}
