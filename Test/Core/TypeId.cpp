#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.TypeId.hpp>

using namespace Futurewalker;

TEST_CASE("TypeId")
{
    SECTION("GetId")
    {
        REQUIRE(TypeId::Get<int>() != TypeId(0U));
        REQUIRE(TypeId::Get<int>() == TypeId::Get<const int>());
        REQUIRE(TypeId::Get<double>() != TypeId::Get<int>());
    }

    SECTION("Get")
    {
        REQUIRE(TypeId::Get<int>() != 0U);
        REQUIRE(TypeId::Get<int>() == TypeId::Get<const int>());
        REQUIRE(TypeId::Get<double>() != TypeId::Get<int>());
    }

    SECTION("hash")
    {
        std::unordered_map<TypeId, int> map;
        map[TypeId::Get<int>()] = 42;
    }
}
