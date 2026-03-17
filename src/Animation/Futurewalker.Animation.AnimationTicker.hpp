// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Animation.AnimationTickerType.hpp"
#include "Futurewalker.Animation.AnimationTickerEvent.hpp"

#include "Futurewalker.Dependency.DependencyNodeType.hpp"

#include "Futurewalker.Event.EventType.hpp"
#include "Futurewalker.Event.EventReceiver.hpp" 

#include "Futurewalker.Signal.Tracker.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.MonotonicTime.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.Pointer.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Animation timer node.
///
/// Dispatches tick notification from ticker source to listeners.
///
class AnimationTicker : NonCopyable
{
public:
    static auto Make() -> Shared<AnimationTicker>;

public:
    AnimationTicker(PassKey<AnimationTicker>);
    virtual ~AnimationTicker();

    auto AddChild(Shared<AnimationTicker> timer, Pointer<AnimationTicker const> after) -> void;
    auto RemoveChild(Shared<AnimationTicker> timer) -> void;

    auto IsActive() const -> Bool;

    auto IsEnabled() const -> Bool;
    auto IsEnabledFromRoot() const -> Bool;
    auto SetEnabled(Bool const enabled) -> void;

    auto RequestTick() -> void;

    auto GetCurrentTime() const -> MonotonicTime;

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

protected:
    auto NotifyRootActiveChanged() -> void;
    auto NotifyRootTickTimeChanged(MonotonicTime const tickTime) -> void;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <Concepts::DerivedFrom<AnimationTicker> Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

private:
    auto GetSelfBase() -> Shared<AnimationTicker>;
    auto GetSelfBase() const -> Shared<AnimationTicker const>;
    auto SetSelfBase(Shared<AnimationTicker> const& self) -> void;
    auto GetParent() -> Shared<AnimationTicker>;
    auto GetParent() const -> Shared<AnimationTicker const>;
    auto SetParent(Shared<AnimationTicker> const& parent) -> void;
    auto IsRoot() const -> Bool;

    auto AdoptChild(Shared<AnimationTicker> const& child, Pointer<AnimationTicker const> after) -> void;
    auto AbandonChild(Shared<AnimationTicker> const& child) -> void;

    auto RemoveFromParent() -> void;

    auto SendEvent(Event<>& event) -> Bool;
    auto ReceiveEvent(Event<>& event) -> Bool;

    auto SendDependencyNodeTickEvent(MonotonicTime const time) -> void;
    auto SendDependencyNodeActiveChangedEvent(Bool const active) -> void;
    auto SendDependencyNodeEnabledChangedEvent(Bool const enabled) -> void;

    auto SendAnimationTickerTickEvent(MonotonicTime const time) -> void;
    auto SendAnimationTickerActiveChangedEvent() -> void;
    auto SendAnimationTickerEnabledChangedEvent() -> void;

private:
    virtual auto RootIsActive() const -> Bool;
    virtual auto RootGetCurrentTime() const -> MonotonicTime;
    virtual auto RootRequestTick() -> void;

private:
    Weak<AnimationTicker> _self;
    Weak<AnimationTicker> _parent;
    AnimationTickerList _children;
    Shared<EventReceiver> _eventReceiver;
    Shared<DependencyNode> _dependencyNode;
    Bool _active = false;
    Bool _enabled = true;
    Bool _enabledFromRoot = true;
    Bool _tickRequested = false;
};

///
/// @brief
///
template <class Self>
auto AnimationTicker::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, AnimationTicker>>(self).GetSelfBase().template UnsafeAs<Self>();
}

///
/// @brief
///
template <Concepts::DerivedFrom<AnimationTicker> Derived, class... Args>
auto AnimationTicker::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto derived = Shared<Derived>::Make(PassKey<AnimationTicker>(), std::forward<Args>(args)...);
    static_cast<AnimationTicker&>(*derived).SetSelfBase(derived);
    return derived;
}
}
}
