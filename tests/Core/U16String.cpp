#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.U16String.hpp>

using namespace Futurewalker;

TEST_CASE("U16String")
{
    SECTION("Default")
    {
        auto str = U16String();
        REQUIRE(str.IsEmpty());
        REQUIRE(str.GetSubString(0, 0).IsEmpty());

        auto c = char32_t();
        REQUIRE(!str.GetChar(0, c));
    }

    SECTION("Single Character")
    {
        auto str = U16String(u'a');
        REQUIRE(str == U16String(u"a"));
        REQUIRE(str == str);
        REQUIRE(!str.IsEmpty());
        REQUIRE(str.GetView().GetSize() == 1);
        REQUIRE(str.GetSubString(0, 1) == U16String(u'a'));
        REQUIRE(str.GetSubString(0, 0) == U16String());
        REQUIRE(str.GetSubString(1, 1) == U16String());

        auto c = char32_t();
        REQUIRE(str.GetChar(0, c));
        REQUIRE(str.GetSubString(0, 1) == U16String(u'a'));
        REQUIRE(c == U'a');

        REQUIRE(!str.IsEmpty());
        REQUIRE(str.GetView().GetSize() == 1);
        REQUIRE(str == U16String(u"a"));
    }
}
