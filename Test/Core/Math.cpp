#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Core.Primitive.hpp>

using namespace Futurewalker;

TEST_CASE("Math")
{
    SECTION("IsFinite/Infinite")
    {
        REQUIRE(Math::IsFinite(0.0));
        REQUIRE(!Math::IsInfinite(-0.0));
        REQUIRE(!Math::IsFinite(std::numeric_limits<float64_t>::quiet_NaN()));
        REQUIRE(!Math::IsInfinite(std::numeric_limits<float64_t>::quiet_NaN()));
        REQUIRE(!Math::IsFinite(std::numeric_limits<float32_t>::infinity()));
        REQUIRE(Math::IsInfinite(std::numeric_limits<float32_t>::infinity()));
    }

    SECTION("IsNaN")
    {
        REQUIRE(!Math::IsNaN(0.0));
        REQUIRE(Math::IsNaN(std::numeric_limits<float64_t>::quiet_NaN()));
        REQUIRE(!Math::IsNaN(std::numeric_limits<float32_t>::infinity()));
    }

    SECTION("Trunc")
    {
        REQUIRE(Math::Trunc(3.14) == 3.0);
        REQUIRE(Math::Trunc(-3.14f) == -3.0f);
        REQUIRE(Math::Trunc(-0.0) == 0.0);
        REQUIRE(Math::IsNaN(Math::Trunc(std::numeric_limits<float64_t>::quiet_NaN())));
        REQUIRE(Math::IsInfinite(Math::Trunc(std::numeric_limits<float32_t>::infinity())));
        REQUIRE(Math::IsInfinite(Math::Trunc(-std::numeric_limits<float32_t>::infinity())));
    }

    SECTION("Floor")
    {
        REQUIRE(Math::Floor(3.14) == 3.0);
        REQUIRE(Math::Floor(-3.14f) == -4.0f);
        REQUIRE(Math::Floor(-0.0) == 0.0);
        REQUIRE(Math::IsNaN(Math::Floor(std::numeric_limits<float64_t>::quiet_NaN())));
        REQUIRE(Math::IsInfinite(Math::Floor(std::numeric_limits<float32_t>::infinity())));
        REQUIRE(Math::IsInfinite(Math::Floor(-std::numeric_limits<float32_t>::infinity())));
    }

    SECTION("Ceil")
    {
        REQUIRE(Math::Ceil(3.14) == 4.0);
        REQUIRE(Math::Ceil(-3.14f) == -3.0f);
        REQUIRE(Math::Ceil(-0.0) == 0.0);
        REQUIRE(Math::IsNaN(Math::Ceil(std::numeric_limits<float64_t>::quiet_NaN())));
        REQUIRE(Math::IsInfinite(Math::Ceil(std::numeric_limits<float32_t>::infinity())));
        REQUIRE(Math::IsInfinite(Math::Ceil(-std::numeric_limits<float32_t>::infinity())));
    }

    SECTION("Abs")
    {
        REQUIRE(Math::Abs(-1.0) == 1.0);
        REQUIRE(Math::Abs(-1.0f) == 1.0f);
        REQUIRE(Math::Abs(0.0) == 0.0);
        REQUIRE(Math::Abs(-0.0) == 0.0);
        REQUIRE(Math::Abs(std::numeric_limits<float32_t>::infinity()) == std::numeric_limits<float32_t>::infinity());
        REQUIRE(Math::Abs(-std::numeric_limits<float32_t>::infinity()) == std::numeric_limits<float32_t>::infinity());
        REQUIRE(Math::IsNaN(Math::Abs(std::numeric_limits<float64_t>::quiet_NaN())));
    }

    SECTION("Sqrt")
    {
        REQUIRE(Math::Sqrt(9.0) == 3.0);
        REQUIRE(Math::Sqrt(16.f) == 4.0f);
    }

    SECTION("Round")
    {
        REQUIRE(Math::Round(-1.5) == -1.0);
        REQUIRE(Math::Round(-1.25) == -1.0);
        REQUIRE(Math::Round(-1.0) == -1.0);
        REQUIRE(Math::Round(-0.75) == -1.0);
        REQUIRE(Math::Round(-0.5) == 0.0);
        REQUIRE(Math::Round(-0.49999999999999994) == 0.0);
        REQUIRE(Math::Round(-0.25) == 0.0);
        REQUIRE(Math::Round(0.0) == 0.0);
        REQUIRE(Math::Round(-0.0) == 0.0);
        REQUIRE(Math::Round(1.0) == 1.0);
        REQUIRE(Math::Round(0.25) == 0.0);
        REQUIRE(Math::Round(0.49999999999999994) == 0);
        REQUIRE(Math::Round(0.5) == 1.0);
        REQUIRE(Math::Round(0.75) == 1.0);
    }

    SECTION("IsNearlyEqual")
    {
        REQUIRE(Math::IsNearlyEqual(0.0, 0.1, 0.1));
        REQUIRE(Math::IsNearlyEqual(0.0, -0.1, 0.1));
        REQUIRE(Math::IsNearlyEqual(0.0, 0.01, 0.1));
        REQUIRE(Math::IsNearlyEqual(0.0, -0.01, 0.1));
        REQUIRE(!Math::IsNearlyEqual(0.0, 0.11, 0.1));
        REQUIRE(!Math::IsNearlyEqual(0.0, -0.11, 0.1));
        REQUIRE(Math::IsNearlyEqual(1.0, 1.0 + 1e-10, 1e-9));
        REQUIRE(Math::IsNearlyEqual(1e-10f, -1e-10f, 1e-9f));
    }

    SECTION("IsNearlyEqualRelative")
    {
        REQUIRE(Math::IsNearlyEqualRelative(100.0, 101.0, 0.01, 0.0));
        REQUIRE(Math::IsNearlyEqualRelative(99.0, 100.0, 0.01, 0.0));
        REQUIRE(Math::IsNearlyEqualRelative(0.1, 0.0, 0.00, 0.1));
        REQUIRE(Math::IsNearlyEqualRelative(0.0, 0.1, 0.00, 0.1));
        REQUIRE(!Math::IsNearlyEqualRelative(0.1, 0.0, 0.00, 0.05));
        REQUIRE(!Math::IsNearlyEqualRelative(0.0, 0.1, 0.00, 0.05));
        REQUIRE(Math::IsNearlyEqualRelative(0.1, 0.0, 1.00, 0.05));
        REQUIRE(Math::IsNearlyEqualRelative(0.0, 0.1, 1.00, 0.05));
        REQUIRE(Math::IsNearlyEqualRelative(1.0, 1.0 + 1e-10, 1e-9, 1e-9));
        REQUIRE(Math::IsNearlyEqualRelative(1e-10f, -1e-10f, 1e-9f, 1e-9f));
    }
}
