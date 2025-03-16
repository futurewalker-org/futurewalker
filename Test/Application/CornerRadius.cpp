#include <catch2/catch_all.hpp>

#include <Futurewalker.Application.CornerRadius.hpp>

using namespace Futurewalker;

TEST_CASE("CornerRadius")
{
    SECTION("Init")
    {
        auto const r = CornerRadius();
        REQUIRE(r.GetTopLeading() == 0);
        REQUIRE(r.GetTopTrailing() == 0);
        REQUIRE(r.GetBottomLeading() == 0);
        REQUIRE(r.GetBottomTrailing() == 0);
        REQUIRE(r == r);
        REQUIRE(!(r != r));
    }

    SECTION("Init")
    {
        auto const r = CornerRadius(1, 2, 3, 4);
        REQUIRE(r.GetTopLeading() == 1);
        REQUIRE(r.GetTopTrailing() == 2);
        REQUIRE(r.GetBottomTrailing() == 3);
        REQUIRE(r.GetBottomLeading() == 4);
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
        REQUIRE(r.GetTopLeading() == 3);
        REQUIRE(r.GetTopTrailing() == 3);
        REQUIRE(r.GetBottomLeading() == 3);
        REQUIRE(r.GetBottomTrailing() == 3);
    }
}
