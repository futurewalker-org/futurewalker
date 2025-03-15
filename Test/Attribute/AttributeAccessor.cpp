#include <corecrt_io.h>
#include <catch2/catch_all.hpp>

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

using namespace Futurewalker;

TEST_CASE("AttributeAccessor")
{
    static auto const Attribute1 = StaticAttribute<SInt32>::MakeWithDefaultValue(42);
    static auto const Attribute2 = StaticAttribute<SInt32>::MakeWithDefaultValue(24);

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
        REQUIRE(accessor.BindAttributeWithDefault(*node, Attribute1, 24));
        REQUIRE(accessor.GetValueOr(0) == 24);
        accessor.SetValue(42);
        REQUIRE(accessor.GetValueOr(0) == 42);
        REQUIRE(accessor.BindAttributeWithDefault(*node, Attribute1, 123));
        REQUIRE(accessor.GetValueOr(0) == 123);
        REQUIRE(accessor.BindAttributeWithDefault(*node, Attribute1, Attribute2));
        REQUIRE(accessor.GetValueOr(0) == 24);
    }
}
