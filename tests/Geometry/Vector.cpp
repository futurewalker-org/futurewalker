#include <catch2/catch_all.hpp>

#include <Futurewalker.Geometry.hpp>

#include <Futurewalker.Unit.hpp>

using namespace Futurewalker;

TEST_CASE("Vector")
{
    auto s = Vector2<Dp>(123, 45);
    REQUIRE(s.x == 123);
    REQUIRE(s.y == 45);
    REQUIRE(Vector2<Dp>::Max(Vector2<Dp>(1, 2), Vector2<Dp>(2, 1)) == Vector2<Dp>(2, 2));
    REQUIRE(Vector2<Dp>::Min(Vector2<Dp>(1, 2), Vector2<Dp>(2, 1)) == Vector2<Dp>(1, 1));
    REQUIRE(Vector2<Dp>::Round(Vector2<Dp>(0.4, 0.5)) == Vector2<Dp>(0, 1));
    REQUIRE(Vector2<Dp>::Floor(Vector2<Dp>(1.4, 1.5)) == Vector2<Dp>(1, 1));
    REQUIRE(Vector2<Dp>::Ceil(Vector2<Dp>(1.4, 1.5)) == Vector2<Dp>(2, 2));
    REQUIRE(!Vector2<Dp>(1, Dp::Infinity()).IsFinite());

    auto t = Vector2<Dp> {
        .x = 213,
        .y = 54,
    };
    REQUIRE(t.x == 213);
    REQUIRE(t.y == 54);
}