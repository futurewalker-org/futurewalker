#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Async.Async.hpp>
#include <Futurewalker.Async.AsyncFunction.hpp>
#include <Futurewalker.Async.ThreadPool.hpp>

using namespace Futurewalker;

auto Func3() -> Async<SInt32>
{
    co_return 24;
}

auto Func2() -> Async<SInt32>
{
    co_return 42;
}

auto Func1() -> Async<SInt32>
{
    co_return co_await Func2() + co_await Func3();
}

auto BadFunc() -> Async<SInt32>
{
    auto a = Func1();
    // We want to ban this kind of code to enforce structured concurrency,
    // but it is currently allowed due to language limitation.
    auto r1 = co_await std::move(a);
    auto r2 = co_await std::move(a);
    co_return r1 + r2;
}

auto ThreadFunc(ThreadPool& pool) -> AsyncTask<SInt32>
{
    co_await pool.Schedule();
    co_return 42;
}

TEST_CASE("Async")
{
    STATIC_REQUIRE(!std::is_default_constructible_v<Async<SInt32>>);
    STATIC_REQUIRE(!std::is_copy_constructible_v<Async<SInt32>>);
    STATIC_REQUIRE(!std::is_move_constructible_v<Async<SInt32>>);

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
}
