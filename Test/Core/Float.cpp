#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Core.Primitive.hpp>

using namespace Futurewalker;

TEST_CASE("Float")
{
    SECTION("Constructor")
    {
        SECTION("Conversion")
        {
            STATIC_REQUIRE(sizeof(Float32) == sizeof(float32_t));
            STATIC_REQUIRE(alignof(Float32) == alignof(float32_t));
            STATIC_REQUIRE(!Concepts::DefaultConstructible<Float32>);
            STATIC_REQUIRE(Concepts::NarrowConvertibleTo<int16_t, Float32>);
            STATIC_REQUIRE(Concepts::NarrowConvertibleTo<float32_t, Float32>);
            STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<int32_t, Float32>);
            STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<int64_t, Float32>);
            STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<float64_t, Float32>);
            STATIC_REQUIRE(Concepts::NarrowConvertibleTo<Float32, Float64>);
            STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<Float64, Float32>);
            STATIC_REQUIRE(Concepts::ExplicitlyConvertibleTo<float32_t, Float64>);
            STATIC_REQUIRE(Concepts::ExplicitlyConvertibleTo<Float64, Float32>);
            STATIC_REQUIRE(Concepts::ImplicitlyConvertibleTo<Float32, Float32>);
            STATIC_REQUIRE(Concepts::ImplicitlyConvertibleTo<Float32, Float64>);
            STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<Float64, Float32>);
            STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<Float32, Float<float32_t, int>>);
            STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<Float32, Float<float64_t, int>>);
            STATIC_REQUIRE(Concepts::ExplicitlyConvertibleTo<Float32, Float<float32_t, int>>);
            STATIC_REQUIRE(Concepts::ExplicitlyConvertibleTo<Float32, Float<float64_t, int>>);
        }

        SECTION("Types")
        {
            STATIC_REQUIRE(Concepts::SameAs<typename Float32::ValueType, float32_t>);
            STATIC_REQUIRE(Concepts::SameAs<typename Float32::TagType, void>);
        }

        SECTION("Construction")
        {
            auto const f = Float32(3.14f);
            REQUIRE(Float32::IsFinite(f));
            REQUIRE(!Float32::IsNaN(f));
            REQUIRE(!Float32::IsInfinite(f));
            REQUIRE(f == 3.14f);
        }
    }

    SECTION("Unary operators")
    {
        auto const f = Float32(1);
        REQUIRE(-f == -1.f);
        REQUIRE(+f == +1.f);
        REQUIRE(f == 1.f);
    }

    SECTION("Operators")
    {
        Float64 i = 42;

        SECTION("operator+=")
        {
            i += 24;
            REQUIRE(i == 66);
        }

        SECTION("operator+")
        {
            REQUIRE(i + 24 == 66);
        }

        SECTION("operator-=")
        {
            i -= 24;
            REQUIRE(i == 18);
        }

        SECTION("operator-")
        {
            REQUIRE(i - 24 == 18);
        }

        SECTION("operator*=")
        {
            i *= 10;
            REQUIRE(i == 420);
        }

        SECTION("operator*")
        {
            REQUIRE(i * 10 == 420);
        }

        SECTION("operator/=")
        {
            i /= 42;
            REQUIRE(i == 1);
        }

        SECTION("operator/")
        {
            REQUIRE(i / 42 == 1);
        }
    }

    SECTION("MinMax")
    {
        auto const f = Float32(42.0);
        REQUIRE(Float32::Min(f, 3.14f) == 3.14f);
        REQUIRE(Float32::Max(f, 3.14f) == 42.0f);
        REQUIRE(Float32::MinMax(f, 3.14f).first == 3.14f);
        REQUIRE(Float32::MinMax(f, 3.14f).second == 42.0f);
    }
}
