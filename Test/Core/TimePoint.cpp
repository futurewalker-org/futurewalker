#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Core.MonotonicTime.hpp>
#include <Futurewalker.Core.SystemTime.hpp>

using namespace Futurewalker;

TEST_CASE("TimePoint")
{
    SECTION("Default")
    {
        constexpr auto t = MonotonicTime();
        REQUIRE(t.GetIntervalSinceEpoch() == 0);
    }

    SECTION("Init")
    {
        auto t1 = MonotonicTime(10);
        REQUIRE(t1.GetIntervalSinceEpoch() == 10);
        auto t2 = t1;
        REQUIRE(t2.GetIntervalSinceEpoch() == 10);
        auto t3 = MonotonicTime();
        t3 = t2;
        REQUIRE(t3.GetIntervalSinceEpoch() == 10);
    }

    SECTION("Interval")
    {
        auto t1 = MonotonicTime(42);
        t1 += 100.5;
        REQUIRE(t1.GetIntervalSinceEpoch() == 142.5);
        t1 -= 100.5;
        REQUIRE(t1.GetIntervalSinceEpoch() == 42);
    }
}
