// SPDX-License-Identifier: MIT

#include "Futurewalker.Async.ThreadPool.hpp"

#include <thread>

namespace FW_DETAIL_NS
{
auto Locator::Resolver<ThreadPool>::Resolve() -> Shared<ThreadPool>
{
    // Ensure enough threads are available to avoid unfair scheduling.
    constexpr SInt64 minimumThreadCount = 4;
    // hardware_concurrency() should only be used as a hint, since it can return 0.
    const SInt64 threadCount = SInt64::Max(minimumThreadCount, std::thread::hardware_concurrency());
    return Shared<ThreadPool>::Make(threadCount);
}
}
