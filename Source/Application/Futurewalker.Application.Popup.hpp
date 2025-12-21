// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PopupType.hpp"
#include "Futurewalker.Application.PopupEvent.hpp"
#include "Futurewalker.Application.WindowType.hpp"
#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.BoxConstraintsType.hpp"

#include "Futurewalker.Attribute.AttributeNodeType.hpp"
#include "Futurewalker.Attribute.AttributeArg.hpp"

#include "Futurewalker.Event.EventReceiverType.hpp"
#include "Futurewalker.Event.EventType.hpp"

#include "Futurewalker.Color.RGBAColor.hpp"

#include "Futurewalker.Signal.TrackerType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Async.Async.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Displays content on popup window.
///
/// Popup can be used for displaying temporary content like context menus.
///
class Popup : NonCopyable
{
public:
    static auto Make(PopupOptions const& options, Shared<View> const& sourceView) -> Shared<Popup>;

    Popup(PassKey<Popup>, PopupOptions const& options, Shared<View> const& sourceView);

    auto IsVisible() const -> Bool;
    auto SetVisible(Bool const visible) -> void;

    auto IsClosed() const -> Bool;
    auto Close() -> Async<void>;

    auto GetContent() -> Shared<View>;
    auto GetContent() const -> Shared<View const>;
    auto SetContent(Shared<View> const& content) -> void;

    auto Measure(BoxConstraints const& constraints) -> Size<Dp>;

    auto SetFrameRect(Rect<Vp> const& rect) -> void;
    auto SetBackgroundColor(AttributeArg<RGBAColor> const& backgroundColor) -> void;

    auto GetAttributeNode() -> AttributeNode&;
    auto GetAttributeNode() const -> AttributeNode const&;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

private:
    auto Initialize() -> void;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto GetWindow() -> Shared<Window>;

private:
    PopupOptions _options;
    Shared<EventReceiver> _eventReceiver;
    Shared<Window> _window;
    Shared<View> _contentView;
    Weak<View> _sourceView;
};
}
}
