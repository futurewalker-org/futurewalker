// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.RootViewType.hpp"
#include "Futurewalker.Application.RootViewDrawInfo.hpp"
#include "Futurewalker.Application.RootViewLayoutInfo.hpp"
#include "Futurewalker.Application.RootFocusNodeType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.ViewLayerManagerType.hpp"
#include "Futurewalker.Application.InputMethodType.hpp" 

#include "Futurewalker.Animation.RootAnimationTimerType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.MonotonicTime.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Root view.
///
class RootView : public View
{
public:
    struct Delegate
    {
        Function<void()> requestFrame;
        Function<MonotonicTime()> getFrameTime;
    };
    static auto Make(Delegate const& delegate) -> Shared<RootView>;
    static auto MakeWithContent(Delegate const& delegate, Shared<View> const& content) -> Shared<RootView>;

    RootView(PassKey<View> key, Delegate const& delegate);

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> view) -> void;

protected:
    auto Initialize() -> void override;

    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveFocusEvent(Event<>& event) -> Async<Bool>;

    auto UpdateAnimation() -> void;
    auto UpdateLayout() -> void;
    auto UpdateVisual() -> void;

    auto SetAnimationTimerActive(Bool const active) -> void;
    auto RequestFrame() -> void;
    auto GetFrameTime() const -> MonotonicTime;

private:
    auto DispatchPointerEvent(Event<>& event) -> Async<void>;
    auto DispatchKeyEvent(Event<>& event) -> Async<void>;
    auto DispatchInputEvent(Event<>& event) -> Async<void>;
    auto GetPointerCaptureView(PointerId const id) -> Shared<View>;
    auto SetPointerCaptureView(PointerId const id, Shared<View> const& view) -> void;
    auto GetPointerOverView(PointerId const id) -> Shared<View>;
    auto SetPointerOverView(PointerId const id, Shared<View> const& view) -> void;

private:
    auto RootIsActive() const -> Bool override;
    auto RootIsAttached() const -> Bool override;
    auto RootGetDisplayScale() const -> DisplayScale override;
    auto RootGetBackingScale() const -> BackingScale override;
    auto RootGetAnimationTimer() -> AnimationTimer& override;
    auto RootGetAnimationTimer() const -> AnimationTimer const& override;
    auto RootGetFocusNode() -> FocusNode& override;
    auto RootGetFocusNode() const -> FocusNode const& override;
    auto RootGetAttributeNode() -> AttributeNode& override;
    auto RootGetAttributeNode() const -> AttributeNode const& override;
    auto RootGetLayer() -> ViewLayer& override;
    auto RootGetLayer() const -> ViewLayer const& override;
    auto RootInvalidateLayout() -> void override;
    auto RootInvalidateVisual() -> void override;
    auto RootCapturePointer(PointerId const id, Shared<View> const& view) -> void override;
    auto RootReleasePointer(PointerId const id, Shared<View> const& view) -> void override;
    auto RootCancelInput(Shared<View> const& view) -> void override;

private:
    Bool _attached = false;
    Bool _active = false;
    DisplayScale _displayScale = 1.0;
    BackingScale _backingScale = 1.0;
    Delegate _delegate;
    Shared<ViewLayerManager> _viewLayerManager;
    Shared<RootAnimationTimer> _animationTimer;
    Shared<RootFocusNode> _focusNode;
    Shared<AttributeNode> _parentAttributeNode;
    Shared<AttributeNode> _attributeNode;
    Shared<ContainerView> _container;
    Shared<InputMethod> _inputMethod;
    RootViewDrawInfo _drawInfo;
    RootViewLayoutInfo _layoutInfo;
    using PointerWeakViewMap = HashMap<PointerId, Weak<View>>;
    PointerWeakViewMap _pointerOverViews;
    PointerWeakViewMap _pointerCaptureViews;
};
}
}
