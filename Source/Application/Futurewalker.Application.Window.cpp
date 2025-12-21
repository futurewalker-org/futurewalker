// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Window.hpp"
#include "Futurewalker.Application.WindowEvent.hpp"
#include "Futurewalker.Application.WindowAttribute.hpp"
#include "Futurewalker.Application.ApplicationContext.hpp"
#include "Futurewalker.Application.FrameEvent.hpp"
#include "Futurewalker.Application.PointerEvent.hpp"
#include "Futurewalker.Application.KeyEvent.hpp"
#include "Futurewalker.Application.InputEvent.hpp"
#include "Futurewalker.Application.InputMethod.hpp"
#include "Futurewalker.Application.RootView.hpp"
#include "Futurewalker.Application.RootViewEvent.hpp"
#include "Futurewalker.Application.ViewLayer.hpp"
#include "Futurewalker.Application.Screen.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.ViewAreaManager.hpp"
#include "Futurewalker.Application.ViewAttribute.hpp"
#include "Futurewalker.Application.PlatformWindowContext.hpp"
#include "Futurewalker.Application.PlatformWindow.hpp"
#include "Futurewalker.Application.PlatformWindowEvent.hpp"
#include "Futurewalker.Application.PlatformFrameEvent.hpp"
#include "Futurewalker.Application.PlatformPointerEvent.hpp"
#include "Futurewalker.Application.PlatformKeyEvent.hpp"
#include "Futurewalker.Application.PlatformInputEvent.hpp"
#include "Futurewalker.Application.PlatformHitTestEvent.hpp"
#include "Futurewalker.Application.PlatformViewLayerContext.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp"

#include "Futurewalker.Action.CommandNode.hpp"
#include "Futurewalker.Action.CommandDispatcherContext.hpp"

#include "Futurewalker.Base.Debug.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include <ranges>

