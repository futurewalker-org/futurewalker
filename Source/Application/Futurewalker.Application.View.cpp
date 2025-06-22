// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.ViewEvent.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.MeasureParameter.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.DrawParameter.hpp"
#include "Futurewalker.Application.HitTestScope.hpp"
#include "Futurewalker.Application.HitTestParameter.hpp"
#include "Futurewalker.Application.HitTestEvent.hpp"
#include "Futurewalker.Application.PointerScope.hpp"
#include "Futurewalker.Application.PointerParameter.hpp"
#include "Futurewalker.Application.PointerEvent.hpp"
#include "Futurewalker.Application.AxisConstraints.hpp"
#include "Futurewalker.Application.ViewLayer.hpp"
#include "Futurewalker.Application.ViewLayerManager.hpp"
#include "Futurewalker.Application.FocusNode.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"
#include "Futurewalker.Event.Event.hpp"

namespace FW_DETAIL_NS
{
namespace
{
class ViewEventNotifyBubble final : public ViewEvent
{
public:
    ViewEventNotifyBubble(Event<>& event)
      : _event {event}
    {
    }

    auto GetEvent() -> Event<>&
    {
        return _event;
    }

private:
    Event<>& _event;
};
}

///
/// @brief Make view.
///
auto View::Make() -> Shared<View>
{
    return View::MakeDerived<View>();
}

///
/// @brief Constructor.
///
View::View(PassKey<View>)
{
}

///
/// @brief Destructor.
///
View::~View()
{
    ForEachChild([](View& view) {
        try
        {
            view.Detach();
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    });
}

///
/// @brief
///
auto View::GetFrameRect() const -> Rect<Dp>
{
    return _layoutInfo.GetFrameRect();
}

///
/// @brief
///
auto View::GetContentRect() const -> Rect<Dp>
{
    return _layoutInfo.GetContentRect();
}

///
/// @brief
///
/// @param position
///
auto View::FindViewByPosition(Point<Dp> const& position) -> Shared<View>
{
    auto parameter = HitTestParameter();
    parameter.SetPosition(position);
    return HitTestScope::HitTestView({}, *this, parameter);
}

///
/// @brief
///
/// @param id
///
auto View::FindViewById(ViewId const id) -> Shared<View>
{
    if (_id == id)
    {
        return GetSelf();
    }

    for (SInt64 i = 0; i < GetChildCount(); ++i)
    {
        if (auto const view = GetChildAt(i))
        {
            if (auto const result = view->FindViewById(id))
            {
                return result;
            }
        }
    }
    return nullptr;
}

///
/// @brief Find tracking view for a pointer event.
///
/// @param event 
///
auto View::FindPointerTrackingView(Event<PointerEvent> const& event) -> Shared<View>
{
    auto const position = event->GetPosition();

    auto view = FindViewByPosition(position);
    while (view)
    {
        if (view->GetPointerTrackingFlags() != ViewPointerTrackingFlags::None)
        {
            return view;
        }
        view = view->GetParent();
    }
    return view;
}

///
/// @brief Convert local position to root position.
///
/// @param point
///
auto View::LocalToRootPoint(Point<Dp> const& point) const -> Point<Dp>
{
    return LocalToAncestorPoint(point, GetRoot());
}

///
/// @brief Convert root position to local position.
///
/// @param point
///
auto View::RootToLocalPoint(Point<Dp> const& point) const -> Point<Dp>
{
    return AncestorToLocalPoint(point, GetRoot());
}

///
/// @brief Convert local coordinate to root coordinate. 
///
/// @param rect 
///
auto View::LocalToRootRect(Rect<Dp> const& rect) const -> Rect<Dp>
{
    return LocalToAncestorRect(rect, GetRoot());
}

///
/// @brief Convert root coordinate to local coordinate.
///
/// @param rect
///
auto View::RootToLocalRect(Rect<Dp> const& rect) const -> Rect<Dp>
{
    return AncestorToLocalRect(rect, GetRoot());
}

///
/// @brief Convert local coordinate to ancestor coordinate.
///
/// @param point 
/// @param ancestor 
///
auto View::LocalToAncestorPoint(Point<Dp> const& point, ReferenceArg<View const> ancestor) const -> Point<Dp>
{
    auto result = point;
    auto self = GetSelf();
    while (self)
    {
        if (self.GetPointer() == ancestor.GetPointer())
        {
            return result;
        }
        result = result + (*self).GetFrameRect().GetTopLeft().As<Offset>();
        self = (*self).GetParent();
    }
    return point;
}

///
/// @brief Convert ancestor coordinate to local coordinate.
///
/// @param point
/// @param ancestor
///
auto View::AncestorToLocalPoint(Point<Dp> const& point, ReferenceArg<View const> ancestor) const -> Point<Dp>
{
    auto result = point;
    auto self = GetSelf();
    while (self)
    {
        if (self.GetPointer() == ancestor.GetPointer())
        {
            return result;
        }
        result = result - (*self).GetFrameRect().GetTopLeft().As<Offset>();
        self = (*self).GetParent();
    }
    return point;
}

///
/// @brief Convert local coordinate to ancestor coordinate.
///
/// @param rect
/// @param ancestor
///
auto View::LocalToAncestorRect(Rect<Dp> const& rect, ReferenceArg<View const> ancestor) const -> Rect<Dp>
{
    if (ancestor)
    {
        return Rect<Dp>(LocalToAncestorPoint(rect.GetTopLeft(), *ancestor), rect.GetSize());
    }
    return rect;
}

///
/// @brief Convert ancestor coordinate to local coordinate. 
///
/// @param rect
/// @param ancestor
///
auto View::AncestorToLocalRect(Rect<Dp> const& rect, ReferenceArg<View const> ancestor) const -> Rect<Dp>
{
    if (ancestor)
    {
        return Rect<Dp>(AncestorToLocalPoint(rect.GetTopLeft(), *ancestor), rect.GetSize());
    }
    return rect;
}

///
/// @brief Returns true if the view is ancestor of given view.
///
/// @param view
///
auto View::IsAncestorOf(ReferenceArg<View const> view) const -> Bool
{
    if (view)
    {
        auto parent = view->GetSelf();
        while ((parent = parent->GetParent()))
        {
            if (parent.GetPointer() == this)
            {
                return true;
            }
        }
    }
    return false;
}

///
/// @brief Returns true if the view is descendant of given view.
///
/// @param view
///
auto View::IsDescendantOf(ReferenceArg<View const> view) const -> Bool
{
    if (view)
    {
        return view->IsAncestorOf(*this);
    }
    return false;
}

///
/// @brief Get path from descendant to this view.
///
/// @param descendant 
///
auto View::GetPathFromDescendant(ReferenceArg<View> descendant) -> ViewArray
{
    if (descendant)
    {
        auto self = GetSelf();
        auto result = ViewArray();
        auto view = descendant->GetSelf();
        while (view)
        {
            result.push_back(view);

            if (view == self)
            {
                return result;
            }
            view = view->GetParent();
        }
    }
    return {};
}

///
/// @brief
///
auto View::IsVisible() const -> Bool
{
    return _visible;
}

///
/// @brief
///
auto View::IsVisibleFromRoot() const -> Bool
{
    return _visibleFromRoot;
}

///
/// @brief
///
/// @param visible
///
auto View::SetVisible(Bool const visible) -> void
{
    if (_visible != visible)
    {
        _visible = visible;

        try
        {
            GetAnimationTimer().SetEnabled(visible);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }

        auto visibleFromRoot = visible;

        if (visibleFromRoot)
        {
            if (auto parent = GetParent())
            {
                visibleFromRoot = parent->_visibleFromRoot;
            }
        }

        if (_visibleFromRoot != visibleFromRoot)
        {
            _visibleFromRoot = visibleFromRoot;

            ForEachChild([&](View& view) { view.NotifyVisibleChanged(visibleFromRoot); });

            try
            {
                auto event = Event<>(Event<ViewEvent::VisibleChanged>());
                SendEventDetached(event);
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
        }
    }
}

///
/// @brief 
///
auto View::IsEnabled() const -> Bool
{
    return _enabled;
}

///
/// @brief 
///
auto View::IsEnabledFromRoot() const -> Bool
{
    return _enabledFromRoot;
}

///
/// @brief
///
/// @param enabled
///
auto View::SetEnabled(Bool const enabled) -> void
{
    if (_enabled != enabled)
    {
        _enabled = enabled;

        auto enabledFromRoot = enabled;

        if (enabledFromRoot)
        {
            if (auto parent = GetParent())
            {
                enabledFromRoot = parent->_enabledFromRoot;
            }
        }

        if (_enabledFromRoot != enabledFromRoot)
        {
            _enabledFromRoot = enabledFromRoot;

            ForEachChild([&](View& view) { view.NotifyEnabledChanged(enabledFromRoot); });

            auto event = Event<>(Event<ViewEvent::EnabledChanged>());
            SendEventDetached(event);
        }
    }
}

///
/// @brief 
///
auto View::IsFocused() const -> Bool
{
    return GetFocusNode().IsFocused();
}

///
/// @brief
///
auto View::SetFocused(Bool const focused) -> void
{
    if (focused)
    {
        GetFocusNode().RequestFocus();
    }
    else
    {
        GetFocusNode().ReleaseFocus();
    }
}

///
/// @brief
///
auto View::IsActive() const -> Bool
{
    if (IsRoot())
    {
        return RootIsActive();
    }
    return _active;
}

///
/// @brief
///
auto View::IsAttached() const -> Bool
{
    if (IsRoot())
    {
        return RootIsAttached();
    }
    return _attached;
}

///
/// @brief 
///
/// @param event 
///
auto View::SendEvent(Event<>& event) -> Async<Bool>
{
    co_return co_await GetEventReceiver().SendEvent(event);
}

///
/// @brief
///
/// @param event
///
auto View::SendEventDetached(Event<>& event) -> Bool
{
    return GetEventReceiver().SendEventDetached(event);
}

///
/// @brief 
///
auto View::RemoveFromParent() -> void
{
    if (auto const parent = GetParent())
    {
        try
        {
            CancelInput();
            parent->AbandonChild(GetSelf());
            Detach();
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }

        auto parameter = Event<ViewEvent::ChildRemoved>::Make();
        parameter->SetRemovedView(GetSelf());
        auto event = Event<>(parameter);
        parent->SendEventDetached(event);
    }
}

///
/// @brief
///
auto View::GetDisplayScale() const -> DisplayScale
{
    if (IsRoot())
    {
        return RootGetDisplayScale();
    }
    return _displayScale;
}

///
/// @brief
///
auto View::GetBackingScale() const -> BackingScale
{
    if (IsRoot())
    {
        return RootGetBackingScale();
    }
    return _backingScale;
}

///
/// @brief
///
auto View::GetLayoutDirection() const -> LayoutDirection
{
    return _layoutInfo.GetLayoutDirection();
}

///
/// @brief
///
auto View::GetRawLayoutDirection() const -> ViewLayoutDirection
{
    return _layoutInfo.GetRawLayoutDirection();
}

///
/// @brief
///
/// @param layoutDirection
///
auto View::SetRawLayoutDirection(ViewLayoutDirection const layoutDirection) -> void
{
    if (_layoutInfo.GetRawLayoutDirection() != layoutDirection)
    {
        _layoutInfo.SetRawLayoutDirection(layoutDirection);

        if (auto parent = GetParent())
        {
            NotifyLayoutDirectionChanged(parent->GetLayoutDirection());
        }
        else
        {
            NotifyLayoutDirectionChanged(GetLayoutDirection());
        }
    }
}

///
/// @brief 
///
auto View::GetTracker() -> Tracker&
{
    return _eventReceiver->GetTracker();
}

///
/// @brief 
///
auto View::GetTracker() const -> Tracker const&
{
    return _eventReceiver->GetTracker();
}

///
/// @brief 
///
auto View::GetEventReceiver() -> EventReceiver&
{
    return _eventReceiver->GetEventReceiver();
}

///
/// @brief 
///
auto View::GetEventReceiver() const -> EventReceiver const&
{
    return _eventReceiver->GetEventReceiver();
}

///
/// @brief 
///
auto View::GetPropertyStore() -> PropertyStore&
{
    return _propertyStore->GetPropertyStore();
}

///
/// @brief 
///
auto View::GetPropertyStore() const -> PropertyStore const&
{
    return _propertyStore->GetPropertyStore();
}

///
/// @brief 
///
auto View::GetAttributeNode() -> AttributeNode&
{
    if (IsRoot())
    {
        return RootGetAttributeNode();
    }
    return *_attributeNode;
}

///
/// @brief
///
auto View::GetAttributeNode() const -> AttributeNode const&
{
    if (IsRoot())
    {
        return RootGetAttributeNode();
    }
    return *_attributeNode;
}

///
/// @brief 
///
/// @param parameter 
///
auto View::EnterMeasureScope(PassKey<MeasureScope>, MeasureParameter const& parameter) -> void
{
    try
    {
        if (_layoutInfo.BeginMeasure(parameter))
        {
            auto scope = MeasureScope(PassKey<View>(), *this, parameter);
            Measure(scope);
            _layoutInfo.EndMeasure();
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
        _layoutInfo.EndMeasure();
    }
}

///
/// @brief 
///
/// @param parameter 
///
auto View::EnterArrangeScope(PassKey<ArrangeScope>, ArrangeParameter const& parameter) -> void
{
    try
    {
        if (_layoutInfo.BeginArrange(parameter))
        {
            try
            {
                auto scope = ArrangeScope(PassKey<View>(), *this, parameter);
                Arrange(scope);
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
            _layoutInfo.EndArrange();
        }

        auto const oldFrameRect = parameter.GetOldFrameRect();
        auto const newFrameRect = parameter.GetNewFrameRect();

        if (oldFrameRect.GetPosition() != newFrameRect.GetPosition())
        {
            GetLayer().SetOffset(newFrameRect.GetPosition().As<Offset>());
        }

        if (oldFrameRect.GetSize() != newFrameRect.GetSize())
        {
            GetLayer().SetSize(newFrameRect.GetSize());
            InvalidateVisual();
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief 
///
/// @param parameter 
///
auto View::EnterDrawScope(PassKey<DrawScope> key) -> void
{
    try
    {
        if (_drawInfo.BeginDrawPath())
        {
            if (_drawInfo.BeginDraw())
            {
                try
                {
                    auto const parameter = DrawParameter();
                    auto scope = DrawScope(PassKey<View>(), *this, parameter);
                    Draw(scope);

                    _drawInfo.SetDisplayScale(GetDisplayScale());
                    _drawInfo.SetBackingScale(GetBackingScale());
                    _drawInfo.SetDisplayList(scope.GetDisplayList({}));
                    _drawInfo.SetClipMode(scope.GetClipMode({}));
                    _drawInfo.SetOpacity(scope.GetOpacity({}));
                    _drawInfo.UpdateLayers(GetLayer());
                }
                catch (...)
                {
                    FW_DEBUG_ASSERT(false);
                }
                _drawInfo.EndDraw();
            }

            ForEachVisibleChild([&](View& view) { view.EnterDrawScope(key); });

            _drawInfo.EndDrawPath();
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief 
///
/// @param parameter 
///
/// @return 
///
auto View::EnterHitTestScope(PassKey<HitTestScope> key, HitTestParameter const& parameter) -> Shared<View>
{
    try
    {
        auto const& pos = parameter.GetPosition();
        auto const rect = GetContentRect();
        if (Rect<Dp>::Intersect(rect, pos))
        {
            for (SInt64 i = GetChildCount() - 1; 0 <= i; --i)
            {
                auto const view = GetChildAt(i);
                if (view && view->IsVisible())
                {
                    auto childParameter = parameter;
                    childParameter.SetPosition(childParameter.GetPosition() - view->GetFrameRect().GetPosition().As<Offset>());
                    if (auto const result = view->EnterHitTestScope(key, childParameter))
                    {
                        return result;
                    }
                }
            }

            try
            {
                auto scope = HitTestScope({}, *this, parameter);
                HitTest(scope);

                if (scope.GetHit({}))
                {
                    return GetSelf();
                }
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
    return {};
}

///
/// @brief 
///
/// @param parameter 
///
/// @return 
///
auto View::EnterPointerScope(PassKey<PointerScope> key, PointerParameter const& parameter) -> Shared<View>
{
    try
    {
        auto const self = GetSelf();
        auto const target = parameter.GetTargetView({});
        auto const phaseFlags = parameter.GetPhaseFlags({});

        if (self != target && !IsAncestorOf(target))
        {
            return {};
        }

        auto dispatch = [&](View& view, PointerParameter const& parameter, Event<PointerEvent> pointerEvent) {
            auto const offset = view.GetFrameRect().GetPosition().As<Offset>();
            pointerEvent->SetPosition(pointerEvent->GetPosition() - offset);
            auto pointerParameter = parameter;
            pointerParameter.SetPointerEvent(pointerEvent);
            return view.EnterPointerScope(key, pointerParameter);
        };

        if (target == self)
        {
            if (((phaseFlags & PointerPhaseFlags::Target) != PointerPhaseFlags::None))
            {
                auto scope = PointerScope({}, *this, parameter);
                Pointer(scope);

                if (scope.GetResult({}))
                {
                    return GetSelf();
                }
            }
        }
        else
        {
            if ((phaseFlags & PointerPhaseFlags::Capture) != PointerPhaseFlags::None)
            {
                auto scope = PointerScope({}, *this, parameter);
                PointerIntercept(scope);

                if (scope.GetResult({}))
                {
                    auto cancelEvent = Event<PointerEvent::Motion::Cancel>();
                    cancelEvent->SetPointerId(parameter.GetPointerEvent()->GetPointerId());
                    cancelEvent->SetPointerType(parameter.GetPointerEvent()->GetPointerType());
                    cancelEvent->SetTimestamp(parameter.GetPointerEvent()->GetTimestamp());
                    cancelEvent->SetPrimaryPointer(parameter.GetPointerEvent()->IsPrimaryPointer());
                    cancelEvent->SetPosition(parameter.GetPointerEvent()->GetPosition());
                    ForEachVisibleChild([&](auto& view) { dispatch(view, parameter, cancelEvent); });
                    return GetSelf();
                }
            }
        }

        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if (auto const view = *it)
            {
                if (auto const result = dispatch(*view, parameter, parameter.GetPointerEvent()))
                {
                    return result;
                }
            }
        }

        if (target != self)
        {
            if (((phaseFlags & PointerPhaseFlags::Bubble) != PointerPhaseFlags::None))
            {
                auto scope = PointerScope({}, *this, parameter);
                Pointer(scope);

                if (scope.GetResult({}))
                {
                    return GetSelf();
                }
            }
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
    return {};
}

///
/// @brief 
///
auto View::GetMeasuredSize(PassKey<MeasureScope>) const -> Size<Dp>
{
    return _layoutInfo.GetMeasuredSize();
}

///
/// @brief
///
auto View::GetMeasuredSize(PassKey<ArrangeScope>) const -> Size<Dp>
{
    return _layoutInfo.GetMeasuredSize();
}

///
/// @brief 
///
/// @param parameter 
/// @param size 
///
auto View::SetMeasuredSize(PassKey<MeasureScope>, MeasureParameter const& parameter, Size<Dp> const& size) -> void
{
    _layoutInfo.SetMeasuredSize(parameter, size);
}

///
/// @brief 
///
auto View::Initialize() -> void
{
}

///
/// @brief Measures size of the view.
///
/// First pass of layout update.
///
/// ### Contracts
///
/// Measure() determines size of the view under given constraints.
///
/// Derived classes must return result by calling MeasureScope::SetMeasuredSize() before exiting from this function.
/// It is also required to call MeasureScope.MeasureChild() to every child of the view.  
///
/// Measure() can be called multiple times during measuring pass.
/// Result of Measure() will be cached so that multiple calls of Measure() with exact same constraints are efficient.
///
/// Measure() will be called only on views which requires layout update.
/// Call InvalidateLayout() to request re-layout of a view in next update pass.
///
/// Result of Measure() must always satisfy constraints given by parent.
///
/// #### Mutating children during Measure()
///
/// Some layout algorithms require mutation of children during Measure(). 
/// Adding/Removing/Modifying child views during Measure() is only supported under specific conditions: 
///
/// 1. Only descendants of currently measured view can be added/removed/modified during Measure().
/// 2. All affected children must be properly measured to reflect latest state before exiting Measure().
///
/// Violating conditions above results in unspecified behavior, such as broken layouts or infinite re-layout loops.
///
/// @param scope Scope of this operation
///
auto View::Measure(MeasureScope& scope) -> void
{
    Size<Dp> maxSize;
    ForEachVisibleChild([&](View& view) { maxSize = Size<Dp>::Max(maxSize, scope.MeasureChild(view)); });

    auto const& parameter = scope.GetParameter();
    auto const width = AxisConstraints::Constrain(parameter.GetWidthConstraints(), maxSize.GetWidth());
    auto const height = AxisConstraints::Constrain(parameter.GetHeightConstraints(), maxSize.GetHeight());
    scope.SetMeasuredSize(width, height);
}

///
/// @brief Arranges positions of children.
///
/// Second pass of layout update.
///
/// Arrange() determines positions of child views.
/// It is required to call ArrangeScope.ArrangeChild() to every child of the view before exiting this function.
///
/// You can obtain previously measured size of child views by ArrangeScope.GetMeasuredSize() for layout computation.
///
/// @param scope Scope of the operation
///
auto View::Arrange(ArrangeScope& scope) -> void
{
    ForEachVisibleChild([&](View& view) {
        if (GetLayoutDirection() == LayoutDirection::LeftToRight)
        {
            scope.ArrangeChild(view, Point<Dp>());
        }
        else
        {
            auto const size = scope.GetMeasuredSize(view);
            auto const x = GetContentRect().GetWidth() - size.GetWidth();
            scope.ArrangeChild(view, Point<Dp>(x, 0));
        }
    });
}

///
/// @brief Draw content of the view.
///
/// Updating visual of the view.
///
/// Draw() determines how contents of the view will be rendered on screen.
/// Drawing commands will be cached and re-used until next call of InvalidateVisual().
///
/// You can also set properties of the layer on which drawing commands will be rendered via DrawScope.
/// Setting layer properties will be more efficient than achieving same effect with drawing commands.
///
/// @param scope Scope of the operation
///
auto View::Draw(DrawScope& scope) -> void
{
    (void)scope;
}

///
/// @brief Hit testing.
///
/// @param scope Scope of the operation
///
auto View::HitTest(HitTestScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();

    auto hitTestParameter = Event<HitTestEvent>::Make();
    hitTestParameter->SetPosition(parameter.GetPosition());
    hitTestParameter->SetHit(IsPointerInteractive());
    auto hitTestEvent = Event<>(hitTestParameter);
    if (SendEventDetached(hitTestEvent))
    {
        if (hitTestEvent.Is<HitTestEvent>())
        {
            hitTestParameter = hitTestEvent.As<HitTestEvent>();
        }
    }
    scope.SetHit(hitTestParameter->GetHit());
}

///
/// @brief Intercepts pointer events.
///
/// @param scope Scope of the operation
///
auto View::PointerIntercept(PointerScope& scope) -> void
{
    scope.SetResult(false);
}

///
/// @brief Dispatches pointer events. 
///
/// @param scope Scope of the operation
///
auto View::Pointer(PointerScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();
    auto const& eventParameter = parameter.GetPointerEvent();
    auto event = Event<>(eventParameter);
    scope.SetResult(SendEventDetached(event));
}

///
/// @brief Invalidate layout of the view
///
auto View::InvalidateLayout() -> void
{
    InvalidateLayoutPath();
}

///
/// @brief Invalidate visual of the view
///
auto View::InvalidateVisual() -> void
{
    _drawInfo.InvalidateLayer();
    InvalidateVisualPath();
}

///
/// @brief Returns true if the view is interactive on pointer events.
///
auto View::IsPointerInteractive() const -> Bool
{
    return _pointerTrackingFlags != ViewPointerTrackingFlags::None;
}

///
/// @brief Get tracking flags.
///
auto View::GetPointerTrackingFlags() const -> ViewPointerTrackingFlags
{
    return _pointerTrackingFlags;
}

///
/// @brief Set tracking flags.
///
auto View::SetPointerTrackingFlags(ViewPointerTrackingFlags const pointerTrackingFlags) -> void
{
    _pointerTrackingFlags = pointerTrackingFlags;
}

///
/// @brief Get tracking flags.
///
auto View::GetFocusTrackingFlags() const -> ViewFocusTrackingFlags
{
    return _focusTrackingFlags;
}

///
/// @brief Set tracking flags.
///
auto View::SetFocusTrackingFlags(ViewFocusTrackingFlags const focusTrackingFlags) -> void
{
    _focusTrackingFlags = focusTrackingFlags;
}

///
/// @brief Get number of child views.
///
auto View::GetChildCount() const -> SInt64
{
    return std::ssize(_children);
}

///
/// @brief Get index of child view.
///
auto View::GetChildIndex(ReferenceArg<View const> view) const -> Optional<SInt64>
{
    auto index = SInt64(0);
    for (auto const& child : _children)
    {
        if (view.GetPointer() == child.GetPointer())
        {
            return index;
        }
        ++index;
    }
    return {};
}

///
/// @brief Get child view at given index.
///
auto View::GetChildAt(SInt64 const index) -> Shared<View>
{
    if (0 <= index && index < std::ssize(_children))
    {
        return *std::next(_children.begin(), static_cast<int64_t>(index));
    }
    return {};
}

///
/// @brief Get child view at given index.
///
auto View::GetChildAt(SInt64 const index) const -> Shared<View const>
{
    if (0 <= index && index < std::ssize(_children))
    {
        return *std::next(_children.begin(), static_cast<int64_t>(index));
    }
    return {};
}

///
/// @brief Get list of child views.
///
auto View::GetChildren() -> ViewArray
{
    return ViewArray(_children.begin(), _children.end());
}

///
/// @brief Get list of child views.
///
auto View::GetChildren() const -> ConstViewArray
{
    return ConstViewArray(_children.begin(), _children.end());
}

///
/// @brief Add child view at specified index.
///
/// @param[in] view View to add.
/// @param[in] index Index of adding view.
///
/// @note When `view` is already added, this function fails.
///
auto View::AddChildAt(Shared<View> view, SInt64 const index) -> void
{
    Shared<View> self = GetSelf();

    if (!view || view == self)
    {
        return;
    }

    if (view->GetParent() == GetSelf())
    {
        return;
    }

    view->RemoveFromParent();

    if (0 <= index && index <= GetChildCount())
    {
        AdoptChild(view, index);
        view->Attach();

        InvalidateLayout();

        if (view->_drawInfo.IsInDrawPath())
        {
            InvalidateVisualPath();
        }
    }
}

///
/// @brief Add child view to back (topmost).
///
auto View::AddChildBack(Shared<View> view) -> void
{
    AddChildAt(view, GetChildCount());
}

///
/// @brief Add child view to front (bottommost).
///
auto View::AddChildFront(Shared<View> view) -> void
{
    AddChildAt(view, 0);
}

///
/// @brief Add user provided view layer.
///
/// @param kind 
///
auto View::AddLayer(ViewLayerKind const kind) -> ViewLayerId
{
    auto const id = _drawInfo.AddSubLayerInfo(kind);
    _drawInfo.AttachSubLayer(GetLayer(), *_layerManager, id);
    return id;
}

///
/// @brief Remove user provided view layer.
///
/// @param id 
///
auto View::RemoveLayer(ViewLayerId const id) -> void
{
    _drawInfo.DetachSubLayer(GetLayer(), id);
    _drawInfo.RemoveSubLayerInfo(id);
}

///
/// @brief Get ViewLayer instance assigned to the id.
///
/// @param id Id returned from previous call of AddLayer().
///
/// @note You should not cache the result of this function since views can destroy its layers to minimize resource usage.
///
auto View::GetLayer(ViewLayerId const id) -> Shared<ViewLayer>
{
    return _drawInfo.GetSubLayer(id);
}

///
/// @brief
///
auto View::NotifyRootChanged() -> void
{
    if (IsRoot())
    {
        NotifyAttachedChanged(IsAttached());
        NotifyActiveChanged(IsActive());
        NotifyDisplayScaleChanged(GetDisplayScale());
        NotifyBackingScaleChanged(GetBackingScale());
    }
}

///
/// @brief Start capturing pointer events.
///
/// @param id Id of pointer to capture.
///
auto View::CapturePointer(PointerId const id) -> void
{
    if (auto const root = GetRoot())
    {
        root->RootCapturePointer(id, GetSelf());
    }
}

///
/// @brief End capturing pointer events.
///
/// @param id  Id of pointer to release.
///
auto View::ReleasePointer(PointerId const id) -> void
{
    if (auto const root = GetRoot())
    {
        root->RootReleasePointer(id, GetSelf());
    }
}

///
/// @brief 
///
auto View::CancelInput() -> void
{
    if (auto const root = GetRoot())
    {
        root->RootCancelInput(GetSelf());
    }
}

///
/// @brief
///
auto View::RootIsActive() const -> Bool
{
    return false;
}

///
/// @brief
///
auto View::RootIsAttached() const -> Bool
{
    return false;
}

///
/// @brief
///
auto View::RootGetDisplayScale() const -> DisplayScale
{
    return DisplayScale(1.0);
}

///
/// @brief
///
auto View::RootGetBackingScale() const -> BackingScale
{
    return BackingScale(1.0);
}

///
/// @brief
///
auto View::RootGetAnimationTimer() -> AnimationTimer&
{
    return *_animationTimer;
}

///
/// @brief
///
auto View::RootGetAnimationTimer() const -> AnimationTimer const&
{
    return *_animationTimer;
}

///
/// @brief
///
auto View::RootGetFocusNode() -> FocusNode&
{
    return *_focusNode;
}

///
/// @brief
///
auto View::RootGetFocusNode() const -> FocusNode const&
{
    return *_focusNode;
}

///
/// @brief
///
auto View::RootGetAttributeNode() -> AttributeNode&
{
    return *_attributeNode;
}

///
/// @brief
///
auto View::RootGetAttributeNode() const -> AttributeNode const&
{
    return *_attributeNode;
}

///
/// @brief
///
auto View::RootGetLayer() -> ViewLayer&
{
    return *_layer;
}

///
/// @brief
///
auto View::RootGetLayer() const -> ViewLayer const&
{
    return *_layer;
}

///
/// @brief
///
auto View::RootInvalidateLayout() -> void
{
}

///
/// @brief
///
auto View::RootInvalidateVisual() -> void
{
}

///
/// @brief 
///
/// @param id 
/// @param view 
///
auto View::RootCapturePointer(PointerId const id, Shared<View> const& view) -> void
{
    (void)id;
    (void)view;
}

///
/// @brief 
///
/// @param id 
/// @param view 
///
auto View::RootReleasePointer(PointerId const id, Shared<View> const& view) -> void
{
    (void)id;
    (void)view;
}

///
/// @brief
///
auto View::RootCancelInput(Shared<View> const& view) -> void
{
    (void)view;
}

///
/// @brief
///
auto View::GetSelfBase() -> Shared<View>
{
    return _self.Lock();
}

///
/// @brief
///
auto View::GetSelfBase() const -> Shared<View const>
{
    return _self.Lock();
}

///
/// @brief
///
auto View::SetParent(Shared<View> const& parent) -> void
{
    auto const oldParent = _parent.Lock();
    if (oldParent != parent)
    {
        _parent = parent;

        if (parent)
        {
            auto const indexAfter = *parent->GetChildIndex(*this) + 1;
            auto const viewAfter = parent->GetChildAt(indexAfter);
            parent->GetAnimationTimer().AddChild(_animationTimer, viewAfter ? &viewAfter->GetAnimationTimer() : nullptr);
            parent->GetFocusNode().AddChild(_focusNode, viewAfter ? &viewAfter->GetFocusNode() : nullptr);
            parent->GetAttributeNode().AddChild(_attributeNode);
            parent->GetLayer().AddChild(_layer, viewAfter ? &viewAfter->GetLayer() : nullptr);
        }
        else
        {
            oldParent->GetAttributeNode().RemoveChild(_attributeNode);
            oldParent->GetFocusNode().RemoveChild(_focusNode);
            oldParent->GetAnimationTimer().RemoveChild(_animationTimer);
            oldParent->GetLayer().RemoveChild(_layer);
        }
    }
}

///
/// @brief
///
auto View::GetParent() -> Shared<View>
{
    return _parent.Lock();
}

///
/// @brief
///
auto View::GetParent() const -> Shared<View const>
{
    return _parent.Lock();
}

///
/// @brief
///
auto View::GetRoot() -> Shared<View>
{
    if (auto parent = GetParent())
    {
        return parent->GetRoot();
    }
    return GetSelf();
}

///
/// @brief
///
auto View::GetRoot() const -> Shared<View const>
{
    if (auto parent = GetParent())
    {
        return parent->GetRoot();
    }
    return GetSelf();
}

///
/// @brief
///
auto View::IsRoot() const -> Bool
{
    return _parent.IsExpired();
}

///
/// @brief
///
auto View::InitializeSelf(Shared<View> const& self) -> void
{
    _self = self;
    _eventReceiver = EventReceiver::Make({.dispatchEvent = [&](Event<>& event, EventFunction const& dispatch) -> Async<Bool> { co_return co_await DispatchEvent(event, dispatch); }});
    _propertyStore = PropertyStore::Make();
    _animationTimer = AnimationTimer::Make();
    _focusNode = FocusNode::Make();
    _attributeNode = AttributeNode::Make();
    _layerManager = Locator::Resolve<ViewLayerManager>();
    _layer = _layerManager->MakeLayer(ViewLayerKindNormal);

    EventReceiver::Connect(*_focusNode, *this, &View::SendEvent);
}

///
/// @brief
///
auto View::GetAnimationTimer() -> AnimationTimer&
{
    if (IsRoot())
    {
        return RootGetAnimationTimer();
    }
    return *_animationTimer;
}

///
/// @brief
///
auto View::GetAnimationTimer() const -> AnimationTimer const&
{
    if (IsRoot())
    {
        return RootGetAnimationTimer();
    }
    return *_animationTimer;
}

///
/// @brief
///
auto View::GetFocusNode() -> FocusNode&
{
    if (IsRoot())
    {
        return RootGetFocusNode();
    }
    return *_focusNode;
}

///
/// @brief
///
auto View::GetFocusNode() const -> FocusNode const&
{
    if (IsRoot())
    {
        return RootGetFocusNode();
    }
    return *_focusNode;
}

///
/// @brief
///
auto View::GetLayer() -> ViewLayer&
{
    if (IsRoot())
    {
        return RootGetLayer();
    }
    return *_layer;
}

///
/// @brief
///
auto View::GetLayer() const -> ViewLayer const&
{
    if (IsRoot())
    {
        return RootGetLayer();
    }
    return *_layer;
}

///
/// @brief
///
/// @param child
/// @param index
///
auto View::AdoptChild(Shared<View> const& child, SInt64 const index) -> void
{
    auto const it = std::next(_children.begin(), static_cast<int64_t>(index));
    _children.insert(it, child);
    child->SetParent(GetSelf());
}

///
/// @brief
///
/// @param child
///
auto View::AbandonChild(Shared<View> const& child) -> void
{
    auto const it = std::find(_children.begin(), _children.end(), child);
    FW_DEBUG_ASSERT(it != _children.end());
    _children.erase(it);
    child->SetParent(nullptr);
}

///
/// @brief
///
/// @param event
/// @param dispatch
///
auto View::DispatchEvent(Event<>& event, EventFunction const& dispatch) -> Async<Bool>
{
    if (event.Is<ViewEvent::Notify>())
    {
        co_return co_await DispatchNotifyEvent(event, dispatch);
    }
    else if (event.Is<ViewEventNotifyBubble>())
    {
        co_return co_await DispatchNotifyBubbleEvent(event, dispatch);
    }
    co_return co_await dispatch(event);
}

///
/// @brief
///
/// @param event
/// @param dispatch
///
auto View::DispatchNotifyEvent(Event<>& event, EventFunction const& dispatch) -> Async<Bool>
{
    try
    {
        auto parameter = event.As<ViewEvent::Notify>();
        parameter->SetSenderView(GetSelf());
        auto notifyEvent = Event<>(parameter);
        if (co_await dispatch(notifyEvent))
        {
            event = notifyEvent;
            co_return true;
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }

    if (auto parent = GetParent())
    {
        auto bubbleEvent = Event<>(Event<ViewEventNotifyBubble>::Make(event));
        co_return co_await parent->SendEvent(bubbleEvent);
    }
    co_return false;
}

///
/// @brief
///
/// @param event
/// @param dispatch
///
auto View::DispatchNotifyBubbleEvent(Event<>& event, EventFunction const& dispatch) -> Async<Bool>
{
    try
    {
        if (co_await dispatch(event.As<ViewEventNotifyBubble>()->GetEvent()))
        {
            co_return true;
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }

    if (auto parent = GetParent())
    {
        co_return co_await parent->SendEvent(event);
    }
    co_return false;
}

///
/// @brief
///
auto View::Attach() -> void
{
    auto const parent = IsRoot() ? GetSelf() : GetParent();
    NotifyAttachedChanged(parent->IsAttached());
    NotifyActiveChanged(parent->IsActive());
    NotifyDisplayScaleChanged(parent->GetDisplayScale());
    NotifyBackingScaleChanged(parent->GetBackingScale());
    NotifyVisibleChanged(parent->IsVisibleFromRoot());
    NotifyEnabledChanged(parent->IsEnabledFromRoot());
    NotifyLayoutDirectionChanged(parent->GetLayoutDirection());
}

///
/// @brief
///
auto View::Detach() -> void
{
    NotifyAttachedChanged(IsAttached());
    NotifyActiveChanged(IsActive());
    NotifyDisplayScaleChanged(GetDisplayScale());
    NotifyBackingScaleChanged(GetBackingScale());
    NotifyVisibleChanged(IsVisible());
    NotifyEnabledChanged(IsEnabled());
    NotifyLayoutDirectionChanged(GetLayoutDirection());
}

///
/// @brief
///
auto View::InvalidateLayoutPath() -> void
{
    if (_layoutInfo.InvalidatePath())
    {
        if (auto parent = GetParent())
        {
            parent->InvalidateLayoutPath();
        }
        else
        {
            RootInvalidateLayout();
        }
    }
}

///
/// @brief
///
auto View::InvalidateVisualPath() -> void
{
    if (_drawInfo.InvalidatePath())
    {
        if (auto parent = GetParent())
        {
            parent->InvalidateVisualPath();
        }
        else
        {
            RootInvalidateVisual();
        }
    }
}

///
/// @brief
///
auto View::NotifyActiveChanged(Bool const active) -> void
{
    try
    {
        if (_active != active)
        {
            _active = active;

            ForEachChild([&](View& view) { view.NotifyActiveChanged(active); });

            auto event = Event<>(Event<ViewEvent::ActiveChanged>());
            SendEventDetached(event);
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief
///
auto View::NotifyAttachedChanged(Bool const attached) -> void
{
    try
    {
        if (_attached != attached)
        {
            _attached = attached;

            if (_attached)
            {
                _drawInfo.AttachAllSubLayers(GetLayer(), *_layerManager);
            }
            else
            {
                _drawInfo.DetachAllSubLayers(GetLayer());
            }

            ForEachChild([&](View& view) { view.NotifyAttachedChanged(attached); });

            if (attached)
            {
                auto event = Event<>(Event<ViewEvent::Attached>());
                SendEventDetached(event);
            }
            else
            {
                auto event = Event<>(Event<ViewEvent::Detached>());
                SendEventDetached(event);
            }
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief
///
auto View::NotifyVisibleChanged(Bool const visible) -> void
{
    try
    {
        if (_visible)
        {
            if (_visibleFromRoot != visible)
            {
                _visibleFromRoot = visible;

                ForEachChild([&](View& view) { view.NotifyVisibleChanged(visible); });

                try
                {
                    auto event = Event<>(Event<ViewEvent::VisibleChanged>());
                    SendEventDetached(event);
                }
                catch (...)
                {
                    FW_DEBUG_ASSERT(false);
                }
            }
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief
///
auto View::NotifyEnabledChanged(Bool const enabled) -> void
{
    try
    {
        if (_enabled)
        {
            if (_enabledFromRoot != enabled)
            {
                _enabledFromRoot = enabled;

                ForEachChild([&](View& view) { view.NotifyEnabledChanged(enabled); });

                try
                {
                    auto event = Event<>(Event<ViewEvent::EnabledChanged>());
                    SendEventDetached(event);
                }
                catch (...)
                {
                    FW_DEBUG_ASSERT(false);
                }
            }
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief
///
auto View::NotifyDisplayScaleChanged(DisplayScale const displayScale) -> void
{
    try
    {
        if (_displayScale != displayScale)
        {
            _displayScale = displayScale;

            // Need to update pixel alignment.
            InvalidateLayout();

            ForEachChild([&](View& view) { view.NotifyDisplayScaleChanged(displayScale); });

            try
            {
                auto event = Event<>(Event<ViewEvent::DisplayScaleChanged>());
                SendEventDetached(event);
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief
///
auto View::NotifyBackingScaleChanged(BackingScale const backingScale) -> void
{
    try
    {
        if (_backingScale != backingScale)
        {
            _backingScale = backingScale;

            ForEachChild([&](View& view) { view.NotifyBackingScaleChanged(backingScale); });

            try
            {
                auto event = Event<>(Event<ViewEvent::BackingScaleChanged>());
                SendEventDetached(event);
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief
///
auto View::NotifyLayoutDirectionChanged(LayoutDirection const layoutDirection) -> void
{
    try
    {
        auto changed = Bool(false);
        auto const rawLayoutDirection = _layoutInfo.GetRawLayoutDirection();
        if (rawLayoutDirection == ViewLayoutDirection::Inherited)
        {
            if (_layoutInfo.GetLayoutDirection() != layoutDirection)
            {
                _layoutInfo.SetLayoutDirection(layoutDirection);
                changed = true;
            }
        }
        else if (rawLayoutDirection == ViewLayoutDirection::LeftToRight)
        {
            if (_layoutInfo.GetLayoutDirection() != LayoutDirection::LeftToRight)
            {
                _layoutInfo.SetLayoutDirection(LayoutDirection::LeftToRight);
                changed = true;
            }
        }
        else if (rawLayoutDirection == ViewLayoutDirection::RightToLeft)
        {
            if (_layoutInfo.GetLayoutDirection() != LayoutDirection::RightToLeft)
            {
                _layoutInfo.SetLayoutDirection(LayoutDirection::RightToLeft);
                changed = true;
            }
        }
        else
        {
            FW_DEBUG_ASSERT(false);
        }

        if (changed)
        {
            ForEachChild([&](View& view) { view.NotifyLayoutDirectionChanged(_layoutInfo.GetLayoutDirection()); });

            try
            {
                auto event = Event<>(Event<ViewEvent::LayoutDirectionChanged>());
                SendEventDetached(event);
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}
}
