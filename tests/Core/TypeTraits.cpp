#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.TypeTraits.hpp>
#include <Futurewalker.Core.Concepts.hpp>

using namespace Futurewalker;

TEST_CASE("TypeTraits")
{
    SECTION("IsComplete")
    {
        class Fwd;
        STATIC_REQUIRE(!TypeTraits::IsComplete<Fwd>);

        class Fwd {};
        STATIC_REQUIRE(TypeTraits::IsComplete<Fwd>);
    }

    SECTION("IsConst")
    {
        STATIC_REQUIRE(!TypeTraits::IsConst<int>);
        STATIC_REQUIRE(TypeTraits::IsConst<const int>);
        STATIC_REQUIRE(!TypeTraits::IsConst<int&>);
        STATIC_REQUIRE(TypeTraits::IsConst<const int&>);
        STATIC_REQUIRE(!TypeTraits::IsConst<int&&>);
        STATIC_REQUIRE(TypeTraits::IsConst<const int&&>);
    }

    SECTION("IsVolatile")
    {
        STATIC_REQUIRE(!TypeTraits::IsVolatile<int>);
        STATIC_REQUIRE(TypeTraits::IsVolatile<volatile int>);
        STATIC_REQUIRE(!TypeTraits::IsVolatile<int&>);
        STATIC_REQUIRE(TypeTraits::IsVolatile<volatile int&>);
        STATIC_REQUIRE(!TypeTraits::IsVolatile<int&&>);
        STATIC_REQUIRE(TypeTraits::IsVolatile<volatile int&&>);
    }

    SECTION("RemoveCV")
    {
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::RemoveCV<int>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::RemoveCV<const int>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::RemoveCV<volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::RemoveCV<const volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::RemoveCV<int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::RemoveCV<const int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::RemoveCV<volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::RemoveCV<const volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::RemoveCV<int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::RemoveCV<const int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::RemoveCV<volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::RemoveCV<const volatile int&&>>);
    }

    SECTION("PropagateRef")
    {
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateRef<int, int>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateRef<int, int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateRef<int, int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateRef<int&, int>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateRef<int&, int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateRef<int&, int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateRef<int&&, int>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateRef<int&&, int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateRef<int&&, int&&>>);
    }

    SECTION("PropagateCV")
    {
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCV<int, int>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCV<int, const int>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCV<int, volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCV<int, const volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCV<const int, int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCV<const int, const int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCV<const int, volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCV<const int, const volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<volatile int, TypeTraits::PropagateCV<volatile int, int>>);
        STATIC_REQUIRE(Concepts::SameAs<volatile int, TypeTraits::PropagateCV<volatile int, const int>>);
        STATIC_REQUIRE(Concepts::SameAs<volatile int, TypeTraits::PropagateCV<volatile int, volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<volatile int, TypeTraits::PropagateCV<volatile int, const volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<const volatile int, TypeTraits::PropagateCV<const volatile int, int>>);
        STATIC_REQUIRE(Concepts::SameAs<const volatile int, TypeTraits::PropagateCV<const volatile int, const int>>);
        STATIC_REQUIRE(Concepts::SameAs<const volatile int, TypeTraits::PropagateCV<const volatile int, volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<const volatile int, TypeTraits::PropagateCV<const volatile int, const volatile int>>);
    }

    SECTION("PropagateCVRef")
    {
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, int>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, const int>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, const int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, const int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, const volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, const volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int, TypeTraits::PropagateCVRef<int, const volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, int>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, const int>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, const int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, const int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, const volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, const volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&, TypeTraits::PropagateCVRef<int&, const volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, int>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, const int>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, const int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, const int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, const volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, const volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<int&&, TypeTraits::PropagateCVRef<int&&, const volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, const int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, const int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, const int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, const volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, const volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int, TypeTraits::PropagateCVRef<const int, const volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, const int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, const int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, const int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, const volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, const volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&, TypeTraits::PropagateCVRef<const int&, const volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, const int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, const int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, const int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, volatile int&&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, const volatile int>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, const volatile int&>>);
        STATIC_REQUIRE(Concepts::SameAs<const int&&, TypeTraits::PropagateCVRef<const int&&, const volatile int&&>>);
    }
}
