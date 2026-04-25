// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformWindowType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"
#include "Futurewalker.Application.PlatformInputMethodType.hpp"
#include "Futurewalker.Application.BoxConstraints.hpp"
#include "Futurewalker.Application.PointerEventType.hpp"
#include "Futurewalker.Application.EdgeInsets.hpp"

#include "Futurewalker.Event.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.MonotonicTime.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform window class.
///
class PlatformWindow : NonCopyable
{
public:
    struct Delegate
    {
        EventFunction sendPointerEvent;
        EventFunction sendKeyEvent;
        EventFunction sendInputEvent;
        EventFunction sendFrameEvent;
        EventFunction sendWindowEvent;
        EventFunction sendHitTestEvent;
    };
    PlatformWindow(PassKey<PlatformWindow>, Delegate const& delegate);

    auto SetDelegate(Delegate const& delegate) -> void;

    virtual ~PlatformWindow() = 0;

    virtual auto IsVisible() -> Bool = 0;
    virtual auto SetVisible(Bool const visible) -> void = 0;
    virtual auto IsActive() -> Bool = 0;
    virtual auto SetActive() -> void = 0;
    virtual auto IsFocused() -> Bool = 0;
    virtual auto SetFocus() -> void = 0;
    virtual auto GetFrameRect() -> Rect<Vp> = 0;
    virtual auto SetFrameRect(Rect<Vp> const& rect) -> void = 0;
    virtual auto CapturePointer(PointerId const id) -> void = 0; 
    virtual auto ReleasePointer(PointerId const id) -> void = 0; 
    virtual auto GetRestoredFrameRect() -> Rect<Vp> = 0;
    virtual auto SetRestoredFrameRect(Rect<Vp> const& rect) -> void = 0;
    virtual auto GetAreaBounds(WindowArea const area) -> std::vector<Rect<Dp>> = 0;
    virtual auto GetAreaInsets(WindowArea const are) -> EdgeInsets = 0;
    virtual auto GetDisplayScale() -> DisplayScale = 0;
    virtual auto GetBackingScale() -> BackingScale = 0;
    virtual auto GetSizeConstraints() -> BoxConstraints = 0;
    virtual auto SetSizeConstraints(BoxConstraints const& constraints) -> void = 0;
    virtual auto GetTitle() -> String = 0;
    virtual auto SetTitle(String const& title) -> void = 0;
    virtual auto Minimize() -> void = 0;
    virtual auto Maximize() -> void = 0;
    virtual auto Restore() -> void = 0;
    virtual auto IsClosed() -> Bool = 0;
    virtual auto RequestClose() -> Async<Bool> = 0;
    virtual auto Close() -> void = 0;
    virtual auto Render() -> void = 0;
    virtual auto RequestFrame() -> void = 0;
    virtual auto GetFrameTime() -> MonotonicTime = 0;
    virtual auto GetViewLayer() -> Shared<PlatformViewLayer> = 0;
    virtual auto GetInputMethod() -> Shared<PlatformInputMethod> = 0;
    virtual auto SetBackgroundColor(RGBColor const& color) -> void = 0;

protected:
    virtual auto Initialize() -> void = 0;

    auto SendWindowEvent(Event<>& event) -> Async<Bool>;
    auto SendFrameEvent(Event<>& event) -> Async<Bool>;
    auto SendPointerEvent(Event<>& event) -> Async<Bool>;
    auto SendKeyEvent(Event<>& event) -> Async<Bool>;
    auto SendInputEvent(Event<>& event) -> Async<Bool>;
    auto SendHitTestEvent(Event<>& event) -> Async<Bool>;

    auto SendWindowEventDetached(Event<>& event) -> Bool;
    auto SendFrameEventDetached(Event<>& event) -> Bool;
    auto SendPointerEventDetached(Event<>& event) -> Bool;
    auto SendKeyEventDetached(Event<>& event) -> Bool;
    auto SendInputEventDetached(Event<>& event) -> Bool;
    auto SendHitTestEventDetached(Event<>& event) -> Bool;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

private:
    auto SendEvent(Event<>& event, EventFunction const& func) -> Async<Bool>;
    auto SendEventDetached(Event<>& event, auto (PlatformWindow::*func)(Event<>&)->Async<Bool>) -> Bool;

private:
    Weak<PlatformWindow> _self;
    Delegate _delegate;
};

///
/// @brief Get self.
///
template <class Self>
auto PlatformWindow::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, PlatformWindow>>(self)._self.Lock().template UnsafeAs<Self>();
}

///
/// @brief Get derived instance.
///
template <class Derived, class... Args>
auto PlatformWindow::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto key = PassKey<PlatformWindow>();
    auto view = Shared<Derived>::Make(key, std::forward<Args>(args)...);
    static_cast<PlatformWindow&>(*view)._self = view;
    static_cast<PlatformWindow&>(*view).Initialize();
    return view;
}
}
}
