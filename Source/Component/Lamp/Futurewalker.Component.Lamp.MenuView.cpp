// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.MenuView.hpp"
#include "Futurewalker.Component.Lamp.MenuItemButton.hpp"
#include "Futurewalker.Component.Lamp.MenuItemViewBuilder.hpp"
#include "Futurewalker.Component.Lamp.TextView.hpp"

#include "Futurewalker.Application.MenuItem.hpp"
#include "Futurewalker.Application.FlexLayout.hpp"
#include "Futurewalker.Application.SizedView.hpp"
#include "Futurewalker.Application.PaddingView.hpp"
#include "Futurewalker.Application.Popup.hpp"

#include "Futurewalker.Action.CommandObserver.hpp"
#include "Futurewalker.Action.CommandNode.hpp"

#include "Futurewalker.Async.ThisThread.hpp"
#include "Futurewalker.Async.AsyncFunction.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto MenuView::Make() -> Shared<MenuView>
{
    return View::MakeDerived<MenuView>();
}

auto MenuView::MakeWithMenu(Menu const& menu) -> Shared<MenuView>
{
    auto view = Make();
    view->SetMenu(menu);
    return view;
}

MenuView::MenuView(PassKey<View> key)
  : View(key)
  , _builder(MenuItemViewBuilder::MakeForMenuView())
{
}

auto MenuView::GetMenu() const -> Menu
{
    auto menu = Menu();
    for (auto const& data : _data)
    {
        menu.AddItem(data.item);
    }
    return menu;
}

auto MenuView::SetMenu(Menu const& menu) -> void
{
    DestroyPopup();
    RebuildData(menu);
    RebuildView();
}

auto MenuView::GetItemViewBuilder() const -> MenuItemViewBuilder
{
    return _builder;
}

auto MenuView::SetItemViewBuilder(MenuItemViewBuilder const& builder) -> void
{
    _builder = builder;
    DestroyPopup();
    RebuildView();
}

auto MenuView::Initialize() -> void
{
    _padding = PaddingView::MakeWithPadding(MenuViewStyle::Padding);
    _container = ContainerView::MakeWithContent(_padding);
    AddChildBack(_container);

    UpdateStyle();

    EventReceiver::Connect(*this, *this, &MenuView::ReceiveEvent);
}

auto MenuView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ViewEvent::EnabledChanged>())
    {
        UpdateStyle();
    }
    else if (event.Is<MenuItemButtonEvent>())
    {
        if (auto const sender = event.As<MenuItemButtonEvent>()->GetSender())
        {
            if (sender && sender->IsChildOf(_flex))
            {
                auto index = SInt64(0);
                for (auto const& data : _data)
                {
                    if (data.view == sender)
                    {
                        if (event.Is<MenuItemButtonEvent::Down>())
                        {
                            CreatePopup();
                            UpdatePopup(index);
                        }
                        else if (event.Is<MenuItemButtonEvent::Enter>())
                        {
                            UpdatePopup(index);
                        }
                        else if (event.Is<MenuItemButtonEvent::Leave>())
                        {
                            _createPopupTask.Cancel();
                        }
                        else if (event.Is<MenuItemButtonEvent::Up>())
                        {
                            if (data.item.GetType() == MenuItemType::Item)
                            {
                                CommandNode::Execute(*this, data.item.GetCommandId());
                                auto notifyEvent = Event<>(Event<MenuViewEvent::Activated>());
                                SendEventDetached(notifyEvent);
                            }
                        }
                        break;
                    }
                    ++index;
                }
            }
        }
        co_return true;
    }
    co_return false;
}

auto MenuView::ReceiveCommandEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<CommandEvent::StateChanged>())
    {
        auto const commandId = event.As<CommandEvent::StateChanged>()->GetCommandId();
        for (auto const& data : _data)
        {
            if (data.item.GetCommandId() == commandId)
            {
                UpdateEnabled(data);
            }
        }
    }
    co_return false;
}

auto MenuView::ReceivePopupEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PopupMenuEvent::Closed>())
    {
        DestroyPopup();
    }
    else if (event.Is<PopupMenuEvent::Activated>())
    {
        DestroyPopup();
        auto activatedEvent = Event<>(Event<MenuViewEvent::Activated>());
        co_await SendEvent(activatedEvent);
    }
    co_return false;
}

auto MenuView::CreatePopup() -> void
{
    if (!_popup)
    {
        _popup = PopupMenu::Make();
        EventReceiver::Connect(*_popup, *this, &MenuView::ReceivePopupEvent);
    }
}

auto MenuView::DestroyPopup() -> void
{
    _popup = nullptr;
    _index = -1;
}

