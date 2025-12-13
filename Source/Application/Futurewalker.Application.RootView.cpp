// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.RootView.hpp"
#include "Futurewalker.Application.RootViewEvent.hpp"
#include "Futurewalker.Application.RootFocusNode.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.ViewEvent.hpp"
#include "Futurewalker.Application.ViewLayer.hpp"
#include "Futurewalker.Application.ViewLayerManager.hpp"
#include "Futurewalker.Application.PointerEvent.hpp"
#include "Futurewalker.Application.FocusEvent.hpp"
#include "Futurewalker.Application.KeyEvent.hpp"
#include "Futurewalker.Application.InputEvent.hpp"
#include "Futurewalker.Application.HitTestEvent.hpp"

#include "Futurewalker.Animation.RootAnimationTimer.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make view.
///
auto RootView::Make(Delegate const& delegate) -> Shared<RootView>
{
    return View::MakeDerived<RootView>(delegate);
}

///
/// @brief Make view with content.
///
auto RootView::MakeWithContent(Delegate const& delegate, Shared<View> const& content) -> Shared<RootView>
{
    auto view = RootView::Make(delegate);
    view->SetContent(content);
    return view;
}

///
/// @brief Constructor.
///
RootView::RootView(PassKey<View> key, Delegate const& delegate)
  : View(key)
  , _delegate {delegate}
{
}

///
/// @brief Get content view.
///
auto RootView::GetContent() -> Shared<View>
{
    if (_container)
    {
        return _container->GetContent();
    }
    return {};
}

///
/// @brief Set content view.
///
auto RootView::SetContent(Shared<View> view) -> void
{
    if (_container)
    {
        _container->SetContent(view);
    }
}

///
/// @brief Initialize view.
///
auto RootView::Initialize() -> void
{
    _viewLayerManager = Locator::Resolve<ViewLayerManager>();
    _animationTimer = RootAnimationTimer::Make({
      .requestTick = [&] { return RequestFrame(); },
      .getCurrentTime = [&] { return GetFrameTime(); },
    });
    _focusNode = RootFocusNode::Make();
    _attributeNode = AttributeNode::Make();
    _commandNode = CommandNode::Make();

    _drawInfo.Initialize();

    _container = ContainerView::Make();

    AddChildFront(_container);

    EventReceiver::Connect(*this, *this, &RootView::ReceiveEvent);
    EventReceiver::Connect(*_focusNode, *this, &RootView::ReceiveFocusEvent);
}

///
/// @brief Handle event.
///
auto RootView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<RootViewEvent::Owner>())
    {
        auto const parameter = event.As<RootViewEvent::Owner>();
        _attached = parameter->GetAttached();
        _active = parameter->GetActive();
        _displayScale = parameter->GetDisplayScale();
        _backingScale = parameter->GetBackingScale();
        _inputMethod = parameter->GetInputMethod();

        _drawInfo.SetParentLayer(parameter->GetParentLayer());

        auto const parentAttributeNode = parameter->GetParentAttributeNode();
        if (_parentAttributeNode != parentAttributeNode)
        {
            if (_parentAttributeNode)
            {
                _parentAttributeNode->RemoveChild(_attributeNode);
            }

            _parentAttributeNode = parentAttributeNode;

            if (_parentAttributeNode)
            {
                _parentAttributeNode->AddChild(_attributeNode);
            }
        }

        auto const parentCommandNode = parameter->GetParentCommandNode();
        if (_parentCommandNode != parentCommandNode)
        {
            if (_parentCommandNode)
            {
                _parentCommandNode->RemoveChild(_commandNode);
            }

            _parentCommandNode = parentCommandNode;

            if (_parentCommandNode)
            {
                _parentCommandNode->AddChild(_commandNode);
            }
        }

        NotifyRootChanged();
    }
    else if (event.Is<RootViewEvent::Resize>())
    {
        auto const parameter = event.As<RootViewEvent::Resize>();
        auto const size = parameter->GetSize();
        if (_layoutInfo.SetSize(size))
        {
            _drawInfo.GetLayer().SetSize(size);
        }
    }
    else if (event.Is<RootViewEvent::Frame>())
    {
        UpdateAnimation();
        UpdateLayout();
        UpdateVisual();
    }
    else if (event.Is<RootViewEvent::Pointer>())
    {
        auto parameter = event.As<RootViewEvent::Pointer>();
        auto& sourceEvent = parameter->GetEvent();
        co_await DispatchPointerEvent(sourceEvent);
        co_return true;
    }
    else if (event.Is<RootViewEvent::Key>())
    {
        auto parameter = event.As<RootViewEvent::Key>();
        auto& sourceEvent = parameter->GetEvent();
        co_await DispatchKeyEvent(sourceEvent);
        co_return true;
    }
    else if (event.Is<RootViewEvent::Input>())
    {
        auto parameter = event.As<RootViewEvent::Input>();
        auto& sourceEvent = parameter->GetEvent();
        co_await DispatchInputEvent(sourceEvent);
        co_return true;
    }
    else if (event.Is<RootViewEvent::HitTest>())
    {
        auto parameter = event.As<RootViewEvent::HitTest>();
        auto& sourceEvent = parameter->GetEvent();
        co_await DispatchHitTestEvent(sourceEvent);
        co_return true;
    }
    else if (event.Is<ViewEvent::Attached>())
    {
        SetAnimationTimerActive(true);
        RequestFrame();
    }
    else if (event.Is<ViewEvent::Detached>())
    {
        SetAnimationTimerActive(false);
    }
    co_return false;
}

