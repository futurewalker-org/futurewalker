// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Animation.RootAnimationTimerType.hpp"
#include "Futurewalker.Animation.AnimationTimer.hpp"

#include "Futurewalker.Core.Function.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class RootAnimationTimer : public AnimationTimer
{
public:
    struct Delegate
    {
        Function<void()> requestTick;
        Function<MonotonicTime()> getCurrentTime;
    };
    static auto Make(Delegate delegate) -> Shared<RootAnimationTimer>;

    RootAnimationTimer(PassKey<AnimationTimer> key, Delegate delegate);

    auto SetActive(const Bool active) -> void;
    
    auto UpdateCurrentTime() -> void;

protected:
    auto RootIsActive() const -> Bool override;
    auto RootGetCurrentTime() const -> MonotonicTime override;
    auto RootRequestTick() -> void override;

private:
    Delegate _delegate;
    Bool _active = false;
};
}
}
