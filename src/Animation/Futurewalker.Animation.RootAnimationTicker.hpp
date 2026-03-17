// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Animation.RootAnimationTickerType.hpp"
#include "Futurewalker.Animation.AnimationTicker.hpp"

#include "Futurewalker.Core.Function.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class RootAnimationTicker : public AnimationTicker
{
public:
    struct Delegate
    {
        Function<void()> requestTick;
        Function<MonotonicTime()> getCurrentTime;
    };
    static auto Make(Delegate delegate) -> Shared<RootAnimationTicker>;

    RootAnimationTicker(PassKey<AnimationTicker> key, Delegate delegate);

    auto SetActive(Bool const active) -> void;
    auto SetTickTime(MonotonicTime const tickTime) -> void;

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
