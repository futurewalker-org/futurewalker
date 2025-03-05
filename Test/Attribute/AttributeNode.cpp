#include <catch2/catch_all.hpp>

#include "Futurewalker.Attribute.AttributeNode.hpp"
#include "Futurewalker.Attribute.AttributeObserver.hpp"

#include "Futurewalker.Base.Locator.hpp"

using namespace Futurewalker;

const auto integerAttribute = StaticAttribute<SInt32>::MakeWithDefaultValue(42);

TEST_CASE("AttributeNode")
{
    Locator::Register<UniqueIdentifierManager>();
    const auto uniqueIdManager = Locator::Resolve<UniqueIdentifierManager>();

    auto node = AttributeNode::Make();

    SECTION("simple get/set")
    {
        REQUIRE(*AttributeNode::GetValue(*node, integerAttribute) == 42);
        AttributeNode::SetValue(*node, integerAttribute, 24);
        REQUIRE(*AttributeNode::GetValue(*node, integerAttribute) == 24);
    }

    SECTION("simple get/set with observer")
    {
        auto observer = AttributeNode::GetObserver(*node, integerAttribute);
        REQUIRE(*observer->GetValue() == 42);
        observer->SetValue(24);
        REQUIRE(*observer->GetValue() == 24);
    }

    SECTION("simple child get/set")
    {
        auto child = AttributeNode::Make();
        node->AddChild(child);

        auto observer = AttributeNode::GetObserver(*child, integerAttribute);
        REQUIRE(*observer->GetValue() == 42);
        observer->SetValue(24);
        REQUIRE(*observer->GetValue() == 24);
    }

    SECTION("simple child overloading")
    {
        auto parentobserver = AttributeNode::GetObserver(*node, integerAttribute);

        auto child = AttributeNode::Make();
        node->AddChild(child);

        auto childobserver = AttributeNode::GetObserver(*child, integerAttribute);

        parentobserver->SetValue(24);

        REQUIRE(*childobserver->GetValue() == 24);
    }

    SECTION("reference to same node")
    {
        static const auto referencingAttribute = StaticAttribute<SInt32>::MakeWithDefaultReference(integerAttribute);

        SECTION("get default value")
        {
            auto observer = AttributeNode::GetObserver(*node, referencingAttribute);
            REQUIRE(*observer->GetValue() == 42);
        }

        SECTION("get default value from other value")
        {
            {
                auto observer = AttributeNode::GetObserver(*node, integerAttribute);
                observer->SetValue(24);
            }
            auto observer = AttributeNode::GetObserver(*node, referencingAttribute);
            REQUIRE(*observer->GetValue() == 24);
        }

        SECTION("changing value")
        {
            REQUIRE(*AttributeNode::GetValue(*node, referencingAttribute) == 42);
            AttributeNode::SetValue(*node, integerAttribute, 24);
            REQUIRE(*AttributeNode::GetValue(*node, referencingAttribute) == 24);
        }
    }

    SECTION("reference to parent node")
    {
        static const auto referencingAttribute = StaticAttribute<SInt32>::MakeWithDefaultReference(integerAttribute);

        auto child = AttributeNode::Make();
        node->AddChild(child);

        {
            auto observer = AttributeNode::GetObserver(*node, integerAttribute);
            observer->SetValue(24);
        }
        {
            auto observer = AttributeNode::GetObserver(*child, referencingAttribute);
            REQUIRE(*observer->GetValue() == 24);
        }
        {
            auto observer = AttributeNode::GetObserver(*child, integerAttribute);
            observer->SetValue(-1);
        }
        {
            auto observer = AttributeNode::GetObserver(*child, referencingAttribute);
            REQUIRE(*observer->GetValue() == -1);
        }
    }

    SECTION("reference from child")
    {
        static const auto referencingAttribute = StaticAttribute<SInt32>::MakeWithDefaultReference(integerAttribute);

        auto child = AttributeNode::Make();
        node->AddChild(child);

        SECTION("simple get")
        {
            REQUIRE(*AttributeNode::GetValue(*child, referencingAttribute) == 42);
        }

        SECTION("simple set on child")
        {
            AttributeNode::SetValue(*child, integerAttribute, 24);

            REQUIRE(*AttributeNode::GetValue(*child, referencingAttribute) == 24);

            AttributeNode::SetValue(*child, integerAttribute, 0);

            REQUIRE(*AttributeNode::GetValue(*child, referencingAttribute) == 0);
        }

        SECTION("simple set on parent")
        {
            AttributeNode::SetValue(*node, integerAttribute, 24);

            REQUIRE(*AttributeNode::GetValue(*child, referencingAttribute) == 24);

            AttributeNode::SetValue(*child, integerAttribute, 0);

            REQUIRE(*AttributeNode::GetValue(*child, referencingAttribute) == 0);
        }

        SECTION("via observer 1")
        {
            {
                auto observer = AttributeNode::GetObserver(*child, referencingAttribute);
                REQUIRE(*observer->GetValue() == 42);
            }
            {
                auto observer = AttributeNode::GetObserver(*child, integerAttribute);
                observer->SetValue(-1);
            }
            {
                auto observer = AttributeNode::GetObserver(*child, referencingAttribute);
                REQUIRE(*observer->GetValue() == -1);
            }
        }
    }

    SECTION("add child")
    {
        SECTION("simple value")
        {
            auto child = AttributeNode::Make();

            REQUIRE(*AttributeNode::GetValue(*child, integerAttribute) == 42);

            AttributeNode::SetValue(*node, integerAttribute, 24);

            node->AddChild(child);

            REQUIRE(*AttributeNode::GetValue(*child, integerAttribute) == 24);
        }

        SECTION("simple reference")
        {
            static const auto testAttribute = StaticAttribute<SInt32>::MakeWithDefaultReference(integerAttribute);

            auto child = AttributeNode::Make();

            REQUIRE(*AttributeNode::GetValue(*child, testAttribute) == 42);

            SECTION("change value of referenced attribute")
            {
                AttributeNode::SetValue(*node, integerAttribute, 24);

                node->AddChild(child);

                REQUIRE(*AttributeNode::GetValue(*child, testAttribute) == 24);
            }
        }
    }

    SECTION("remove child")
    {
        SECTION("simple value")
        {
            const auto child = AttributeNode::Make();

            node->AddChild(child);

            REQUIRE(*AttributeNode::GetValue(*child, integerAttribute) == 42);

            node->RemoveChild(child);

            REQUIRE(*AttributeNode::GetValue(*child, integerAttribute) == 42);
        }

        SECTION("simple value")
        {
            const auto child = AttributeNode::Make();

            node->AddChild(child);

            AttributeNode::SetValue(*node, integerAttribute, 24);

            REQUIRE(*AttributeNode::GetValue(*child, integerAttribute) == 24);

            node->RemoveChild(child);

            REQUIRE(*AttributeNode::GetValue(*child, integerAttribute) == 42);

            node->AddChild(child);
            REQUIRE(*AttributeNode::GetValue(*child, integerAttribute) == 24);
        }

        SECTION("simple value")
        {
            const auto child1 = AttributeNode::Make();
            const auto child2 = AttributeNode::Make();

            node->AddChild(child1);
            child1->AddChild(child2);

            AttributeNode::SetValue(*node, integerAttribute, 24);

            REQUIRE(*AttributeNode::GetValue(*child2, integerAttribute) == 24);

            node->RemoveChild(child1);

            REQUIRE(*AttributeNode::GetValue(*child2, integerAttribute) == 42);

            node->AddChild(child1);

            REQUIRE(*AttributeNode::GetValue(*child2, integerAttribute) == 24);
        }

    }
}
