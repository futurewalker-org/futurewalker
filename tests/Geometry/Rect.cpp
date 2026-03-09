#include <catch2/catch_all.hpp>

#include <Futurewalker.Geometry.hpp>

#include <Futurewalker.Unit.hpp>

using namespace Futurewalker;

TEST_CASE("Rect")
{
    auto r = Rect2<Dp>(1, 2, 3, 4);
    REQUIRE(r.GetLeft() == 1);
    REQUIRE(r.GetTop() == 2);
    REQUIRE(r.GetRight() == 3);
    REQUIRE(r.GetBottom() == 4);

    r = Rect2<Dp>(Point<Dp>(1, 2), Size<Dp>(3, 4));
    REQUIRE(r.GetLeft() == 1);
    REQUIRE(r.GetTop() == 2);
    REQUIRE(r.GetRight() == 4);
    REQUIRE(r.GetBottom() == 6);

    REQUIRE(!Rect<Dp>::Infinite().IsFinite());
    REQUIRE(!Rect<Dp>(4, 3, 2, 1).IsSorted());
    REQUIRE(Rect<Dp>::Sort(Rect<Dp>(4, 3, 2, 1)) == Rect<Dp>(2, 1, 4, 3));
    REQUIRE(Rect<Dp>::Inflate(Rect<Dp>(1, 2, 3, 4), -1, 1) == Rect<Dp>(2, 1, 2, 5));
    REQUIRE(Rect<Dp>::Intersect(Rect<Dp>(0, 0, 2, 2), Rect<Dp>(1, 1, 3, 3)) == Rect<Dp>(1, 1, 2, 2));
    REQUIRE(Rect<Dp>::Intersect(Rect<Dp>(-5, -5, 1, 1), Point<Dp>(-1, 0)));
    REQUIRE(Rect<Dp>::Union(Rect<Dp>(-1, -1, 0, 0), Rect<Dp>(3, 3, 10, 10)) == Rect<Dp>(-1, -1, 10, 10));
}
