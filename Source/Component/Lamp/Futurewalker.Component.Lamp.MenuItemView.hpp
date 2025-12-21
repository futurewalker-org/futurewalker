// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.MenuItemViewType.hpp"
#include "Futurewalker.Component.Lamp.MenuViewStyle.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.PaddingView.hpp"
#include "Futurewalker.Application.EdgeInsets.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Layout view for menu item.  
///
class MenuItemView : public View
{
public:
    static auto Make() -> Shared<MenuItemView>;

    MenuItemView(PassKey<View> key);

    auto GetLeadingView() const -> Shared<View>;
    auto SetLeadingView(Shared<View> const& leadingView) -> void;

    auto GetMiddleView() const -> Shared<View>;
    auto SetMiddleView(Shared<View> const& middleView) -> void;

    auto GetTrailingView() const -> Shared<View>;
    auto SetTrailingView(Shared<View> const& trailingView) -> void;

protected:
    auto Initialize() -> void override;

private:
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;
    auto UpdatePadding() -> void;

private:
    Shared<ContainerView> _leadingContainer;
    Shared<ContainerView> _middleContainer;
    Shared<ContainerView> _trailingContainer;
    Shared<PaddingView> _paddingView;
    AttributeAccessor<EdgeInsets> _itemPadding;
};
}
}
