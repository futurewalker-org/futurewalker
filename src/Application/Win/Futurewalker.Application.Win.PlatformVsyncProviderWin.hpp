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
#include "Futurewalker.Core.PassKey.hpp"

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
    static auto Make() -> Shared<PlatformVsyncProviderWin>;

    PlatformVsyncProviderWin(PassKey<PlatformVsyncProviderWin>);
    ~PlatformVsyncProviderWin();

    auto GetCurrentFrameTime() const -> MonotonicTime;
    auto PostFrameCallback(Weak<void> data, PlatformVsyncCallbackFunction callback) -> void;

private:
    auto HasCallback() const -> Bool;
    auto GetCallbacks() -> std::vector<CallbackData>;
    auto WaitForCallbackOnThread() -> Bool;

    auto RequestStop() -> void;
    auto StopRequested() const -> Bool;

    static auto DispatchCallbacks(PlatformVsyncFrameInfo const frameInfo, Weak<PlatformVsyncProviderWin> const weakSelf) -> Task<void>;

private:
    Weak<PlatformVsyncProviderWin> _self;
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
