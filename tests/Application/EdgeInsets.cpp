#include <catch2/catch_all.hpp>

#include <Futurewalker.Application.EdgeInsets.hpp>

using namespace Futurewalker;

TEST_CASE("EdgeInsets")
{
    SECTION("Init")
    {
        auto const e = EdgeInsets();
        REQUIRE(e.leading == 0);
        REQUIRE(e.trailing == 0);
        REQUIRE(e.top == 0);
        REQUIRE(e.bottom == 0);
        REQUIRE(e == e);
        REQUIRE(!(e != e));
    }

    SECTION("Init")
    {
        auto const e = EdgeInsets(1, 2, 3, 4);
        REQUIRE(e.leading == 1);
        REQUIRE(e.top == 2);
        REQUIRE(e.trailing == 3);
        REQUIRE(e.bottom == 4);
    }

    SECTION("Assign")
    {
        auto e = EdgeInsets();
        e = EdgeInsets::MakeUniform(1);
        REQUIRE(e == EdgeInsets::MakeUniform(1));
    }

    SECTION("Symmetric")
    {
        auto const e = EdgeInsets::MakeSymmetric(1, 2);
        REQUIRE(e.leading == 1);
        REQUIRE(e.trailing == 1);
        REQUIRE(e.top == 2);
        REQUIRE(e.bottom == 2);
    }

    SECTION("Uniform")
    {
        auto const e = EdgeInsets::MakeUniform(1);
        REQUIRE(e.leading == 1);
        REQUIRE(e.trailing == 1);
        REQUIRE(e.top == 1);
        REQUIRE(e.bottom == 1);
    }
}
