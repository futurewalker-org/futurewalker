// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Animation.AnimationTicker.hpp"
#include "Futurewalker.Animation.AnimationTickerEvent.hpp"

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
auto AnimationTicker::Make() -> Shared<AnimationTicker>
{
    return MakeDerived<AnimationTicker>();
}

///
/// @brief
///
AnimationTicker::AnimationTicker(PassKey<AnimationTicker>)
{
    _eventReceiver = EventReceiver::Make();
    _dependencyNode = DependencyNode::Make({.dispatchEvent = [&](auto& e) { return ReceiveEvent(e); }});
}

///
/// @brief
///
AnimationTicker::~AnimationTicker()
{
}

///
/// @brief
///
/// @param[in] child A child timer to add.
///
auto AnimationTicker::AddChild(Shared<AnimationTicker> timer, Pointer<AnimationTicker const> after) -> void
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
auto AnimationTicker::RemoveChild(Shared<AnimationTicker> timer) -> void
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
auto AnimationTicker::IsActive() const -> Bool
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
auto AnimationTicker::IsEnabled() const -> Bool
{
    return _enabled;
}

///
/// @brief
///
auto AnimationTicker::IsEnabledFromRoot() const -> Bool
{
    return _enabledFromRoot;
}

///
/// @brief
///
auto AnimationTicker::SetEnabled(Bool const enabled) -> void
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

            SendAnimationTickerEnabledChangedEvent();

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
auto AnimationTicker::RequestTick() -> void
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
/// @brief Get current tick time.
///
/// @note The time returned by this function may be more recent than the last tick notification.
///
auto AnimationTicker::GetCurrentTime() const -> MonotonicTime
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
auto AnimationTicker::NotifyRootActiveChanged() -> void
{
    if (IsRoot())
    {
        SendDependencyNodeActiveChangedEvent(IsActive());
    }
}

///
/// @brief
///
auto AnimationTicker::NotifyRootTickTimeChanged(MonotonicTime const tickTime) -> void
{
    if (IsRoot())
    {
        SendDependencyNodeTickEvent(tickTime);
    }
}

///
/// @brief
///
auto AnimationTicker::GetSelfBase() -> Shared<AnimationTicker>
{
    return _self.Lock();
}

///
/// @brief
///
auto AnimationTicker::GetSelfBase() const -> Shared<AnimationTicker const>
{
    return _self.Lock();
}

///
/// @brief
///
/// @param self
///
auto AnimationTicker::SetSelfBase(Shared<AnimationTicker> const& self) -> void
{
    _self = self;
}

///
/// @brief
///
auto AnimationTicker::GetParent() -> Shared<AnimationTicker>
{
    return _parent.Lock();
}

///
/// @brief
///
auto AnimationTicker::GetParent() const -> Shared<AnimationTicker const>
{
    return _parent.Lock();
}

///
/// @brief
///
auto AnimationTicker::IsRoot() const -> Bool
{
    return _parent.IsExpired();
}

///
/// @brief
///
/// @param parent
///
auto AnimationTicker::SetParent(Shared<AnimationTicker> const& parent) -> void
{
    _parent = parent;
}

///
/// @brief
///
/// @param child
///
auto AnimationTicker::AdoptChild(Shared<AnimationTicker> const& child, Pointer<AnimationTicker const> after) -> void
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
auto AnimationTicker::AbandonChild(Shared<AnimationTicker> const& child) -> void
{
    _dependencyNode->RemoveChild(child->_dependencyNode);
    _children.erase(std::find(_children.begin(), _children.end(), child));
    child->SetParent(nullptr);
}

///
/// @brief
///
auto AnimationTicker::RemoveFromParent() -> void
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
auto AnimationTicker::SendEvent(Event<>& event) -> Bool
{
    return GetEventReceiver().SendEventDetached(event);
}

