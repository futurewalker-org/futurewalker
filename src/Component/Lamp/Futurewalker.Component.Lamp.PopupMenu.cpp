// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.PopupMenu.hpp"
#include "Futurewalker.Component.Lamp.MenuView.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"
#include "Futurewalker.Component.Lamp.Color.hpp"
#include "Futurewalker.Component.Lamp.PopupFrameView.hpp"

#include "Futurewalker.Application.Popup.hpp"
#include "Futurewalker.Application.Screen.hpp"
#include "Futurewalker.Application.ViewLayoutFunction.hpp"

namespace FW_LAMP_DETAIL_NS
{
///
/// @brief Create popup menu.
///
auto PopupMenu::Make() -> Shared<PopupMenu>
{
    auto popup = Shared<PopupMenu>::Make(PassKey<PopupMenu>());
    popup->Initialize();
    return popup;
}

///
/// @brief Constructor.
///
PopupMenu::PopupMenu(PassKey<PopupMenu>)
{
}

///
/// @brief Get menu to be displayed in popup.
///
auto PopupMenu::GetMenu() const -> Menu
{
    return _menu;
}

///
/// @brief Set menu to be displayed in popup.
///
/// @param menu Menu.
///
/// @note The change will be reflected when Show() is called next time.
///
auto PopupMenu::SetMenu(Menu const& menu) -> void
{
    _menu = menu;
}

///
/// @brief Get box constraints for popup menu.
///
auto PopupMenu::GetConstraints() const -> BoxConstraints
{
    return _constraints;
}

///
/// @brief Set size constraints for popup menu.
///
/// @param[in] constraints Size constraints.
///
/// @note The change will be reflected when Show() is called next time.
///
auto PopupMenu::SetConstraints(BoxConstraints const& constraints) -> void
{
    _constraints = constraints;
}

///
/// @brief Set anchor rectangle in the source view's coordinate space.
///
/// @param rect Anchor rectangle.
///
/// @note The change will be reflected when Show() is called next time.
///
auto PopupMenu::SetAnchorRect(Rect<Dp> const& rect) -> void
{
    _anchorRect = rect;
}

///
/// @brief Set anchor edge.  
///
/// @param edge Anchor edge.
///
/// @note The change will be reflected when Show() is called next time.
///
auto PopupMenu::SetAnchorEdge(PopupAnchorEdge const edge) -> void
{
    _anchorEdge = edge;
}

///
/// @brief Set anchor alignment.
///
/// @param alignment Anchor alignment.
///
/// @note The change will be reflected when Show() is called next time.
///
auto PopupMenu::SetAnchorAlignment(PopupAnchorAlignment const alignment) -> void
{
    _alignment = alignment;
}

///
/// @brief Show popup menu.
///
/// @param sourceView Source view of the popup menu.
///
auto PopupMenu::Show(Shared<View> const& sourceView) -> void
{
    CreatePopup(sourceView);
    UpdatePopup();
}

///
/// @brief Close popup menu.
///
auto PopupMenu::Close() -> void
{
    if (_popup && !_popup->IsClosed())
    {
        _popup->Close();
    }
}

///
/// @brief 
///
auto PopupMenu::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

///
/// @brief 
///
auto PopupMenu::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}

///
/// @brief 
///
auto PopupMenu::GetTracker() -> Weak<void>
{
    return _eventReceiver;
}

///
/// @brief 
///
auto PopupMenu::GetTracker() const -> Weak<void const>
{
    return _eventReceiver;
}

///
/// @brief 
///
auto PopupMenu::Initialize() -> void
{
    _eventReceiver = EventReceiver::Make();
    _menuView = MenuView::Make();
    EventReceiver::Connect(*_menuView, *this, &PopupMenu::ReceiveEvent);
}

///
/// @brief 
///
auto PopupMenu::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<PopupEvent::Closed>())
    {
        DestroyPopup();
        auto closeEvent = Event<>(Event<PopupMenuEvent::Closed>());
        GetEventReceiver().SendEvent(closeEvent);
        return true;
    }
    else if (event.Is<MenuViewEvent::Activated>())
    {
        DestroyPopup();
        auto const commandId = event.As<MenuViewEvent::Activated>()->GetCommandId();
        auto activatedEventParameter = Event<PopupMenuEvent::Activated>();
        activatedEventParameter->SetCommandId(commandId);
        auto activatedEvent = Event<>(std::move(activatedEventParameter));
        return GetEventReceiver().SendEvent(activatedEvent);
    }
    return false;
}

///
/// @brief Creates new popup instance if not created yet.
///
/// @param[in] sourceView Source view of the popup menu.
///
auto PopupMenu::CreatePopup(Shared<View> const& sourceView) -> void
{
    if (!_popup)
    {
        _sourceView = sourceView;
        _popup = Popup::Make({.allowActiveOwnerPopup = true}, sourceView);
        auto frame = PopupFrameView::MakeWithContent(_menuView);
        frame->SetBlurRadius(Style::Size60);
        frame->SetCornerRadius(Style::CornerRadiusSmall);
        frame->SetShadowColor(Color::Neutral0);
        frame->SetShadowAlpha(0.5);
        _popup->SetContent(frame);
        _popup->SetBackgroundColor(RGBAColor());
        EventReceiver::Connect(*_popup, *this, &PopupMenu::ReceiveEvent);
    }
}

///
/// @brief Destroys popup instance.
///
auto PopupMenu::DestroyPopup() -> void
{
    _popup.Reset();
    _sourceView.Reset();
}

///
/// @brief Update popup's position, size, and menu content.
///
auto PopupMenu::UpdatePopup() -> void
{
    if (!_popup)
    {
        return;
    }

    if (auto const sourceView = _sourceView.Lock())
    {
        auto const sourceRect = sourceView->LocalToGlobalRect(_anchorRect);

        if (auto const sourceScreen = Screen::GetScreenFromRect(sourceRect))
        {
            if (auto const screenInfo = sourceScreen->GetInfo())
            {
                if (_menuView)
                {
                    _menuView->SetMenu(_menu);
                }
                auto const blurRadius = UnitFunction::ConvertDpToVp(AttributeNode::GetValue<Style::Size60>(*_menuView).GetValueOr(0), screenInfo->displayScale);
                auto const popupSizeWithShadow = UnitFunction::ConvertDpToVp(_popup->Measure(BoxConstraints::MakeUnconstrained()), screenInfo->displayScale);
                auto const popupSize = Size<Vp>(popupSizeWithShadow.width - blurRadius * 2, popupSizeWithShadow.height - blurRadius * 2);
                auto const screenRect = screenInfo->workArea;
                auto const isRtl = sourceView->GetLayoutDirection() == LayoutDirection::RightToLeft;
                auto const popupPos = ViewLayoutFunction::CalcPopupPosition(sourceRect, popupSize, screenRect, _anchorEdge, _alignment, blurRadius, isRtl);
                auto const popupRect = Rect<Vp>::Offset(Rect<Vp>::Make(popupPos, popupSizeWithShadow), Vector2<Vp>(-blurRadius, -blurRadius));
                _popup->SetFrameRect(popupRect);
                _popup->SetVisible(true);
            }
        }
    }
}
}
