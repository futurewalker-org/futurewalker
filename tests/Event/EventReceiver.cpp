#include <catch2/catch_all.hpp>

#include <Futurewalker.Event.EventReceiver.hpp>

using namespace Futurewalker;

TEST_CASE("EventReceiver")
{
    struct TestEvent
    {
    };

    SECTION("Free")
    {
        auto b = Bool(false);
        auto eventReceiver = EventReceiver::Make();
        EventReceiver::Connect(*eventReceiver, [&](Event<>&) -> Bool {
            b = true;
            return true;
        });
        Event event;
        REQUIRE(eventReceiver->SendEvent(event));
        REQUIRE(b);
    }

    SECTION("Member")
    {
        struct TestClass1
        {
            SInt32 eventCount = 0;

            TestClass1()
            {
                _eventReceiver = EventReceiver::Make();
            }

            auto ReceiveEvent(Event<>&) -> Bool
            {
                ++eventCount;
                return true;
            }

            auto GetTracker() -> Weak<void>
            {
                return _eventReceiver;
            }

            auto GetEventReceiver() -> EventReceiver&
            {
                return *_eventReceiver;
            }

        private:
            Shared<EventReceiver> _eventReceiver;
        };

        auto testClass = Shared<TestClass1>::Make();
        auto connection = EventReceiver::Connect(*testClass, *testClass, &TestClass1::ReceiveEvent);

        auto event = Event();
        REQUIRE(testClass->GetEventReceiver().SendEvent(event));
        REQUIRE(testClass->GetEventReceiver().SendEvent(event));
        REQUIRE(testClass->eventCount == 2);
        connection.Disconnect();
        REQUIRE(!testClass->GetEventReceiver().SendEvent(event));
        REQUIRE(testClass->eventCount == 2);
    }

    SECTION("Composition")
    {
        struct TestClass2
        {
            TestClass2()
            {
                _eventReceiver = EventReceiver::Make({.dispatchEvent = [&](Event<>& e, EventFunction const& d) -> Bool { return DispatchEvent(e, d); }});
            }

            auto SendEvent(Event<>& event) -> Bool
            {
                return GetEventReceiver().SendEvent(event);
            }

            auto DispatchEvent(Event<>& event, EventFunction const& dispatch) -> Bool
            {
                return dispatch(event);
            }

            auto ReceiveEvent(Event<>&) -> Bool
            {
                return true;
            }

            auto GetTracker() -> Weak<void>
            {
                return _eventReceiver;
            }

            auto GetEventReceiver() -> EventReceiver&
            {
                return *_eventReceiver;
            }

        private:
            Shared<EventReceiver> _eventReceiver;
        };

        auto testClass = Shared<TestClass2>::Make();
        EventReceiver::Connect(*testClass, *testClass, &TestClass2::ReceiveEvent);
        auto event = Event();
        REQUIRE(testClass->SendEvent(event));
    }
}