///
/// @brief
///
/// @param event
///
auto AnimationTicker::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<DependencyNodeEvent::Notify>())
    {
        auto const notifyEvent = event.As<DependencyNodeEvent::Notify>()->GetEvent();

        if (notifyEvent.Is<DependencyNodeNotifyTick>())
        {
            if (IsActive() && IsEnabledFromRoot())
            {
                if (_tickRequested)
                {
                    _tickRequested = false;
                    SendAnimationTickerTickEvent(notifyEvent.As<DependencyNodeNotifyTick>()->time);
                    return false;
                }
            }
            return true;
        }
        else if (notifyEvent.Is<DependencyNodeNotifyActiveChanged>())
        {
            auto const active = notifyEvent.As<DependencyNodeNotifyActiveChanged>()->active;
            if (_active != active)
            {
                _active = active;
                SendAnimationTickerActiveChangedEvent();
            }
            return false;
        }
        else if (notifyEvent.Is<DependencyNodeNotifyEnabledChanged>())
        {
            auto const enabled = notifyEvent.As<DependencyNodeNotifyEnabledChanged>()->enabled;
            if (_enabled && _enabledFromRoot != enabled)
            {
                _enabledFromRoot = enabled;
                SendAnimationTickerEnabledChangedEvent();
            }
            return !_enabled;
        }
    }
    return false;
}

///
/// @brief
///
auto AnimationTicker::GetTracker() -> Tracker&
{
    return _eventReceiver->GetTracker();
}

///
/// @brief
///
auto AnimationTicker::GetTracker() const -> Tracker const&
{
    return _eventReceiver->GetTracker();
}

///
/// @brief
///
auto AnimationTicker::GetEventReceiver() -> EventReceiver&
{
    return _eventReceiver->GetEventReceiver();
}

///
/// @brief
///
auto AnimationTicker::GetEventReceiver() const -> EventReceiver const&
{
    return _eventReceiver->GetEventReceiver();
}

///
/// @brief
///
auto AnimationTicker::SendDependencyNodeTickEvent(MonotonicTime const time) -> void
{
    auto parameter = Event<DependencyNodeNotifyTick>();
    parameter->time = time;
    _dependencyNode->NotifyWithEvent(parameter);
}

///
/// @brief
///
auto AnimationTicker::SendDependencyNodeActiveChangedEvent(Bool const active) -> void
{
    auto parameter = Event<DependencyNodeNotifyActiveChanged>();
    parameter->active = active;
    _dependencyNode->NotifyWithEvent(parameter);
}

///
/// @brief
///
auto AnimationTicker::SendDependencyNodeEnabledChangedEvent(Bool const enabled) -> void
{
    auto parameter = Event<DependencyNodeNotifyEnabledChanged>();
    parameter->enabled = enabled;
    _dependencyNode->NotifyWithEvent(parameter);
}

///
/// @brief
///
auto AnimationTicker::SendAnimationTickerTickEvent(MonotonicTime const time) -> void
{
    auto parameter = Event<AnimationTickerEvent::Tick>();
    parameter->SetTime(time);
    auto timerEvent = Event<>(parameter);
    SendEvent(timerEvent);
}

///
/// @brief
///
auto AnimationTicker::SendAnimationTickerActiveChangedEvent() -> void
{
    auto event = Event<>(Event<AnimationTickerEvent::ActiveChanged>());
    SendEvent(event);
}

///
/// @brief
///
auto AnimationTicker::SendAnimationTickerEnabledChangedEvent() -> void
{
    auto event = Event<>(Event<AnimationTickerEvent::EnabledChanged>());
    SendEvent(event);
}

///
/// @brief
///
auto AnimationTicker::RootIsActive() const -> Bool
{
    return false;
}

///
/// @brief
///
auto AnimationTicker::RootGetCurrentTime() const -> MonotonicTime
{
    return {};
}

///
/// @brief
///
auto AnimationTicker::RootRequestTick() -> void
{
}
}
