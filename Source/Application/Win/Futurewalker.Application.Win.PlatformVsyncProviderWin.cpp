// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformVsyncProviderWin.hpp"
#include "Futurewalker.Application.MainThread.hpp" 

#include "Futurewalker.Base.Debug.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Core.Exception.hpp"

#include <dcomp.h>

namespace FW_DETAIL_NS
{
///
/// @brief Constructor.
///
PlatformVsyncProviderWin::PlatformVsyncProviderWin()
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
            this->WaitForCallbackOnThread();

            const auto count = UINT(1);
            const auto result = ::DCompositionWaitForCompositorClock(count, &event, INFINITE);

            if (StopRequested())
            {
                break;
            }

            if (result == WAIT_OBJECT_0 + count)
            {
                MonotonicTime frameTime;
                while (!GetLastCompletedFrameTime(frameTime))
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }

                const auto frameInfo = PlatformVsyncFrameInfo {
                  .targetTimestamp = frameTime,
                };

                auto callbacks = this->GetCallbacks();
                if (!callbacks.empty())
                {
                    AsyncFunction::Spawn(DispatchCallbacks(frameInfo, std::move(callbacks)));
                }
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
    auto frameTime = MonotonicTime();
    if (GetLastCompletedFrameTime(frameTime))
    {
        return frameTime;
    }
    return {};
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
            return cb.data.Lock() == data.Lock();
        }
        return false;
    });

    if (it != _callbacks.end())
    {
        it->callback = callback;
        return;
    }
    _callbacks.emplace_back(data, callback);
    _condVar.notify_one();
}

///
/// @brief Returns true if there is a callback.
///
auto PlatformVsyncProviderWin::GetCallbacks() -> std::vector<CallbackData>
{
    auto lock = std::unique_lock(_mutex);
    return std::move(_callbacks);
}

///
/// @brief Wait for callback on thread.
///
auto PlatformVsyncProviderWin::WaitForCallbackOnThread() -> Bool
{
    auto lock = std::unique_lock(_mutex);
    _condVar.wait(lock, [&] { return !_callbacks.empty() || _stop; });
    return !_stop;
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
/// @brief Get last completed frame time.
///
auto PlatformVsyncProviderWin::GetLastCompletedFrameTime(MonotonicTime& frameTime) -> Bool
{
    auto frequency = LARGE_INTEGER();
    if (!::QueryPerformanceFrequency(&frequency))
    {
        FW_DEBUG_LOG_ERROR("QueryPerformanceFrequency failed");
        FW_DEBUG_ASSERT(false);
        return false;
    }

    // Get ID of most recently completed frame.
    // DCompositionGetStatistics() can fail on frames obtained by COMPOSITION_FRAME_ID_CREATED.
    auto frameId = COMPOSITION_FRAME_ID();
    HRESULT hr = ::DCompositionGetFrameId(COMPOSITION_FRAME_ID_COMPLETED, &frameId);
    if (FAILED(hr))
    {
        FW_DEBUG_LOG_ERROR("No composition frame ID of type COMPOSITION_FRAME_ID_COMPLETED");
        FW_DEBUG_ASSERT(false);
        return false;
    }

    // Might be possible to query per-output timing info.
    auto stats = COMPOSITION_FRAME_STATS();
    hr = ::DCompositionGetStatistics(frameId, &stats, 0, NULL, NULL);
    if (FAILED(hr))
    {
        FW_DEBUG_LOG_ERROR("Failed to get composition frame stats");
        FW_DEBUG_ASSERT(false);
        return false;
    }
    frameTime = static_cast<float64_t>(stats.targetTime) / frequency.QuadPart;
    return true;
}

///
/// @brief Runs all callbacks on main thread.
///
/// @return A task.
///
auto PlatformVsyncProviderWin::DispatchCallbacks(PlatformVsyncFrameInfo const frameInfo, std::vector<CallbackData> const callbacks) -> Task<void>
{
    try
    {
        co_await MainThread::Schedule();

        if (auto provider = Locator::GetInstance<PlatformVsyncProviderWin>())
        {
            for (const auto& callback : callbacks)
            {
                if (auto data = callback.data.Lock())
                {
                    if (callback.callback)
                    {
                        callback.callback(data, frameInfo);
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
}

///
/// @brief
///
auto Locator::Resolver<PlatformVsyncProviderWin>::Resolve() -> Shared<PlatformVsyncProviderWin>
{
    return Shared<PlatformVsyncProviderWin>::Make();
}
}
