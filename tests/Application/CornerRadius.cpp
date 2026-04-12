#include <catch2/catch_all.hpp>

#include <Futurewalker.Application.CornerRadius.hpp>

using namespace Futurewalker;

TEST_CASE("CornerRadius")
{
    SECTION("Init")
    {
        auto const r = CornerRadius();
        REQUIRE(r.topLeading == 0);
        REQUIRE(r.topTrailing == 0);
        REQUIRE(r.bottomLeading == 0);
        REQUIRE(r.bottomTrailing == 0);
        REQUIRE(r == r);
        REQUIRE(!(r != r));
    }

    SECTION("Init")
    {
        auto const r = CornerRadius(1, 2, 3, 4);
        REQUIRE(r.topLeading == 1);
        REQUIRE(r.topTrailing == 2);
        REQUIRE(r.bottomTrailing == 3);
        REQUIRE(r.bottomLeading == 4);
    }

    SECTION("Assign")
    {
        auto r = CornerRadius();
        r = CornerRadius::MakeUniform(1);
        REQUIRE(r == CornerRadius::MakeUniform(1));
    }

    SECTION("Uniform")
    {
        auto const r = CornerRadius::MakeUniform(3);
        REQUIRE(r.topLeading == 3);
        REQUIRE(r.topTrailing == 3);
        REQUIRE(r.bottomLeading == 3);
        REQUIRE(r.bottomTrailing == 3);
    }
}
