// SPDX-License-Identifier: MIT

#include "Futurewalker.Animation.RootAnimationTimer.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief 
///
/// @param delegate
///
auto RootAnimationTimer::Make(Delegate delegate) -> Shared<RootAnimationTimer>
{
    return AnimationTimer::MakeDerived<RootAnimationTimer>(std::move(delegate));
}

///
/// @brief 
///
/// @param key 
/// @param delegate 
///
RootAnimationTimer::RootAnimationTimer(PassKey<AnimationTimer> key, Delegate delegate)
  : AnimationTimer(key)
  , _delegate {std::move(delegate)}
{
}

///
/// @brief
///
/// @param active
///
auto RootAnimationTimer::SetActive(Bool const active) -> void
{
    if (_active != active)
    {
        _active = active;
        NotifyRootActiveChanged();
    }
}

///
/// @brief
///
auto RootAnimationTimer::UpdateCurrentTime() -> void
{
    NotifyRootCurrentTimeChanged();
}

///
/// @brief
///
auto RootAnimationTimer::RootIsActive() const -> Bool
{
    return _active;
}

///
/// @brief
///
auto RootAnimationTimer::RootGetCurrentTime() const -> MonotonicTime
{
    if (_delegate.getCurrentTime)
    {
        return _delegate.getCurrentTime();
    }
    return {};
}

///
/// @brief
///
auto RootAnimationTimer::RootRequestTick() -> void
{
    if (_delegate.requestTick)
    {
        _delegate.requestTick();
    }
}
}
