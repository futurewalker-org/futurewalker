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

        auto c = char16_t();
        REQUIRE(!str.GetChar(c, 0));

        str.Reserve(1);
        REQUIRE(str.IsEmpty());

        str.Resize(1);
        REQUIRE(!str.IsEmpty());
        REQUIRE(str.GetChar(c, 0));
        REQUIRE(c == u'\0');
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

        auto c = char16_t();
        REQUIRE(str.GetChar(c, 0));
        REQUIRE(c == u'a');

        str.Reserve(0);
        str.Reserve(1);
        str.Reserve(5);
        REQUIRE(!str.IsEmpty());
        REQUIRE(str.GetView().GetSize() == 1);
        REQUIRE(str == U16String(u"a"));

        str.Resize(2);
        REQUIRE(str.GetChar(c, 0));
        REQUIRE(c == u'a');
        REQUIRE(str.GetChar(c, 1));
        REQUIRE(c == u'\0');
        REQUIRE(str == U16String(u"a\0", 2));
        REQUIRE(str.GetSubString(0, 1) == U16String(u'a'));
        REQUIRE(str.GetSubString(1, 2) == U16String(u'\0'));
    }
}