namespace FW_DETAIL_NS
{
///
/// @brief Create window object.
///
/// @param options Window options.
///
/// @return Non-null window object.
///
auto Window::Make(WindowOptions const& options) -> Shared<Window>
{
    return Window::MakeDerived<Window>(options);
}

///
/// @brief Constructor.
///
Window::Window(PassKey<Window>)
{
    _propertyStore = PropertyStore::Make();
}

///
/// @brief Destructor.
///
Window::~Window()
{
    if (_platformObject)
    {
        _platformObject->SetDelegate({});
        _platformObject->Destroy();
    }
}

///
/// @brief Get visibility of window.
///
auto Window::IsVisible() const -> Bool
{
    if (!IsClosed())
    {
        return _platformObject->IsVisible();
    }
    return false;
}

///
/// @brief Set visibility of window.
///
/// @param visible New visibility of the window.
///
auto Window::SetVisible(Bool const visible) -> void
{
    if (!IsClosed())
    {
        _platformObject->SetVisible(visible);
    }
}

///
/// @brief Get activity of window.
///
auto Window::IsActive() const -> Bool
{
    if (!IsClosed())
    {
        return _platformObject->IsActive();
    }
    return false;
}

///
/// @brief Get focus of window.
///
auto Window::IsFocused() const -> Bool
{
    if (!IsClosed())
    {
        return _platformObject->IsFocused();
    }
    return false;
}

///
/// @brief Get size of window.
///
auto Window::GetFrameRect() const -> Rect<Vp>
{
    if (!IsClosed())
    {
        return _platformObject->GetFrameRect();
    }
    return {};
}

///
/// @brief Set frame rectangle of window.
///
/// @param rect New frame rectangle of the window.
///
/// @note When the window is minimized, changes the frame rectangle to be used when restoring the window.
/// @note When the window is maximized, restores the window and changes the frame rectangle.
///
auto Window::SetFrameRect(Rect<Vp> const& rect) -> void
{
    if (!IsClosed())
    {
        _platformObject->SetFrameRect(rect);
    }
}

///
/// @brief Get restored frame rectangle of window.
///
auto Window::GetRestoredFrameRect() const -> Rect<Vp>
{
    if (!IsClosed())
    {
        return _platformObject->GetRestoredFrameRect();
    }
    return {};
}

///
/// @brief Set restored frame rectangle of window.
///
/// @param rect New restored frame rectangle of the window.
///
/// @note When the window is minimized or maximized, changes the frame rectangle to be used when restoring the window.
///
auto Window::SetRestoredFrameRect(Rect<Vp> const& rect) -> void
{
    if (!IsClosed())
    {
        _platformObject->SetRestoredFrameRect(rect);
    }
}

///
/// @brief Convert local point to global point.
///
auto Window::LocalToGlobalPoint(Point<Dp> const& point) const -> Point<Vp>
{
    auto const position = GetFrameRect().GetPosition();
    return position + UnitFunction::ConvertDpToVp(point, GetDisplayScale()).As<Offset>();
}

///
/// @brief Convert global point to local point.
///
auto Window::GlobalToLocalPoint(Point<Vp> const& point) const -> Point<Dp>
{
    auto const position = GetFrameRect().GetPosition();
    return UnitFunction::ConvertVpToDp(point - position.As<Offset>(), GetDisplayScale());
}

///
/// @brief Convert local rect to global rect.
///
auto Window::LocalToGlobalRect(Rect<Dp> const& rect) const -> Rect<Vp>
{
    auto const lt = LocalToGlobalPoint(rect.GetTopLeft());
    auto const rb = LocalToGlobalPoint(rect.GetBottomRight());
    return Rect<Vp>(lt.GetX(), lt.GetY(), rb.GetX(), rb.GetY());
}

///
/// @brief Convert global rect to local rect.
///
auto Window::GlobalToLocalRect(Rect<Vp> const& rect) const -> Rect<Dp>
{
    auto const lt = GlobalToLocalPoint(rect.GetTopLeft());
    auto const rb = GlobalToLocalPoint(rect.GetBottomRight());
    return Rect<Dp>(lt.GetX(), lt.GetY(), rb.GetX(), rb.GetY());
}

///
/// @brief Convert local point to root point.
///
auto Window::LocalToRootViewPoint(Point<Dp> const& point) const -> Point<Dp>
{
    const auto clientRect = GetClientRect();
    return Point<Dp>(point.GetX() - clientRect.GetLeft(), point.GetY() - clientRect.GetTop());
}

///
/// @brief Convert root point to local point.
///
auto Window::RootViewToLocalPoint(Point<Dp> const& point) const -> Point<Dp>
{
    const auto clientRect = GetClientRect();
    return Point<Dp>(point.GetX() + clientRect.GetLeft(), point.GetY() + clientRect.GetTop());
}

///
/// @brief
///
auto Window::LocalToRootViewRect(Rect<Dp> const& rect) const -> Rect<Dp>
{
    const auto lt = LocalToRootViewPoint(rect.GetTopLeft());
    const auto rb = LocalToRootViewPoint(rect.GetBottomRight());
    return Rect<Dp>(lt.GetX(), lt.GetY(), rb.GetX(), rb.GetY());
}

///
/// @brief
///
auto Window::RootViewToLocalRect(Rect<Dp> const& rect) const -> Rect<Dp>
{
    const auto lt = RootViewToLocalPoint(rect.GetTopLeft());
    const auto rb = RootViewToLocalPoint(rect.GetBottomRight());
    return Rect<Dp>(lt.GetX(), lt.GetY(), rb.GetX(), rb.GetY());
}

///
/// @brief Get title of window.
///
auto Window::GetTitle() const -> String
{
    if (!IsClosed())
    {
        return _platformObject->GetTitle();
    }
    return {};
}

///
/// @brief Set title of the window.
///
/// @param title New title of the window.
///
auto Window::SetTitle(String const& title) -> void
{
    if (!IsClosed())
    {
        _platformObject->SetTitle(title);
    }
}

///
/// @brief Get size constraints of the window.
///
auto Window::GetSizeConstraints() const -> BoxConstraints
{
    if (!IsClosed())
    {
        return _platformObject->GetSizeConstraints();
    }
    return BoxConstraints::MakeUnconstrained();
}

///
/// @brief Set size constraints of the window.
///
/// @param constraints New size constraints of the window.
///
auto Window::SetSizeConstraints(BoxConstraints const& constraints)
{
    if (!IsClosed())
    {
        _platformObject->SetSizeConstraints(constraints);
    }
}

///
/// @brief Get display scale of the window.
///
auto Window::GetDisplayScale() const -> DisplayScale
{
    if (!IsClosed())
    {
        return _platformObject->GetDisplayScale();
    }
    return 1.0;
}

///
/// @brief Get backing scale of the window.
///
auto Window::GetBackingScale() const -> BackingScale
{
    if (!IsClosed())
    {
        return _platformObject->GetBackingScale();
    }
    return 1.0;
}

///
/// @brief Get screen the window belongs to.
///
auto Window::GetScreen() const -> Shared<Screen>
{
    return Screen::GetScreenFromWindow(_platformObject);
}

///
/// @brief Send event to the window.
///
/// @param event Event to send.
///
/// @return True if the event is handled.
///
auto Window::SendEvent(Event<>& event) -> Async<Bool>
{
    co_return co_await GetEventReceiver().SendEvent(event);
}

///
/// @brief Minimize the window.
///
auto Window::Minimize() -> void
{
    if (!IsClosed())
    {
        _platformObject->Minimize();
    }
}

///
/// @brief Maximize the window.
///
auto Window::Maximize() -> void
{
    if (!IsClosed())
    {
        _platformObject->Maximize();
    }
}

///
/// @brief Restore the window from minimized or maximized state.
///
auto Window::Restore() -> void
{
    if (!IsClosed())
    {
        _platformObject->Restore();
    }
}

///
/// @brief Get content view of the window.
///
auto Window::GetContent() -> Shared<View>
{
    if (_frameView)
    {
        return _frameView->GetContent();
    }
    return {};
}

///
/// @brief Set content view of the window.
///
/// @param contentView New content view of the window.
///
auto Window::SetContent(Shared<View> contentView) -> void
{
    if (_frameView)
    {
        _frameView->SetContent(contentView);
    }
}

///
/// @brief
///
auto Window::MeasureSize(BoxConstraints const& constraints) -> Size<Dp>
{
    if (!IsClosed())
    {
        auto const clientInsets = _platformObject->GetAreaInsets(WindowArea::Frame);
        auto const cs = BoxConstraints::Offset(constraints, -clientInsets.GetTotalWidth(), -clientInsets.GetTotalHeight());
        auto clientSize = MeasureScope::MeasureView(*_frameView, cs);
        return Size<Dp>(clientSize.GetWidth() + clientInsets.GetTotalWidth(), clientSize.GetHeight() + clientInsets.GetTotalHeight());
    }
    return {};
}

///
/// @brief
///
auto Window::SetBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    _backgroundColor.SetAttributeArg(color);
}

