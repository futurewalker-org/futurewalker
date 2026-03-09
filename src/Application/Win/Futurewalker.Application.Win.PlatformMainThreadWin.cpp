// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformMainThreadWin.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationWin.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Exception.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
PlatformMainThreadWin::PlatformMainThreadWin()
{
}

///
/// @brief
///
auto PlatformMainThreadWin::IsMainThread() const -> Bool
{
    if (auto applicationContext = Locator::GetInstance<PlatformApplicationContext>().TryAs<PlatformApplicationContextWin>())
    {
        if (auto application = applicationContext->GetCurrentApplication())
        {
            return application->IsMainThread();
        }
    }
    return false;
}

///
/// @brief
///
auto PlatformMainThreadWin::Schedule() -> AsyncTask<void>
{
    if (auto applicationContext = Locator::GetInstance<PlatformApplicationContext>().TryAs<PlatformApplicationContextWin>())
    {
        if (auto application = applicationContext->GetCurrentApplication())
        {
            co_return co_await application->Schedule();
        }
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
    if (auto applicationContext = Locator::GetInstance<PlatformApplicationContext>().TryAs<PlatformApplicationContextWin>())
    {
        if (auto application = applicationContext->GetCurrentApplication())
        {
            co_return co_await application->ScheduleAfter(delay);
        }
    }
    throw Exception(ErrorCode::Failure);
}

///
/// @brief
///
/// @param[in] tid
///
auto Locator::Resolver<PlatformMainThreadWin>::Resolve() -> Shared<PlatformMainThreadWin>
{
    return Shared<PlatformMainThreadWin>::Make();
}
}
