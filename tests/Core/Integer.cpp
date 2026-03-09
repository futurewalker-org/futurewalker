#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Core.Primitive.hpp>

using namespace Futurewalker;

TEST_CASE("Integer")
{
    SECTION("Conversion")
    {
        STATIC_REQUIRE(sizeof(SInt32) == sizeof(int32_t));
        STATIC_REQUIRE(alignof(SInt32) == alignof(int32_t));
        STATIC_REQUIRE(!Concepts::DefaultConstructible<SInt32>);
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<int32_t, SInt32>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<int64_t, SInt32>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<float32_t, SInt32>);
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<SInt32, SInt32>);
        STATIC_REQUIRE(Concepts::NarrowConvertibleTo<SInt32, SInt64>);
        STATIC_REQUIRE(!Concepts::NarrowConvertibleTo<SInt64, SInt32>);
        STATIC_REQUIRE(Concepts::ExplicitlyConvertibleTo<int64_t, SInt32>);
        STATIC_REQUIRE(Concepts::ExplicitlyConvertibleTo<SInt64, SInt32>);
        STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<SInt32, Integer<int32_t, int>>);
        STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<SInt32, Integer<int64_t, int>>);
        STATIC_REQUIRE(Concepts::ExplicitlyConvertibleTo<SInt32, Integer<int32_t, int>>);
        STATIC_REQUIRE(Concepts::ExplicitlyConvertibleTo<SInt32, Integer<int64_t, int>>);
        STATIC_REQUIRE(std::is_standard_layout_v<SInt32>);
        STATIC_REQUIRE(std::is_trivially_copyable_v<SInt32>);
        STATIC_REQUIRE(std::is_trivially_copy_assignable_v<SInt32>);
        STATIC_REQUIRE(std::is_trivially_destructible_v<SInt32>);
        STATIC_REQUIRE(!std::is_trivially_constructible_v<SInt32>);
        STATIC_REQUIRE(!std::is_trivially_default_constructible_v<SInt32>);
    }

    SECTION("Type")
    {
        STATIC_REQUIRE(Concepts::SameAs<typename SInt32::ValueType, int32_t>);
        STATIC_REQUIRE(Concepts::SameAs<typename SInt32::TagType, void>);
    }

    SECTION("Constructor")
    {
        SECTION("Default")
        {
            SInt32 i = 0;
            REQUIRE(i == 0);
            REQUIRE(i == SInt32(0));
            REQUIRE(static_cast<std::int32_t>(i) == 0);
        }

        SECTION("Value")
        {
            SInt32 i = 42;
            REQUIRE(i == 42);
            REQUIRE(i == SInt32(42));
            REQUIRE(static_cast<std::int32_t>(i) == 42);
        }

        SECTION("Implicit Conversion")
        {
            SInt32 i = 42;
            auto x = static_cast<UInt32>(i);
            REQUIRE(x == 42U);
        }
    }

    SECTION("Operators")
    {
        SInt32 i = 42;

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

        SECTION("operator++")
        {
            REQUIRE(i++ == 42);
            REQUIRE(i == 43);
            REQUIRE(++i == 44);
        }

        SECTION("operator--")
        {
            REQUIRE(i-- == 42);
            REQUIRE(i == 41);
            REQUIRE(--i == 40);
        }

        SECTION("operator%=")
        {
            i %= 10;
            REQUIRE(i == 2);
        }

        SECTION("operator%")
        {
            REQUIRE(i % 10 == 2);
        }
    }
}
