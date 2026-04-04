// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformApplicationContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformApplicationMac.hpp"

namespace FW_DETAIL_NS
{
auto PlatformApplicationContextMac::Make(Shared<PlatformMainThread> mainThread, Shared<PlatformWindowContext> windowContext) -> Shared<PlatformApplicationContextMac>
{
    auto const key = PassKey<PlatformApplicationContextMac>();
    auto context = Shared<PlatformApplicationContextMac>::Make(key, mainThread, windowContext);
    context->_self = context;
    return context;
}

PlatformApplicationContextMac::PlatformApplicationContextMac(PassKey<PlatformApplicationContextMac>, Shared<PlatformMainThread> mainThread, Shared<PlatformWindowContext> windowContext)
  : PlatformApplicationContext()
  , _mainThread {mainThread}
  , _windowContext {windowContext}
{
    @autoreleasepool
    {
        [NSApplication sharedApplication];
    }
}

auto PlatformApplicationContextMac::MakePlatformApplication(PlatformApplication::Delegate delegate) -> Shared<PlatformApplication>
{
    return PlatformApplicationMac::Make(delegate, _self.Lock());
}

auto PlatformApplicationContextMac::Schedule() -> AsyncTask<void>
{
    co_await _mainThread->Schedule();
}

auto PlatformApplicationContextMac::ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>
{
    co_await _mainThread->ScheduleAfter(delay);
}

auto Locator::Resolver<PlatformApplicationContextMac>::Resolve() -> Shared<PlatformApplicationContextMac>
{
    auto mainThread = Locator::Resolve<PlatformMainThread>();
    auto windowContext = Locator::Resolve<PlatformWindowContext>();
    return PlatformApplicationContextMac::Make(mainThread, windowContext);
}
}
