#include <catch2/catch_all.hpp>

#include <Futurewalker.Signal.hpp>

using namespace Futurewalker;

TEST_CASE("Tracker")
{
    auto t = Tracker::Make();
    auto w = Tracker::Track(*t);
    REQUIRE(!w.IsExpired());
    t = nullptr;
    REQUIRE(w.IsExpired());
}
