#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Async.ThreadPool.hpp>
#include <Futurewalker.Async.AsyncFunction.hpp>
#include <Futurewalker.Async.LazyTask.hpp>

#include <thread>

using namespace Futurewalker;

TEST_CASE("ThreadPool")
{
    auto const pool = Locator::ResolveWithDefault<ThreadPool>();

    SECTION("Schedule")
    {
        AsyncFunction::SyncWaitFn([=]() -> LazyTask<void> {
            auto const tid1 = std::this_thread::get_id();
            co_await pool->Schedule();
            auto const tid2 = std::this_thread::get_id();
            REQUIRE(tid1 != tid2);
        });
    }

    SECTION("ScheduleAfter")
    {
        AsyncFunction::SyncWaitFn([=]() -> LazyTask<void> {
            auto const tid1 = std::this_thread::get_id();
            co_await pool->ScheduleAfter(std::chrono::milliseconds(100));
            auto const tid2 = std::this_thread::get_id();
            REQUIRE(tid1 != tid2);
        });
    }

    SECTION("Cancellation")
    {
        auto h = AsyncFunction::SpawnFn([=]() -> LazyTask<void> { co_await pool->ScheduleAfter(std::chrono::seconds(1000)); });
        h.Cancel();
        REQUIRE(!h.WaitFor(std::chrono::seconds(1)));
    }
}
