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
        EventReceiver::Connect(*eventReceiver, [&](Event&) -> Async<Bool> {
            b = true;
            co_return true;
        });
        Event event;
        REQUIRE(AsyncFunction::SyncWait(eventReceiver->SendEvent(event)));
        REQUIRE(b);
    }

    SECTION("Member")
    {
        struct TestClass1
        {
            SInt32 eventCount = 0;

            TestClass1()
            {
                m_eventReceiver = EventReceiver::Make();
            }

            auto ReceiveEvent(Event&) -> Async<Bool>
            {
                ++eventCount;
                co_return true;
            }

            auto GetTracker() -> Tracker&
            {
                return GetEventReceiver().GetTracker();
            }

            auto GetEventReceiver() -> EventReceiver&
            {
                return *m_eventReceiver;
            }

        private:
            Shared<EventReceiver> m_eventReceiver;
        };

        auto testClass = Shared<TestClass1>::Make();
        auto connection = EventReceiver::Connect(*testClass, *testClass, &TestClass1::ReceiveEvent);

        auto event = Event();
        REQUIRE(AsyncFunction::SyncWait(testClass->GetEventReceiver().SendEvent(event)));
        REQUIRE(AsyncFunction::SyncWait(testClass->GetEventReceiver().SendEvent(event)));
        REQUIRE(testClass->eventCount == 2);
        connection.Disconnect();
        REQUIRE(!AsyncFunction::SyncWait(testClass->GetEventReceiver().SendEvent(event)));
        REQUIRE(testClass->eventCount == 2);
    }

    SECTION("Composition")
    {
        struct TestClass2
        {
            TestClass2()
            {
                m_eventReceiver = EventReceiver::Make({.dispatchEvent = [&](Event& e, const EventFunction& d) -> Async<Bool> { co_return co_await DispatchEvent(e, d); }});
            }

            auto SendEvent(Event& event) -> Async<Bool>
            {
                co_return co_await GetEventReceiver().SendEvent(event);
            }

            auto DispatchEvent(Event& event, const EventFunction& dispatch) -> Async<Bool>
            {
                co_return co_await dispatch(event);
            }

            auto ReceiveEvent(Event&) -> Async<Bool>
            {
                co_return true;
            }

            auto GetTracker() -> Tracker&
            {
                return GetEventReceiver().GetTracker();
            }

            auto GetEventReceiver() -> EventReceiver&
            {
                return *m_eventReceiver;
            }

        private:
            Shared<EventReceiver> m_eventReceiver;
        };

        auto testClass = Shared<TestClass2>::Make();
        EventReceiver::Connect(*testClass, *testClass, &TestClass2::ReceiveEvent);
        auto event = Event();
        REQUIRE(AsyncFunction::SyncWaitFn([&]() -> Future<Bool> { co_return co_await testClass->SendEvent(event); }));
    }
}
