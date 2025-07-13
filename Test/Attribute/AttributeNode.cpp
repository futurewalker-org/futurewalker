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
            static auto const TestAttribute = StaticAttribute<SInt32>::MakeWithDefaultReference<&IntegerAttribute>();

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
