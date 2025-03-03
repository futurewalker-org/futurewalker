#include <catch2/catch_all.hpp>

#include <Futurewalker.Graphics.Color.hpp>

using namespace Futurewalker;

TEST_CASE("RGBColor")
{
    auto c = RGBColor(0, 0.5, 1);
    REQUIRE(c.GetRed() == 0);
    REQUIRE(c.GetGreen() == 0.5);
    REQUIRE(c.GetBlue() == 1);
    REQUIRE(c == c);
    REQUIRE(c != RGBColor(0, 0, 0));
}
