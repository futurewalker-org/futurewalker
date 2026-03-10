#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Core.TimeInterval.hpp>

using namespace Futurewalker;

TEST_CASE("TimeInterval default constructor initializes to zero")
{
    auto const ti = TimeInterval();
    REQUIRE(ti.GetIntNanoseconds() == 0);
    REQUIRE(ti.GetIntMicroseconds() == 0);
    REQUIRE(ti.GetIntMilliseconds() == 0);
    REQUIRE(ti.GetIntSeconds() == 0);
    REQUIRE(ti.GetNanoseconds() == 0.0);
    REQUIRE(ti.GetMicroseconds() == 0.0);
    REQUIRE(ti.GetMilliseconds() == 0.0);
    REQUIRE(ti.GetSeconds() == 0.0);
}

TEST_CASE("TimeInterval::MakeFromNanoseconds")
{
    auto const ti = TimeInterval::MakeFromNanoseconds(123'456'789'012);
    REQUIRE(ti.GetIntNanoseconds() == 123'456'789'012);
    REQUIRE(ti.GetIntMicroseconds() == 123'456'789);
    REQUIRE(ti.GetIntMilliseconds() == 123'456);
    REQUIRE(ti.GetIntSeconds() == 123);
}

TEST_CASE("TimeInterval::MakeFromMicroseconds")
{
    auto const ti = TimeInterval::MakeFromMicroseconds(5'000'000);
    REQUIRE(ti.GetIntNanoseconds() == 5'000'000'000);
    REQUIRE(ti.GetIntMicroseconds() == 5'000'000);
    REQUIRE(ti.GetIntMilliseconds() == 5'000);
    REQUIRE(ti.GetIntSeconds() == 5);
}

TEST_CASE("TimeInterval::MakeFromMilliseconds")
{
    auto const ti = TimeInterval::MakeFromMilliseconds(2'500);
    REQUIRE(ti.GetIntNanoseconds() == 2'500'000'000);
    REQUIRE(ti.GetIntMicroseconds() == 2'500'000);
    REQUIRE(ti.GetIntMilliseconds() == 2'500);
    REQUIRE(ti.GetIntSeconds() == 2);
}

TEST_CASE("TimeInterval::MakeFromSeconds")
{
    auto const ti = TimeInterval::MakeFromSeconds(10);
    REQUIRE(ti.GetIntNanoseconds() == 10'000'000'000);
    REQUIRE(ti.GetIntMicroseconds() == 10'000'000);
    REQUIRE(ti.GetIntMilliseconds() == 10'000);
    REQUIRE(ti.GetIntSeconds() == 10);
}

TEST_CASE("TimeInterval float getters")
{
    auto const ti = TimeInterval::MakeFromNanoseconds(1'500'000'000);
    REQUIRE(ti.GetSeconds() == 1.5);
    REQUIRE(ti.GetMilliseconds() == 1'500.0);
    REQUIRE(ti.GetMicroseconds() == 1'500'000.0);
    REQUIRE(ti.GetNanoseconds() == 1'500'000'000.0);
}

TEST_CASE("TimeInterval negative values")
{
    auto const ti = TimeInterval::MakeFromSeconds(-3);
    REQUIRE(ti.GetIntSeconds() == -3);
    REQUIRE(ti.GetIntNanoseconds() == -3'000'000'000);
    REQUIRE(ti.GetSeconds() == -3.0);
}

TEST_CASE("TimeInterval operator+=")
{
    auto ti = TimeInterval::MakeFromSeconds(2);
    ti += TimeInterval::MakeFromSeconds(3);
    REQUIRE(ti.GetIntSeconds() == 5);
}

TEST_CASE("TimeInterval operator-=")
{
    auto ti = TimeInterval::MakeFromSeconds(5);
    ti -= TimeInterval::MakeFromSeconds(3);
    REQUIRE(ti.GetIntSeconds() == 2);
}

TEST_CASE("TimeInterval operator*=")
{
    auto ti = TimeInterval::MakeFromNanoseconds(6);
    ti *= TimeInterval::MakeFromNanoseconds(7);
    REQUIRE(ti.GetIntNanoseconds() == 42);
}

TEST_CASE("TimeInterval operator/=")
{
    auto ti = TimeInterval::MakeFromNanoseconds(42);
    ti /= TimeInterval::MakeFromNanoseconds(7);
    REQUIRE(ti.GetIntNanoseconds() == 6);
}

TEST_CASE("TimeInterval operator%=")
{
    auto ti = TimeInterval::MakeFromNanoseconds(10);
    ti %= TimeInterval::MakeFromNanoseconds(3);
    REQUIRE(ti.GetIntNanoseconds() == 1);
}

TEST_CASE("TimeInterval unary operator-")
{
    auto const ti = TimeInterval::MakeFromSeconds(5);
    auto const neg = -ti;
    REQUIRE(neg.GetIntSeconds() == -5);
    REQUIRE(neg.GetIntNanoseconds() == -5'000'000'000);
}

TEST_CASE("TimeInterval unary operator+")
{
    auto const ti = TimeInterval::MakeFromSeconds(5);
    auto const pos = +ti;
    REQUIRE(pos.GetIntSeconds() == 5);
}

TEST_CASE("TimeInterval unary minus of negative is positive")
{
    auto const ti = TimeInterval::MakeFromSeconds(-3);
    auto const neg = -ti;
    REQUIRE(neg.GetIntSeconds() == 3);
}

TEST_CASE("TimeInterval binary operator+")
{
    auto const a = TimeInterval::MakeFromMilliseconds(100);
    auto const b = TimeInterval::MakeFromMilliseconds(200);
    auto const c = a + b;
    REQUIRE(c.GetIntMilliseconds() == 300);
}

TEST_CASE("TimeInterval binary operator-")
{
    auto const a = TimeInterval::MakeFromMilliseconds(500);
    auto const b = TimeInterval::MakeFromMilliseconds(200);
    auto const c = a - b;
    REQUIRE(c.GetIntMilliseconds() == 300);
}

TEST_CASE("TimeInterval binary operator*")
{
    auto const a = TimeInterval::MakeFromNanoseconds(5);
    auto const b = TimeInterval::MakeFromNanoseconds(4);
    auto const c = a * b;
    REQUIRE(c.GetIntNanoseconds() == 20);
}

TEST_CASE("TimeInterval binary operator/")
{
    auto const a = TimeInterval::MakeFromNanoseconds(20);
    auto const b = TimeInterval::MakeFromNanoseconds(4);
    auto const c = a / b;
    REQUIRE(c.GetIntNanoseconds() == 5);
}

TEST_CASE("TimeInterval binary operator%")
{
    auto const a = TimeInterval::MakeFromNanoseconds(17);
    auto const b = TimeInterval::MakeFromNanoseconds(5);
    auto const c = a % b;
    REQUIRE(c.GetIntNanoseconds() == 2);
}

TEST_CASE("TimeInterval operator==")
{
    auto const a = TimeInterval::MakeFromSeconds(1);
    auto const b = TimeInterval::MakeFromMilliseconds(1'000);
    auto const c = TimeInterval::MakeFromSeconds(2);
    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

TEST_CASE("TimeInterval operator<=>")
{
    auto const a = TimeInterval::MakeFromSeconds(1);
    auto const b = TimeInterval::MakeFromSeconds(2);
    auto const c = TimeInterval::MakeFromSeconds(1);

    REQUIRE(a < b);
    REQUIRE(b > a);
    REQUIRE(a <= b);
    REQUIRE(b >= a);
    REQUIRE(a <= c);
    REQUIRE(a >= c);
    REQUIRE_FALSE(a > b);
    REQUIRE_FALSE(b < a);
}

TEST_CASE("TimeInterval zero comparisons")
{
    auto const zero = TimeInterval();
    auto const pos = TimeInterval::MakeFromNanoseconds(1);
    auto const neg = TimeInterval::MakeFromNanoseconds(-1);

    REQUIRE(neg < zero);
    REQUIRE(zero < pos);
    REQUIRE(neg < pos);
}

TEST_CASE("TimeInterval compound operations do not modify originals")
{
    auto const original = TimeInterval::MakeFromSeconds(5);
    auto const result = original + TimeInterval::MakeFromSeconds(3);
    REQUIRE(original.GetIntSeconds() == 5);
    REQUIRE(result.GetIntSeconds() == 8);
}

TEST_CASE("TimeInterval integer truncation on getters")
{
    auto const ti = TimeInterval::MakeFromNanoseconds(1'999'999'999);
    REQUIRE(ti.GetIntSeconds() == 1);
    REQUIRE(ti.GetIntMilliseconds() == 1'999);
    REQUIRE(ti.GetIntMicroseconds() == 1'999'999);
    REQUIRE(ti.GetIntNanoseconds() == 1'999'999'999);
}

TEST_CASE("TimeInterval mixed unit arithmetic")
{
    auto const sec = TimeInterval::MakeFromSeconds(1);
    auto const ms = TimeInterval::MakeFromMilliseconds(500);
    auto const sum = sec + ms;
    REQUIRE(sum.GetIntMilliseconds() == 1'500);
    REQUIRE(sum.GetIntNanoseconds() == 1'500'000'000);
}

TEST_CASE("TimeInterval factory method equivalences")
{
    REQUIRE(TimeInterval::MakeFromSeconds(1) == TimeInterval::MakeFromMilliseconds(1'000));
    REQUIRE(TimeInterval::MakeFromMilliseconds(1) == TimeInterval::MakeFromMicroseconds(1'000));
    REQUIRE(TimeInterval::MakeFromMicroseconds(1) == TimeInterval::MakeFromNanoseconds(1'000));
    REQUIRE(TimeInterval::MakeFromSeconds(1) == TimeInterval::MakeFromNanoseconds(1'000'000'000));
}
