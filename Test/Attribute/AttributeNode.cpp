#include <catch2/catch_all.hpp>

#include "Futurewalker.Attribute.AttributeNode.hpp"
#include "Futurewalker.Attribute.AttributeObserver.hpp"

#include "Futurewalker.Base.Locator.hpp"

using namespace Futurewalker;

FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, IntegerAttribute, 42);

TEST_CASE("AttributeNode")
{
    Locator::Register<UniqueIdentifierManager>();
    auto const uniqueIdManager = Locator::Resolve<UniqueIdentifierManager>();

    SECTION("simple get/set")
    {
        auto node = AttributeNode::Make();
        REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*node) == 42);
        AttributeNode::SetValue<&IntegerAttribute>(*node, 24);
        REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*node) == 24);
    }

    SECTION("simple get/set with observer")
    {
        auto node = AttributeNode::Make();
        auto observer = AttributeNode::GetObserver<&IntegerAttribute>(*node);
        REQUIRE(*observer->GetValue() == 42);
        observer->SetValue(24);
        REQUIRE(*observer->GetValue() == 24);
    }

    SECTION("simple child get/set")
    {
        auto node = AttributeNode::Make();
        auto child = AttributeNode::Make();
        node->AddChild(child);

        auto observer = AttributeNode::GetObserver<&IntegerAttribute>(*child);
        REQUIRE(*observer->GetValue() == 42);
        observer->SetValue(24);
        REQUIRE(*observer->GetValue() == 24);
    }

    SECTION("simple child overloading")
    {
        auto node = AttributeNode::Make();
        auto parentobserver = AttributeNode::GetObserver<&IntegerAttribute>(*node);

        auto child = AttributeNode::Make();
        node->AddChild(child);

        auto childobserver = AttributeNode::GetObserver<&IntegerAttribute>(*child);

        parentobserver->SetValue(24);

        REQUIRE(*childobserver->GetValue() == 24);
    }

    SECTION("reference to same node")
    {
        SECTION("get default value")
        {
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, ReferencingAttribute, IntegerAttribute);
            auto node = AttributeNode::Make();
            auto observer = AttributeNode::GetObserver<&ReferencingAttribute>(*node);
            REQUIRE(*observer->GetValue() == 42);
        }

        SECTION("get default value from other value")
        {
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, ReferencingAttribute, IntegerAttribute);
            auto node = AttributeNode::Make();
            {
                auto observer = AttributeNode::GetObserver<&ReferencingAttribute>(*node);
                observer->SetValue(24);
            }
            auto observer = AttributeNode::GetObserver<&ReferencingAttribute>(*node);
            REQUIRE(*observer->GetValue() == 24);
        }

        SECTION("changing value")
        {
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, ReferencingAttribute, IntegerAttribute);
            auto node = AttributeNode::Make();
            REQUIRE(*AttributeNode::GetValue<&ReferencingAttribute>(*node) == 42);
            AttributeNode::SetValue<&ReferencingAttribute>(*node, 24);
            REQUIRE(*AttributeNode::GetValue<&ReferencingAttribute>(*node) == 24);
        }

        SECTION("changing value")
        {
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, ReferencingAttribute, IntegerAttribute);
            auto node = AttributeNode::Make();
            REQUIRE(*AttributeNode::GetValue<&ReferencingAttribute>(*node) == 42);
            AttributeNode::SetValue<&IntegerAttribute>(*node, 24);
            REQUIRE(*AttributeNode::GetValue<&ReferencingAttribute>(*node) == 24);
        }

        SECTION("indirect update")
        {
            auto node = AttributeNode::Make();
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attr1, 1);
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attr2, 2);
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, Attr3, Attr1);
            AttributeNode::SetReference<&Attr3>(*node, Attr2);
            AttributeNode::SetValue<&Attr2>(*node, 42);
            REQUIRE(*AttributeNode::GetValue<&Attr3>(*node) == 42);
        }
    }

    SECTION("reference to parent node")
    {
        FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, ReferencingAttribute, IntegerAttribute);

        auto node = AttributeNode::Make();
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

    SECTION("reference to distant parent node")
    {
        FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attr1, 0);
        FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, Attr2, Attr1);
        FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, Attr3, Attr2);

        auto node = AttributeNode::Make();
        AttributeNode::SetValue<&Attr1>(*node, 12);

        auto child = AttributeNode::Make();
        AttributeNode::SetValue<&Attr2>(*child, 45);
        REQUIRE((int)*AttributeNode::GetValue<&Attr3>(*child) == 45);
        REQUIRE((int)*AttributeNode::GetValue<&Attr2>(*child) == 45);

        node->AddChild(child);
        REQUIRE(*AttributeNode::GetValue<&Attr3>(*child) == 45);

        node->RemoveChild(child);
        REQUIRE(*AttributeNode::GetValue<&Attr3>(*child) == 45);

        node->AddChild(child);
        REQUIRE(*AttributeNode::GetValue<&Attr3>(*child) == 45);

        auto node2 = AttributeNode::Make();
        node2->AddChild(node);
        REQUIRE((int)*AttributeNode::GetValue<&Attr2>(*child) == 45);
        REQUIRE((int)*AttributeNode::GetValue<&Attr3>(*child) == 45);

        auto node3 = AttributeNode::Make();
        node3->AddChild(node2);
        REQUIRE((int)*AttributeNode::GetValue<&Attr2>(*child) == 45);
        REQUIRE((int)*AttributeNode::GetValue<&Attr3>(*child) == 45);
    }

    SECTION("reference from child")
    {
        FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, ReferencingAttribute, IntegerAttribute);

        auto node = AttributeNode::Make();
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

    SECTION("computed on same node")
    {
        SECTION("simple compute")
        {
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attr1, 21);
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(SInt32, ComputedAttribute, [](auto a) { return a * 2; }, Attr1);
            auto node = AttributeNode::Make();
            REQUIRE(*AttributeNode::GetValue<&ComputedAttribute>(*node) == 42);
            AttributeNode::SetValue<&Attr1>(*node, 10);
            REQUIRE(*AttributeNode::GetValue<&ComputedAttribute>(*node) == 20);
        }

        SECTION("simple compute")
        {
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attr1, 2);
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(SInt32, ComputedAttribute, [](auto a, auto b) { return a * b; }, Attr1, Attr1);
            auto node = AttributeNode::Make();
            REQUIRE(*AttributeNode::GetValue<&ComputedAttribute>(*node) == 4);
            AttributeNode::SetValue<&Attr1>(*node, 10);
            REQUIRE(*AttributeNode::GetValue<&ComputedAttribute>(*node) == 100);
        }

        SECTION("simple compute")
        {
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attr1, 10);
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attr2, 32);
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(SInt32, ComputedAttribute, [](auto a, auto b) { return a + b; }, Attr1, Attr2);
            auto node = AttributeNode::Make();
            REQUIRE(*AttributeNode::GetValue<&ComputedAttribute>(*node) == 42);
            AttributeNode::SetValue<&Attr1>(*node, 24);
            REQUIRE(*AttributeNode::GetValue<&ComputedAttribute>(*node) == 56);
            AttributeNode::SetValue<&Attr2>(*node, 18);
            REQUIRE(*AttributeNode::GetValue<&ComputedAttribute>(*node) == 42);
        }

        SECTION("simple compute with reference")
        {
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attr1, 21);
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, Attr2, Attr1);
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(SInt32, ComputedAttribute, [](auto a) { return a * 2; }, Attr2);
            auto node = AttributeNode::Make();
            REQUIRE(*AttributeNode::GetValue<&ComputedAttribute>(*node) == 42);
            AttributeNode::SetValue<&Attr1>(*node, 10);
            REQUIRE(*AttributeNode::GetValue<&ComputedAttribute>(*node) == 20);
            AttributeNode::SetReference<&Attr2>(*node, IntegerAttribute);
            REQUIRE(*AttributeNode::GetValue<&ComputedAttribute>(*node) == 84);
            AttributeNode::SetValue<&IntegerAttribute>(*node, 15);
            REQUIRE(*AttributeNode::GetValue<&ComputedAttribute>(*node) == 30);
        }
    }

    SECTION("add child")
    {
        SECTION("simple value")
        {
            auto node = AttributeNode::Make();
            auto child = AttributeNode::Make();

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child) == 42);

            AttributeNode::SetValue<&IntegerAttribute>(*node, 24);

            node->AddChild(child);

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child) == 24);
        }

        SECTION("simple reference")
        {
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, TestAttribute, IntegerAttribute);

            auto node = AttributeNode::Make();
            auto child = AttributeNode::Make();

            REQUIRE(*AttributeNode::GetValue<&TestAttribute>(*child) == 42);

            SECTION("change value of referenced attribute")
            {
                AttributeNode::SetValue<&IntegerAttribute>(*node, 24);

                node->AddChild(child);

                REQUIRE(*AttributeNode::GetValue<&TestAttribute>(*child) == 24);
            }
        }

        SECTION("indirect update")
        {
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attr1, 0);
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attr2, 0);
            FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, Attr3, Attr1);

            auto parent = AttributeNode::Make();
            AttributeNode::SetValue<&Attr1>(*parent, 42);
            AttributeNode::SetValue<&Attr2>(*parent, 24);

            auto child = AttributeNode::Make();

            AttributeNode::SetReference<&Attr3>(*child, Attr2);

            parent->AddChild(child);

            REQUIRE(*AttributeNode::GetValue<&Attr3>(*child) == 24);

            parent->RemoveChild(child);

            REQUIRE(*AttributeNode::GetValue<&Attr3>(*child) == 0);
        }
    }

    SECTION("remove child")
    {
        SECTION("simple value")
        {
            auto node = AttributeNode::Make();
            auto const child = AttributeNode::Make();

            node->AddChild(child);

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child) == 42);

            node->RemoveChild(child);

            REQUIRE(*AttributeNode::GetValue<&IntegerAttribute>(*child) == 42);
        }

        SECTION("simple value")
        {
            auto node = AttributeNode::Make();
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

            auto node = AttributeNode::Make();
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
