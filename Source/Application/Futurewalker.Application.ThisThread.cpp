// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.ThisThread.hpp"
#include "Futurewalker.Application.PlatformEventLoopContext.hpp"
#include "Futurewalker.Application.PlatformEventLoop.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
auto ThisThread::Schedule() -> Async<void>
{
    if (auto context = Locator::GetInstance<PlatformEventLoopContext>())
    {
        if (auto loop = context->GetEventLoopForThread())
        {
            co_return co_await ResumesOnSameThreadTask(LazyTask(loop->Schedule()));
        }
    }
    throw Exception(ErrorCode::Failure);
}
}
}
