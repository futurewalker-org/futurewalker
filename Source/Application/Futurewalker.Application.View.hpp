// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.RootViewType.hpp"
#include "Futurewalker.Application.MeasureScopeType.hpp"
#include "Futurewalker.Application.MeasureParameterType.hpp"
#include "Futurewalker.Application.ArrangeScopeType.hpp"
#include "Futurewalker.Application.ArrangeParameterType.hpp"
#include "Futurewalker.Application.DrawScopeType.hpp"
#include "Futurewalker.Application.DrawParameterType.hpp"
#include "Futurewalker.Application.HitTestScope.hpp"
#include "Futurewalker.Application.PointerEvent.hpp"
#include "Futurewalker.Application.ViewLayoutInfo.hpp"
#include "Futurewalker.Application.ViewDrawInfo.hpp"
#include "Futurewalker.Application.LayoutDirection.hpp"
#include "Futurewalker.Application.ViewLayerManagerType.hpp"
#include "Futurewalker.Application.FocusNodeType.hpp"

#include "Futurewalker.Animation.AnimationTimer.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.ReferenceArg.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

#include "Futurewalker.Event.EventType.hpp"
#include "Futurewalker.Event.EventReceiverType.hpp"

#include <functional>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief View class.
///
/// Base class of all UI controls and layouts.
///
/// ### Deriving from View
///
/// Subclasses derive from `View` have to be allocated by View's factory function.
/// Subclasses should provide `static` functions which internally call `View::MakeDerived<DerivedView>()` to create instances.
/// Note the first argument of subclasses constructor have to be `PassKey<View>`, which will be implicitly passed from `View::MakeDerived<Derived>()`.
///
/// ### View initialization
///
/// Constructors are sometimes too early to perform initialization.
/// For example, connecting event callbacks requires lifetime tracking, which is unavailable during constructor call.
/// Instead, it is recommended to provide initialization code by overriding `Initialize()` function.
/// `View::MakeDerived()` will call `View::Initialize()` after setting up lifetime tracking, so it is safe to connect callbacks there.
///
/// Constructors can still be used for setting default values for member variables, etc.
///
class View : NonCopyable
{
public:
    static auto Make() -> Shared<View>;

public:
    View() = delete;
    virtual ~View();

    View(PassKey<View>);

    auto GetFrameRect() const -> Rect<Dp>;
    auto GetContentRect() const -> Rect<Dp>;

    auto FindViewByPosition(Point<Dp> const& position) -> Shared<View>;
    auto FindViewById(ViewId const id) -> Shared<View>;

    auto FindPointerTrackingView(Event<PointerEvent> const& event) -> Shared<View>;

    auto LocalToRootPoint(Point<Dp> const& point) const -> Point<Dp>;
    auto RootToLocalPoint(Point<Dp> const& point) const -> Point<Dp>;

    auto LocalToRootRect(Rect<Dp> const& rect) const -> Rect<Dp>;
    auto RootToLocalRect(Rect<Dp> const& rect) const -> Rect<Dp>;

    auto LocalToAncestorPoint(Point<Dp> const& point, ReferenceArg<View const> ancestor) const -> Point<Dp>;
    auto AncestorToLocalPoint(Point<Dp> const& point, ReferenceArg<View const> ancestor) const -> Point<Dp>;

    auto LocalToAncestorRect(Rect<Dp> const& rect, ReferenceArg<View const> ancestor) const -> Rect<Dp>;
    auto AncestorToLocalRect(Rect<Dp> const& rect, ReferenceArg<View const> ancestor) const -> Rect<Dp>;

    auto IsAncestorOf(ReferenceArg<View const> view) const -> Bool;
    auto IsDescendantOf(ReferenceArg<View const> view) const -> Bool;

    auto IsParentOf(ReferenceArg<View const> view) const -> Bool;
    auto IsChildOf(ReferenceArg<View const> view) const -> Bool;

    auto GetPathFromDescendant(ReferenceArg<View> descendant) -> ViewArray;

    auto IsVisible() const -> Bool;
    auto IsVisibleFromRoot() const -> Bool;
    auto SetVisible(Bool const visible) -> void;

    auto IsEnabled() const -> Bool;
    auto IsEnabledFromRoot() const -> Bool;
    auto SetEnabled(Bool const enabled) -> void;

    auto IsFocused() const -> Bool;
    auto SetFocused(Bool const focused) -> void;

    auto IsActive() const -> Bool;
    auto IsAttached() const -> Bool;

