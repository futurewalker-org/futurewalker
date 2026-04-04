// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.Prelude.hpp"
#include "Futurewalker.Application.Mac.PlatformApplicationContextMac.hpp"
#include "Futurewalker.Application.PlatformApplication.hpp"

#include "Futurewalker.Async.ThisThread.hpp"

@class PlatformApplicationDelegate;

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

    auto Run() -> Async<void> override;
    auto Exit() -> void override;
    auto IsRunning() -> Bool override;
    auto IsActive() -> Bool override;
    auto IsForeground() -> Bool override;
    auto SetMainMenu(Menu const& menu) -> void override;


protected:
    auto Initialize() -> void override;

private:
    auto Schedule() -> AsyncTask<void>;
    auto ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>;

private:
    Shared<PlatformApplicationContextMac> _context;
    Shared<ThisThread::Scheduler> _thisThreadScheduler;
    __strong PlatformApplicationDelegate* _delegate = nil;
};
}
