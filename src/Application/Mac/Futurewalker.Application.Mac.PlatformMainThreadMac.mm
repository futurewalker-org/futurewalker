// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformMainThreadMac.hpp"

#import <AppKit/AppKit.h>

#include <coroutine>

namespace FW_DETAIL_NS
{
auto PlatformMainThreadMac::IsMainThread() const -> Bool
{
    @autoreleasepool
    {
        return [NSThread isMainThread];
    }
}

auto PlatformMainThreadMac::Schedule() -> AsyncTask<void>
{
    struct Awaitable
    {
        auto await_ready()
        {
            return false;
        }

        auto await_suspend(std::coroutine_handle<> c)
        {
            @autoreleasepool
            {
                dispatch_async(dispatch_get_main_queue(), ^{
                    c.resume();
                });
            }
        }

        auto await_resume()
        {
        }
    };
    co_await Awaitable();
}

auto PlatformMainThreadMac::ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>
{
    struct Awaitable
    {
        std::chrono::nanoseconds _delay;

        auto await_ready()
        {
            return false;
        }

        auto await_suspend(std::coroutine_handle<> c)
        {
            @autoreleasepool
            {
                dispatch_after(dispatch_time(DISPATCH_TIME_NOW, _delay.count()), dispatch_get_main_queue(), ^{
                    c.resume();
                });
            }
        }

        auto await_resume()
        {
        }
    };
    co_await Awaitable(delay);
}

auto Locator::Resolver<PlatformMainThreadMac>::Resolve() -> Shared<PlatformMainThreadMac>
{
    return Shared<PlatformMainThreadMac>::Make();
}
}
