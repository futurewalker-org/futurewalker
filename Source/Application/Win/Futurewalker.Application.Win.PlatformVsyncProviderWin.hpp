// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.PlatformVsyncProviderWinType.hpp"

#include "Futurewalker.Async.Task.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.MonotonicTime.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Vsync tick provider.
///
class PlatformVsyncProviderWin : NonCopyable
{
public:
    PlatformVsyncProviderWin();
    ~PlatformVsyncProviderWin();

    auto GetCurrentFrameTime() const -> MonotonicTime;
    
    auto RegisterCallback(Weak<void> data, PlatformVsyncCallbackFunction callback) -> void;
    auto UnregisterCallback(Weak<void> data) -> void;

private:
    auto HasCallback() const -> Bool;
    auto WaitForCallbackOnThread() -> Bool;

    auto RequestStop() -> void;
    auto StopRequested() const -> Bool;

    static auto GetLastCompletedFrameTime(MonotonicTime& frameTime) -> Bool;
    static auto DispatchCallbacks(PlatformVsyncFrameInfo const frameInfo) -> Task<void>;

private:
    struct CallbackData
    {
        Weak<void> data;
        PlatformVsyncCallbackFunction callback;
    };
    mutable std::mutex _mutex;
    std::condition_variable _condVar;
    std::vector<CallbackData> _callbacks;
    std::jthread _thread;
    Bool _stop = false;
    HANDLE _event = NULL;
};

///
/// @brief
///
template <>
struct Locator::Resolver<PlatformVsyncProviderWin>
{
    using Interface = PlatformVsyncProviderWin;
    static auto Resolve() -> Shared<PlatformVsyncProviderWin>;
};
}
}