///
/// @brief Get whether the window is closed.
///
auto Window::IsClosed() const -> Bool
{
    if (_platformObject)
    {
        return _platformObject->IsClosed();
    }
    return true;
}

///
/// @brief Request to close the window.
///
auto Window::Close() -> Async<Bool>
{
    if (!IsClosed())
    {
        co_return co_await _platformObject->Close();
    }
    co_return true;
}

///
/// @brief Get tracker of the window.
///
auto Window::GetTracker() -> Tracker&
{
    return GetEventReceiver().GetTracker();
}

///
/// @brief Get tracker of the window.
///
auto Window::GetTracker() const -> Tracker const&
{
    return GetEventReceiver().GetTracker();
}

///
/// @brief Get event receiver of the window.
///
auto Window::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

///
/// @brief Get event receiver of the window.
///
auto Window::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}

///
/// @brief Get property store of the window.
///
auto Window::GetPropertyStore() -> PropertyStore&
{
    return *_propertyStore;
}

///
/// @brief Get property store of the window.
///
auto Window::GetPropertyStore() const -> PropertyStore const&
{
    return *_propertyStore;
}

///
/// @brief Initialize the window.
///
auto Window::Initialize() -> void
{
}

///
/// @brief Get attribute node of the window.
///
auto Window::GetAttributeNode() -> AttributeNode&
{
    return *_attributeNode;
}

///
/// @brief Get attribute node of the window.
///
auto Window::GetAttributeNode() const -> AttributeNode const&
{
    return *_attributeNode;
}

///
/// @brief Receive window event.
///
auto Window::ReceiveWindowEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<WindowEvent::SizeChanged>())
    {
        if (_rootView && _platformObject)
        {
            ResizeRootView(GetClientRect().GetSize());
            UpdateRootView();
        }
    }
    else if (event.Is<WindowEvent::AreaChanged>())
    {
        UpdateAreaRects();
    }
    co_return false;
}

///
/// @brief
///
auto Window::ReceiveFrameEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<FrameEvent::Tick>())
    {
        UpdateRootView();
    }
    co_return false;
}

///
/// @brief Receive pointer event.
///
auto Window::ReceivePointerEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PointerEvent>())
    {
        if (_rootView)
        {
            auto rootViewEvent = Event<>(Event<RootViewEvent::Pointer>::Make(event));
            co_await _rootView->SendEvent(rootViewEvent);
        }
    }
    co_return false;
}

///
/// @brief
///
auto Window::ReceiveKeyEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<KeyEvent>())
    {
        if (_rootView)
        {
            auto rootViewEvent = Event<>(Event<RootViewEvent::Key>::Make(event));
            co_await _rootView->SendEvent(rootViewEvent);
        }
    }
    co_return false;
}

///
/// @brief
///
auto Window::ReceiveHitTestEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<HitTestEvent>())
    {
        if (_rootView)
        {
            auto rootViewEvent = Event<>(Event<RootViewEvent::HitTest>::Make(event));
            co_return co_await _rootView->SendEvent(rootViewEvent);
        }
    }
    co_return false;
}

