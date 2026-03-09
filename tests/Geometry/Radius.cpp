#include <catch2/catch_all.hpp>

#include <Futurewalker.Geometry.hpp>

#include <Futurewalker.Unit.hpp>

using namespace Futurewalker;

TEST_CASE("Radius")
{
    auto r = Radius<Dp>(123, 45);
    REQUIRE(r.GetX() == 123);
    REQUIRE(r.GetY() == 45);
    REQUIRE(Radius<Dp>::Max(Radius<Dp>(1, 2), Radius<Dp>(2, 1)) == Radius<Dp>(2, 2));
    REQUIRE(Radius<Dp>::Min(Radius<Dp>(1, 2), Radius<Dp>(2, 1)) == Radius<Dp>(1, 1));
    REQUIRE(Radius<Dp>::Round(Radius<Dp>(0.4, 0.5)) == Radius<Dp>(0, 1));
    REQUIRE(Radius<Dp>::Floor(Radius<Dp>(1.4, 1.5)) == Radius<Dp>(1, 1));
    REQUIRE(Radius<Dp>::Ceil(Radius<Dp>(1.4, 1.5)) == Radius<Dp>(2, 2));
    REQUIRE(!Radius<Dp>(1, Dp::Infinity()).IsFinite());
}