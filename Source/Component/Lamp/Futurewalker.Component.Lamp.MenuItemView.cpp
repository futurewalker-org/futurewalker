// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.MenuItemView.hpp"

#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.PaddingView.hpp"
#include "Futurewalker.Application.FlexLayout.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto MenuItemView::Make() -> Shared<MenuItemView>
{
    return View::MakeDerived<MenuItemView>();
}

MenuItemView::MenuItemView(PassKey<View> key)
  : View(key)
{
}

auto MenuItemView::GetLeadingView() const -> Shared<View>
{
    return _leadingContainer->GetContent();
}

auto MenuItemView::SetLeadingView(Shared<View> const& leadingView) -> void
{
    _leadingContainer->SetContent(leadingView);
}

auto MenuItemView::GetMiddleView() const -> Shared<View>
{
    return _middleContainer->GetContent();
}

auto MenuItemView::SetMiddleView(Shared<View> const& middleView) -> void
{
    _middleContainer->SetContent(middleView);
}

auto MenuItemView::GetTrailingView() const -> Shared<View>
{
    return _trailingContainer->GetContent();
}

auto MenuItemView::SetTrailingView(Shared<View> const& trailingView) -> void
{
    _trailingContainer->SetContent(trailingView);
}

auto MenuItemView::Initialize() -> void
{
    _leadingContainer = ContainerView::Make();
    _middleContainer = ContainerView::Make();
    _trailingContainer = ContainerView::Make();

    auto flex = FlexLayout::Make();
    flex->SetDirection(FlexLayoutDirection::Row);
    flex->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    flex->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
    flex->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    flex->AddChild(_leadingContainer);
    flex->AddChild(_middleContainer);
    flex->AddChild(_trailingContainer);
    FlexLayout::SetChildGrowFactor(_middleContainer, 1);
    FlexLayout::SetChildShrinkFactor(_middleContainer, 1);
    FlexLayout::SetChildMainAxisFlexibility(_middleContainer, FlexLayoutMainAxisFlexibility::Expand);

    _paddingView = PaddingView::MakeWithPaddingAndContent(EdgeInsets(), flex);
    AddChildBack(_paddingView);

    _itemPadding.BindAndConnectAttribute(*this, &MenuItemView::ReceiveAttributeEvent, MenuViewStyle::ItemPadding);

    UpdatePadding();
}

auto MenuItemView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        UpdatePadding();
    }
    co_return false;
}

auto MenuItemView::UpdatePadding() -> void
{
    auto const padding = _itemPadding.GetValueOrDefault();

    _paddingView->SetPadding(padding.GetVertical());

    if (GetLeadingView())
    {
        FlexLayout::SetChildMargin(_leadingContainer, EdgeInsets(padding.GetLeading(), 0, 0, 0));
    }

    FlexLayout::SetChildMargin(_middleContainer, padding.GetHorizontal());

    if (GetTrailingView())
    {
        FlexLayout::SetChildMargin(_trailingContainer, EdgeInsets(0, 0, padding.GetTrailing(), 0));
    }
}
}