///
/// @brief
///
auto Window::ReceiveInputEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<InputEvent>())
    {
        if (_rootView)
        {
            auto rootViewEvent = Event<>(Event<RootViewEvent::Input>::Make(event));
            co_await _rootView->SendEvent(rootViewEvent);
        }
    }
    co_return false;
}

///
/// @brief
///
auto Window::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        if (event.As<AttributeEvent::ValueChanged>()->GetId() == _backgroundColor.GetAttributeId())
        {
            UpdateBackgroundColor();
        }
    }
    co_return false;
}

///
/// @brief
///
auto Window::GetSelfBase() -> Shared<Window>
{
    return _self.Lock();
}

///
/// @brief
///
auto Window::GetSelfBase() const -> Shared<Window const>
{
    return _self.Lock();
}

///
/// @brief
///
auto Window::Realize(WindowOptions const& options) -> void
{
    if (_platformObject)
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    auto owner = Shared<PlatformWindow>();
    if (options.owner)
    {
        owner = options.owner->_platformObject;
    }

    const auto platformOptions = PlatformWindowOptions {
        .behavior = options.behavior,
        .backgroundStyle = options.backgroundStyle,
        .owner = owner,
        .closable = options.closable,
        .resizable = options.resizable,
        .minimizable = options.minimizable,
        .hasFrame = options.hasFrame,
        .hasTitleBar = options.hasTitleBar,
        .allowActiveOwnerPopup = options.allowActiveOwnerPopup,
    };

    const auto delegate = PlatformWindow::Delegate {
        .sendPointerEvent = [this](Event<>& e) -> Async<Bool> { co_return co_await HandlePlatformPointerEvent(e); },
        .sendKeyEvent = [this](Event<>& e) -> Async<Bool> { co_return co_await HandlePlatformKeyEvent(e); },
        .sendInputEvent = [this](Event<>& e) -> Async<Bool> { co_return co_await HandlePlatformInputEvent(e); },
        .sendFrameEvent = [this](Event<>& e) -> Async<Bool> { co_return co_await HandlePlatformFrameEvent(e); },
        .sendWindowEvent = [this](Event<>& e) -> Async<Bool> { co_return co_await HandlePlatformWindowEvent(e); },
        .sendHitTestEvent = [this](Event<>& e) -> Async<Bool> { co_return co_await HandlePlatformHitTestEvent(e); },
    };

    _platformObject = _platformContext->MakePlatformWindow(platformOptions, delegate);
}

///
/// @brief
///
auto Window::InitializeSelf(WindowOptions const& options, Shared<Window> const& self) -> void
{
    _self = self;

    _owner = options.owner;
    _behavior = options.behavior;

    _platformContext = Locator::Resolve<PlatformWindowContext>();

    _eventReceiver = EventReceiver::Make({
        .dispatchEvent = [&](Event<>& event, EventFunction const& dispatch) -> Async<Bool> { co_return co_await DispatchEvent(event, dispatch); },
    });

    if (options.attributeNode)
    {
        _attributeNode = options.attributeNode;
    }
    else
    {
        _attributeNode = AttributeNode::Make();
        if (auto const applicationContext = Locator::GetInstance<ApplicationContext>())
        {
            applicationContext->GetAttributeNode().AddChild(_attributeNode);
        }
    }

    if (options.commandNode)
    {
        _commandNode = options.commandNode;
    }
    else
    {
        _commandNode = CommandNode::Make();
        if (auto const commandDispatcherContext = Locator::GetInstance<CommandDispatcherContext>())
        {
            commandDispatcherContext->GetCommandNode().AddChild(_commandNode);
        }
    }

    EventReceiver::Connect(*this, *this, &Window::ReceiveWindowEvent);
    EventReceiver::Connect(*this, *this, &Window::ReceiveFrameEvent);
    EventReceiver::Connect(*this, *this, &Window::ReceivePointerEvent);
    EventReceiver::Connect(*this, *this, &Window::ReceiveKeyEvent);
    EventReceiver::Connect(*this, *this, &Window::ReceiveInputEvent);
    EventReceiver::Connect(*this, *this, &Window::ReceiveHitTestEvent);

    Realize(options);

    _areaManager = ViewAreaManager::Make();

    _frameView = ContainerView::Make();

    _rootViewLayer = ViewLayer::Make(_platformObject->GetViewLayer());

    _inputMethod = InputMethod::Make(_platformObject->GetInputMethod());

    _rootView = RootView::MakeWithContent(
      {
          .requestFrame = [&] { return RequestFrame(); },
          .capturePointer = [&](PointerId const id) { return CapturePointer(id); },
          .releasePointer = [&](PointerId const id) { return ReleasePointer(id); },
          .getFrameTime = [&] { return GetFrameTime(); },
          .localToGlobalPoint = [&](Point<Dp> const& point) { return LocalToGlobalPoint(RootViewToLocalPoint(point)); },
          .globalToLocalPoint = [&](Point<Vp> const& point) { return RootViewToLocalPoint(GlobalToLocalPoint(point)); },
          .makeOwnedWindow = [&](WindowOptions const& options) { return MakeOwnedWindow(options); },
      },
      _frameView);

    AttributeNode::SetValue<&ViewAttribute::AreaManager>(*this, _areaManager);

    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, BackgroundColor, {});
    _backgroundColor.BindAttribute(*this, BackgroundColor);

    EventReceiver::Connect(_backgroundColor, *this, &Window::ReceiveAttributeEvent);

    UpdateBackgroundColor();
    UpdateAreaRects();
    UpdateRootViewLayer();

    AttachRootView();
    ResizeRootView(GetClientRect().GetSize());
}

