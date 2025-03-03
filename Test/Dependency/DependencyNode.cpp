#include <catch2/catch_all.hpp>

#include <Futurewalker.Dependency.DependencyNode.hpp>

using namespace Futurewalker;

TEST_CASE("DependencyNode")
{
    SECTION("Make")
    {
        auto dep = DependencyNode::Make({});
        REQUIRE(dep);
        dep->Notify();
        dep->NotifyWithEvent(Event());
        dep->GetPropertyStore();
    }

    SECTION("Parent")
    {
        auto p = DependencyNode::Make({});
        auto c = DependencyNode::Make({});

        p->AddChild(c, nullptr);

        REQUIRE(p->GetChildCount() == 1);
        REQUIRE(p->GetChildAt(0) == c);
        REQUIRE(c->GetParent() == p);

        p->Notify();
        p->NotifyWithEvent(Event());
        p->RemoveChild(c);

        REQUIRE(p->GetChildCount() == 0);
        REQUIRE(p->GetChildAt(0) == nullptr);
        REQUIRE(c->GetParent() == nullptr);
    }
}
