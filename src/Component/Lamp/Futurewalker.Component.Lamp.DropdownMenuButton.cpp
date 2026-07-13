// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.DropdownMenuButton.hpp"
#include "Futurewalker.Component.Lamp.MenuButtonView.hpp"
#include "Futurewalker.Component.Lamp.MenuItemView.hpp"
#include "Futurewalker.Component.Lamp.MenuItemButton.hpp"
#include "Futurewalker.Component.Lamp.TextView.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"
#include "Futurewalker.Component.Lamp.Color.hpp"
#include "Futurewalker.Component.Lamp.PopupFrameView.hpp"
#include "Futurewalker.Component.Lamp.PopupMenuButtonStyle.hpp" // FIXME

#include "Futurewalker.Application.Popup.hpp"
#include "Futurewalker.Application.PaddingView.hpp"
#include "Futurewalker.Application.FlexLayout.hpp"
#include "Futurewalker.Application.BoxView.hpp"
#include "Futurewalker.Application.Screen.hpp"
#include "Futurewalker.Application.ViewLayoutFunction.hpp"

#include "Futurewalker.Resource.ResourceManager.hpp"
#include "Futurewalker.Resource.Resource.hpp"

#include "Resource/Futurewalker.Component.Lamp.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto DropdownMenuButton::Make() -> Shared<DropdownMenuButton>
{
    return View::MakeDerived<DropdownMenuButton>();
}

auto DropdownMenuButton::MakeWithItems(std::span<const ItemInfo> items) -> Shared<DropdownMenuButton>
{
    auto button = Make();
    button->SetItems(items);
    return button;
}

auto DropdownMenuButton::AddItem(ItemInfo const& item) -> void
{
    _items.push_back(item);
    UpdateItems();
}

auto DropdownMenuButton::AddItems(std::span<const ItemInfo> items) -> void
{
    _items.insert(_items.end(), items.begin(), items.end());
    UpdateItems();
}

auto DropdownMenuButton::InsertItem(SInt64 const index, ItemInfo const& item) -> void
{
    _items.insert(_items.begin() + static_cast<std::size_t>(index), item);
    UpdateItems();
}

auto DropdownMenuButton::RemoveItem(SInt64 const index) -> void
{
    _items.erase(_items.begin() + static_cast<std::size_t>(index));
    UpdateItems();
}

auto DropdownMenuButton::GetItemCount() const -> SInt64
{
    return static_cast<SInt64>(_items.size());
}

auto DropdownMenuButton::GetItem(SInt64 const index) const -> Optional<ItemInfo>
{
    if (index < 0 || index >= static_cast<SInt64>(_items.size()))
    {
        return {};
    }
    return _items[static_cast<std::size_t>(index)];
}

auto DropdownMenuButton::GetItems() const -> std::vector<ItemInfo>
{
    return _items;
}

auto DropdownMenuButton::SetItems(std::span<const ItemInfo> items) -> void
{
    _items.assign(items.begin(), items.end());
    UpdateItems();
}

auto DropdownMenuButton::GetCurrentIndex() const -> SInt64
{
    return _currentIndex;
}

auto DropdownMenuButton::SetCurrentIndex(SInt64 const index) -> void
{
    if (index < 0 || index >= static_cast<SInt64>(_items.size()))
    {
        FW_DEBUG_LOG_ERROR("DropdownMenuButton::SetCurrentIndex: Index out of bounds");
        return;
    }
    _currentIndex = index;
}

auto DropdownMenuButton::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(String, AttributeText, {});

    _placeholderText.BindAndConnectAttributeWithDefaultValue(*this, &DropdownMenuButton::ReceiveAttributeEvent, AttributeText, {});

    _itemColumn = FlexLayout::Make();

    _buttonText = Lamp::TextView::Make();
    auto buttonContentPadding = PaddingView::MakeWithContent(_buttonText);
    buttonContentPadding->SetPadding(PopupMenuButtonStyle::Padding);

    _button = MenuButtonView::MakeWithContent(buttonContentPadding);
    _button->SetBackgroundColor(PopupMenuButtonStyle::BackgroundColor);
    _button->SetBackgroundAlpha(PopupMenuButtonStyle::BackgroundAlpha);
    _button->SetDisabledBackgroundColor(PopupMenuButtonStyle::DisabledBackgroundColor);
    _button->SetDisabledBackgroundAlpha(PopupMenuButtonStyle::DisabledBackgroundAlpha);
    _button->SetBorderColor(PopupMenuButtonStyle::BorderColor);
    _button->SetBorderAlpha(PopupMenuButtonStyle::BorderAlpha);
    _button->SetBorderWidth(PopupMenuButtonStyle::BorderWidth);
    _button->SetDisabledBorderColor(PopupMenuButtonStyle::DisabledBorderColor);
    _button->SetDisabledBorderAlpha(PopupMenuButtonStyle::DisabledBorderAlpha);
    _button->SetDisabledBorderWidth(PopupMenuButtonStyle::DisabledBorderWidth);
    _button->SetFocusedBorderColor(PopupMenuButtonStyle::FocusedBorderColor);
    _button->SetFocusedBorderAlpha(PopupMenuButtonStyle::FocusedBorderAlpha);
    _button->SetFocusedBorderWidth(PopupMenuButtonStyle::FocusedBorderWidth);
    _button->SetHighlightColor(PopupMenuButtonStyle::HighlightColor);
    _button->SetHoverHighlightAlpha(PopupMenuButtonStyle::HoverHighlightAlpha);
    _button->SetPressHighlightAlpha(PopupMenuButtonStyle::PressHighlightAlpha);
    _button->SetCornerRadius(PopupMenuButtonStyle::CornerRadius);

    AddChildBack(_button);

    EventReceiver::Connect(*this, *this, &DropdownMenuButton::ReceiveEvent);
    EventReceiver::Connect(*_itemColumn, *this, &DropdownMenuButton::ReceiveEvent);
}