///
/// @brief Request frame event.
///
auto Window::RequestFrame() -> void
{
    if (_platformObject)
    {
        _platformObject->RequestFrame();
    }
}

///
/// @brief
///
auto Window::GetFrameTime() const -> MonotonicTime
{
    if (_platformObject)
    {
        return _platformObject->GetFrameTime();
    }
    return {};
}

///
/// @brief
///
auto Window::CapturePointer(PointerId const id) -> void
{
    if (_platformObject)
    {
        _platformObject->CapturePointer(id);
    }
}

///
/// @brief
///
auto Window::ReleasePointer(PointerId const id) -> void
{
    if (_platformObject)
    {
        _platformObject->ReleasePointer(id);
    }
}

///
/// @brief
///
/// @param event
/// @param dispatch
///
auto Window::DispatchEvent(Event<>& event, EventFunction const& dispatch) -> Async<Bool>
{
    co_return co_await dispatch(event);
}

///
/// @brief
///
auto Window::AttachRootView() -> void
{
    if (_rootView)
    {
        auto parameter = Event<RootViewEvent::Owner>();
        parameter->SetActive(IsActive());
        parameter->SetAttached(true);
        parameter->SetDisplayScale(GetDisplayScale());
        parameter->SetBackingScale(GetBackingScale());
        parameter->SetParentLayer(_rootViewLayer);
        parameter->SetParentAttributeNode(_attributeNode);
        parameter->SetParentCommandNode(_commandNode);
        parameter->SetInputMethod(_inputMethod);
        auto event = Event<>(parameter);
        _rootView->SendEventDetached(event);
    }
}

///
/// @brief
///
auto Window::DetachRootView() -> void
{
    if (_rootView)
    {
        auto parameter = Event<RootViewEvent::Owner>();
        parameter->SetActive(false);
        parameter->SetAttached(false);
        parameter->SetBackingScale(1.0);
        parameter->SetDisplayScale(1.0);
        parameter->SetParentAttributeNode(nullptr);
        parameter->SetParentCommandNode(nullptr);
        parameter->SetParentLayer(nullptr);
        parameter->SetInputMethod(nullptr);
        auto event = Event<>(parameter);
        _rootView->SendEventDetached(event);
    }
}

///
/// @brief
///
auto Window::ResizeRootView(Size<Dp> const& size) -> void
{
    if (_rootView)
    {
        auto parameter = Event<RootViewEvent::Resize>();
        parameter->SetSize(size);
        auto event = Event<>(parameter);
        _rootView->SendEventDetached(event);
    }
}

///
/// @brief
///
auto Window::UpdateRootView() -> void
{
    if (_rootView)
    {
        auto parameter = Event<RootViewEvent::Frame>();
        auto event = Event<>(parameter);
        _rootView->SendEventDetached(event);

        if (_platformObject)
        {
            _platformObject->Render();
        }
    }
}

///
/// @brief
///
auto Window::GetClientRect() const -> Rect<Dp>
{
    if (_platformObject)
    {
        auto const clientInsets = _platformObject->GetAreaInsets(WindowArea::Frame);
        auto const size = UnitFunction::ConvertVpToDp(GetFrameRect().GetSize(), GetDisplayScale());
        auto const localRect = Rect<Dp>({}, size);
        return Rect<Dp>(
          localRect.GetLeft() + clientInsets.GetLeading(),
          localRect.GetTop() + clientInsets.GetTop(),
          localRect.GetRight() - clientInsets.GetTrailing(),
          localRect.GetBottom() - clientInsets.GetBottom());
    }
    return {};
}

