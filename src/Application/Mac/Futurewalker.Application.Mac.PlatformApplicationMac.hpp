// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformApplicationMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformApplicationContextMac.hpp"
#include "Futurewalker.Application.PlatformApplication.hpp"

#include "Futurewalker.Async.ThisThread.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Platform application.
///
class PlatformApplicationMac : public PlatformApplication
{
public:
    static auto Make(Delegate const& delegate, Shared<PlatformApplicationContextMac> context) -> Shared<PlatformApplicationMac>;

    PlatformApplicationMac(PassKey<PlatformApplication>, Delegate const& delegate, Shared<PlatformApplicationContextMac> context);

    auto RequestQuit() -> void override;
    auto IsActive() -> Bool override;
    auto IsForeground() -> Bool override;
    auto SetMainMenu(Menu const& menu) -> void override;

    auto Run(Function<void()> const& cleanup) -> Async<void>;


protected:
    auto Initialize() -> void override;
    auto CallbackOnFinishLaunching() -> void;
    auto CallbackOnBecomeActive() -> void;
    auto CallbackOnResignActive() -> void;
    auto CallbackOnShouldTerminate() -> BOOL;
    auto CallbackOnWillTerminate() -> void;

private:
    auto Schedule() -> AsyncTask<void>;
    auto ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>;
    auto SetActive(Bool const active) -> void;

private:
    Shared<PlatformApplicationContextMac> _context;
    Shared<ThisThread::Scheduler> _thisThreadScheduler;
    Pointer<Function<void()> const> _cleanup = nullptr;
    __strong PlatformApplicationDelegate* _delegate = nil;
};
}