    auto SendEvent(Event<>& event) -> Async<Bool>;
    auto SendEventDetached(Event<>& event) -> Bool;

    auto RemoveFromParent() -> void;

    auto GetDisplayScale() const -> DisplayScale;
    auto GetBackingScale() const -> BackingScale;

    auto GetLayoutDirection() const -> LayoutDirection;
    auto GetRawLayoutDirection() const -> ViewLayoutDirection;
    auto SetRawLayoutDirection(ViewLayoutDirection const layoutDirection) -> void;

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

    auto GetPropertyStore() -> PropertyStore&;
    auto GetPropertyStore() const -> PropertyStore const&;

    auto GetAttributeNode() -> AttributeNode&;
    auto GetAttributeNode() const -> AttributeNode const&;

    auto EnterMeasureScope(PassKey<MeasureScope>, MeasureParameter const& parameter) -> void;
    auto EnterArrangeScope(PassKey<ArrangeScope>, ArrangeParameter const& parameter) -> void;
    auto EnterDrawScope(PassKey<DrawScope>) -> void;
    auto EnterHitTestScope(PassKey<HitTestScope>, HitTestParameter const& parameter) -> Shared<View>;

    auto GetMeasuredSize(PassKey<MeasureScope>) const -> Size<Dp>;
    auto GetMeasuredSize(PassKey<ArrangeScope>) const -> Size<Dp>;
    auto SetMeasuredSize(PassKey<MeasureScope>, MeasureParameter const& parameter, Size<Dp> const& size) -> void;

protected:
    virtual auto Initialize() -> void;
    virtual auto Measure(MeasureScope& scope) -> void;
    virtual auto Arrange(ArrangeScope& scope) -> void;
    virtual auto Draw(DrawScope& scope) -> void;
    virtual auto HitTest(HitTestScope& scope) -> void;

    auto InvalidateLayout() -> void;
    auto InvalidateVisual() -> void;

    auto IsPointerInteractive() const -> Bool;

    auto GetPointerTrackingFlags() const -> ViewPointerTrackingFlags;
    auto SetPointerTrackingFlags(ViewPointerTrackingFlags const pointerTrackingFlags) -> void;

    auto GetFocusTrackingFlags() const -> ViewFocusTrackingFlags;
    auto SetFocusTrackingFlags(ViewFocusTrackingFlags const focusTrackingFlags) -> void;

    auto GetChildCount() const -> SInt64;
    auto GetChildIndex(ReferenceArg<View const> view) const -> Optional<SInt64>;
    auto GetChildAt(SInt64 const index) -> Shared<View>;
    auto GetChildAt(SInt64 const index) const -> Shared<View const>;
    auto GetChildren() -> ViewArray;
    auto GetChildren() const -> ConstViewArray;

    auto AddChildAt(Shared<View> view, SInt64 const index) -> void;
    auto AddChildBack(Shared<View> view) -> void;
    auto AddChildFront(Shared<View> view) -> void;

    auto AddLayer(ViewLayerKind const kind) -> ViewLayerId;
    auto RemoveLayer(ViewLayerId const id) -> void;
    auto GetLayer(ViewLayerId const id) -> Shared<ViewLayer>;

    auto NotifyRootChanged() -> void;

    auto CapturePointer(PointerId const id) -> void;
    auto ReleasePointer(PointerId const id) -> void;

    auto CancelInput() -> void;

    auto DispatchPointerEventFromRoot(PassKey<RootView>, Event<PointerEvent> const& event, Shared<View> const& target, PointerPhaseFlags const phase) -> Shared<View>;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <class F>
    auto ForEachChild(F f) -> void;

    template <class F>
    auto ForEachVisibleChild(F f) -> void;

    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

private:
    virtual auto RootIsActive() const -> Bool;
    virtual auto RootIsAttached() const -> Bool;
    virtual auto RootGetDisplayScale() const -> DisplayScale;
    virtual auto RootGetBackingScale() const -> BackingScale;
    virtual auto RootGetAnimationTimer() -> AnimationTimer&;
    virtual auto RootGetAnimationTimer() const -> AnimationTimer const&;
    virtual auto RootGetFocusNode() -> FocusNode&;
    virtual auto RootGetFocusNode() const -> FocusNode const&;
    virtual auto RootGetAttributeNode() -> AttributeNode&;
    virtual auto RootGetAttributeNode() const -> AttributeNode const&;
    virtual auto RootGetLayer() -> ViewLayer&;
    virtual auto RootGetLayer() const -> ViewLayer const&;
    virtual auto RootInvalidateLayout() -> void;
    virtual auto RootInvalidateVisual() -> void;
    virtual auto RootCapturePointer(PointerId const id, Shared<View> const& view) -> void;
    virtual auto RootReleasePointer(PointerId const id, Shared<View> const& view) -> void;
    virtual auto RootCancelInput(Shared<View> const& view) -> void;

private:
    auto GetSelfBase() -> Shared<View>;
    auto GetSelfBase() const -> Shared<View const>;
    auto SetParent(Shared<View> const& parent) -> void;
    auto GetParent() -> Shared<View>;
    auto GetParent() const -> Shared<View const>;
    auto GetRoot() -> Shared<View>;
    auto GetRoot() const -> Shared<View const>;
    auto IsRoot() const -> Bool;