///
/// @brief
///
auto RootView::ReceiveFocusEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<FocusEvent>())
    {
        if (auto focus = _focusNode->GetFocusedNode())
        {
            if (focus != _focusNode)
            {
                if (_inputMethod)
                {
                    if (event.Is<FocusEvent::FocusWillChange>())
                    {
                        auto inputEvent = Event<>(Event<InputEvent::Detach>());
                        co_await focus->SendEvent(inputEvent);
                    }
                    else if (event.Is<FocusEvent::FocusDidChange>())
                    {
                        auto inputEventParameter = Event<InputEvent::Attach>();
                        inputEventParameter->SetInputMethod(_inputMethod);
                        auto inputEvent = Event<>(inputEventParameter);
                        co_await focus->SendEvent(inputEvent);
                    }
                }
            }
        }
        co_return true;
    }
    co_return false;
}

///
/// @brief
///
auto RootView::UpdateAnimation() -> void
{
    _animationTimer->UpdateCurrentTime();
}

///
/// @brief
///
auto RootView::UpdateLayout() -> void
{
    try
    {
        auto const size = _layoutInfo.GetSize();
        MeasureScope::MeasureView(*this, BoxConstraints::MakeExact(size.GetWidth(), size.GetHeight()));
        ArrangeScope::ArrangeView({}, *this);
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief
///
auto RootView::UpdateVisual() -> void
{
    try
    {
        DrawScope::DrawRootView({}, *this);
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief
///
auto RootView::SetAnimationTimerActive(Bool const active) -> void
{
    _animationTimer->SetActive(active);
}

///
/// @brief
///
auto RootView::RequestFrame() -> void
{
    if (_delegate.requestFrame)
    {
        _delegate.requestFrame();
    }
}

///
/// @brief
///
auto RootView::GetFrameTime() const -> MonotonicTime
{
    if (_delegate.getFrameTime)
    {
        return _delegate.getFrameTime();
    }
    return {};
}

///
/// @brief 
///
auto RootView::DispatchPointerEvent(Event<>& event) -> Async<void>
{
    if (event.Is<PointerEvent>())
    {
        auto const parameter = event.As<PointerEvent>();
        auto const pointerId = parameter->GetPointerId();

        if (parameter.Is<PointerEvent::Motion::Enter>() || parameter.Is<PointerEvent::Motion::Over>())
        {
            co_return;
        }

        if (parameter.Is<PointerEvent::Motion::Leave>() || parameter.Is<PointerEvent::Motion::Out>() || parameter.Is<PointerEvent::Motion::Cancel>())
        {
            if (auto const overView = GetPointerOverView(pointerId))
            {
                SetPointerOverView(pointerId, nullptr);

                auto oldEnterViews = GetPathFromDescendant(overView);

                auto outEventParameter = Event<PointerEvent::Motion::Out>();
                static_cast<PointerEvent::Motion&>(*outEventParameter) = *parameter.As<PointerEvent::Motion>();
                DispatchPointerEventFromRoot(PassKey<RootView>(), outEventParameter, overView, PointerPhaseFlags::Target);

                for (auto const& oldEnterView : oldEnterViews)
                {
                    if (IsAncestorOf(oldEnterView))
                    {
                        auto leaveEventParameter = Event<PointerEvent::Motion::Leave>();
                        static_cast<PointerEvent::Motion&>(*leaveEventParameter) = *parameter.As<PointerEvent::Motion>();
                        DispatchPointerEventFromRoot(PassKey<RootView>(), leaveEventParameter, oldEnterView, PointerPhaseFlags::Target);
                    }
                }
            }
            co_return;
        }

        // 1. Use HitTest to pick a candidate for sending events. This becomes initial `target` view.
        //    When there's a view capturing pointer events, it will becomes `target` view insdead.
        // 2. Propagate `Capture` phase event until it reaches initial `target`, to see if any of ancestors intercepts propagation.
        //    NOTE: Only core motion events ("Move", "Down", "Up") can be intercepted.
        //    Then now we know which view will actually receive pointer event.
        // 3. Send `Out` event if new target differ from previously hovered view.
        // 4. Send `Leave` event to views which no longer intersect with new pointer position.
        // 5. Send `Enter` event to views which become intersect with new pointer position.
        // 6. Send original event to target view.
        //
        // One thing we have to be careful about is views can be removed during event propagation.

        auto targetView = Shared<View>();
        if (auto const captureView = GetPointerCaptureView(pointerId))
        {
            targetView = captureView;
        }
        else if (auto const trackingView = FindPointerTrackingView(parameter))
        {
            targetView = trackingView;
        }

        if (targetView)
        {
            if (auto const interceptView = DispatchPointerEventFromRoot(PassKey<RootView>(), event.As<PointerEvent>(), targetView, PointerPhaseFlags::Capture))
            {
                targetView = interceptView;
            }
        }

        auto const overView = GetPointerOverView(pointerId);
        if (overView != targetView)
        {
            SetPointerOverView(pointerId, targetView);

            auto oldEnterViews = GetPathFromDescendant(overView);
            auto newEnterViews = GetPathFromDescendant(targetView);

            if (overView)
            {
                auto outEventParameter = Event<PointerEvent::Motion::Out>();
                PointerEvent::Copy(*outEventParameter, *parameter);
                DispatchPointerEventFromRoot(PassKey<RootView>(), outEventParameter, overView, PointerPhaseFlags::Target);
            }

            for (auto const& oldEnterView : oldEnterViews)
            {
                auto const it = std::find(newEnterViews.begin(), newEnterViews.end(), oldEnterView);
                if (it == newEnterViews.end())
                {
                    if (IsAncestorOf(oldEnterView))
                    {
                        auto leaveEventParameter = Event<PointerEvent::Motion::Leave>();
                        PointerEvent::Copy(*leaveEventParameter, *parameter);
                        DispatchPointerEventFromRoot(PassKey<RootView>(), leaveEventParameter, oldEnterView, PointerPhaseFlags::Target);
                    }
                }
            }

            for (auto const& newEnterView : newEnterViews)
            {
                auto const it = std::find(oldEnterViews.rbegin(), oldEnterViews.rend(), newEnterView);
                if (it == oldEnterViews.rend())
                {
                    if (IsAncestorOf(newEnterView))
                    {
                        auto enterEventParameter = Event<PointerEvent::Motion::Enter>();
                        PointerEvent::Copy(*enterEventParameter, *parameter);
                        DispatchPointerEventFromRoot(PassKey<RootView>(), enterEventParameter, newEnterView, PointerPhaseFlags::Target);
                    }
                }
            }

            if (targetView)
            {
                auto targetEventParameter = Event<PointerEvent::Motion::Over>();
                PointerEvent::Copy(*targetEventParameter, *parameter);
                DispatchPointerEventFromRoot(PassKey<RootView>(), targetEventParameter, targetView, PointerPhaseFlags::Target);
            }
        }

        if (targetView)
        {
            DispatchPointerEventFromRoot(PassKey<RootView>(), event.As<PointerEvent>(), targetView, PointerPhaseFlags::Target | PointerPhaseFlags::Bubble);
        }
    }
}

///
/// @brief Dispatch key event.
///
auto RootView::DispatchKeyEvent(Event<>& event) -> Async<void>
{
    if (event.Is<KeyEvent>())
    {
        if (auto focus = _focusNode->GetFocusedNode())
        {
            co_await focus->SendEvent(event);
        }
    }
}

///
/// @brief Dispatch input event.
///
auto RootView::DispatchInputEvent(Event<>& event) -> Async<void>
{
    (void)event;
    co_return;
}

///
/// @brief
///
auto RootView::DispatchHitTestEvent(Event<>& event) -> Async<void>
{
    if (event.Is<HitTestEvent>())
    {
        auto hitTestEvent = event.As<HitTestEvent>();
        DispatchHitTestEventFromRoot(PassKey<RootView>(), hitTestEvent);
        event = hitTestEvent;
    }
    co_return;
}

///
/// @brief
///
/// @param id
///
auto RootView::GetPointerCaptureView(PointerId const id) -> Shared<View>
{
    auto const it = _pointerCaptureViews.find(id);
    if (it != _pointerCaptureViews.end())
    {
        return it->second.Lock();
    }
    return {};
}

///
/// @brief
///
/// @param id
/// @param view
///
auto RootView::SetPointerCaptureView(PointerId const id, Shared<View> const& view) -> void
{
    _pointerCaptureViews.insert_or_assign(id, view);
}

///
/// @brief
///
/// @param id
///
auto RootView::GetPointerOverView(PointerId const id) -> Shared<View>
{
    auto const it = _pointerOverViews.find(id);
    if (it != _pointerOverViews.end())
    {
        return it->second.Lock();
    }
    return {};
}

///
/// @brief
///
/// @param id
/// @param view
///
auto RootView::SetPointerOverView(PointerId const id, Shared<View> const& view) -> void
{
    _pointerOverViews.insert_or_assign(id, view);
}

///
/// @brief
///
auto RootView::RootIsActive() const -> Bool
{
    return _active;
}

///
/// @brief
///
auto RootView::RootIsAttached() const -> Bool
{
    return _attached;
}

///
/// @brief
///
auto RootView::RootGetDisplayScale() const -> DisplayScale
{
    return _displayScale;
}

///
/// @brief
///
auto RootView::RootGetBackingScale() const -> BackingScale
{
    return _backingScale;
}

///
/// @brief
///
auto RootView::RootGetAnimationTimer() -> AnimationTimer&
{
    return *_animationTimer;
}

///
/// @brief
///
auto RootView::RootGetAnimationTimer() const -> AnimationTimer const&
{
    return *_animationTimer;
}

///
/// @brief
///
auto RootView::RootGetFocusNode() -> FocusNode&
{
    return *_focusNode;
}

///
/// @brief
///
auto RootView::RootGetFocusNode() const -> FocusNode const&
{
    return *_focusNode;
}

///
/// @brief
///
auto RootView::RootGetAttributeNode() -> AttributeNode&
{
    return *_attributeNode;
}

///
/// @brief
///
auto RootView::RootGetAttributeNode() const -> AttributeNode const&
{
    return *_attributeNode;
}

///
/// @brief
///
auto RootView::RootGetCommandNode() -> CommandNode&
{
    return *_commandNode;
}

///
/// @brief
///
auto RootView::RootGetCommandNode() const -> CommandNode const&
{
    return *_commandNode;
}

///
/// @brief
///
auto RootView::RootGetLayer() -> ViewLayer&
{
    return _drawInfo.GetLayer();
}

///
/// @brief
///
auto RootView::RootGetLayer() const -> ViewLayer const&
{
    return _drawInfo.GetLayer();
}

///
/// @brief
///
auto RootView::RootInvalidateLayout() -> void
{
    RequestFrame();
}

///
/// @brief
///
auto RootView::RootInvalidateVisual() -> void
{
    RequestFrame();
}

///
/// @brief
///
auto RootView::RootLocalToGlobalPoint(Point<Dp> const& point) const -> Point<Vp>
{
    if (_delegate.localToGlobalPoint)
    {
        return _delegate.localToGlobalPoint(point);
    }
    return UnitFunction::ConvertDpToVp(point, GetDisplayScale());
}

///
/// @brief
///
auto RootView::RootGlobalToLocalPoint(Point<Vp> const& point) const -> Point<Dp>
{
    if (_delegate.globalToLocalPoint)
    {
        return _delegate.globalToLocalPoint(point);
    }
    return UnitFunction::ConvertVpToDp(point, GetDisplayScale());
}

///
/// @brief
///
auto RootView::RootCapturePointer(PointerId const id, Shared<View> const& view) -> void
{
    SetPointerCaptureView(id, view);
}

///
/// @brief
///
auto RootView::RootReleasePointer(PointerId const id, Shared<View> const& view) -> void
{
    if (GetPointerCaptureView(id) == view)
    {
        SetPointerCaptureView(id, nullptr);
    }
}

///
/// @brief
///
auto RootView::RootCancelInput(Shared<View> const& view) -> void
{
    if (view)
    {
        // TODO: send events.
    }
}

///
/// @brief
///
auto RootView::RootMakeOwnedWindow(WindowOptions const& options) -> Shared<Window>
{
    if (_delegate.makeOwnedWindow)
    {
        return _delegate.makeOwnedWindow(options);
    }
    return {};
}
}