auto DropdownMenuButton::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<PopupEvent::Closed>())
    {
        DestroyPopup();
        return true;
    }
    else if (event.Is<MenuButtonViewEvent::Down>())
    {
        if (!_items.empty())
        {
            CreatePopup();
            UpdatePopup();
        }
        return true;
    }
    else if (event.Is<MenuItemButtonEvent::Up>())
    {
        DestroyPopup();

        auto sender = event.As<MenuItemButtonEvent>()->GetSender();
        for (auto i = SInt64(0); i < _itemColumn->GetAddedChildCount(); ++i)
        {
            if (_itemColumn->GetAddedChildAt(i) == sender)
            {
                if (_currentIndex != i)
                {
                    _currentIndex = i;
                    UpdateButtonText();
                    auto notifyEventParameter = Event<DropdownMenuButtonEvent::CurrentItemChanged>();
                    notifyEventParameter->SetCurrentIndex(_currentIndex);
                    auto notifyEvent = Event<>(std::move(notifyEventParameter));
                    SendEvent(notifyEvent);
                }
                break;
            }
        }
        return true;
    }
    return false;
}

auto DropdownMenuButton::ReceiveAttributeEvent(Event<>& event) -> Bool
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        UpdateButtonText();
        return true;
    }
    return false;
}

auto DropdownMenuButton::SetPlaceholderText(AttributeArg<String> const& text) -> void
{
    _placeholderText.SetAttributeArg(text);
    UpdateButtonText();
}

DropdownMenuButton::DropdownMenuButton(PassKey<View> key)
  : View(key)
{
}

auto DropdownMenuButton::CreatePopup() -> void
{
    if (!_popup)
    {
        _popup = Popup::Make({.allowActiveOwnerPopup = true}, GetSelf());
        auto padding = PaddingView::MakeWithContent(_itemColumn);
        padding->SetPadding(Style::PaddingExtraSmall);
        auto box = BoxView::MakeWithContent(padding);
        box->SetBackgroundColor(Style::ColorSurfaceContainerLow);
        auto frame = PopupFrameView::MakeWithContent(box);
        frame->SetBlurRadius(Style::Size60);
        frame->SetCornerRadius(Style::CornerRadiusSmall);
        frame->SetShadowColor(Color::Neutral0);
        frame->SetShadowAlpha(0.5);
        _popup->SetContent(frame);
        _popup->SetBackgroundColor(RGBAColor());
        EventReceiver::Connect(*_popup, *this, &DropdownMenuButton::ReceiveEvent);
    }
}

auto DropdownMenuButton::UpdatePopup() -> void
{
    if (!_popup)
    {
        return;
    }

    auto const sourceRect = LocalToGlobalRect(GetContentRect());

    if (auto const sourceScreen = Screen::GetScreenFromRect(sourceRect))
    {
        if (auto const screenInfo = sourceScreen->GetInfo())
        {
            auto test = AttributeNode::GetValue<Style::Size60>(*this).GetValueOr(0);
            (void)test;
            auto const blurRadius = UnitFunction::ConvertDpToVp(AttributeNode::GetValue<Style::Size60>(*_itemColumn).GetValueOr(0), screenInfo->displayScale);
            auto const popupSizeWithShadow = UnitFunction::ConvertDpToVp(_popup->Measure(BoxConstraints::MakeUnconstrained()), screenInfo->displayScale);
            auto const popupSize = Size<Vp>(popupSizeWithShadow.width - blurRadius * 2, popupSizeWithShadow.height - blurRadius * 2);
            auto const screenRect = screenInfo->workArea;
            auto const isRtl = GetLayoutDirection() == LayoutDirection::RightToLeft;
            auto const popupPos = ViewLayoutFunction::CalcPopupPosition(sourceRect, popupSize, screenRect, PopupAnchorEdge::Bottom, PopupAnchorAlignment::Start, isRtl);
            auto const popupRect = Rect<Vp>::Offset(Rect<Vp>::Make(popupPos, popupSizeWithShadow), Vector<Vp>(-blurRadius, -blurRadius));
            _popup->SetFrameRect(popupRect);
            _popup->SetVisible(true);
        }
    }
}

auto DropdownMenuButton::DestroyPopup() -> void
{
    if (_popup)
    {
        _popup->Close();
        _popup = nullptr;
    }
}

auto DropdownMenuButton::UpdateItems() -> void
{
    _itemColumn->RemoveChildren();

    for (auto const& item : _items)
    {
        auto menuItem = MenuItemView::Make();
        menuItem->SetMiddleView(Lamp::TextView::MakeWithText(item.name));
        auto menuItemButton = MenuItemButton::MakeWithContent(menuItem);
        _itemColumn->AddChild(menuItemButton);
    }

    if (_currentIndex < 0 || static_cast<SInt64>(_items.size()) <= _currentIndex)
    {
        _currentIndex = -1;
    }
    UpdateButtonText();
    UpdatePopup();
}

auto DropdownMenuButton::UpdateButtonText() -> void
{
    if (0 <= _currentIndex && _currentIndex < static_cast<SInt64>(_items.size()))
    {
        _buttonText->SetText(_items[static_cast<std::size_t>(_currentIndex)].name);
    }
    else
    {
        _buttonText->SetText(_placeholderText.GetValueOrDefault());
    }
}
}