///
/// @brief
///
auto Window::UpdateBackgroundColor() -> void
{
    if (!IsClosed())
    {
        _platformObject->SetBackgroundColor(_backgroundColor.GetValueOrDefault().GetRGBColor());
    }
}

///
/// @brief
///
auto Window::UpdateAreaRects() -> void
{
    if (!IsClosed() && _areaManager)
    {
        auto const frameInsets = _platformObject->GetAreaInsets(WindowArea::Frame);
        auto const titleBarInsets = _platformObject->GetAreaInsets(WindowArea::TitleBar);
        auto const titleBarRects = _platformObject->GetAreaBounds(WindowArea::TitleBar);
        auto const localTitleBarInsets = EdgeInsets::Max(titleBarInsets - frameInsets, EdgeInsets());
        auto const localTitleBarRects = titleBarRects
                                        | std::ranges::views::transform([&](auto const& rect) { return Rect<Dp>::Offset(rect, -Offset<Dp>(frameInsets.GetLeading(), frameInsets.GetTop())); })
                                        | std::ranges::to<std::vector>();
        _areaManager->SetAreaInsets(ViewArea::TitleBar, localTitleBarInsets);
        _areaManager->SetAreaBounds(ViewArea::TitleBar, localTitleBarRects);
    }
}

///
/// @brief
///
auto Window::UpdateRootViewLayer() -> void
{
    if (_rootViewLayer)
    {
        _rootViewLayer->SetSize(UnitFunction::ConvertVpToDp(GetFrameRect().GetSize(), GetDisplayScale()));
    }
}

///
/// @brief
///
/// @return
///
auto Window::GetPlatformViewLayer() -> Shared<PlatformViewLayer>
{
    return _platformObject->GetViewLayer();
}

///
/// @brief
///
/// @param event
///
/// @return
///
auto Window::HandlePlatformWindowEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PlatformWindowEvent::VisibleChanged>())
    {
        auto windowEventParameter = Event<WindowEvent::VisibleChanged>();
        windowEventParameter->SetVisible(IsVisible());
        auto windowEvent = Event<>(windowEventParameter);
        co_await SendEvent(windowEvent);
        co_return true;
    }
    else if (event.Is<PlatformWindowEvent::ActiveChanged>())
    {
        auto windowEventParameter = Event<WindowEvent::ActiveChanged>();
        windowEventParameter->SetActive(IsActive());
        auto windowEvent = Event<>(windowEventParameter);
        co_await SendEvent(windowEvent);
        co_return true;
    }
    else if (event.Is<PlatformWindowEvent::FocusedChanged>())
    {
        auto windowEventParameter = Event<WindowEvent::FocusedChanged>();
        windowEventParameter->SetFocused(IsFocused());
        auto windowEvent = Event<>(windowEventParameter);
        co_await SendEvent(windowEvent);
        co_return true;
    }
    else if (event.Is<PlatformWindowEvent::CloseRequested>())
    {
        auto platformParameter = event.As<PlatformWindowEvent::CloseRequested>();
        auto windowEventParameter = Event<WindowEvent::CloseRequested>();
        windowEventParameter->SetCancelled(platformParameter->IsCancelled());
        auto windowEvent = Event<>(windowEventParameter);
        if (co_await SendEvent(windowEvent))
        {
            if (windowEvent.Is<WindowEvent::CloseRequested>())
            {
                auto const cancelled = windowEvent.As<WindowEvent::CloseRequested>()->IsCancelled();
                platformParameter->SetCancelled(cancelled);
                event = platformParameter;
            }
        }
        co_return true;
    }
    else if (event.Is<PlatformWindowEvent::Closed>())
    {
        auto windowEvent = Event<>(Event<WindowEvent::Closed>());
        co_await SendEvent(windowEvent);
        co_return true;
    }
    else if (event.Is<PlatformWindowEvent::SizeChanged>())
    {
        UpdateRootViewLayer();
        auto windowEventParameter = Event<WindowEvent::SizeChanged>();
        windowEventParameter->SetSize(GetFrameRect().GetSize());
        auto windowEvent = Event<>(windowEventParameter);
        co_await SendEvent(windowEvent);
        co_return true;
    }
    else if (event.Is<PlatformWindowEvent::PositionChanged>())
    {
        auto windowEventParameter = Event<WindowEvent::PositionChanged>();
        windowEventParameter->SetPosition(GetFrameRect().GetPosition());
        auto windowEvent = Event<>(windowEventParameter);
        co_await SendEvent(windowEvent);
        co_return true;
    }
    else if (event.Is<PlatformWindowEvent::AreaChanged>())
    {
        auto windowEventParameter = Event<WindowEvent::AreaChanged>();
        auto windowEvent = Event<>(windowEventParameter);
        co_await SendEvent(windowEvent);
        co_return true;
    }
    co_return false;
}

