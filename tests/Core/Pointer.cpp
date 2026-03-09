#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.Pointer.hpp>

#include <cstdint>
#include <memory>

using namespace Futurewalker;

TEST_CASE("Pointer")
{
    STATIC_REQUIRE(!std::is_default_constructible_v<Pointer<const char>>);

    SECTION("Construct from nullptr")
    {
        constexpr Pointer<const char> p1 = nullptr;
        //constexpr Pointer<const char> p2 = 0; // should not work.
        //constexpr Pointer<const char> p2 = false; // should not work.
        REQUIRE(!p1);
        STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<int, Pointer<const char>>);
        STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<bool, Pointer<const char>>);
    }

    SECTION("Assign from nullptr")
    {
        Pointer<const char> p = nullptr;
        REQUIRE(!p);

        p = nullptr;
        REQUIRE(!p);
    }

    SECTION("Construct from pointer")
    {
        const char* s1 = "1";
        const char* s2 = "2";

        Pointer<const char> p = s1;
        REQUIRE(p);
        REQUIRE(p == s1);

        p = s2;
        REQUIRE(p);
        REQUIRE(p == s2);
    }

    SECTION("Construct from string literal")
    {
        Pointer<const char> p = "str";
        REQUIRE(p);
    }

    SECTION("Comparison")
    {
        Pointer<const char> p = "str";
        REQUIRE(p == p);
        p = nullptr;
        REQUIRE(p == p);
    }

    SECTION("Const conversion")
    {
        STATIC_REQUIRE(Concepts::ImplicitlyConvertibleTo<Pointer<char>, Pointer<const char>>);
        STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<Pointer<const char>, Pointer<char>>);
        STATIC_REQUIRE(Concepts::ImplicitlyConvertibleTo<Pointer<char>, Pointer<void>>);
        STATIC_REQUIRE(Concepts::ImplicitlyConvertibleTo<Pointer<char>, Pointer<const void>>);

        STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<Pointer<void>, Pointer<char>>);
        STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<Pointer<void>, Pointer<const char>>);
        STATIC_REQUIRE(Concepts::ExplicitlyConvertibleTo<Pointer<void>, Pointer<char>>);
        STATIC_REQUIRE(Concepts::ExplicitlyConvertibleTo<Pointer<void>, Pointer<const char>>);

        STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<Pointer<const void>, Pointer<char>>);
        STATIC_REQUIRE(!Concepts::ImplicitlyConvertibleTo<Pointer<const void>, Pointer<const char>>);
        STATIC_REQUIRE(!Concepts::ExplicitlyConvertibleTo<Pointer<const void>, Pointer<char>>);
        STATIC_REQUIRE(Concepts::ExplicitlyConvertibleTo<Pointer<const void>, Pointer<const char>>);
    }

    SECTION("void pointer conversions")
    {
        Pointer<const char> p1 = "Hello";
        Pointer<const void> p2 = p1;
        Pointer<const char> p3 = static_cast<Pointer<const char>>(p2);
        REQUIRE(p3);
    }

    SECTION("Pointer arithmetic")
    {
        Pointer<const char> s = "abcdef";
        REQUIRE(*s == 'a');
        REQUIRE(s[5] == 'f');
        {
            auto p = s + 1;
            REQUIRE(*p == 'b');
            ++p;
            REQUIRE(*p == 'c');
            p += 2;
            REQUIRE(*p == 'e');
            p -= 1;
            REQUIRE(*p == 'd');
            REQUIRE(*p-- == 'd');
            REQUIRE(*p == 'c');
            REQUIRE(*++p == 'd');
        }
    }
}
