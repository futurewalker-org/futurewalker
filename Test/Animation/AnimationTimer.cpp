#include <catch2/catch_all.hpp>

#include <Futurewalker.Animation.AnimationTimer.hpp>
#include <Futurewalker.Animation.RootAnimationTimer.hpp>

#include <Futurewalker.Core.MonotonicClock.hpp>

using namespace Futurewalker;

TEST_CASE("AnimationTimer")
{
    SECTION("Init")
    {
        auto at = AnimationTimer::Make();
        REQUIRE(at);
        REQUIRE(!at->IsActive());
        REQUIRE(at->IsEnabled());
        REQUIRE(at->GetCurrentTime() == MonotonicTime(0));

        at->GetTracker();
        at->GetEventReceiver();
    }

    SECTION("Child")
    {
        auto requestCount = 0;
        auto r = RootAnimationTimer::Make(RootAnimationTimer::Delegate {
          .requestTick = [&] { ++requestCount; },
          .getCurrentTime = [] { return MonotonicClock::GetCurrentTime(); },
        });
        auto c = AnimationTimer::Make();

        r->AddChild(c, nullptr);

        REQUIRE(!c->IsActive());
        r->SetActive(true);
        REQUIRE(c->IsActive());

        c->RequestTick();
        REQUIRE(requestCount == 1);

        REQUIRE(r->IsEnabled());
        REQUIRE(r->IsEnabledFromRoot());
        REQUIRE(c->IsEnabled());
        REQUIRE(c->IsEnabledFromRoot());
        r->SetEnabled(false);
        REQUIRE(!r->IsEnabled());
        REQUIRE(!r->IsEnabledFromRoot());
        REQUIRE(c->IsEnabled());
        REQUIRE(!c->IsEnabledFromRoot());

        r->RemoveChild(c);
        REQUIRE(c->IsEnabled());
        REQUIRE(c->IsEnabledFromRoot());

        r->AddChild(c, nullptr);
        REQUIRE(c->IsEnabled());
        REQUIRE(!c->IsEnabledFromRoot());

        auto cc = AnimationTimer::Make();
        c->AddChild(cc, nullptr);
        REQUIRE(cc->IsEnabled());
        REQUIRE(!cc->IsEnabledFromRoot());

        r->SetEnabled(true);
        REQUIRE(r->IsEnabledFromRoot());
        REQUIRE(c->IsEnabledFromRoot());
        REQUIRE(cc->IsEnabledFromRoot());

        c->SetEnabled(false);
        REQUIRE(!c->IsEnabled());
        REQUIRE(!c->IsEnabledFromRoot());
        REQUIRE(cc->IsEnabled());
        REQUIRE(!cc->IsEnabledFromRoot());

        r->SetActive(false);
        REQUIRE(!r->IsActive());
        REQUIRE(!c->IsActive());
        REQUIRE(!cc->IsActive());
    }
}
