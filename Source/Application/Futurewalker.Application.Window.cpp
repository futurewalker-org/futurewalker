// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Window.hpp"
#include "Futurewalker.Application.WindowEvent.hpp"
#include "Futurewalker.Application.WindowAttribute.hpp"
#include "Futurewalker.Application.WindowAreaManager.hpp"
#include "Futurewalker.Application.WindowFrameView.hpp"
#include "Futurewalker.Application.PlatformWindowContext.hpp"
#include "Futurewalker.Application.PlatformWindow.hpp"
#include "Futurewalker.Application.PlatformWindowEvent.hpp"
#include "Futurewalker.Application.PlatformFrameEvent.hpp"
#include "Futurewalker.Application.PlatformPointerEvent.hpp"
#include "Futurewalker.Application.PlatformKeyEvent.hpp"
#include "Futurewalker.Application.PlatformInputEvent.hpp"
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

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp"

#include "Futurewalker.Base.Debug.hpp"
#include "Futurewalker.Base.Locator.hpp"

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
    _attributeNode = AttributeNode::Make();
}

///
/// @brief Destructor.
///
Window::~Window()
{
    try
    {
        if (!IsClosed())
        {
            _platformObject->Close();
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
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
auto Window::GetSize() const -> Size<Dp>
{
    if (!IsClosed())
    {
        return _platformObject->GetSize();
    }
    return {};
}

///
/// @brief Set size of window.
///
/// @param size New size of the window.
///
auto Window::SetSize(Size<Dp> const& size) -> void
{
    if (!IsClosed())
    {
        _platformObject->SetSize(size);
    }
}

///
/// @brief Get position of window.
///
auto Window::GetPosition() const -> Point<Dp>
{
    if (!IsClosed())
    {
        return _platformObject->GetPosition();
    }
    return {};
}

///
/// @brief Set position of window.
///
/// @param position New position of the window.
///
auto Window::SetPosition(Point<Dp> const& position) -> void
{
    if (!IsClosed())
    {
        _platformObject->SetPosition(position);
    }
}

///
/// @brief Get frame rectangle of the window.
///
/// @return Frame rectangle of the window in global coordinate.
///
auto Window::GetFrameRect() const -> Rect<Dp>
{
    if (!IsClosed())
    {
        return _platformObject->GetFrameRect();
    }
    return {};
}

///
/// @brief Get content rectangle of the window.
///
/// @return Content rectangle of the window in local coordinate.
///
auto Window::GetContentRect() const -> Rect<Dp>
{
    if (!IsClosed())
    {
        return _platformObject->GetContentRect();
    }
    return {};
}

///
/// @brief Convert local point to root point.
///
auto Window::LocalToRootViewPoint(Point<Dp> const& point) const -> Point<Dp>
{
    const auto contentRect = GetContentRect();
    return Point<Dp>(point.GetX() - contentRect.GetLeft(), point.GetY() - contentRect.GetTop());
}

///
/// @brief Convert root point to local point.
///
auto Window::RootViewToLocalPoint(Point<Dp> const& point) const -> Point<Dp>
{
    const auto contentRect = GetContentRect();
    return Point<Dp>(point.GetX() + contentRect.GetLeft(), point.GetY() + contentRect.GetTop());
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
/// @brief
///
auto Window::LocalToOwnerPoint(Point<Dp> const& point, ReferenceArg<Window const> owner) const -> Point<Dp>
{
    (void)owner;
    return point;
}

///
/// @brief
///
auto Window::OwnerToLocalPoint(Point<Dp> const& point, ReferenceArg<Window const> owner) const -> Point<Dp>
{
    (void)owner;
    return point;
}

///
/// @brief
///
auto Window::LocalToOwnerRect(Rect<Dp> const& rect, ReferenceArg<Window const> owner) const -> Rect<Dp>
{
    (void)owner;
    return rect;
}

///
/// @brief
///
auto Window::OwnerToLocalRect(Rect<Dp> const& rect, ReferenceArg<Window const> owner) const -> Rect<Dp>
{
    (void)owner;
    return rect;
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
    return 0.0;
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
    return 0.0;
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
        auto cancelled = Bool(false);
        auto event = Event<>(Event<WindowEvent::CloseRequested>());
        if (co_await SendEvent(event))
        {
            if (event.Is<WindowEvent::CloseRequested>())
            {
                cancelled = event.As<WindowEvent::CloseRequested>()->IsCancelled();
            }
        }

        if (!cancelled)
        {
            _platformObject->Close();
        }
        co_return !cancelled;
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
            ResizeRootView(GetContentRect().GetSize());
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
        .backgroundColor = options.backgroundColor,
        .owner = owner,
        .closable = options.closable,
        .resizable = options.resizable,
        .minimizable = options.minimizable,
        .hasFrame = options.hasFrame,
        .hasTitleBar = options.hasTitleBar,
    };

    const auto delegate = PlatformWindow::Delegate {
        .sendPointerEvent = [this](Event<>& e) -> Async<Bool> { co_return co_await HandlePlatformPointerEvent(e); },
        .sendKeyEvent = [this](Event<>& e) -> Async<Bool> { co_return co_await HandlePlatformKeyEvent(e); },
        .sendInputEvent = [this](Event<>& e) -> Async<Bool> { co_return co_await HandlePlatformInputEvent(e); },
        .sendFrameEvent = [this](Event<>& e) -> Async<Bool> { co_return co_await HandlePlatformFrameEvent(e); },
        .sendWindowEvent = [this](Event<>& e) -> Async<Bool> { co_return co_await HandlePlatformWindowEvent(e); },
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

    if (const auto applicationContext = Locator::GetInstance<ApplicationContext>())
    {
        applicationContext->GetAttributeNode().AddChild(_attributeNode);
    }

    EventReceiver::Connect(*this, *this, &Window::ReceiveWindowEvent);
    EventReceiver::Connect(*this, *this, &Window::ReceiveFrameEvent);
    EventReceiver::Connect(*this, *this, &Window::ReceivePointerEvent);
    EventReceiver::Connect(*this, *this, &Window::ReceiveKeyEvent);
    EventReceiver::Connect(*this, *this, &Window::ReceiveInputEvent);

    Realize(options);

    _areaManager = WindowAreaManager::Make();

    _frameView = WindowFrameView::Make();

    _rootViewLayer = ViewLayer::Make(_platformObject->GetViewLayer());

    _inputMethod = InputMethod::Make(_platformObject->GetInputMethod());

    _rootView = RootView::MakeWithContent(
      {
          .requestFrame = [&] { return RequestFrame(); },
          .getFrameTime = [&] { return GetFrameTime(); },
      },
      _frameView);

    AttributeNode::SetValue<&WindowAttribute::AreaManager>(*this, _areaManager);

    UpdateAreaRects();

    AttachRootView();
    ResizeRootView(GetContentRect().GetSize());
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
    }
}

///
/// @brief
///
auto Window::UpdateAreaRects() -> void
{
    if (_areaManager && _platformObject)
    {
        _areaManager->SetTitleBarAreaRect(_platformObject->GetAreaRect(WindowArea::TitleBar));
        _areaManager->SetTitleBarAvailableAreaRect(_platformObject->GetAreaRect(WindowArea::TitleBarContent));
        _areaManager->SetContentAreaRect(_platformObject->GetAreaRect(WindowArea::Content));
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
        auto windowEventParameter = Event<WindowEvent::ActiveChanged>::Make();
        windowEventParameter->SetActive(IsActive());
        auto windowEvent = Event<>(windowEventParameter);
        co_await SendEvent(windowEvent);
        co_return true;
    }
    else if (event.Is<PlatformWindowEvent::FocusedChanged>())
    {
        auto windowEventParameter = Event<WindowEvent::FocusedChanged>::Make();
        windowEventParameter->SetFocused(IsFocused());
        auto windowEvent = Event<>(windowEventParameter);
        co_await SendEvent(windowEvent);
        co_return true;
    }
    else if (event.Is<PlatformWindowEvent::CloseRequested>())
    {
        co_await Close();
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
        auto windowEventParameter = Event<WindowEvent::SizeChanged>();
        windowEventParameter->SetSize(GetSize());
        auto windowEvent = Event<>(windowEventParameter);
        co_await SendEvent(windowEvent);
        co_return true;
    }
    else if (event.Is<PlatformWindowEvent::PositionChanged>())
    {
        auto windowEventParameter = Event<WindowEvent::PositionChanged>();
        windowEventParameter->SetPosition(GetPosition());
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
            to = swipeEvent;
        }
        else if (from.Is<PlatformPointerEvent::Action::Scroll>())
        {
            auto scrollEvent = Event<PointerEvent::Action::Scroll>();
            auto const platformScroll = from.As<PlatformPointerEvent::Action::Scroll>();
            copyPointerEventParameter(*scrollEvent, *platformScroll);
            scrollEvent->SetPhase(platformScroll->GetPhase());
            scrollEvent->SetDeltaX(platformScroll->GetDeltaX());
            scrollEvent->SetDeltaY(platformScroll->GetDeltaY());
            scrollEvent->SetPrecision(platformScroll->GetPrecision());
            scrollEvent->SetModifierState(platformScroll->GetModifierState());
            to = scrollEvent;
        }
    }
    return to;
}
}
