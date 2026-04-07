// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.PopupMenu.hpp"
#include "Futurewalker.Component.Lamp.MenuView.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"
#include "Futurewalker.Component.Lamp.Color.hpp"
#include "Futurewalker.Component.Lamp.PopupFrameView.hpp"

#include "Futurewalker.Application.Popup.hpp"
#include "Futurewalker.Application.Screen.hpp"

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
auto PopupMenu::SetAnchorEdge(PopupMenuAnchorEdge const edge) -> void
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
auto PopupMenu::SetAnchorAlignment(PopupMenuAnchorAlignment const alignment) -> void
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
auto PopupMenu::Close() -> Async<void>
{
    if (_popup && !_popup->IsClosed())
    {
        co_await _popup->Close();
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
auto PopupMenu::GetTracker() -> Tracker&
{
    return GetEventReceiver().GetTracker();
}

///
/// @brief 
///
auto PopupMenu::GetTracker() const -> Tracker const&
{
    return GetEventReceiver().GetTracker();
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
auto PopupMenu::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PopupEvent::Closed>())
    {
        _popup.Reset();
        _sourceView.Reset();
        auto closeEvent = Event<>(Event<PopupMenuEvent::Closed>());
        co_await GetEventReceiver().SendEvent(closeEvent);
        co_return true;
    }
    else if (event.Is<MenuViewEvent::Activated>())
    {
        auto activatedEvent = Event<>(Event<PopupMenuEvent::Activated>());
        co_await GetEventReceiver().SendEvent(activatedEvent);
        co_return true;
    }
    co_return false;
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
        frame->SetShadowColor(Color::Neutral10);
        frame->SetShadowAlpha(0.3);
        _popup->SetContent(frame);
        _popup->SetBackgroundColor(RGBAColor());
        EventReceiver::Connect(*_popup, *this, &PopupMenu::ReceiveEvent);
    }
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
                auto const blurRadius = UnitFunction::ConvertDpToVp(AttributeNode::GetValue<&Style::Size60>(*_menuView).GetValueOr(0), screenInfo->displayScale);
                auto const popupSizeWithShadow = UnitFunction::ConvertDpToVp(_popup->Measure(BoxConstraints::MakeUnconstrained()), screenInfo->displayScale);
                auto const popupSize = Size<Vp>(popupSizeWithShadow.width - blurRadius * 2, popupSizeWithShadow.height - blurRadius * 2);
                auto const screenRect = screenInfo->workArea;
                auto const isRtl = sourceView->GetLayoutDirection() == LayoutDirection::RightToLeft;
                auto const popupPos = CalcPopupPosition(sourceRect, popupSize, screenRect, isRtl);
                auto const popupRect = Rect<Vp>::Offset(Rect<Vp>::Make(popupPos, popupSizeWithShadow), Vector<Vp>(-blurRadius, -blurRadius));
                _popup->SetFrameRect(popupRect);
                _popup->SetVisible(true);
            }
        }
    }
}

///
/// @brief Calculate popup position.
///
/// @param sourceRect Rectangle of the source view in global coordinate space.
/// @param popupSize Size of the popup menu.
/// @param screenRect Rectangle of the screen's work area.
/// @param rtl True if layout direction is right-to-left.
///
auto PopupMenu::CalcPopupPosition(Rect<Vp> const& sourceRect, Size<Vp> const& popupSize, Rect<Vp> const& screenRect, Bool const rtl) const -> Point<Vp>
{
    auto const anchorEdge = _anchorEdge;
    auto const alignment = _alignment;

    if (anchorEdge == PopupMenuAnchorEdge::Leading || anchorEdge == PopupMenuAnchorEdge::Trailing)
    {
        auto x = Vp(0);
        if ((!rtl && anchorEdge == PopupMenuAnchorEdge::Leading) || (rtl && anchorEdge == PopupMenuAnchorEdge::Trailing))
        {
            x = sourceRect.x0 - popupSize.width;
            x = Vp::Max(screenRect.x0, x);
        }
        else if ((!rtl && anchorEdge == PopupMenuAnchorEdge::Trailing) || (rtl && anchorEdge == PopupMenuAnchorEdge::Leading))
        {
            x = sourceRect.x1;
            x = Vp::Min(x, screenRect.x1 - popupSize.width);
        }

        if (rtl)
        {
            x = Vp::Min(x, screenRect.x1 - popupSize.width);
        }
        else
        {
            x = Vp::Max(x, screenRect.x0);
        }

        auto y = Vp(0);
        if (alignment == PopupMenuAnchorAlignment::Start)
        {
            y = sourceRect.y0;
        }
        else if (alignment == PopupMenuAnchorAlignment::Center)
        {
            y = (sourceRect.y0 + sourceRect.y1 - popupSize.height) / 2;
        }
        else if (alignment == PopupMenuAnchorAlignment::End)
        {
            y = sourceRect.y1 - popupSize.height;
            y = Vp::Min(y, screenRect.y1 - popupSize.height);
        }
        y = Vp::Max(y, screenRect.y0);

        return {x, y};
    }
    else if (anchorEdge == PopupMenuAnchorEdge::Top || anchorEdge == PopupMenuAnchorEdge::Bottom)
    {
        auto y = Vp(0);
        if (anchorEdge == PopupMenuAnchorEdge::Top)
        {
            y = sourceRect.y0 - popupSize.height;
        }
        else if (anchorEdge == PopupMenuAnchorEdge::Bottom)
        {
            y = sourceRect.y1;
            y = Vp::Min(y, screenRect.y1 - popupSize.height);
        }
        y = Vp::Max(y, screenRect.y0);

        auto x = Vp(0);
        if ((!rtl && alignment == PopupMenuAnchorAlignment::Start) || (rtl && alignment == PopupMenuAnchorAlignment::End))
        {
            x = sourceRect.x0;
            x = Vp::Min(x, screenRect.x1 - popupSize.width);
        }
        else if ((!rtl && alignment == PopupMenuAnchorAlignment::End) || (rtl && alignment == PopupMenuAnchorAlignment::Start))
        {
            x = sourceRect.x1 - popupSize.width;
            x = Vp::Max(x, screenRect.x0);
        }
        else if (alignment == PopupMenuAnchorAlignment::Center)
        {
            x = (sourceRect.x0 + sourceRect.x1 - popupSize.width) / 2;
        }

        if (rtl)
        {
            x = Vp::Min(x, screenRect.x1 - popupSize.width);
        }
        else
        {
            x = Vp::Max(x, screenRect.x0);
        }

        return {x, y};
    }
    else
    {
        FW_DEBUG_LOG_ERROR("Invalid PopupMenuAnchorEdge");
        FW_DEBUG_ASSERT(false);
    }
    return {};
}
}
