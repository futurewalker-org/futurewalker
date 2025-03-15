#include <catch2/catch_all.hpp>

#include "Futurewalker.Attribute.AttributeNode.hpp"
#include "Futurewalker.Attribute.AttributeObserver.hpp"

#include "Futurewalker.Base.Locator.hpp"

using namespace Futurewalker;

static auto const IntegerAttribute = StaticAttribute<SInt32>::MakeWithDefaultValue(42);

TEST_CASE("AttributeNode")
{
    Locator::Register<UniqueIdentifierManager>();
    auto const uniqueIdManager = Locator::Resolve<UniqueIdentifierManager>();

    auto node = AttributeNode::Make();

    SECTION("simple get/set")
    {
        REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*node) == 42);
        AttributeNode::SetValue<&IntegerAttribute>(*node, 24);
        REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*node) == 24);
    }

    SECTION("simple get/set with observer")
    {
        auto observer = AttributeNode::GetObserver<&IntegerAttribute>(*node);
        REQUIRE(*observer->GetValue() == 42);
        observer->SetValue(24);
        REQUIRE(*observer->GetValue() == 24);
    }

    SECTION("simple child get/set")
    {
        auto child = AttributeNode::Make();
        node->AddChild(child);

        auto observer = AttributeNode::GetObserver<&IntegerAttribute>(*child);
        REQUIRE(*observer->GetValue() == 42);
        observer->SetValue(24);
        REQUIRE(*observer->GetValue() == 24);
    }

    SECTION("simple child overloading")
    {
        auto parentobserver = AttributeNode::GetObserver<&IntegerAttribute>(*node);

        auto child = AttributeNode::Make();
        node->AddChild(child);

        auto childobserver = AttributeNode::GetObserver<&IntegerAttribute>(*child);

        parentobserver->SetValue(24);

        REQUIRE(*childobserver->GetValue() == 24);
    }

    SECTION("reference to same node")
    {
        static auto const ReferencingAttribute = StaticAttribute<SInt32>::MakeWithDefaultReference<&IntegerAttribute>();

        SECTION("get default value")
        {
            auto observer = AttributeNode::GetObserver<&ReferencingAttribute>(*node);
            REQUIRE(*observer->GetValue() == 42);
        }

        SECTION("get default value from other value")
        {
            {
                auto observer = AttributeNode::GetObserver<&ReferencingAttribute>(*node);
                observer->SetValue(24);
            }
            auto observer = AttributeNode::GetObserver<&ReferencingAttribute>(*node);
            REQUIRE(*observer->GetValue() == 24);
        }

        SECTION("changing value")
        {
            REQUIRE(*AttributeNode::GetValue<&ReferencingAttribute>(*node) == 42);
            AttributeNode::SetValue<&ReferencingAttribute>(*node, 24);
            REQUIRE(*AttributeNode::GetValue<&ReferencingAttribute>(*node) == 24);
        }
    }

    SECTION("reference to parent node")
    {
        static auto const ReferencingAttribute = StaticAttribute<SInt32>::MakeWithDefaultReference<&IntegerAttribute>();

        auto child = AttributeNode::Make();
        node->AddChild(child);

        {
            auto observer = AttributeNode::GetObserver<&IntegerAttribute>(*node);
            observer->SetValue(24);
        }
        {
            auto observer = AttributeNode::GetObserver<&ReferencingAttribute>(*child);
            REQUIRE(*observer->GetValue() == 24);
        }
        {
            auto observer = AttributeNode::GetObserver<&IntegerAttribute>(*child);
            observer->SetValue(-1);
        }
        {
            auto observer = AttributeNode::GetObserver<&ReferencingAttribute>(*child);
            REQUIRE(*observer->GetValue() == -1);
        }
    }

    SECTION("reference from child")
    {
        static auto const ReferencingAttribute = StaticAttribute<SInt32>::MakeWithDefaultReference<&IntegerAttribute>();

        auto child = AttributeNode::Make();
        node->AddChild(child);

        SECTION("simple get")
        {
            REQUIRE(*AttributeNode::GetValue<&ReferencingAttribute>(*child) == 42);
        }

        SECTION("simple set on child")
        {
            AttributeNode::SetValue<&IntegerAttribute>(*child, 24);

            REQUIRE(*AttributeNode::GetValue<&ReferencingAttribute>(*child) == 24);

            AttributeNode::SetValue<&IntegerAttribute>(*child, 0);

            REQUIRE(*AttributeNode::GetValue<&ReferencingAttribute>(*child) == 0);
        }

        SECTION("simple set on parent")
        {
            AttributeNode::SetValue<&IntegerAttribute>(*node, 24);

            REQUIRE(*AttributeNode::GetValue<&ReferencingAttribute>(*child) == 24);

            AttributeNode::SetValue<&IntegerAttribute>(*child, 0);

            REQUIRE(*AttributeNode::GetValue<&ReferencingAttribute>(*child) == 0);
        }

        SECTION("via observer 1")
        {
            {
                auto observer = AttributeNode::GetObserver<&ReferencingAttribute>(*child);
                REQUIRE(*observer->GetValue() == 42);
            }
            {
                auto observer = AttributeNode::GetObserver<&IntegerAttribute>(*child);
                observer->SetValue(-1);
            }
            {
                auto observer = AttributeNode::GetObserver<&ReferencingAttribute>(*child);
                REQUIRE(*observer->GetValue() == -1);
            }
        }
    }

    SECTION("add child")
    {
        SECTION("simple value")
        {
            auto child = AttributeNode::Make();

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child) == 42);

            AttributeNode::SetValue<&IntegerAttribute>(*node, 24);

            node->AddChild(child);

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child) == 24);
        }

        SECTION("simple reference")
        {
            static auto const TestAttribute = StaticAttribute<SInt32>::MakeWithDefaultReference<&IntegerAttribute>();

            auto child = AttributeNode::Make();

            REQUIRE(*AttributeNode::GetValue<&TestAttribute>(*child) == 42);

            SECTION("change value of referenced attribute")
            {
                AttributeNode::SetValue<&IntegerAttribute>(*node, 24);

                node->AddChild(child);

                REQUIRE(*AttributeNode::GetValue<&TestAttribute>(*child) == 24);
            }
        }
    }

    SECTION("remove child")
    {
        SECTION("simple value")
        {
            auto const child = AttributeNode::Make();

            node->AddChild(child);

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child) == 42);

            node->RemoveChild(child);

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child) == 42);
        }

        SECTION("simple value")
        {
            auto const child = AttributeNode::Make();

            node->AddChild(child);

            AttributeNode::SetValue<&IntegerAttribute>(*node, 24);

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child) == 24);

            node->RemoveChild(child);

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child) == 42);

            node->AddChild(child);
            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child) == 24);
        }

        SECTION("simple value")
        {
            auto const child1 = AttributeNode::Make();
            auto const child2 = AttributeNode::Make();

            node->AddChild(child1);
            child1->AddChild(child2);

            AttributeNode::SetValue<&IntegerAttribute>(*node, 24);

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child2) == 24);

            node->RemoveChild(child1);

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child2) == 42);

            node->AddChild(child1);

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child2) == 24);
        }
    }
}
