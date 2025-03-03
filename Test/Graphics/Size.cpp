#include <catch2/catch_all.hpp>

#include <Futurewalker.Graphics.Size.hpp>
#include <Futurewalker.Graphics.GraphicsUnits.hpp>

using namespace Futurewalker;

TEST_CASE("Size")
{
    auto s = Size<Dp>(123, 45);
    REQUIRE(s.GetWidth() == 123);
    REQUIRE(s.GetHeight() == 45);
    REQUIRE(Size<Dp>::Max(Size<Dp>(1, 2), Size<Dp>(2, 1)) == Size<Dp>(2, 2));
    REQUIRE(Size<Dp>::Min(Size<Dp>(1, 2), Size<Dp>(2, 1)) == Size<Dp>(1, 1));
    REQUIRE(Size<Dp>::Round(Size<Dp>(0.4, 0.5)) == Size<Dp>(0, 1));
    REQUIRE(Size<Dp>::Floor(Size<Dp>(1.4, 1.5)) == Size<Dp>(1, 1));
    REQUIRE(Size<Dp>::Ceil(Size<Dp>(1.4, 1.5)) == Size<Dp>(2, 2));
    REQUIRE(Size<Dp>(123) == Size<Dp>(123, 123));
    REQUIRE(Size<Dp>(-100, 100).IsEmpty());
    REQUIRE(Size<Dp>(-100, 0).IsEmpty());
    REQUIRE(Size<Dp>(-100, -100).IsEmpty());
    REQUIRE(Size<Dp>(0, 0).IsEmpty());
    REQUIRE(Size<Dp>(0, 1).IsEmpty());
    REQUIRE(!Size<Dp>(1, 1).IsEmpty());
    REQUIRE(!Size<Dp>(1, Dp::Infinity()).IsFinite());
    REQUIRE(Size<Dp>::Normalize(Size<Dp>(-100, 100)) == Size<Dp>(0, 100));
}
