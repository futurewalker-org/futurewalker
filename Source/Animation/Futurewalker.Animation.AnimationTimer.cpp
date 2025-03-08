// SPDX-License-Identifier: MIT

#include "Futurewalker.Animation.AnimationTimer.hpp"
#include "Futurewalker.Animation.AnimationTimerEvent.hpp"

#include "Futurewalker.Dependency.DependencyNode.hpp"
#include "Futurewalker.Dependency.DependencyNodeEvent.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
namespace
{
class DependencyNodeNotifyTick final : public EventParameter
{
public:
    MonotonicTime time;
};

class DependencyNodeNotifyActiveChanged final : public EventParameter
{
public:
    Bool active = false;
};

class DependencyNodeNotifyEnabledChanged final : public EventParameter
{
public:
    Bool enabled = false;
};
}

///
/// @brief
///
auto AnimationTimer::Make() -> Shared<AnimationTimer>
{
    return MakeDerived<AnimationTimer>();
}

///
/// @brief
///
AnimationTimer::AnimationTimer(PassKey<AnimationTimer>)
{
    _eventReceiver = EventReceiver::Make();
    _dependencyNode = DependencyNode::Make({.dispatchEvent = [&](auto& e) { return ReceiveEvent(e); }});
}

///
/// @brief
///
AnimationTimer::~AnimationTimer()
{
}

///
/// @brief
///
/// @param[in] child A child timer to add.
///
auto AnimationTimer::AddChild(Shared<AnimationTimer> timer, Pointer<AnimationTimer const> after) -> void
{
    if (timer)
    {
        timer->RemoveFromParent();

        AdoptChild(timer, after);

        if (timer->_tickRequested)
        {
            RequestTick();
        }
        timer->SendDependencyNodeEnabledChangedEvent(IsEnabledFromRoot());
        timer->SendDependencyNodeActiveChangedEvent(IsActive());
    }
}

///
/// @brief 
///
/// @param timer 
///
auto AnimationTimer::RemoveChild(Shared<AnimationTimer> timer) -> void
{
    if (!timer)
    {
        return;
    }

    for (auto const& child : _children)
    {
        if (timer == child)
        {
            child->RemoveFromParent();
            return;
        }
    }
}

///
/// @brief
///
auto AnimationTimer::IsActive() const -> Bool
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
auto AnimationTimer::IsEnabled() const -> Bool
{
    return _enabled;
}

///
/// @brief
///
auto AnimationTimer::IsEnabledFromRoot() const -> Bool
{
    return _enabledFromRoot;
}

///
/// @brief
///
auto AnimationTimer::SetEnabled(Bool const enabled) -> void
{
    if (_enabled != enabled)
    {
        _enabled = enabled;

        auto enabledFromRoot = enabled;

        if (enabledFromRoot)
        {
            if (auto const parent = GetParent())
            {
                enabledFromRoot = parent->_enabledFromRoot;
            }
        }

        if (_enabledFromRoot != enabledFromRoot)
        {
            _enabledFromRoot = enabledFromRoot;

            SendAnimationTimerEnabledChangedEvent();

            for (auto const& child : _children)
            {
                child->SendDependencyNodeEnabledChangedEvent(enabledFromRoot);
            }
        }
    }
}

///
/// @brief
///
auto AnimationTimer::RequestTick() -> void
{
    if (!_tickRequested)
    {
        _tickRequested = true;

        if (auto parent = GetParent())
        {
            return parent->RequestTick();
        }
        RootRequestTick();
    }
}

///
/// @brief
///
auto AnimationTimer::GetCurrentTime() const -> MonotonicTime
{
    if (auto parent = GetParent())
    {
        return parent->GetCurrentTime();
    }
    return RootGetCurrentTime();
}

///
/// @brief
///
auto AnimationTimer::NotifyRootActiveChanged() -> void
{
    if (IsRoot())
    {
        SendDependencyNodeActiveChangedEvent(IsActive());
    }
}

///
/// @brief
///
auto AnimationTimer::NotifyRootCurrentTimeChanged() -> void
{
    if (IsRoot())
    {
        SendDependencyNodeTickEvent(GetCurrentTime());
    }
}

///
/// @brief
///
auto AnimationTimer::GetSelfBase() -> Shared<AnimationTimer>
{
    return _self.Lock();
}

///
/// @brief
///
auto AnimationTimer::GetSelfBase() const -> Shared<AnimationTimer const>
{
    return _self.Lock();
}

///
/// @brief
///
/// @param self
///
auto AnimationTimer::SetSelfBase(Shared<AnimationTimer> const& self) -> void
{
    _self = self;
}

///
/// @brief
///
auto AnimationTimer::GetParent() -> Shared<AnimationTimer>
{
    return _parent.Lock();
}

///
/// @brief
///
auto AnimationTimer::GetParent() const -> Shared<AnimationTimer const>
{
    return _parent.Lock();
}

///
/// @brief
///
auto AnimationTimer::IsRoot() const -> Bool
{
    return _parent.IsExpired();
}

///
/// @brief
///
/// @param parent
///
auto AnimationTimer::SetParent(Shared<AnimationTimer> const& parent) -> void
{
    _parent = parent;
}

