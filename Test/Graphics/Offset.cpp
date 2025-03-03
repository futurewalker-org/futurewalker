#include <catch2/catch_all.hpp>

#include <Futurewalker.Graphics.Offset.hpp>
#include <Futurewalker.Graphics.GraphicsUnits.hpp>

using namespace Futurewalker;

TEST_CASE("Offset")
{
    auto s = Offset<Dp>(123, 45);
    REQUIRE(s.GetDeltaX() == 123);
    REQUIRE(s.GetDeltaY() == 45);
    REQUIRE(Offset<Dp>::Max(Offset<Dp>(1, 2), Offset<Dp>(2, 1)) == Offset<Dp>(2, 2));
    REQUIRE(Offset<Dp>::Min(Offset<Dp>(1, 2), Offset<Dp>(2, 1)) == Offset<Dp>(1, 1));
    REQUIRE(Offset<Dp>::Round(Offset<Dp>(0.4, 0.5)) == Offset<Dp>(0, 1));
    REQUIRE(Offset<Dp>::Floor(Offset<Dp>(1.4, 1.5)) == Offset<Dp>(1, 1));
    REQUIRE(Offset<Dp>::Ceil(Offset<Dp>(1.4, 1.5)) == Offset<Dp>(2, 2));
    REQUIRE(!Offset<Dp>(1, Dp::Infinity()).IsFinite());
}