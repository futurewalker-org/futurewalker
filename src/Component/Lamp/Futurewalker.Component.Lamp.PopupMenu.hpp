// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.PopupMenuType.hpp"
#include "Futurewalker.Component.Lamp.PopupMenuEvent.hpp"
#include "Futurewalker.Component.Lamp.MenuViewType.hpp"

#include "Futurewalker.Application.PopupType.hpp"
#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.Menu.hpp"
#include "Futurewalker.Application.BoxConstraints.hpp"

#include "Futurewalker.Event.EventReceiverType.hpp"
#include "Futurewalker.Event.EventType.hpp"

#include "Futurewalker.Signal.TrackerType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Async.Async.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Popup menu.
///
/// Displays a menu in a popup window anchored to a rectangle in a source view.
///
class PopupMenu : NonCopyable
{
public:
    static auto Make() -> Shared<PopupMenu>;

    PopupMenu(PassKey<PopupMenu>);

    auto GetMenu() const -> Menu;
    auto SetMenu(Menu const& menu) -> void;

    auto GetConstraints() const -> BoxConstraints;
    auto SetConstraints(BoxConstraints const& constraints) -> void;

    auto SetAnchorRect(Rect<Dp> const& rect) -> void;
    auto SetAnchorEdge(PopupMenuAnchorEdge const edge) -> void;
    auto SetAnchorAlignment(PopupMenuAnchorAlignment const alignment) -> void;

    auto Show(Shared<View> const& sourceView) -> void;
    auto Close() -> Async<void>;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

protected:
    auto Initialize() -> void;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;

private:
    auto CreatePopup(Shared<View> const& sourceView) -> void;
    auto DestroyPopup() -> void;
    auto UpdatePopup() -> void;
    auto CalcPopupPosition(Rect<Vp> const& sourceRect, Size<Vp> const& popupSize, Rect<Vp> const& screenRect, Bool const rtl) const -> Point<Vp>;

private:
    Menu _menu;
    PopupMenuAnchorEdge _anchorEdge = PopupMenuAnchorEdge::Bottom;
    PopupMenuAnchorAlignment _alignment = PopupMenuAnchorAlignment::Start;
    BoxConstraints _constraints = BoxConstraints::MakeUnconstrained();
    Rect<Dp> _anchorRect;
    Shared<EventReceiver> _eventReceiver;
    Shared<Popup> _popup;
    Shared<MenuView> _menuView;
    Weak<View> _sourceView;
};
}
}
