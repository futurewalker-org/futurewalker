#include <catch2/catch_all.hpp>

#include "Futurewalker.Attribute.AttributeValue.hpp"

using namespace Futurewalker;

TEST_CASE("AttributeValue")
{
    SECTION("Init")
    {
        auto val = AttributeValue(SInt32(42));
        REQUIRE(*val.GetValue<SInt32>() == 42);
        REQUIRE(!val.GetValue<SInt32 const>()); // FIXME: might be better to ignore cv.
    }

    SECTION("Copy")
    {
        auto v1 = AttributeValue(SInt32(42));
        auto v2 = v1;
        REQUIRE(v1 == v2);
        REQUIRE(!(v1 != v2));
        REQUIRE(*v1.GetValue<SInt32>() == 42);
        REQUIRE(*v2.GetValue<SInt32>() == 42);
    }

    SECTION("Move")
    {
        auto v1 = AttributeValue(SInt32(42));
        auto v2 = std::move(v1);
        REQUIRE(v1 == v2);
        REQUIRE(!(v1 != v2));
        REQUIRE(*v1.GetValue<SInt32>() == 42);
        REQUIRE(*v2.GetValue<SInt32>() == 42);
    }

    SECTION("Assign")
    {
        auto v1 = AttributeValue(SInt32(42));
        auto v2 = AttributeValue(Float64(3.14));
        v1 = v2;
        REQUIRE(*v1.GetValue<Float64>() == 3.14);
        v2 = v2;
        REQUIRE(*v2.GetValue<Float64>() == 3.14);
    }
}
