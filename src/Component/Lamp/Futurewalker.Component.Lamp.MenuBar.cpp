// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.MenuBar.hpp"
#include "Futurewalker.Component.Lamp.PopupMenu.hpp"
#include "Futurewalker.Component.Lamp.TextView.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"
#include "Futurewalker.Component.Lamp.PopupMenu.hpp"
#include "Futurewalker.Component.Lamp.MenuView.hpp"
#include "Futurewalker.Component.Lamp.MenuItemView.hpp"
#include "Futurewalker.Component.Lamp.MenuItemButton.hpp"

#include "Futurewalker.Application.FlexLayout.hpp" 
#include "Futurewalker.Application.PaddingView.hpp"
#include "Futurewalker.Application.MenuItem.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto MenuBar::Make() -> Shared<MenuBar>
{
    return View::MakeDerived<MenuBar>();
}

auto MenuBar::MakeWithMenu(Menu const& menu) -> Shared<MenuBar>
{
    auto view = Make();
    view->SetMenu(menu);
    return view;
}

MenuBar::MenuBar(PassKey<View> key)
  : View(key)
{
}

auto MenuBar::GetMenu() const -> Menu
{
    return _menu;
}

auto MenuBar::SetMenu(Menu const& menu) -> void
{
    _menu = menu;
    BuildView();
}

auto MenuBar::Initialize() -> void
{
    EventReceiver::Connect(*this, *this, &MenuBar::ReceiveViewEvent);
}

auto MenuBar::ReceiveViewEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<MenuItemButtonEvent>())
    {
        auto const sender = event.As<MenuItemButtonEvent>()->GetSender();
        if (event.Is<MenuItemButtonEvent::Down>())
        {
            if (auto const info = FindMenuItemInfo(sender))
            {
                CreatePopup();
                UpdatePopup(*info);
                co_return true;
            }
        }
        else if (event.Is<MenuItemButtonEvent::Enter>())
        {
            if (auto const info = FindMenuItemInfo(sender))
            {
                UpdatePopup(*info);
                co_return true;
            }
        }
    }
    co_return false;
}

auto MenuBar::ReceivePopupMenuEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PopupMenuEvent::Closed>())
    {
        DestroyPopup();
    }
    else if (event.Is<PopupMenuEvent::Activated>())
    {
        DestroyPopup();
        CommandNode::Execute(*this, event.As<PopupMenuEvent::Activated>()->GetCommandId());
        co_return true;
    }
    co_return false;
}

auto MenuBar::FindMenuItemInfo(Shared<View> const& button) const -> Optional<MenuItemInfo>
{
    for (auto const& info : _itemInfo)
    {
        if (info.button == button)
        {
            return info;
        }
    }
    return {};
}

auto MenuBar::CreatePopup() -> void
{
    if (!_popup)
    {
        _popup = PopupMenu::Make();
        EventReceiver::Connect(*_popup, *this, &MenuBar::ReceivePopupMenuEvent);
    }
}

auto MenuBar::DestroyPopup() -> void
{
    _popup = nullptr;
}

auto MenuBar::UpdatePopup(MenuItemInfo const& info) -> void
{
    if (_popup && info.button)
    {
        auto const buttonRect = info.button->LocalToAncestorRect(info.button->GetContentRect(), GetSelf());
        _popup->SetMenu(info.item.GetSubMenu());
        _popup->SetAnchorRect(buttonRect);
        _popup->SetAnchorEdge(PopupMenuAnchorEdge::Bottom);
        _popup->SetAnchorAlignment(PopupMenuAnchorAlignment::Start);
        _popup->Show(GetSelf());
    }
}

auto MenuBar::BuildView() -> void
{
    if (_row)
    {
        _row->RemoveFromParent();
        _row = nullptr;
    }

    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(EdgeInsets, AttributeItemPadding, [](auto p1, auto p2) { return p1.GetHorizontal() + p2.GetVertical(); }, Style::PaddingSmall, Style::PaddingExtraSmall);

    auto row = FlexLayout::Make();
    row->SetDirection(FlexLayoutDirection::Row);
    row->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
    row->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Center);
    row->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);

    _itemInfo.clear();
    for (auto const& item : _menu.GetItems())
    {
        auto text = TextView::MakeWithText(item.GetTitle());
        auto itemView = MenuItemView::MakeWithContents(nullptr, text, nullptr);
        auto button = MenuItemButton::MakeWithContent(itemView);
        AttributeNode::SetReference<&MenuItemViewStyle::Padding>(*button, AttributeItemPadding);
        button->SetEnabled(item.GetSubMenu().GetItems().size() > 0);
        row->AddChild(button);
        _itemInfo.push_back(MenuItemInfo {item, button});
    }
    AddChildBack(row);
}
}