    auto InitializeSelf(Shared<View> const& self) -> void;

    auto GetAnimationTimer() -> AnimationTimer&;
    auto GetAnimationTimer() const -> AnimationTimer const&;

    auto GetFocusNode() -> FocusNode&;
    auto GetFocusNode() const -> FocusNode const&;

    auto GetLayer() -> ViewLayer&;
    auto GetLayer() const -> ViewLayer const&;

    auto AdoptChild(Shared<View> const& child, SInt64 const index) -> void;
    auto AbandonChild(Shared<View> const& child) -> void;

    auto DispatchEvent(Event<>& event, EventFunction const& dispatch) -> Async<Bool>;
    auto DispatchNotifyEvent(Event<>& event, EventFunction const& dispatch) -> Async<Bool>;
    auto DispatchNotifyBubbleEvent(Event<>& event, EventFunction const& dispatch) -> Async<Bool>;
    auto DispatchPointerEvent(Event<PointerEvent> const& pointerEvent, Shared<View> const& target, PointerPhaseFlags const phase) -> Shared<View>;

    auto Attach() -> void;
    auto Detach() -> void;

    auto InvalidateLayoutPath() -> void;
    auto InvalidateVisualPath() -> void;

    auto NotifyActiveChanged(Bool const active) -> void;
    auto NotifyAttachedChanged(Bool const attached) -> void;
    auto NotifyVisibleChanged(Bool const visible) -> void;
    auto NotifyEnabledChanged(Bool const enabled) -> void;
    auto NotifyDisplayScaleChanged(DisplayScale const displayScale) -> void;
    auto NotifyBackingScaleChanged(BackingScale const backingScale) -> void;
    auto NotifyLayoutDirectionChanged(LayoutDirection const layoutDirection) -> void;

private:
    Unique<EventReceiver> _eventReceiver;
    Unique<PropertyStore> _propertyStore;
    Weak<View> _self;
    Weak<View> _parent;
    ViewList _children;
    ViewId _id = 0u;
    ViewLayoutInfo _layoutInfo;
    ViewDrawInfo _drawInfo;
    Shared<AnimationTimer> _animationTimer;
    Shared<FocusNode> _focusNode;
    Shared<AttributeNode> _attributeNode;
    Shared<ViewLayerManager> _layerManager;
    Shared<ViewLayer> _layer;
    Bool _visible = true;
    Bool _visibleFromRoot = true;
    Bool _enabled = true;
    Bool _enabledFromRoot = true;
    Bool _active = false;
    Bool _attached = false;
    DisplayScale _displayScale = 1.0;
    BackingScale _backingScale = 1.0;
    ViewPointerTrackingFlags _pointerTrackingFlags = ViewPointerTrackingFlags::None;
    ViewFocusTrackingFlags _focusTrackingFlags = ViewFocusTrackingFlags::None;
};

///
/// @brief Get self.
///
template <class Self>
auto View::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, View>>(self).GetSelfBase().template Assume<Self>();
}

///
/// @brief Enumerate over children.
///
template <class F>
auto View::ForEachChild(F f) -> void
{
    for (const auto& view : _children)
    {
        std::invoke(f, *view);
    }
}

///
/// @brief Enumerate over visible children.
///
template <class F>
auto View::ForEachVisibleChild(F f) -> void
{
    for (const auto& view : _children)
    {
        if (view->IsVisible())
        {
            std::invoke(f, *view);
        }
    }
}

///
/// @brief Allocate derived classes.
///
/// @param args Arguments for constructor of derived classes.
///
template <class Derived, class... Args>
auto View::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto key = PassKey<View>();
    auto view = Shared<Derived>::Make(key, std::forward<Args>(args)...);
    static_cast<View&>(*view).InitializeSelf(view);
    static_cast<View&>(*view).Initialize();
    return view;
}
}
}
