// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformMainThread.hpp"
#include "Futurewalker.Application.Win.PlatformEventLoopContextWinType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include <thread>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Win32 implementation of PlatformMainThread.
///
class PlatformMainThreadWin : public PlatformMainThread
{
public:
    PlatformMainThreadWin(Shared<PlatformEventLoopContextWin> eventLoopContext, const std::thread::id tid);
    auto IsMainThread() const -> Bool override;
    auto Schedule() -> AsyncTask<void> override;
    auto ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void> override;

private:
    Shared<PlatformEventLoopContextWin> _eventLoopContext;
    const std::thread::id _tid;
};

///
/// @brief
///
template <>
struct Locator::Resolver<PlatformMainThreadWin>
{
    using Interface = PlatformMainThread;
    static auto Resolve(const std::thread::id tid = std::this_thread::get_id()) -> Shared<PlatformMainThreadWin>;
};
}
}
