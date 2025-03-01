#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.Concepts.hpp>

#include <vector>
#include <list>

using namespace Futurewalker;

TEST_CASE("Concepts")
{
    SECTION("NarrowConvertibleTo")
    {
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<int32_t, int32_t>);
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<int32_t, int64_t>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<int64_t, int32_t>);
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<uint32_t, uint32_t>);
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<uint32_t, uint64_t>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<uint64_t, uint32_t>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<uint32_t, int32_t>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<int32_t, uint32_t>);
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<uint32_t, int64_t>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<int32_t, uint64_t>);
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<float32_t, float32_t>);
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<float32_t, float64_t>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<float64_t, float32_t>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<int32_t, float32_t>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<float32_t, int32_t>);
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<int32_t, float64_t>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<float32_t, int64_t>);
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<uint32_t, float64_t>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<uint64_t, float64_t>);
    }

    SECTION("SpecializationOf")
    {
        STATIC_REQUIRE(Concepts::SpecializationOf<std::vector<int>, std::vector>);
        STATIC_REQUIRE(!Concepts::SpecializationOf<std::list<int>, std::vector>);
    }
}