///
/// @brief
///
/// @param child
///
auto AnimationTimer::AdoptChild(Shared<AnimationTimer> const& child, Pointer<AnimationTimer const> after) -> void
{
    child->SetParent(GetSelf());
    _children.insert(std::find_if(_children.begin(), _children.end(), [&](auto const& child) { return child.GetPointer() == after; }), child);
    _dependencyNode->AddChild(child->_dependencyNode, after ? after->_dependencyNode.GetPointer() : nullptr);
}

///
/// @brief
///
/// @param child
///
auto AnimationTimer::AbandonChild(Shared<AnimationTimer> const& child) -> void
{
    _dependencyNode->RemoveChild(child->_dependencyNode);
    _children.erase(std::find(_children.begin(), _children.end(), child));
    child->SetParent(nullptr);
}

///
/// @brief
///
auto AnimationTimer::RemoveFromParent() -> void
{
    if (auto parent = GetParent())
    {
        parent->AbandonChild(GetSelf());
        SendDependencyNodeActiveChangedEvent(IsActive());
        SendDependencyNodeEnabledChangedEvent(IsEnabled());
    }
}

///
/// @brief
///
auto AnimationTimer::SendEvent(Event& event) -> Bool
{
    return GetEventReceiver().SendEventDetached(event);
}

///
/// @brief
///
/// @param event
///
auto AnimationTimer::ReceiveEvent(Event& event) -> Bool
{
    if (event.Is<DependencyNodeEvent::Notify>())
    {
        auto const& notifyEvent = event.As<DependencyNodeEvent::Notify>().GetEvent();

        if (notifyEvent.Is<DependencyNodeNotifyTick>())
        {
            if (IsActive() && IsEnabledFromRoot())
            {
                if (_tickRequested)
                {
                    _tickRequested = false;
                    SendAnimationTimerTickEvent(notifyEvent.As<DependencyNodeNotifyTick>().time);
                    return false;
                }
            }
            return true;
        }
        else if (notifyEvent.Is<DependencyNodeNotifyActiveChanged>())
        {
            auto const active = notifyEvent.As<DependencyNodeNotifyActiveChanged>().active;
            if (_active != active)
            {
                _active = active;
                SendAnimationTimerActiveChangedEvent();
            }
            return false;
        }
        else if (notifyEvent.Is<DependencyNodeNotifyEnabledChanged>())
        {
            auto const enabled = notifyEvent.As<DependencyNodeNotifyEnabledChanged>().enabled;
            if (_enabled && _enabledFromRoot != enabled)
            {
                _enabledFromRoot = enabled;
                SendAnimationTimerEnabledChangedEvent();
            }
            return !_enabled;
        }
    }
    return false;
}

///
/// @brief
///
auto AnimationTimer::GetTracker() -> Tracker&
{
    return _eventReceiver->GetTracker();
}

///
/// @brief
///
auto AnimationTimer::GetTracker() const -> Tracker const&
{
    return _eventReceiver->GetTracker();
}

///
/// @brief
///
auto AnimationTimer::GetEventReceiver() -> EventReceiver&
{
    return _eventReceiver->GetEventReceiver();
}

///
/// @brief
///
auto AnimationTimer::GetEventReceiver() const -> EventReceiver const&
{
    return _eventReceiver->GetEventReceiver();
}

///
/// @brief
///
auto AnimationTimer::SendDependencyNodeTickEvent(MonotonicTime const time) -> void
{
    auto parameter = DependencyNodeNotifyTick();
    parameter.time = time;
    _dependencyNode->NotifyWithEvent(Event(parameter));
}

///
/// @brief
///
auto AnimationTimer::SendDependencyNodeActiveChangedEvent(Bool const active) -> void
{
    auto parameter = DependencyNodeNotifyActiveChanged();
    parameter.active = active;
    auto event = Event(parameter);
    _dependencyNode->NotifyWithEvent(event);
}

///
/// @brief
///
auto AnimationTimer::SendDependencyNodeEnabledChangedEvent(Bool const enabled) -> void
{
    auto parameter = DependencyNodeNotifyEnabledChanged();
    parameter.enabled = enabled;
    auto event = Event(parameter);
    _dependencyNode->NotifyWithEvent(event);
}

///
/// @brief
///
auto AnimationTimer::SendAnimationTimerTickEvent(MonotonicTime const time) -> void
{
    auto parameter = AnimationTimerEvent::Tick();
    parameter.SetTime(time);
    auto timerEvent = Event(parameter);
    SendEvent(timerEvent);
}

///
/// @brief
///
auto AnimationTimer::SendAnimationTimerActiveChangedEvent() -> void
{
    auto event = Event(AnimationTimerEvent::ActiveChanged());
    SendEvent(event);
}

///
/// @brief
///
auto AnimationTimer::SendAnimationTimerEnabledChangedEvent() -> void
{
    auto event = Event(AnimationTimerEvent::EnabledChanged());
    SendEvent(event);
}

///
/// @brief
///
auto AnimationTimer::RootIsActive() const -> Bool
{
    return false;
}

///
/// @brief
///
auto AnimationTimer::RootGetCurrentTime() const -> MonotonicTime
{
    return {};
}

///
/// @brief
///
auto AnimationTimer::RootRequestTick() -> void
{
}
}
