// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Animation.AnimationTicker.hpp"
#include "Futurewalker.Animation.AnimationTickerEvent.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
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
}

///
/// @brief
///
AnimationTicker::~AnimationTicker() = default;

///
/// @brief
///
/// @param[in] child A child timer to add.
/// @param[in] position Position to add new child.
///
auto AnimationTicker::AddChild(Shared<AnimationTicker> timer, Pointer<AnimationTicker const> position) -> void
{
    if (timer)
    {
        timer->RemoveFromParent();

        AdoptChild(timer, position);

        if (timer->_tickRequested)
        {
            RequestTick();
        }
        timer->NotifyEnabledChanged(IsEnabledFromRoot());
        timer->NotifyActiveChanged(IsActive());
    }
}

///
/// @brief 
///
/// @param timer 
///
auto AnimationTicker::RemoveChild(Shared<AnimationTicker> timer) -> void
{
    if (timer && timer->GetParent() == GetSelf())
    {
        timer->RemoveFromParent();
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

            for (auto const& child : _children)
            {
                child->NotifyEnabledChanged(enabledFromRoot);
            }

            SendAnimationTickerEnabledChangedEvent();
        }

        if (_enabled && _tickRequested)
        {
            if (auto const parent = GetParent())
            {
                parent->RequestTick();
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
        NotifyActiveChanged(IsActive());
    }
}

///
/// @brief
///
auto AnimationTicker::NotifyRootTickTimeChanged(MonotonicTime const tickTime) -> void
{
    if (IsRoot())
    {
        NotifyTick(tickTime);
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
/// @param[in] child New child node.
/// @param[in] position Position to add new child.
///
auto AnimationTicker::AdoptChild(Shared<AnimationTicker> const& child, Pointer<AnimationTicker const> position) -> void
{
    child->SetParent(GetSelf());
    _children.insert(std::find_if(_children.begin(), _children.end(), [&](auto const& c) { return c.GetPointer() == position; }), child);
}

///
/// @brief
///
/// @param child
///
auto AnimationTicker::AbandonChild(Shared<AnimationTicker> const& child) -> void
{
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
        NotifyActiveChanged(IsActive());
        NotifyEnabledChanged(IsEnabled());
    }
}

///
/// @brief
///
auto AnimationTicker::SendEvent(Event<>& event) -> Bool
{
    return GetEventReceiver().SendEvent(event);
}

///
/// @brief
///
auto AnimationTicker::GetTracker() -> Weak<void>
{
    return _self;
}

///
/// @brief
///
auto AnimationTicker::GetTracker() const -> Weak<void const>
{
    return _self;
}

///
/// @brief
///
auto AnimationTicker::GetEventReceiver() -> EventReceiver&
{
    return _eventReceiver;
}

///
/// @brief
///
auto AnimationTicker::GetEventReceiver() const -> EventReceiver const&
{
    return _eventReceiver;
}

///
/// @brief
///
auto AnimationTicker::NotifyTick(MonotonicTime const time) -> void
{
    if (_tickRequested)
    {
        FW_DEBUG_ASSERT(IsActive());

        if (IsEnabledFromRoot())
        {
            _tickRequested = false;

            SendAnimationTickerTickEvent(time);

            for (auto const& child : _children)
            {
                child->NotifyTick(time);
            }
        }
    }
}

///
/// @brief
///
auto AnimationTicker::NotifyActiveChanged(Bool const active) -> void
{
    if (_active != active)
    {
        _active = active;

        for (auto const& child : _children)
        {
            child->NotifyActiveChanged(active);
        }

        SendAnimationTickerActiveChangedEvent();
    }
}

///
/// @brief
///
auto AnimationTicker::NotifyEnabledChanged(Bool const enabled) -> void
{
    if (_enabled)
    {
        if (_enabledFromRoot != enabled)
        {
            _enabledFromRoot = enabled;

            for (auto const& child : _children)
            {
                child->NotifyEnabledChanged(enabled);
            }

            SendAnimationTickerEnabledChangedEvent();
        }
    }
}

///
/// @brief
///
auto AnimationTicker::SendAnimationTickerTickEvent(MonotonicTime const time) -> void
{
    auto parameter = Event<AnimationTickerEvent::Tick>();
    parameter->SetTime(time);
    auto timerEvent = Event<>(std::move(parameter));
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
