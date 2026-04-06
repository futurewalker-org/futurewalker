#include <catch2/catch_all.hpp>

#include <Futurewalker.Geometry.hpp>

#include <Futurewalker.Unit.hpp>

using namespace Futurewalker;

TEST_CASE("Vector")
{
    auto s = Vector<Dp>(123, 45);
    REQUIRE(s.x == 123);
    REQUIRE(s.y == 45);
    REQUIRE(Vector<Dp>::Max(Vector<Dp>(1, 2), Vector<Dp>(2, 1)) == Vector<Dp>(2, 2));
    REQUIRE(Vector<Dp>::Min(Vector<Dp>(1, 2), Vector<Dp>(2, 1)) == Vector<Dp>(1, 1));
    REQUIRE(Vector<Dp>::Round(Vector<Dp>(0.4, 0.5)) == Vector<Dp>(0, 1));
    REQUIRE(Vector<Dp>::Floor(Vector<Dp>(1.4, 1.5)) == Vector<Dp>(1, 1));
    REQUIRE(Vector<Dp>::Ceil(Vector<Dp>(1.4, 1.5)) == Vector<Dp>(2, 2));
    REQUIRE(!Vector<Dp>(1, Dp::Infinity()).IsFinite());

    auto t = Vector<Dp> {
        .x = 213,
        .y = 54,
    };
    REQUIRE(t.x == 213);
    REQUIRE(t.y == 54);
}