// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Animation.RootAnimationTicker.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief 
///
/// @param delegate
///
auto RootAnimationTicker::Make(Delegate delegate) -> Shared<RootAnimationTicker>
{
    return AnimationTicker::MakeDerived<RootAnimationTicker>(std::move(delegate));
}

///
/// @brief 
///
/// @param key 
/// @param delegate 
///
RootAnimationTicker::RootAnimationTicker(PassKey<AnimationTicker> key, Delegate delegate)
  : AnimationTicker(key)
  , _delegate {std::move(delegate)}
{
}

///
/// @brief
///
/// @param active
///
auto RootAnimationTicker::SetActive(Bool const active) -> void
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
auto RootAnimationTicker::UpdateCurrentTime() -> void
{
    NotifyRootCurrentTimeChanged();
}

///
/// @brief
///
auto RootAnimationTicker::RootIsActive() const -> Bool
{
    return _active;
}

///
/// @brief
///
auto RootAnimationTicker::RootGetCurrentTime() const -> MonotonicTime
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
auto RootAnimationTicker::RootRequestTick() -> void
{
    if (_delegate.requestTick)
    {
        _delegate.requestTick();
    }
}
}
