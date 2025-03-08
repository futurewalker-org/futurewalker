// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Animation.AnimationTimerType.hpp"

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
class AnimationTimer : NonCopyable
{
public:
    static auto Make() -> Shared<AnimationTimer>;

public:
    AnimationTimer(PassKey<AnimationTimer>);
    virtual ~AnimationTimer();

    auto AddChild(Shared<AnimationTimer> timer, Pointer<AnimationTimer const> after) -> void;
    auto RemoveChild(Shared<AnimationTimer> timer) -> void;

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
    auto NotifyRootCurrentTimeChanged() -> void;

    template <class Self>
    auto GetSelf(this Self& self) -> Shared<Self>;

    template <Concepts::DerivedFrom<AnimationTimer> Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

private:
    auto GetSelfBase() -> Shared<AnimationTimer>;
    auto GetSelfBase() const -> Shared<AnimationTimer const>;
    auto SetSelfBase(Shared<AnimationTimer> const& self) -> void;
    auto GetParent() -> Shared<AnimationTimer>;
    auto GetParent() const -> Shared<AnimationTimer const>;
    auto SetParent(Shared<AnimationTimer> const& parent) -> void;
    auto IsRoot() const -> Bool;

    auto AdoptChild(Shared<AnimationTimer> const& child, Pointer<AnimationTimer const> after) -> void;
    auto AbandonChild(Shared<AnimationTimer> const& child) -> void;

    auto RemoveFromParent() -> void;

    auto SendEvent(Event& event) -> Bool;
    auto ReceiveEvent(Event& event) -> Bool;

    auto SendDependencyNodeTickEvent(MonotonicTime const time) -> void;
    auto SendDependencyNodeActiveChangedEvent(Bool const active) -> void;
    auto SendDependencyNodeEnabledChangedEvent(Bool const enabled) -> void;

    auto SendAnimationTimerTickEvent(MonotonicTime const time) -> void;
    auto SendAnimationTimerActiveChangedEvent() -> void;
    auto SendAnimationTimerEnabledChangedEvent() -> void;

private:
    virtual auto RootIsActive() const -> Bool;
    virtual auto RootGetCurrentTime() const -> MonotonicTime;
    virtual auto RootRequestTick() -> void;

private:
    Weak<AnimationTimer> _self;
    Weak<AnimationTimer> _parent;
    AnimationTimerList _children;
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
auto AnimationTimer::GetSelf(this Self& self) -> Shared<Self>
{
    return static_cast<TypeTraits::PropagateCVRef<Self&, AnimationTimer>>(self).GetSelfBase().template Assume<Self>();
}

///
/// @brief
///
template <Concepts::DerivedFrom<AnimationTimer> Derived, class... Args>
auto AnimationTimer::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto derived = Shared<Derived>::Make(PassKey<AnimationTimer>(), std::forward<Args>(args)...);
    static_cast<AnimationTimer&>(*derived).SetSelfBase(derived);
    return derived;
}
}
}
