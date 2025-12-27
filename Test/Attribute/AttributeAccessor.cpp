#include <catch2/catch_all.hpp>

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

using namespace Futurewalker;

TEST_CASE("AttributeAccessor")
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attribute1, 42);
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attribute2, 24);

    auto const idManager = Locator::ResolveWithDefault<UniqueIdentifierManager>();
    auto const node = AttributeNode::Make();

    SECTION("Default")
    {
        AttributeAccessor<SInt32> accessor;
        REQUIRE(!accessor.GetValue());
        REQUIRE(accessor.GetValueOr(42) == 42);
    }

    SECTION("Bind")
    {
        AttributeAccessor<SInt32> accessor;
        REQUIRE(accessor.BindAttribute(*node, Attribute1));
        REQUIRE(accessor.GetValueOr(0) == 42);
        accessor.SetValue(24);
        REQUIRE(accessor.GetValueOr(0) == 24);
        accessor.SetValue(0);
        REQUIRE(accessor.GetValueOr(0) == 0);
        accessor.SetReference(Attribute2);
        REQUIRE(accessor.GetValueOr(0) == 24);
        accessor.UnbindAttribute();
        REQUIRE(!accessor.GetValue());
    }

    SECTION("BindWithDefault")
    {
        AttributeAccessor<SInt32> accessor;
        REQUIRE(accessor.BindAttributeWithDefaultValue(*node, Attribute1, 24));
        REQUIRE(accessor.GetValueOr(0) == 24);
        accessor.SetValue(42);
        REQUIRE(accessor.GetValueOr(0) == 42);
        REQUIRE(accessor.BindAttributeWithDefaultValue(*node, Attribute1, 123));
        REQUIRE(accessor.GetValueOr(0) == 123);
        REQUIRE(accessor.BindAttributeWithDefaultReference(*node, Attribute1, Attribute2));
        REQUIRE(accessor.GetValueOr(0) == 24);
    }

    SECTION("indirect update")
    {
        FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, Attr1, 0);
        FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(SInt32, Attr2, Attr1);

        auto parent = AttributeNode::Make();
        AttributeNode::SetValue<&Attr1>(*parent, 24);
        AttributeNode::SetValue<&Attr1>(*parent, 42);

        auto child = AttributeNode::Make();

        auto accessor = AttributeAccessor<SInt32>();
        REQUIRE(accessor.BindAttribute(*child, Attr2));

        parent->AddChild(child);

        REQUIRE(accessor.GetValueOr(0) == 42);

        parent->RemoveChild(child);

        REQUIRE(*AttributeNode::GetValue<&Attr2>(*child) == 0);
    }
}
