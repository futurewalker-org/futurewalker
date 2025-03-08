// SPDX-License-Identifier: MIT

#include "Futurewalker.Async.ThreadPool.hpp"

#include <thread>

namespace FW_DETAIL_NS
{
auto Locator::Resolver<ThreadPool>::Resolve() -> Shared<ThreadPool>
{
    // Ensure enough threads are available to avoid unfair scheduling.
    auto constexpr minimumThreadCount = 4;
    // hardware_concurrency() should only be used as a hint, since it can return 0.
    auto const threadCount = SInt64::Max(minimumThreadCount, std::thread::hardware_concurrency());
    return Shared<ThreadPool>::Make(threadCount);
}
}
