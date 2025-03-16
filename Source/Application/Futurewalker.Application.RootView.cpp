// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.RootView.hpp"
#include "Futurewalker.Application.RootViewEvent.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.PointerScope.hpp"
#include "Futurewalker.Application.ViewEvent.hpp"
#include "Futurewalker.Application.ViewLayer.hpp"
#include "Futurewalker.Application.ViewLayerManager.hpp"
#include "Futurewalker.Application.PointerEvent.hpp"

#include "Futurewalker.Animation.RootAnimationTimer.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make view.
///
auto RootView::Make(Delegate delegate) -> Shared<RootView>
{
    return View::MakeDerived<RootView>(std::move(delegate));
}

///
/// @brief Constructor.
///
RootView::RootView(PassKey<View> key, Delegate delegate)
  : View(key)
  , _delegate {delegate}
{
    _viewLayerManager = Locator::Resolve<ViewLayerManager>();

    _animationTimer = RootAnimationTimer::Make({
      .requestTick = [&] { return RequestFrame(); },
      .getCurrentTime = [&] { return GetFrameTime(); },
    });

    _attributeNode = AttributeNode::Make();
    _drawInfo.Initialize();
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
    _container = ContainerView::Make();

    AddChildFront(_container);

    EventReceiver::Connect(*this, *this, &RootView::ReceiveEvent);
}

///
/// @brief Handle event.
///
auto RootView::ReceiveEvent(Event& event) -> Async<Bool>
{
    if (event.Is<RootViewEvent::Owner>())
    {
        auto const& parameter = event.As<RootViewEvent::Owner>();
        _attached = parameter.GetAttached();
        _active = parameter.GetActive();
        _displayScale = parameter.GetDisplayScale();
        _backingScale = parameter.GetBackingScale();

        _drawInfo.SetParentLayer(parameter.GetParentLayer());

        auto const parentAttributeNode = parameter.GetParentAttributeNode();
        if (parentAttributeNode != _parentAttributeNode)
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
        NotifyRootChanged();
    }
    else if (event.Is<RootViewEvent::Resize>())
    {
        auto const& parameter = event.As<RootViewEvent::Resize>();
        auto const size = parameter.GetSize();
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
        auto& parameter = event.As<RootViewEvent::Pointer>();
        auto& sourceEvent = parameter.GetEvent();
        co_await DispatchPointerEvent(sourceEvent);
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
        MeasureScope::MeasureRootView({}, *this, _layoutInfo.GetSize());
        ArrangeScope::ArrangeRootView({}, *this);
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
auto RootView::DispatchPointerEvent(Event& event) -> Async<void>
{
    if (event.Is<PointerEvent>())
    {
        auto const& parameter = event.As<PointerEvent>();
        auto const pointerEventType = parameter.GetEventType();
        auto const pointerId = parameter.GetPointerId();

        if (pointerEventType == PointerEventType::Enter || pointerEventType == PointerEventType::Over)
        {
            co_return;
        }

        if (pointerEventType == PointerEventType::Leave || pointerEventType == PointerEventType::Out || pointerEventType == PointerEventType::Cancel)
        {
            if (auto const overView = GetPointerOverView(pointerId))
            {
                SetPointerOverView(pointerId, nullptr);

                auto oldEnterViews = GetPathFromDescendant(overView);

                auto outEventParameter = parameter;
                outEventParameter.SetEventType(PointerEventType::Out);
                auto outParameter = PointerParameter(outEventParameter);
                outParameter.SetTargetView(overView);
                outParameter.SetPhaseFlags(PointerEventPhaseFlags::Target);
                PointerScope::DispatchRootView({}, *this, outParameter);

                for (auto const& oldEnterView : oldEnterViews)
                {
                    if (IsAncestorOf(oldEnterView))
                    {
                        auto leaveEventParameter = parameter;
                        leaveEventParameter.SetEventType(PointerEventType::Leave);
                        auto leaveParameter = PointerParameter(leaveEventParameter);
                        leaveParameter.SetTargetView(oldEnterView);
                        leaveParameter.SetPhaseFlags(PointerEventPhaseFlags::Target);
                        PointerScope::DispatchRootView({}, *this, leaveParameter);
                    }
                }
            }
            co_return;
        }

        if (pointerEventType == PointerEventType::Up)
        {
            int i = 42;
            (void)i;
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
            auto trackingEventParameter = parameter;
            auto trackingParameter = PointerParameter(trackingEventParameter);
            trackingParameter.SetTargetView(targetView);
            trackingParameter.SetPhaseFlags(PointerEventPhaseFlags::Capture);
            if (auto const interceptView = PointerScope::DispatchRootView({}, *this, trackingParameter))
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
                auto outEventParameter = parameter;
                outEventParameter.SetEventType(PointerEventType::Out);
                auto outParameter = PointerParameter(outEventParameter);
                outParameter.SetTargetView(overView);
                outParameter.SetPhaseFlags(PointerEventPhaseFlags::Target);
                PointerScope::DispatchRootView({}, *this, outParameter);
            }

            for (auto const& oldEnterView : oldEnterViews)
            {
                auto const it = std::find(newEnterViews.begin(), newEnterViews.end(), oldEnterView);
                if (it == newEnterViews.end())
                {
                    if (IsAncestorOf(oldEnterView))
                    {
                        auto leaveEventParameter = parameter;
                        leaveEventParameter.SetEventType(PointerEventType::Leave);
                        auto leaveParameter = PointerParameter(leaveEventParameter);
                        leaveParameter.SetTargetView(oldEnterView);
                        leaveParameter.SetPhaseFlags(PointerEventPhaseFlags::Target);
                        PointerScope::DispatchRootView({}, *this, leaveParameter);
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
                        auto enterEventParameter = parameter;
                        enterEventParameter.SetEventType(PointerEventType::Enter);
                        auto enterParameter = PointerParameter(enterEventParameter);
                        enterParameter.SetTargetView(newEnterView);
                        enterParameter.SetPhaseFlags(PointerEventPhaseFlags::Target);
                        PointerScope::DispatchRootView({}, *this, enterParameter);
                    }
                }
            }

            if (targetView)
            {
                auto targetEventParameter = parameter;
                targetEventParameter.SetEventType(PointerEventType::Over);
                auto targetParameter = PointerParameter(targetEventParameter);
                targetParameter.SetTargetView(targetView);
                targetParameter.SetPhaseFlags(PointerEventPhaseFlags::Target);
                PointerScope::DispatchRootView({}, *this, targetParameter);
            }
        }

        if (targetView)
        {
            auto outEventParameter = parameter;
            auto outParameter = PointerParameter(outEventParameter);
            outParameter.SetTargetView(targetView);
            outParameter.SetPhaseFlags(PointerEventPhaseFlags::Target);
            PointerScope::DispatchRootView({}, *this, outParameter);
        }
    }
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
}
