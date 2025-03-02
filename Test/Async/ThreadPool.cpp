#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Async.ThreadPool.hpp>
#include <Futurewalker.Async.AsyncFunction.hpp>
#include <Futurewalker.Async.Task.hpp>

#include <Futurewalker.Base.Win.PlatformDebugWin.hpp>

#include <thread>
#include <iostream>

using namespace Futurewalker;

class PlatformDebugTest : public PlatformDebug
{
public:
    auto Print(const StringView message) -> void override
    {
        auto str = std::string((const char*)(const char8_t*)message.GetData(), (size_t)message.GetSize());
        std::cerr << str << std::endl;
    }

    auto Break() -> void override
    {
    }
};

template <>
struct Locator::Resolver<PlatformDebugTest>
{
    using Interface = PlatformDebug;
    static auto Resolve() -> Shared<PlatformDebugTest>
    {
        return Shared<PlatformDebugTest>::Make();
    }
};

TEST_CASE("ThreadPool")
{
    auto debug = Locator::ResolveWithDefault<PlatformDebug, PlatformDebugTest>();
    auto pool = Locator::ResolveWithDefault<ThreadPool>();

    SECTION("Schedule")
    {
        AsyncFunction::SyncWaitFn([=] -> Task<void> {
            const auto tid1 = std::this_thread::get_id();
            co_await pool->Schedule();
            const auto tid2 = std::this_thread::get_id();
            REQUIRE(tid1 != tid2);
        });
    }

    SECTION("ScheduleAfter")
    {
        AsyncFunction::SyncWaitFn([=] -> Task<void> {
            const auto tid1 = std::this_thread::get_id();
            co_await pool->ScheduleAfter(std::chrono::milliseconds(100));
            const auto tid2 = std::this_thread::get_id();
            REQUIRE(tid1 != tid2);
        });
    }

    SECTION("Cancellation")
    {
        auto h = AsyncFunction::SpawnFn([=] -> Task<void> { co_await pool->ScheduleAfter(std::chrono::seconds(1000)); });
        h.Cancel();
        REQUIRE(!h.WaitFor(std::chrono::seconds(1)));
    }
}
