#include <catch2/catch_all.hpp>

#include <Futurewalker.Geometry.hpp>

#include <Futurewalker.Unit.hpp>

using namespace Futurewalker;

TEST_CASE("Point")
{
    auto s = Point<Dp>(123, 45);
    REQUIRE(s.x == 123);
    REQUIRE(s.y == 45);
    REQUIRE(Point<Dp>::Max(Point<Dp>(1, 2), Point<Dp>(2, 1)) == Point<Dp>(2, 2));
    REQUIRE(Point<Dp>::Min(Point<Dp>(1, 2), Point<Dp>(2, 1)) == Point<Dp>(1, 1));
    REQUIRE(Point<Dp>::Round(Point<Dp>(0.4, 0.5)) == Point<Dp>(0, 1));
    REQUIRE(Point<Dp>::Floor(Point<Dp>(1.4, 1.5)) == Point<Dp>(1, 1));
    REQUIRE(Point<Dp>::Ceil(Point<Dp>(1.4, 1.5)) == Point<Dp>(2, 2));
    REQUIRE(!Point<Dp>(1, Dp::Infinity()).IsFinite());
    REQUIRE(Point<Dp>::DistanceBetween(Point<Dp>(-1, -1), Point<Dp>(2, 3)) == 5);

    auto t = Point<Dp> {
        .x = 213,
        .y = 54,
    };
    REQUIRE(t.x == 213);
    REQUIRE(t.y == 54);
}
