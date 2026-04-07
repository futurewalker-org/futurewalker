#include <catch2/catch_all.hpp>

#include <Futurewalker.Geometry.hpp>

#include <Futurewalker.Unit.hpp>

using namespace Futurewalker;

TEST_CASE("Radius")
{
    auto r = Radius<Dp>(123, 45);
    REQUIRE(r.x == 123);
    REQUIRE(r.y == 45);
    REQUIRE(Radius<Dp>::Max(Radius<Dp>(1, 2), Radius<Dp>(2, 1)) == Radius<Dp>(2, 2));
    REQUIRE(Radius<Dp>::Min(Radius<Dp>(1, 2), Radius<Dp>(2, 1)) == Radius<Dp>(1, 1));
    REQUIRE(Radius<Dp>::Round(Radius<Dp>(0.4, 0.5)) == Radius<Dp>(0, 1));
    REQUIRE(Radius<Dp>::Floor(Radius<Dp>(1.4, 1.5)) == Radius<Dp>(1, 1));
    REQUIRE(Radius<Dp>::Ceil(Radius<Dp>(1.4, 1.5)) == Radius<Dp>(2, 2));
    REQUIRE(!Radius<Dp>(1, Dp::Infinity()).IsFinite());
    REQUIRE(Radius<Dp>(1, 2).IsFinite());
}

TEST_CASE("Radius IsNearlyEquals")
{
    REQUIRE(Radius<Dp>::IsNearlyEquals(Radius<Dp>(1.0, 2.0), Radius<Dp>(1.0, 2.0)));
    REQUIRE(Radius<Dp>::IsNearlyEquals(Radius<Dp>(1.0, 2.0), Radius<Dp>(1.0 + 1e-7, 2.0 - 1e-7)));
    REQUIRE(!Radius<Dp>::IsNearlyEquals(Radius<Dp>(1.0, 2.0), Radius<Dp>(1.1, 2.0)));
    REQUIRE(!Radius<Dp>::IsNearlyEquals(Radius<Dp>(1.0, 2.0), Radius<Dp>(1.0, 2.1)));
    REQUIRE(Radius<Dp>::IsNearlyEquals(Radius<Dp>(1.0, 2.0), Radius<Dp>(1.05, 2.0), 0.1));
}

TEST_CASE("Radius IsNearlyEqualRelative")
{
    REQUIRE(Radius<Dp>::IsNearlyEqualRelative(Radius<Dp>(1.0, 2.0), Radius<Dp>(1.0, 2.0)));
    REQUIRE(Radius<Dp>::IsNearlyEqualRelative(Radius<Dp>(1000.0, 2000.0), Radius<Dp>(1000.0 + 1e-4, 2000.0 - 1e-4)));
    REQUIRE(!Radius<Dp>::IsNearlyEqualRelative(Radius<Dp>(1.0, 2.0), Radius<Dp>(3.0, 2.0)));
    REQUIRE(!Radius<Dp>::IsNearlyEqualRelative(Radius<Dp>(1.0, 2.0), Radius<Dp>(1.0, 4.0)));
    // Verify x and y are compared independently (not swapped)
    REQUIRE(!Radius<Dp>::IsNearlyEqualRelative(Radius<Dp>(1.0, 100.0), Radius<Dp>(100.0, 1.0)));
}
