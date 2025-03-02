#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.Optional.hpp>

using namespace Futurewalker;

TEST_CASE("Optional")
{
    SECTION("Default")
    {
        Optional<int> opt;
        REQUIRE(!opt.HasValue());

        opt = {};
        REQUIRE(!opt.HasValue());
    }

    SECTION("int")
    {
        Optional<int> opt = 42;
        REQUIRE(opt.HasValue());
        REQUIRE(*opt == 42);

        opt = 24;
        REQUIRE(*opt == 24);

        opt = Optional<int>();
        REQUIRE(!opt.HasValue());
    }

    SECTION("vector<int>")
    {
        Optional<std::vector<int>> opt = {};
        REQUIRE(!opt.HasValue());
        opt = {};
        REQUIRE(!opt.HasValue());

        opt = std::vector {1, 2, 3};
        REQUIRE(opt.HasValue());
        REQUIRE(opt->size() == 3);

        std::vector vec = {4, 5, 6, 7};
        opt = vec;
        REQUIRE(opt->size() == 4);

        opt = std::move(vec);
        REQUIRE(vec.empty());
        REQUIRE(opt->size() == 4);
    }
}
