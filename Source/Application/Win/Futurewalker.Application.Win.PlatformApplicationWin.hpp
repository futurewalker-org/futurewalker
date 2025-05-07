// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.PlatformApplicationWinType.hpp"
#include "Futurewalker.Application.PlatformApplication.hpp"
#include "Futurewalker.Application.PlatformEventLoop.hpp"

#include "Futurewalker.Application.Win.PlatformApplicationContextWin.hpp"

#include "Futurewalker.Application.PlatformMainThread.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class PlatformApplicationWin : public PlatformApplication
{
public:
    PlatformApplicationWin(Delegate delegate, Shared<PlatformApplicationContextWin> context, Shared<PlatformEventLoop> eventLoop, Shared<PlatformMainThread> mainThread);
    auto Run() -> Async<void> override;
    auto Exit() -> void override;
    auto IsRunning() -> Bool override;
    auto IsActive() -> Bool override;
    auto IsForeground() -> Bool override;

    auto SetActive(Bool const active) -> void;

private:
    auto SyncSendApplicationEvent(Event const& event) -> void;

private:
    Shared<PlatformApplicationContextWin> _context;
    Shared<PlatformEventLoop> _eventLoop;
    Shared<PlatformMainThread> _mainThread;
    Bool _active = false;
};
}
}
