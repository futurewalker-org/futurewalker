// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformVsyncProviderWinType.hpp"

#include "Futurewalker.Async.Task.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.MonotonicTime.hpp"
#include "Futurewalker.Core.Blank.hpp"

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
    struct CallbackData
    {
        Weak<void> data;
        PlatformVsyncCallbackFunction callback;
    };

public:
    PlatformVsyncProviderWin();
    ~PlatformVsyncProviderWin();

    auto GetCurrentFrameTime() const -> MonotonicTime;
    
    auto PostFrameCallback(Weak<void> data, PlatformVsyncCallbackFunction callback) -> void;

private:
    auto GetCallbacks() -> std::vector<CallbackData>;
    auto WaitForCallbackOnThread() -> Bool;

    auto RequestStop() -> void;
    auto StopRequested() const -> Bool;

    static auto GetLastCompletedFrameTime(MonotonicTime& frameTime) -> Bool;
    static auto DispatchCallbacks(PlatformVsyncFrameInfo const frameInfo, std::vector<CallbackData> const callbacks, std::weak_ptr<Blank> const tracker) -> Task<void>;

private:
    mutable std::mutex _mutex;
    std::condition_variable _condVar;
    std::shared_ptr<Blank> _tracker;
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
