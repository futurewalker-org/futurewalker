// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.WindowType.hpp"
#include "Futurewalker.Application.WindowAreaManagerType.hpp"
#include "Futurewalker.Application.WindowFrameViewType.hpp"
#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.ViewLayerType.hpp"
#include "Futurewalker.Application.RootViewType.hpp"
#include "Futurewalker.Application.ScreenType.hpp"
#include "Futurewalker.Application.BoxConstraints.hpp"
#include "Futurewalker.Application.InputMethodType.hpp"

#include "Futurewalker.Application.PlatformWindowType.hpp"
#include "Futurewalker.Application.PlatformWindowContextType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Signal.TrackerType.hpp"

#include "Futurewalker.Event.EventReceiverType.hpp"
#include "Futurewalker.Event.EventType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.PropertyStore.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.ReferenceArg.hpp"
#include "Futurewalker.Core.TypeTraits.hpp"
#include "Futurewalker.Core.MonotonicTime.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Window class.
///
/// Window provides abstraction over platform's native window objects.
///
/// ### Window creation
///
/// Use `Window::Make()` to create window instance.  
/// `WindowOptions` provides various customization options.  
///
/// ```cpp
/// auto window = Window::Make(WindowOptions {
///   .behavior = WindowBehavior::Normal,
///   // add other options here...
/// });
/// ```
///
/// ### Window lifecycle
///
/// While you can manage lifetime of `Window` itself, underlying representation of `Window` has its own lifecycle.  
///
/// `WindowEvent::CloseRequested` is used to query whether the window is closable or not before closing window.
/// In some cases, windows must be closed . In those cases `WindowEvent::CloseRequested` will not be called.
///
/// When a `Window` is closed, it will receive `WindowEvent::Closed`. After receiving this event, `IsClosed()` will return `true`.  
///
/// After closed event, further window operations (including `SetVisible()`) will be ignored and have not effect.  
/// There's no way to restore closed window to non-closed state.  
///
/// ### Window ownership
///
/// `Window` can be "owned" by other `Window`.  
/// Owned windows will always appear above its owner in z-order.  
///
/// When owner window is closed, its owning windows will also be closed.  
/// `WindowEvent::CloseRequested` will not be sent to owned windows in this case.  
///
/// ### Window coordinate space
///
/// For a toplevel window, `GetFrameRect()` will return its position relative to the origin of the `Desktop` it belongs to.  
/// For an owned window, it returns **relative** position to owner window's top left corner.
///
/// ### Window visibility
///
/// Initially, `Window` is configured as invisible, non-minimized state.  
/// Use `SetVisible()` to make `Window` appear on screen.
///
/// ### Window position and size
///
/// Windows created with `WindowOptions::resizable` set to `true` can be resized by user.  
/// `WindowEvent::GeometryChanged` will be sent to the window when its geometry has changed.  
///
/// Application can also set `Window`'s geometry by calling `SetFrameRect()`.   
///
/// ### Window activation and keyboard focus
///
/// An active window is a window which an user is currently actively interacting with.  
/// Currently active windows return true on `IsActive()`. `WindowEvent::ActiveChanged` will be send when active state of the window is changed.  
///
class Window : NonCopyable
{
public:
    static auto Make(WindowOptions const& options) -> Shared<Window>;

public:
    Window(PassKey<Window>);
    virtual ~Window();

    auto IsVisible() const -> Bool;
    auto SetVisible(Bool const visible) -> void;

    auto IsActive() const -> Bool;
    auto IsFocused() const -> Bool;

    auto GetSize() const -> Size<Dp>;
    auto SetSize(Size<Dp> const& size) -> void;

    auto GetPosition() const -> Point<Dp>;
    auto SetPosition(Point<Dp> const& position) -> void;

    auto GetFrameRect() const -> Rect<Dp>;
    auto GetContentRect() const -> Rect<Dp>;

    auto LocalToRootViewPoint(Point<Dp> const& point) const -> Point<Dp>;
    auto RootViewToLocalPoint(Point<Dp> const& point) const -> Point<Dp>;

    auto LocalToRootViewRect(Rect<Dp> const& rect) const -> Rect<Dp>;
    auto RootViewToLocalRect(Rect<Dp> const& rect) const -> Rect<Dp>;

