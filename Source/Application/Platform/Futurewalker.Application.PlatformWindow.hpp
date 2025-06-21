// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformWindowType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"
#include "Futurewalker.Application.PlatformInputMethodType.hpp"
#include "Futurewalker.Application.BoxConstraints.hpp"

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
    };
    PlatformWindow(PassKey<PlatformWindow>, Delegate const& delegate);

    virtual ~PlatformWindow() = 0;

    virtual auto IsVisible() -> Bool = 0;
    virtual auto SetVisible(Bool const visible) -> void = 0;
    virtual auto IsActive() -> Bool = 0;
    virtual auto SetActive() -> void = 0;
    virtual auto IsFocused() -> Bool = 0;
    virtual auto SetFocus() -> void = 0;
    virtual auto GetSize() -> Size<Dp> = 0;
    virtual auto SetSize(Size<Dp> const& size) -> void = 0;
    virtual auto GetPosition() -> Point<Dp> = 0;
    virtual auto SetPosition(Point<Dp> const& position) -> void = 0;
    virtual auto GetFrameRect() -> Rect<Dp> = 0;
    virtual auto GetContentRect() -> Rect<Dp> = 0;
    virtual auto GetAreaRect(WindowArea const area) -> Rect<Dp> = 0;
    virtual auto GetDisplayScale() -> DisplayScale = 0;
    virtual auto GetBackingScale() -> BackingScale = 0;
    virtual auto GetSizeConstraints() -> BoxConstraints = 0;
    virtual auto SetSizeConstraints(BoxConstraints const& constraints) -> void = 0;
    virtual auto GetTitle() const -> String = 0;
    virtual auto SetTitle(String const& title) -> void = 0;
    virtual auto Minimize() -> void = 0;
    virtual auto Maximize() -> void = 0;
    virtual auto Restore() -> void = 0;
    virtual auto IsClosed() -> Bool = 0;
    virtual auto Close() -> void = 0;
    virtual auto RequestFrame() -> void = 0;
    virtual auto GetFrameTime() const -> MonotonicTime = 0;
    virtual auto GetViewLayer() -> Shared<PlatformViewLayer> = 0;
    virtual auto GetInputMethod() -> Shared<PlatformInputMethod> = 0;

protected:
    virtual auto Initialize() -> void = 0;

    auto SendPointerEvent(Event& event) -> Async<Bool>;
    auto SendKeyEvent(Event& event) -> Async<Bool>;
    auto SendInputEvent(Event& event) -> Async<Bool>;
    auto SendFrameEvent(Event& event) -> Async<Bool>;
    auto SendWindowEvent(Event& event) -> Async<Bool>;

    auto SendWindowEventDetached(Event const& event) -> void;
    auto SendFrameEventDetached(Event const& event) -> void;
    auto SendPointerEventDetached(Event const& event) -> void;
    auto SendKeyEventDetached(Event const& event) -> void;
    auto SendInputEventDetached(Event const& event) -> void;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

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
    return static_cast<TypeTraits::PropagateCVRef<Self&, PlatformWindow>>(self)._self.Lock().template Assume<Self>();
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
