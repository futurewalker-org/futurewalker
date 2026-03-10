#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Core.MonotonicTime.hpp>

using namespace Futurewalker;

using TP = MonotonicTime;

TEST_CASE("TimePoint default constructor")
{
    auto const tp = TP();
    auto const tp2 = TP();
    REQUIRE(tp == tp2);
}

TEST_CASE("TimePoint::MakeFromNanoseconds")
{
    auto const tp = TP::MakeFromNanoseconds(123'456'789'012);
    auto const expected = TP::MakeFromNanoseconds(123'456'789'012);
    REQUIRE(tp == expected);
}

TEST_CASE("TimePoint::MakeFromMicroseconds")
{
    auto const tp = TP::MakeFromMicroseconds(5'000'000);
    auto const expected = TP::MakeFromNanoseconds(5'000'000'000);
    REQUIRE(tp == expected);
}

TEST_CASE("TimePoint::MakeFromMilliseconds")
{
    auto const tp = TP::MakeFromMilliseconds(2'500);
    auto const expected = TP::MakeFromNanoseconds(2'500'000'000);
    REQUIRE(tp == expected);
}

TEST_CASE("TimePoint::MakeFromSeconds")
{
    auto const tp = TP::MakeFromSeconds(10);
    auto const expected = TP::MakeFromNanoseconds(10'000'000'000);
    REQUIRE(tp == expected);
}

TEST_CASE("TimePoint factory unit equivalences")
{
    REQUIRE(TP::MakeFromSeconds(1) == TP::MakeFromMilliseconds(1'000));
    REQUIRE(TP::MakeFromMilliseconds(1) == TP::MakeFromMicroseconds(1'000));
    REQUIRE(TP::MakeFromMicroseconds(1) == TP::MakeFromNanoseconds(1'000));
    REQUIRE(TP::MakeFromSeconds(1) == TP::MakeFromNanoseconds(1'000'000'000));
}

TEST_CASE("TimePoint operator== same values")
{
    auto const a = TP::MakeFromSeconds(5);
    auto const b = TP::MakeFromSeconds(5);
    REQUIRE(a == b);
}

TEST_CASE("TimePoint operator== different values")
{
    auto const a = TP::MakeFromSeconds(5);
    auto const b = TP::MakeFromSeconds(6);
    REQUIRE_FALSE(a == b);
}

TEST_CASE("TimePoint operator<=>")
{
    auto const a = TP::MakeFromSeconds(1);
    auto const b = TP::MakeFromSeconds(2);
    auto const c = TP::MakeFromSeconds(1);

    REQUIRE(a < b);
    REQUIRE(b > a);
    REQUIRE(a <= b);
    REQUIRE(b >= a);
    REQUIRE(a <= c);
    REQUIRE(a >= c);
    REQUIRE_FALSE(a > b);
    REQUIRE_FALSE(b < a);
}

TEST_CASE("TimePoint operator+= with TimeInterval")
{
    auto tp = TP::MakeFromSeconds(2);
    tp += TimeInterval::MakeFromSeconds(3);
    REQUIRE(tp == TP::MakeFromSeconds(5));
}

TEST_CASE("TimePoint operator-= with TimeInterval")
{
    auto tp = TP::MakeFromSeconds(5);
    tp -= TimeInterval::MakeFromSeconds(3);
    REQUIRE(tp == TP::MakeFromSeconds(2));
}

TEST_CASE("TimePoint + TimeInterval")
{
    auto const tp = TP::MakeFromSeconds(2);
    auto const result = tp + TimeInterval::MakeFromSeconds(3);
    REQUIRE(result == TP::MakeFromSeconds(5));
}

TEST_CASE("TimePoint - TimeInterval")
{
    auto const tp = TP::MakeFromSeconds(5);
    auto const result = tp - TimeInterval::MakeFromSeconds(3);
    REQUIRE(result == TP::MakeFromSeconds(2));
}

TEST_CASE("TimePoint - TimePoint yields TimeInterval")
{
    auto const a = TP::MakeFromSeconds(5);
    auto const b = TP::MakeFromSeconds(2);
    auto const interval = a - b;
    REQUIRE(interval == TimeInterval::MakeFromSeconds(3));
}

TEST_CASE("TimePoint - TimePoint negative interval")
{
    auto const a = TP::MakeFromSeconds(2);
    auto const b = TP::MakeFromSeconds(5);
    auto const interval = a - b;
    REQUIRE(interval == TimeInterval::MakeFromSeconds(-3));
}

TEST_CASE("TimePoint - same TimePoint yields zero interval")
{
    auto const tp = TP::MakeFromSeconds(7);
    auto const interval = tp - tp;
    REQUIRE(interval == TimeInterval::MakeFromNanoseconds(0));
}

TEST_CASE("TimePoint + TimeInterval does not modify original")
{
    auto const original = TP::MakeFromSeconds(5);
    auto const result = original + TimeInterval::MakeFromSeconds(3);
    REQUIRE(original == TP::MakeFromSeconds(5));
    REQUIRE(result == TP::MakeFromSeconds(8));
}

TEST_CASE("TimePoint - TimeInterval does not modify original")
{
    auto const original = TP::MakeFromSeconds(5);
    auto const result = original - TimeInterval::MakeFromSeconds(3);
    REQUIRE(original == TP::MakeFromSeconds(5));
    REQUIRE(result == TP::MakeFromSeconds(2));
}

TEST_CASE("TimePoint mixed unit arithmetic")
{
    auto const tp = TP::MakeFromSeconds(1);
    auto const result = tp + TimeInterval::MakeFromMilliseconds(500);
    REQUIRE(result == TP::MakeFromMilliseconds(1'500));
}

TEST_CASE("TimePoint adding zero interval is identity")
{
    auto const tp = TP::MakeFromSeconds(42);
    auto const result = tp + TimeInterval();
    REQUIRE(result == tp);
}

TEST_CASE("TimePoint subtracting zero interval is identity")
{
    auto const tp = TP::MakeFromSeconds(42);
    auto const result = tp - TimeInterval();
    REQUIRE(result == tp);
}

TEST_CASE("TimePoint default constructed equals MakeFromNanoseconds(0)")
{
    REQUIRE(TP() == TP::MakeFromNanoseconds(0));
}

TEST_CASE("TimePoint roundtrip via subtraction and addition")
{
    auto const base = TP::MakeFromSeconds(10);
    auto const offset = TimeInterval::MakeFromMilliseconds(2'500);
    auto const moved = base + offset;
    auto const recovered = moved - offset;
    REQUIRE(recovered == base);
}

TEST_CASE("TimePoint comparison at nanosecond precision")
{
    auto const a = TP::MakeFromNanoseconds(1'000'000'000);
    auto const b = TP::MakeFromNanoseconds(1'000'000'001);
    REQUIRE(a < b);
    REQUIRE_FALSE(a == b);
    REQUIRE((b - a) == TimeInterval::MakeFromNanoseconds(1));
}
