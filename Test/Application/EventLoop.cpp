#include <catch2/catch_all.hpp>

#include <Futurewalker.Application.PlatformEventLoopContext.hpp>
#include <Futurewalker.Application.PlatformEventLoop.hpp>

#include <Futurewalker.Application.Win.PlatformEventLoopContextWin.hpp>
#include <Futurewalker.Base.Win.PlatformInstanceHandleWin.hpp>

#include <Futurewalker.Async.ThreadPool.hpp>
#include <Futurewalker.Async.AsyncFunction.hpp>

using namespace Futurewalker;

TEST_CASE("EventLoop")
{
    Locator::Register<PlatformInstanceHandleWin>(::GetModuleHandle(0));
    Locator::Register<PlatformEventLoopContextWin>();
    Locator::Register<ThreadPool>();

    auto eventLoopContext = Locator::Resolve<PlatformEventLoopContext>();
    REQUIRE(eventLoopContext);

    auto eventLoop = eventLoopContext->GetEventLoopForThread();
    REQUIRE(eventLoop);

    SECTION("Exit")
    {
        auto start = 0;
        auto exit = 0;

        AsyncFunction::SyncWait(eventLoop->Run({
          .startTask = [&]() -> Async<void> {
              ++start;
              eventLoop->Exit();
              co_return;
          },
          .exitTask = [&]() -> Async<void> {
              ++exit;
              co_return;
          },
        }));

        REQUIRE(start == 1);
        REQUIRE(exit == 1);
    }
}
