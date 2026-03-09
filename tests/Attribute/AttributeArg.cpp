#include <catch2/catch_all.hpp>

#include "Futurewalker.Attribute.AttributeArg.hpp"

using namespace Futurewalker;

auto TestFunc(AttributeArg<SInt32> arg, auto test)
{
    test(arg);
}

FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SInt32, attr, 24);

TEST_CASE("AttributeArg")
{
    auto const attrPtr = StaticPointer(&attr);

    TestFunc(42, [](auto& arg) {
        REQUIRE(!arg.GetReference());
        REQUIRE(arg.GetValue());
    });

    TestFunc(attr, [&](auto& arg) {
        REQUIRE(arg.GetReference());
        REQUIRE(arg.GetReference()->GetPointer() == attrPtr);
        REQUIRE(!arg.GetValue());
    });
}