///
/// @brief
///
auto Window::HandlePlatformFrameEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PlatformFrameEvent::Tick>())
    {
        auto const parameter = event.As<PlatformFrameEvent::Tick>();
        auto frameEventParameter = Event<FrameEvent::Tick>();
        frameEventParameter->SetTargetTimestamp(parameter->GetTargetTimestamp());
        auto frameEvent = Event<>(frameEventParameter);
        co_await SendEvent(frameEvent);
        co_return true;
    }
    co_return false;
}

///
/// @brief
///
/// @param event
///
/// @return
///
auto Window::HandlePlatformPointerEvent(Event<>& event) -> Async<Bool>
{
    auto sendingEvent = ConvertPointerEvent(event);
    co_return co_await SendEvent(sendingEvent);
}

auto Window::HandlePlatformKeyEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PlatformKeyEvent::Down>())
    {
        auto const parameter = event.As<PlatformKeyEvent::Down>();
        auto sendingParameter = Event<KeyEvent::Down>();
        sendingParameter->SetKey(parameter->GetKey());
        sendingParameter->SetUnmodifiedKey(parameter->GetUnmodifiedKey());
        sendingParameter->SetText(parameter->GetText());
        sendingParameter->SetModifiers(parameter->GetModifiers());
        sendingParameter->SetRepeat(parameter->IsRepeat());
        sendingParameter->SetTimestamp(parameter->GetTimestamp());
        auto sendingEvent = Event<>(sendingParameter);
        co_return co_await SendEvent(sendingEvent);
    }
    else if (event.Is<PlatformKeyEvent::Up>())
    {
        auto const parameter = event.As<PlatformKeyEvent::Up>();
        auto sendingParameter = Event<KeyEvent::Up>();
        sendingParameter->SetTimestamp(parameter->GetTimestamp());
        auto sendingEvent = Event<>(sendingParameter);
        co_return co_await SendEvent(sendingEvent);
    }
    co_return false;
}

auto Window::HandlePlatformInputEvent(Event<>& event) -> Async<Bool>
{
    (void)event;
    co_return false;
}

auto Window::HandlePlatformHitTestEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PlatformHitTestEvent>())
    {
        auto parameter = event.As<PlatformHitTestEvent>();
        auto hitTestEventParameter = Event<HitTestEvent>();
        hitTestEventParameter->SetPosition(LocalToRootViewPoint(parameter->GetPosition()));
        auto hitTestEvent = Event<>(hitTestEventParameter);
        if (co_await SendEvent(hitTestEvent))
        {
            if (hitTestEvent.Is<HitTestEvent>())
            {
                if (hitTestEvent.Is<HitTestEvent>())
                {
                    parameter->SetHit(hitTestEvent.As<HitTestEvent>()->GetHit());
                    event = parameter;
                }
            }
            co_return true;
        }
    }
    co_return false;
}

