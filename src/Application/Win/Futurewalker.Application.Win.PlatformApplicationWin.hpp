// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformApplicationWinType.hpp"
#include "Futurewalker.Application.PlatformApplication.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationContextWin.hpp"

#include "Futurewalker.Async.ThreadPool.hpp"
#include "Futurewalker.Async.ThisThread.hpp"
#include "Futurewalker.Async.AsyncTask.hpp"
#include "Futurewalker.Async.LazyTask.hpp"

#include <deque>
#include <thread>

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
    static auto Make(Delegate delegate, Shared<PlatformApplicationContextWin> context) -> Shared<PlatformApplicationWin>;

    PlatformApplicationWin(PassKey<PlatformApplication>, Delegate delegate, Shared<PlatformApplicationContextWin> context);

    auto RequestQuit() -> void override;
    auto IsActive() -> Bool override;
    auto IsForeground() -> Bool override;
    auto SetMainMenu(Menu const& menu) -> void override;

    auto SetStarted() -> void;
    auto SetQuitting() -> void;
    auto SetActive(Bool const active) -> void;

protected:
    auto Initialize() -> void override;

private:
    Shared<PlatformApplicationContextWin> _context;
    Bool _started = false;
    Bool _quitting = false;
    Bool _active = false;
};
}
}
