#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Async.Task.hpp>
#include <Futurewalker.Async.AsyncFunction.hpp>

using namespace Futurewalker;
using namespace FW_DETAIL_NS;

TEST_CASE("Task")
{
    SECTION("Constructor")
    {
        auto task = Task<void>();
        REQUIRE_THROWS(AsyncFunction::SyncWait(std::move(task)));
    }

    SECTION("Move")
    {
        auto task = []() -> Task<SInt32> { co_return 42; }();
        auto task2 = std::move(task);
        REQUIRE_THROWS(AsyncFunction::SyncWait(std::move(task)));
        REQUIRE(AsyncFunction::SyncWait(std::move(task2)) == 42);
    }

    SECTION("Empty")
    {
        auto task = []() -> Task<void> { co_return; }();
        REQUIRE_NOTHROW(AsyncFunction::SyncWait(std::move(task)));
    }

    SECTION("")
    {
        auto task = []() -> Task<SInt32> { co_return 42; }();
        REQUIRE(AsyncFunction::SyncWait(std::move(task)) == 42);
    }

    SECTION("")
    {
        auto task1 = []() -> Task<SInt32> { co_return 42; }();
        auto task2 = []() -> Task<SInt32> { co_return 24; }();
        auto task3 = [&]() -> Task<SInt32> { co_return co_await std::move(task1) + co_await std::move(task2); }();
        REQUIRE(AsyncFunction::SyncWait(std::move(task3)) == 66);
    }

    SECTION("")
    {
        REQUIRE_THROWS(AsyncFunction::SyncWaitFn([]() -> Task<void> {
            throw Exception(ErrorCode::Failure);
            co_return;
        }));
    }

    SECTION("")
    {
        auto task = []() -> Task<void> {
            throw Exception(ErrorCode::Failure);
            co_return;
        }();

        REQUIRE_THROWS(AsyncFunction::SyncWaitFn([&]() -> Task<void> { co_await std::move(task); }));
    }

    SECTION("Cancel")
    {
        auto handle = AsyncFunction::SpawnFn([]() -> Task<SInt32> { co_return 42; });
        handle.Cancel();
        REQUIRE(*(handle.SyncWait()) == 42);
    }
}