auto Window::ConvertPointerEvent(Event<> const& from) const noexcept -> Event<>
{
    auto copyPointerEventParameter = [this](PointerEvent& to, PlatformPointerEvent const& from) -> void {
        to.SetPointerId(from.GetPointerId());
        to.SetPointerType(from.GetPointerType());
        to.SetTimestamp(from.GetTimestamp());
        to.SetPrimaryPointer(from.IsPrimaryPointer());
        to.SetPosition(LocalToRootViewPoint(from.GetPosition()));
    };

    auto copyPointerMotionParameter = [&](PointerEvent::Motion& to, PlatformPointerEvent::Motion const& from) -> void { copyPointerEventParameter(to, from); };

    auto copyPointerMotionDetailedParameter = [&](auto& to, auto const& from) -> void {
        copyPointerEventParameter(to, from);
        to.SetTiltX(from.GetTiltX());
        to.SetTiltY(from.GetTiltY());
        to.SetAzimuth(from.GetAzimuth());
        to.SetAltitude(from.GetAltitude());
        to.SetTwist(from.GetTwist());
        to.SetPressure(from.GetPressure());
        to.SetTangentialPressure(from.GetTangentialPressure());
        to.SetButton(from.GetButton());
        to.SetButtons(from.GetButtons());
        to.SetModifiers(from.GetModifiers());
    };

    auto to = Event<PointerEvent>();
    if (from.Is<PlatformPointerEvent::Motion>())
    {
        if (from.Is<PlatformPointerEvent::Motion::Down>())
        {
            auto downEvent = Event<PointerEvent::Motion::Down>();
            auto const platformDown = from.As<PlatformPointerEvent::Motion::Down>();
            copyPointerMotionDetailedParameter(*downEvent, *platformDown);
            to = downEvent;
        }
        else if (from.Is<PlatformPointerEvent::Motion::Up>())
        {
            auto upEvent = Event<PointerEvent::Motion::Up>();
            auto const platformUp = from.As<PlatformPointerEvent::Motion::Up>();
            copyPointerMotionDetailedParameter(*upEvent, *platformUp);
            to = upEvent;
        }
        else if (from.Is<PlatformPointerEvent::Motion::Move>())
        {
            auto moveEvent = Event<PointerEvent::Motion::Move>();
            auto const platformMove = from.As<PlatformPointerEvent::Motion::Move>();
            copyPointerMotionDetailedParameter(*moveEvent, *platformMove);
            to = moveEvent;
        }
        else if (from.Is<PlatformPointerEvent::Motion::Enter>())
        {
            auto enterEvent = Event<PointerEvent::Motion::Enter>();
            auto const platformEnter = from.As<PlatformPointerEvent::Motion::Enter>();
            copyPointerMotionParameter(*enterEvent, *platformEnter);
            to = enterEvent;
        }
        else if (from.Is<PlatformPointerEvent::Motion::Leave>())
        {
            auto leaveEvent = Event<PointerEvent::Motion::Leave>();
            auto const platformLeave = from.As<PlatformPointerEvent::Motion::Leave>();
            copyPointerMotionParameter(*leaveEvent, *platformLeave);
            to = leaveEvent;
        }
        else if (from.Is<PlatformPointerEvent::Motion::Cancel>())
        {
            auto cancelEvent = Event<PointerEvent::Motion::Cancel>();
            auto const platformCancel = from.As<PlatformPointerEvent::Motion::Cancel>();
            copyPointerMotionParameter(*cancelEvent, *platformCancel);
            to = cancelEvent;
        }
    }
    else if (from.Is<PlatformPointerEvent::Action>())
    {
        if (from.Is<PlatformPointerEvent::Action::Magnify>())
        {
            auto magnifyEvent = Event<PointerEvent::Action::Magnify>();
            auto const platformMagnify = from.As<PlatformPointerEvent::Action::Magnify>();
            copyPointerEventParameter(*magnifyEvent, *platformMagnify);
            magnifyEvent->SetPhase(platformMagnify->GetPhase());
            magnifyEvent->SetScale(platformMagnify->GetScale());
            to = magnifyEvent;
        }
        else if (from.Is<PlatformPointerEvent::Action::Rotate>())
        {
            auto rotateEvent = Event<PointerEvent::Action::Rotate>();
            auto const platformRotate = from.As<PlatformPointerEvent::Action::Rotate>();
            copyPointerEventParameter(*rotateEvent, *platformRotate);
            rotateEvent->SetPhase(platformRotate->GetPhase());
            rotateEvent->SetRotation(platformRotate->GetRotation());
            to = rotateEvent;
        }
        else if (from.Is<PlatformPointerEvent::Action::Swipe>())
        {
            auto swipeEvent = Event<PointerEvent::Action::Swipe>();
            auto const platformSwipe = from.As<PlatformPointerEvent::Action::Swipe>();
            copyPointerEventParameter(*swipeEvent, *platformSwipe);
            swipeEvent->SetPhase(platformSwipe->GetPhase());
            swipeEvent->SetDeltaX(platformSwipe->GetDeltaX());
            swipeEvent->SetDeltaY(platformSwipe->GetDeltaY());
            swipeEvent->SetInertial(platformSwipe->IsInertial());
            to = swipeEvent;
        }
        else if (from.Is<PlatformPointerEvent::Action::Scroll>())
        {
            auto scrollEvent = Event<PointerEvent::Action::Scroll>();
            auto const platformScroll = from.As<PlatformPointerEvent::Action::Scroll>();
            copyPointerEventParameter(*scrollEvent, *platformScroll);
            scrollEvent->SetDeltaX(platformScroll->GetDeltaX());
            scrollEvent->SetDeltaY(platformScroll->GetDeltaY());
            scrollEvent->SetPrecision(platformScroll->GetPrecision());
            scrollEvent->SetModifiers(platformScroll->GetModifiers());
            to = scrollEvent;
        }
    }
    return to;
}

///
/// @brief Make owned window.
///
/// @param options Base window options.
///
auto Window::MakeOwnedWindow(WindowOptions const& options) -> Shared<Window>
{
    auto ownedOptions = options;
    ownedOptions.owner = GetSelf();
    return Window::Make(ownedOptions);
}
}
