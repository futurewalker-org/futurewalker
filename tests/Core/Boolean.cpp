#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Core.Primitive.hpp>

using namespace Futurewalker;

TEST_CASE("Boolean")
{
    auto const t = Bool(true);
    REQUIRE(t);
    REQUIRE(!!t);
    REQUIRE(t == true);
    REQUIRE(!t == false);
    REQUIRE(t != false);
    REQUIRE(!t != true);

    auto const f = Bool(false);
    REQUIRE_FALSE(f);
    REQUIRE(!f);
    REQUIRE(f == false);
    REQUIRE(!f == true);
    REQUIRE(f != true);
    REQUIRE(!f != false);
}
