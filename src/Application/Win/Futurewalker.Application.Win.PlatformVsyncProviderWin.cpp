// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformVsyncProviderWin.hpp"
#include "Futurewalker.Application.MainThread.hpp" 

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.MonotonicClock.hpp"

#include <dcomp.h>

namespace FW_DETAIL_NS
{
auto PlatformVsyncProviderWin::Make() -> Shared<PlatformVsyncProviderWin>
{
    auto vsyncProvider = Shared<PlatformVsyncProviderWin>::Make(PassKey<PlatformVsyncProviderWin>());
    vsyncProvider->_self = vsyncProvider;
    return vsyncProvider;
}

///
/// @brief Constructor.
///
PlatformVsyncProviderWin::PlatformVsyncProviderWin(PassKey<PlatformVsyncProviderWin>)
{
    _event = ::CreateEventW(NULL, FALSE, FALSE, NULL);
    if (!_event)
    {
        throw Exception(ErrorCode::Failure, "CreateEventW failed");
    }

    _thread = std::jthread([this, event = _event] {
        auto frequency = LARGE_INTEGER();
        ::QueryPerformanceFrequency(&frequency);

        while (true)
        {
            if (!WaitForCallbackOnThread())
            {
                break;
            }

            const auto count = UINT(1);
            const auto result = ::DCompositionWaitForCompositorClock(count, &event, INFINITE);

            if (StopRequested())
            {
                break;
            }

            if (result == WAIT_OBJECT_0 + count)
            {
                {
                    auto lock = std::unique_lock(_mutex);
                    _state = State::Dispatching;
                }
                auto const frameTime = GetCurrentFrameTime();
                AsyncFunction::Spawn(DispatchCallbacks(frameTime, _self)).Detach();
            }
            else if (result == WAIT_TIMEOUT || result == WAIT_FAILED)
            {
                FW_DEBUG_LOG_ERROR("DCompositionWaitForCompositorClock failed");
                FW_DEBUG_ASSERT(false);
            }
        }
    });
}

///
/// @brief Destructor.
///
PlatformVsyncProviderWin::~PlatformVsyncProviderWin()
{
    RequestStop();
    _thread.join();
}

///
/// @brief Get current frame time.
///
auto PlatformVsyncProviderWin::GetCurrentFrameTime() const -> MonotonicTime
{
    return MonotonicClock::GetNow();
}

///
/// @brief Register callback.
///
/// @param data User data.
/// @param callback Callback function.
///
auto PlatformVsyncProviderWin::PostFrameCallback(Weak<void> data, PlatformVsyncCallbackFunction callback) -> void
{
    if (data.IsExpired())
    {
        return;
    }

    auto lock = std::unique_lock(_mutex);

    const auto it = std::find_if(_callbacks.begin(), _callbacks.end(), [&](const auto& cb) {
        const auto lhs = cb.data.Lock();
        const auto rhs = data.Lock();
        if (lhs && rhs)
        {
            return lhs == rhs;
        }
        return false;
    });

    if (it != _callbacks.end())
    {
        it->callback = callback;
        return;
    }
    _callbacks.emplace_back(data, callback);

    if (_state == State::Idle)
    {
        _state = State::Requesting;
        _condVar.notify_one();
    }
}

///
/// @brief Remove posted frame callback.
///
/// @param data User data.
///
auto PlatformVsyncProviderWin::RemoveFrameCallback(Weak<void> data) -> void
{
    auto lock = std::unique_lock(_mutex);

    std::erase_if(_callbacks, [&](const auto& cb) {
        const auto lhs = cb.data.Lock();
        const auto rhs = data.Lock();
        if (lhs && rhs)
        {
            return lhs == rhs;
        }
        return false;
    });
}

///
/// @brief Consume callbacks and return them.
///
auto PlatformVsyncProviderWin::ConsumeCallbacks() -> std::vector<CallbackData>
{
    auto lock = std::unique_lock(_mutex);
    return std::exchange(_callbacks, {});
}

///
/// @brief Wait for callback on thread.
///
auto PlatformVsyncProviderWin::WaitForCallbackOnThread() -> Bool
{
    auto lock = std::unique_lock(_mutex);
    _condVar.wait(lock, [&] { return _state == State::Requesting || _stop; });
    return !_stop;
}

///
/// @brief End dispatching callbacks.
///
auto PlatformVsyncProviderWin::EndDispatching() -> void
{
    auto lock = std::unique_lock(_mutex);

    if (_state == State::Dispatching)
    {
        if (!_callbacks.empty())
        {
            _state = State::Requesting;
            _condVar.notify_one();
        }
        else
        {
            _state = State::Idle;
        }
    }
}

///
/// @brief Request stop.
///
auto PlatformVsyncProviderWin::RequestStop() -> void
{
    auto lock = std::unique_lock(_mutex);
    _stop = true;
    _condVar.notify_one();
    ::SetEvent(_event);
}

///
/// @brief Returns true if stop is requested.
///
auto PlatformVsyncProviderWin::StopRequested() const -> Bool
{
    auto lock = std::unique_lock(_mutex);
    return _stop;
}

///
/// @brief Runs all callbacks on main thread.
///
/// @return A task.
///
auto PlatformVsyncProviderWin::DispatchCallbacks(MonotonicTime const frameTime, Weak<PlatformVsyncProviderWin> const weakSelf) -> Task<void>
{
    try
    {
        co_await MainThread::Schedule();

        if (auto const self = weakSelf.Lock())
        {
            auto const callbacks = self->ConsumeCallbacks();
            for (auto const& callback : callbacks)
            {
                if (auto const data = callback.data.Lock())
                {
                    if (callback.callback)
                    {
                        try
                        {
                            auto frameInfo = PlatformVsyncFrameInfo {
                                .targetTimestamp = frameTime,
                            };
                            callback.callback(data, frameInfo);
                        }
                        catch (...)
                        {
                            FW_DEBUG_ASSERT(false);
                        }
                    }
                }
            }
        }
    }
    catch (...)
    {
        FW_DEBUG_LOG_ERROR("Failed to dispatch vsync event");
        FW_DEBUG_ASSERT(false);
    }

    if (auto const self = weakSelf.Lock())
    {
        self->EndDispatching();
    }
}

///
/// @brief
///
auto Locator::Resolver<PlatformVsyncProviderWin>::Resolve() -> Shared<PlatformVsyncProviderWin>
{
    return PlatformVsyncProviderWin::Make();
}
}
