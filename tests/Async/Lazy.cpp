#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Async.Lazy.hpp>
#include <Futurewalker.Async.Async.hpp>
#include <Futurewalker.Async.AsyncFunction.hpp>
#include <Futurewalker.Async.ThreadPool.hpp>

using namespace Futurewalker;

auto Func3() -> Lazy<SInt32>
{
    co_return 24;
}

auto Func2() -> Lazy<SInt32>
{
    co_return 42;
}

auto Func1() -> Lazy<SInt32>
{
    co_return co_await Func2() + co_await Func3();
}

auto BadFunc() -> Lazy<SInt32>
{
    auto a = Func1();
    auto r1 = co_await std::move(a);
    auto r2 = co_await std::move(a);
    co_return r1 + r2;
}

auto ThreadFunc(ThreadPool& pool) -> LazyTask<SInt32>
{
    co_await pool.Schedule();
    co_return 42;
}

TEST_CASE("Lazy")
{
    STATIC_REQUIRE(!std::is_default_constructible_v<Lazy<SInt32>>);
    STATIC_REQUIRE(!std::is_copy_constructible_v<Lazy<SInt32>>);
    STATIC_REQUIRE(std::is_move_constructible_v<Lazy<SInt32>>);

    SECTION("SingleThreaded")
    {
        REQUIRE(AsyncFunction::SyncWait(Func1()) == 66);
        REQUIRE_THROWS(AsyncFunction::SyncWait(BadFunc()));
    }

    SECTION("MultiThreaded")
    {
        auto pool = Shared<ThreadPool>::Make(4);
        REQUIRE(AsyncFunction::SyncWait(ThreadFunc(*pool)) == 42);
    }

    SECTION("Unstructured")
    {
        auto lazy = Func1();
        REQUIRE(AsyncFunction::SyncWait(std::move(lazy)) == 66);
        REQUIRE_THROWS(AsyncFunction::SyncWait(std::move(lazy)));
        lazy = Func1();
        REQUIRE(AsyncFunction::SyncWait(std::move(lazy)) == 66);
        REQUIRE_THROWS(AsyncFunction::SyncWait(std::move(lazy)));
        lazy = Func2();
        lazy = Func3();
        REQUIRE(AsyncFunction::SyncWait(std::move(lazy)) == 24);
        REQUIRE_THROWS(AsyncFunction::SyncWait(std::move(lazy)));
        lazy = Func1();
        auto moved = std::move(lazy);
        REQUIRE_THROWS(AsyncFunction::SyncWait(std::move(lazy)));
        REQUIRE(AsyncFunction::SyncWait(std::move(moved)) == 66);
        REQUIRE_THROWS(AsyncFunction::SyncWait(std::move(moved)));
    }

    SECTION("Conversion from Async")
    {
        auto async = []() -> Async<SInt32> { co_return 42; };
        REQUIRE(AsyncFunction::SyncWait(Lazy(async())) == 42);
    }
}