auto MenuView::UpdatePopup(SInt64 const index) -> void
{
    FW_DEBUG_ASSERT(0 <= index && index <= std::ssize(_data));

    if (_popup)
    {
        if (_index != index)
        {
            _index = index;

            auto const& data = _data.at(static_cast<std::size_t>(index));
            auto const& item = data.item;
            auto const& view = data.view;
            if (item.GetType() == MenuItemType::SubMenu)
            {
                if (view)
                {
                    auto const buttonRect = view->LocalToAncestorRect(view->GetContentRect(), GetSelf());
                    _popup->SetMenu(item.GetSubMenu());
                    _popup->SetAnchorRect(buttonRect);
                    _popup->SetAnchorEdge(PopupMenuAnchorEdge::Trailing);
                    _popup->SetAnchorAlignment(PopupMenuAnchorAlignment::Start);
                    _popup->Show(GetSelf());
                }
            }
            else
            {
                _createPopupTask = AsyncFunction::SpawnFn([weakSelf = Weak(GetSelf())]() -> Async<void> {
                    co_await ThisThread::ScheduleAfter(std::chrono::milliseconds(400));
                    if (auto self = weakSelf.Lock())
                    {
                        self->DestroyPopup();
                    }
                });
            }
        }
    }
    else
    {
        _createPopupTask = AsyncFunction::SpawnFn([index, weakSelf = Weak(GetSelf())]() -> Async<void> {
            co_await ThisThread::ScheduleAfter(std::chrono::milliseconds(400));
            if (auto self = weakSelf.Lock())
            {
                self->CreatePopup();
                self->UpdatePopup(index);
            }
        });
    }
}

auto MenuView::RebuildData(Menu const& menu) -> void
{
    _data.clear();
    for (auto const& item : menu.GetItems())
    {
        auto data = ItemData {
            .item = item,
        };

        if (item.GetType() == MenuItemType::Item)
        {
            if (item.GetCommandId() != 0U)
            {
                data.command = CommandNode::GetObserver(*this, item.GetCommandId());
                EventReceiver::Connect(*data.command, *this, &MenuView::ReceiveCommandEvent);
            }
        }
        _data.push_back(std::move(data));
    }
}

auto MenuView::RebuildView() -> void
{
    if (_flex)
    {
        _flex->RemoveFromParent();
        _flex = nullptr;
    }

    _flex = FlexLayout::Make();
    _flex->SetDirection(FlexLayoutDirection::Column);
    _flex->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
    _flex->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    _flex->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
    _flex->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);

    auto hasLeadingIcon = false;
    auto hasTrailingIcon = false;
    for (auto const& data : _data)
    {
        auto const& item = data.item;

        if (!item.GetLeadingIcon().IsEmpty())
        {
            hasLeadingIcon = true;
        }

        if (!item.GetTrailingIcon().IsEmpty())
        {
            hasTrailingIcon = true;
        }

        if (!item.GetSubMenu().GetItems().empty())
        {
            hasTrailingIcon = true;
        }

        if (!item.GetAccessKey().IsEmpty())
        {
            hasTrailingIcon = true;
        }
    }

    for (auto& data : _data)
    {
        auto& item = data.item;

        if (item.GetLeadingIcon().IsEmpty() && hasLeadingIcon)
        {
            item.SetLeadingIcon(Icon::MakeBlank());
        }

        if (item.GetTrailingIcon().IsEmpty() && hasTrailingIcon)
        {
            item.SetTrailingIcon(Icon::MakeBlank());
        }
    }

    auto i = 0uz;
    for (auto& data : _data)
    {
        auto const itemType = data.item.GetType();
        auto const itemView = _builder.Build(data.item);
        if (itemType == MenuItemType::Item || itemType == MenuItemType::SubMenu)
        {
            auto button = MenuItemButton::Make();
            button->SetContent(itemView);
            _flex->AddChild(button);
            data.view = button;
        }
        else
        {
            _flex->AddChild(itemView);
            data.view = itemView;
        }

        UpdateEnabled(data);

        if (i < _data.size() - 1)
        {
            auto const spacer = SizedView::Make();
            spacer->SetWidth(Dp::Infinity());
            spacer->SetHeight(MenuViewStyle::ItemSpacing);
            _flex->AddChild(spacer);
        }
        ++i;
    }
    _padding->SetContent(_flex);
}

auto MenuView::UpdateStyle() -> void
{
    _container->SetCornerRadius(MenuViewStyle::CornerRadius);

    if (IsEnabledFromRoot())
    {
        _container->SetBackgroundColor(MenuViewStyle::BackgroundColor);
    }
    else
    {
        _container->SetBackgroundColor(MenuViewStyle::DisabledBackgroundColor);
    }
}

auto MenuView::UpdateEnabled(ItemData const& data) -> void
{
    auto const& item = data.item;
    auto const& view = data.view;

    if (view)
    {
        if (item.GetType() == MenuItemType::Item)
        {
            if (data.command)
            {
                view->SetEnabled(data.command->IsCommandEnabled());
            }
            else
            {
                view->SetEnabled(false);
            }
        }
        else if (item.GetType() == MenuItemType::SubMenu)
        {
            view->SetEnabled(!item.GetSubMenu().IsEmpty());
        }
    }
}
}
