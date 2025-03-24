// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformMainThreadWin.hpp"
#include "Futurewalker.Application.Win.PlatformEventLoopContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformEventLoopWin.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Exception.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
PlatformMainThreadWin::PlatformMainThreadWin(Shared<PlatformEventLoopContextWin> eventLoopContext, const std::thread::id tid)
  : _eventLoopContext {eventLoopContext.As<PlatformEventLoopContextWin>()}
  , _tid {tid}
{
    FW_DEBUG_ASSERT(false);
}

///
/// @brief
///
auto PlatformMainThreadWin::IsMainThread() const -> Bool
{
    return std::this_thread::get_id() == _tid;
}

///
/// @brief
///
auto PlatformMainThreadWin::Schedule() -> AsyncTask<void>
{
    if (auto eventLoop = _eventLoopContext->GetEventLoopForThreadId(_tid))
    {
        co_return co_await eventLoop->Schedule();
    }
    throw Exception(ErrorCode::Failure);
}

///
/// @brief
///
/// @param delay
///
auto PlatformMainThreadWin::ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>
{
    if (auto eventLoop = _eventLoopContext->GetEventLoopForThreadId(_tid))
    {
        co_return co_await eventLoop->ScheduleAfter(delay);
    }
    throw Exception(ErrorCode::Failure);
}

///
/// @brief
///
/// @param[in] tid
///
auto Locator::Resolver<PlatformMainThreadWin>::Resolve(const std::thread::id tid) -> Shared<PlatformMainThreadWin>
{
    auto eventLoopContext = Locator::Resolve<PlatformEventLoopContext>().As<PlatformEventLoopContextWin>();
    return Shared<PlatformMainThreadWin>::Make(eventLoopContext, tid);
}
}
