#include <catch2/catch_all.hpp>

#include <Futurewalker.Event.hpp>

using namespace Futurewalker;

TEST_CASE("Event")
{
    SECTION("Empty")
    {
        auto event = Event();
        auto copy = event;
    }

    SECTION("EventType")
    {
        struct TestEvent : EventParameter
        {
            TestEvent() = default;
            TestEvent(TestEvent const&) = default;
            int i = 42;
        };

        auto event = Event<TestEvent>::Make();
        REQUIRE(event->i == 42);
        REQUIRE(event.Is<TestEvent>());
        REQUIRE(event.As<TestEvent>()->i == 42);

        SECTION("Copy")
        {
            auto const copy = event;
            REQUIRE(copy.As<TestEvent>()->i == 42);

            event->i = 24;
            REQUIRE(event->i == 24);
            REQUIRE(copy.As<TestEvent>()->i == 42);
        }

        SECTION("Assign")
        {
            auto copy = Event();
            copy = event;
            REQUIRE(copy.As<TestEvent>()->i == 42);

            event->i = 24;
            REQUIRE(event->i == 24);
            REQUIRE(copy.As<TestEvent>()->i == 42);
        }
    }

    SECTION("Polymorphic")
    {
        struct BaseEvent : EventParameter
        {
            virtual ~BaseEvent() = default;
        };

        struct DerivedEvent : BaseEvent
        {
        };

        auto const event = Event<DerivedEvent>::Make();
        REQUIRE(event.Is<BaseEvent>());
        REQUIRE(event.Is<DerivedEvent>());
    }
}