    auto LocalToOwnerPoint(Point<Dp> const& point, ReferenceArg<Window const> owner) const -> Point<Dp>;
    auto OwnerToLocalPoint(Point<Dp> const& point, ReferenceArg<Window const> owner) const -> Point<Dp>;

    auto LocalToOwnerRect(Rect<Dp> const& rect, ReferenceArg<Window const> owner) const -> Rect<Dp>;
    auto OwnerToLocalRect(Rect<Dp> const& rect, ReferenceArg<Window const> owner) const -> Rect<Dp>;

    auto GetTitle() const -> String;
    auto SetTitle(String const& title) -> void;

    auto GetSizeConstraints() const -> BoxConstraints;
    auto SetSizeConstraints(BoxConstraints const& constraints);

    auto GetDisplayScale() const -> DisplayScale;
    auto GetBackingScale() const -> BackingScale;

    auto GetScreen() const -> Shared<Screen>;

    auto SendEvent(Event& event) -> Async<Bool>;

    auto Minimize() -> void;
    auto Maximize() -> void;
    auto Restore() -> void;

    auto IsClosed() const -> Bool;
    auto Close() -> Async<Bool>;

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> content) -> void;

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

    auto GetPropertyStore() -> PropertyStore&;
    auto GetPropertyStore() const -> PropertyStore const&;

    auto GetAttributeNode() -> AttributeNode&;
    auto GetAttributeNode() const -> AttributeNode const&;

protected:
    virtual auto Initialize() -> void;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <class Derived, class... Args>
    static auto MakeDerived(WindowOptions const& options, Args&&... args) -> Shared<Derived>;

private:
    auto ReceiveWindowEvent(Event& event) -> Async<Bool>;
    auto ReceiveFrameEvent(Event& event) -> Async<Bool>;
    auto ReceivePointerEvent(Event& event) -> Async<Bool>;
    auto ReceiveKeyEvent(Event& event) -> Async<Bool>;
    auto ReceiveInputEvent(Event& event) -> Async<Bool>;

private:
    auto GetSelfBase() -> Shared<Window>;
    auto GetSelfBase() const -> Shared<Window const>;
    auto Realize(WindowOptions const& options) -> void;
    auto InitializeSelf(WindowOptions const& options, Shared<Window> const& self) -> void;

    auto RequestFrame() -> void;
    auto GetFrameTime() const -> MonotonicTime;

    auto DispatchEvent(Event& event, EventFunction const& dispatch) -> Async<Bool>;

    auto AttachRootView() -> void;
    auto DetachRootView() -> void;

    auto ResizeRootView(Size<Dp> const& size) -> void;
    auto UpdateRootView() -> void;

    auto UpdateAreaRects() -> void;

    auto GetPlatformViewLayer() -> Shared<PlatformViewLayer>;

    auto HandlePlatformWindowEvent(Event& event) -> Async<Bool>;
    auto HandlePlatformFrameEvent(Event& event) -> Async<Bool>;
    auto HandlePlatformPointerEvent(Event& event) -> Async<Bool>;
    auto HandlePlatformKeyEvent(Event& event) -> Async<Bool>;
    auto HandlePlatformInputEvent(Event& event) -> Async<Bool>;

private:
    Weak<Window> _self;
    Weak<Window> _owner;
    WindowBehavior _behavior = WindowBehavior::Normal;
    Unique<PropertyStore> _propertyStore;
    Shared<AttributeNode> _attributeNode;
    Shared<PlatformWindowContext> _platformContext;
    Shared<PlatformWindow> _platformObject;
    Unique<EventReceiver> _eventReceiver;
    Shared<ViewLayer> _rootViewLayer;
    Shared<InputMethod> _inputMethod;
    Shared<RootView> _rootView;
    Shared<WindowFrameView> _frameView;
    Shared<WindowAreaManager> _areaManager;
};

///
/// @brief
///
template <class Self>
auto Window::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, Window>>(self).GetSelfBase().template Assume<Self>();
}

///
/// @brief
///
template <class Derived, class... Args>
auto Window::MakeDerived(WindowOptions const& options, Args&&... args) -> Shared<Derived>
{
    auto key = PassKey<Window>();
    auto window = Shared<Derived>::Make(key, std::forward<Args>(args)...);
    static_cast<Window&>(*window).InitializeSelf(options, window);
    static_cast<Window&>(*window).Initialize();
    return window;
}
}
}
