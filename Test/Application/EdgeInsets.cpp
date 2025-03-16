#include <catch2/catch_all.hpp>

#include <Futurewalker.Application.EdgeInsets.hpp>

using namespace Futurewalker;

TEST_CASE("EdgeInsets")
{
    SECTION("Init")
    {
        auto const e = EdgeInsets();
        REQUIRE(e.GetLeading() == 0);
        REQUIRE(e.GetTrailing() == 0);
        REQUIRE(e.GetTop() == 0);
        REQUIRE(e.GetBottom() == 0);
        REQUIRE(e == e);
        REQUIRE(!(e != e));
    }

    SECTION("Init")
    {
        auto const e = EdgeInsets(1, 2, 3, 4);
        REQUIRE(e.GetLeading() == 1);
        REQUIRE(e.GetTop() == 2);
        REQUIRE(e.GetTrailing() == 3);
        REQUIRE(e.GetBottom() == 4);
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
        REQUIRE(e.GetLeading() == 1);
        REQUIRE(e.GetTrailing() == 1);
        REQUIRE(e.GetTop() == 2);
        REQUIRE(e.GetBottom() == 2);
    }

    SECTION("Uniform")
    {
        auto const e = EdgeInsets::MakeUniform(1);
        REQUIRE(e.GetLeading() == 1);
        REQUIRE(e.GetTrailing() == 1);
        REQUIRE(e.GetTop() == 1);
        REQUIRE(e.GetBottom() == 1);
    }
}
