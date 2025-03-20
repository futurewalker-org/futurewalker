#include <catch2/catch_all.hpp>

#include <Futurewalker.Application.BoxConstraints.hpp>

using namespace Futurewalker;

TEST_CASE("BoxConstraints")
{
    SECTION("Unconstrained")
    {
        auto const c = BoxConstraints::MakeUnconstrained();
        REQUIRE(!c.GetMaxSize().IsFinite());
        REQUIRE(c.GetMinSize().IsEmpty());
        REQUIRE(!c.GetWidthConstraints().IsBounded());
        REQUIRE(!c.GetHeightConstraints().IsBounded());
        REQUIRE(c == c);
        REQUIRE(!(c != c));
    }

    SECTION("Exact")
    {
        auto const c = BoxConstraints::MakeExact(100, 200);
        REQUIRE(c.GetMaxSize() == Size<Dp>(100, 200));
        REQUIRE(c.GetMinSize() == Size<Dp>(100, 200));
        REQUIRE(c.GetWidthConstraints() == AxisConstraints::MakeExact(100));
        REQUIRE(c.GetHeightConstraints() == AxisConstraints::MakeExact(200));
    }

    SECTION("MinMax")
    {
        auto const c = BoxConstraints::MakeMinMax(AxisConstraints::MakeMinMax(50, 100), AxisConstraints::MakeMinMax(75, 150));
        REQUIRE(c.GetMaxSize() == Size<Dp>(100, 150));
        REQUIRE(c.GetMinSize() == Size<Dp>(50, 75));
        REQUIRE(c.GetWidthConstraints() == AxisConstraints::MakeMinMax(50, 100));
        REQUIRE(c.GetHeightConstraints() == AxisConstraints::MakeMinMax(75, 150));
    }
}
