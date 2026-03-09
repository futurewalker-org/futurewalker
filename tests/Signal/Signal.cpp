#include <catch2/catch_all.hpp>

#include <Futurewalker.Signal.hpp>

using namespace Futurewalker;

TEST_CASE("Signal")
{
    SECTION("Default")
    {
        auto sig = Signal<Bool(int&), SignalCombinerAnyOf>();

        REQUIRE(sig.IsEmpty());
        REQUIRE(sig.GetSlotCount() == 0);
        sig.DisconnectAll();
    }

    SECTION("Connect")
    {
        auto sig = Signal<Bool(), SignalCombinerAnyOf>();

        SECTION("")
        {
            sig.Connect([&]() -> Bool { return true; }, SignalConnectPosition::back);
            REQUIRE(sig());
        }

        SECTION("")
        {
            sig.Connect([&]() -> Bool { return false; }, SignalConnectPosition::back);
            REQUIRE(!sig());
        }

        SECTION("")
        {
            int i = 0;
            sig.Connect([&]() -> Bool { ++i; return false; }, SignalConnectPosition::back);
            sig.Connect([&]() -> Bool { ++i; return true; }, SignalConnectPosition::back);
            REQUIRE(sig());
            REQUIRE(i == 2);
        }
    }

    SECTION("Async Combiner")
    {
        int i = 0;
        auto sig = Signal<Lazy<Bool>(int&), AsyncSignalCombinerAnyOf>();
        auto async = sig(i);
    }
}
