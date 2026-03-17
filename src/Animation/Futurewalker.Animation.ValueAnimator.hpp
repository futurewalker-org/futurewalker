// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Animation.ValueAnimatorType.hpp"
#include "Futurewalker.Animation.AnimationTicker.hpp"
#include "Futurewalker.Animation.AnimationInterpolationType.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.TimeInterval.hpp"
#include "Futurewalker.Core.Blank.hpp"
#include "Futurewalker.Core.NonCopyable.hpp" 

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Simple value animator.
///
/// @tparam T Type of the value to animate.
///
template <class T>
class ValueAnimator : NonCopyable
{
public:
    ///
    /// @brief Constructor.
    ///
    /// @param initialValue Initial value.
    ///
    explicit ValueAnimator(T const& initialValue)
      : _initialValue {initialValue}
      , _targetValue {initialValue}
      , _currentValue {initialValue}
    {
        _tracker = Tracker::Make();
    }

    ///
    /// @brief Bind the animator to an animation ticker.
    ///
    /// @param owner Owner of the animation ticker.
    ///
    template <class Owner>
    auto BindAnimationTicker(Owner& owner) -> void
    {
        _ticker = &owner.GetAnimationTicker();
        _connection = EventReceiver::Connect(*_ticker, *this, &ValueAnimator::ReceiveTickerEvent);
        _lastTickTime = _ticker->GetCurrentTime().GetIntervalSinceEpoch();
    }

    ///
    /// @brief Get current value of the animator.
    ///
    auto GetCurrentValue() const -> T const&
    {
        return _currentValue;
    }

    ///
    /// @brief Set duration of the animation.
    ///
    /// @param duration Duration of the animation.
    ///
    auto SetDuration(TimeInterval const& duration) -> void
    {
        _duration = duration;
    }

    ///
    /// @brief Set interpolation function of the animation.
    ///
    auto SetInterpolation(AnimationInterpolationFunction func) -> void
    {
        _interpolationFunction = std::move(func);
    }

    ///
    /// @brief Set update callback of the animation.
    ///
    /// @param callback Update callback to call when the current value is changed.
    ///
    auto SetUpdateCallback(Function<void()> callback) -> void
    {
        _updateCallback = std::move(callback);
    }

    ///
    /// @brief Set completion callback of the animation.
    ///
    /// @param callback Completion callback to call when the animation is completed.
    ///
    auto SetCompletionCallback(Function<void()> callback) -> void
    {
        _completionCallback = std::move(callback);
    }

    ///
    /// @brief Check if the animator is currently animating.
    ///
    auto IsAnimating() const -> Bool
    {
        return _animating;
    }

    ///
    /// @brief Animate to target value.
    ///
    /// @param targetValue Target value to animate to.
    ///
    auto AnimateTo(T const& targetValue) -> void
    {
        if (!_animating)
        {
            if (_ticker)
            {
                _lastTickTime = _ticker->GetCurrentTime().GetIntervalSinceEpoch();
            }
            else
            {
                _lastTickTime = TimeInterval();
            }
        }

        if (TimeInterval() <= _duration)
        {
            _animating = true;
            _initialValue = _currentValue;
            _targetValue = targetValue;
            _startTime = _lastTickTime;
            RequestNextTick();
        }
        else
        {
            _animating = false;
            _targetValue = targetValue;
            _initialValue = targetValue;
            _currentValue = targetValue;
            NotifyUpdate();
            NotifyCompletion();
        }
    }

    ///
    /// @brief Set current value to target value immediately without animation.
    ///
    /// @param targetValue Target value to jump to.
    ///
    auto JumpTo(T const& targetValue) -> void
    {
        auto const wasAnimating = std::exchange(_animating, false);
        _targetValue = targetValue;
        _initialValue = targetValue;
        _currentValue = targetValue;

        NotifyUpdate();

        if (wasAnimating)
        {
            NotifyCompletion();
        }
    }

    auto GetTracker() -> Tracker&
    {
        return *_tracker;
    }

    auto GetTracker() const -> Tracker const&
    {
        return *_tracker;
    }

private:
    auto ReceiveTickerEvent(Event<>& event) -> Async<Bool>
    {
        if (event.Is<AnimationTickerEvent::Tick>())
        {
            auto const tickTime = event.As<AnimationTickerEvent::Tick>()->GetTime().GetIntervalSinceEpoch();
            if (_lastTickTime != tickTime)
            {
                _lastTickTime = tickTime;

                if (_animating)
                {
                    if (_startTime == TimeInterval())
                    {
                        _startTime = tickTime;
                    }

                    if (_startTime + _duration <= tickTime)
                    {
                        _animating = false;
                        _initialValue = _targetValue;
                        _currentValue = _targetValue;
                        NotifyUpdate();
                        NotifyCompletion();
                    }
                    else
                    {
                        if (tickTime != _startTime)
                        {
                            auto const totalSeconds = _duration.GetSeconds();
                            auto const elapsedSeconds = (tickTime - _startTime).GetSeconds();
                            auto t = Float64::Clamp(static_cast<Float64>(elapsedSeconds / totalSeconds), 0, 1);
                            if (_interpolationFunction)
                            {
                                t = _interpolationFunction(t);
                            }
                            _currentValue = _initialValue + (_targetValue - _initialValue) * t;
                            NotifyUpdate();
                        }
                        RequestNextTick();
                    }
                }
            }
            else
            {
                if (_animating)
                {
                    RequestNextTick();
                }
            }
        }
        co_return false;
    }

    auto RequestNextTick() -> void
    {
        if (_ticker)
        {
            _ticker->RequestTick();
        }
    }

    auto NotifyUpdate() -> void
    {
        if (_updateCallback)
        {
            _updateCallback();
        }
    }

    auto NotifyCompletion() -> void
    {
        if (_completionCallback)
        {
            _completionCallback();
        }
    }

private:
    Pointer<AnimationTicker> _ticker = nullptr;
    Bool _animating = false;
    T _initialValue;
    T _targetValue;
    T _currentValue;
    TimeInterval _duration;
    TimeInterval _lastTickTime;
    TimeInterval _startTime;
    AnimationInterpolationFunction _interpolationFunction;
    ScopedSignalConnection _connection;
    Function<void()> _updateCallback;
    Function<void()> _completionCallback;
    Shared<Tracker> _tracker;
};
}
}
