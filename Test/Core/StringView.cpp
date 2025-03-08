#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.StringView.hpp>

#include <cstdint>
#include <memory>

using namespace Futurewalker;

TEST_CASE("StringView")
{
    SECTION("Empty")
    {
        auto const sv = StringView();
        REQUIRE(sv.IsEmpty());
        REQUIRE(sv.GetSize() == 0);
        REQUIRE(sv.GetData() == nullptr);
    }

    SECTION("Ctor")
    {
        auto const sv = StringView(u8"ABC");
        REQUIRE(!sv.IsEmpty());
        REQUIRE(sv.GetSize() == 3);
        REQUIRE(sv.GetData() != nullptr);
    }

    SECTION("Ctor")
    {
        auto const s = u8"ABC";
        auto const sv = StringView(s, 2);
        REQUIRE(sv.GetSize() == 2);
        REQUIRE(sv.GetData() != nullptr);
    }

    SECTION("Substring")
    {
        auto const sv = StringView(u8"ABCDEF");

        SECTION("0,0")
        {
            auto const sv2 = sv.GetSubstring(0, 0);
            REQUIRE(sv2.IsEmpty());
            REQUIRE(sv2.GetSize() == 0);
            REQUIRE(sv2.GetData() != nullptr);
        }

        SECTION("0,100")
        {
            auto const sv2 = sv.GetSubstring(0, 100);
            REQUIRE(!sv2.IsEmpty());
            REQUIRE(sv2.GetSize() == 6);
            REQUIRE(sv2.GetData() != nullptr);
        }

        SECTION("5,100")
        {
            auto const sv2 = sv.GetSubstring(5, 100);
            REQUIRE(!sv2.IsEmpty());
            REQUIRE(sv2.GetSize() == 1);
            REQUIRE(sv2.GetData() != nullptr);
        }

        SECTION("100, 5")
        {
            auto const sv2 = sv.GetSubstring(100, 5);
            REQUIRE(sv2.IsEmpty());
            REQUIRE(sv2.GetSize() == 0);
            REQUIRE(sv2.GetData() != nullptr);
        }

        SECTION("-100,100")
        {
            auto const sv2 = sv.GetSubstring(-100, 100);
            REQUIRE(!sv2.IsEmpty());
            REQUIRE(sv2.GetSize() == 6);
            REQUIRE(sv2.GetData() != nullptr);
        }

        SECTION("-100,0")
        {
            auto const sv2 = sv.GetSubstring(-100, 0);
            REQUIRE(sv2.IsEmpty());
            REQUIRE(sv2.GetSize() == 0);
            REQUIRE(sv2.GetData() != nullptr);
        }

        SECTION("1,3")
        {
            auto const sv2 = sv.GetSubstring(1, 3);
            REQUIRE(sv2.GetSize() == 2);
            REQUIRE(sv2.GetData() != nullptr);
            REQUIRE(sv2[0] == u8'B');
            REQUIRE(sv2[1] == u8'C');
        }

        SECTION("3,1")
        {
            auto const sv2 = sv.GetSubstring(3, 1);
            REQUIRE(sv2.IsEmpty());
            REQUIRE(sv2.GetSize() == 0);
            REQUIRE(sv2.GetData() != nullptr);
        }


        SECTION("6,6")
        {
            auto const sv2 = sv.GetSubstring(6, 6);
            REQUIRE(sv2.IsEmpty());
            REQUIRE(sv2.GetSize() == 0);
            REQUIRE(sv2.GetData() != nullptr);
        }
    }

    SECTION("Contains")
    {
        auto const sv = StringView(u8"ABCDEF");

        SECTION("Empty")
        {
            auto const sv2 = StringView();
            REQUIRE(!sv.Contains(sv2));
        }

        SECTION("ABCDEF")
        {
            auto const sv2 = StringView(u8"ABCDEF");
            REQUIRE(sv.Contains(sv2));
        }

        SECTION("ABC")
        {
            auto const sv2 = StringView(u8"ABC");
            REQUIRE(sv.Contains(sv2));
        }

        SECTION("BCD")
        {
            auto const sv2 = StringView(u8"BCD");
            REQUIRE(sv.Contains(sv2));
        }

        SECTION("CDE")
        {
            auto const sv2 = StringView(u8"CDE");
            REQUIRE(sv.Contains(sv2));
        }

        SECTION("DEF")
        {
            auto const sv2 = StringView(u8"DEF");
            REQUIRE(sv.Contains(sv2));
        }

        SECTION("EFG")
        {
            auto const sv2 = StringView(u8"EFG");
            REQUIRE(!sv.Contains(sv2));
        }

        SECTION("F")
        {
            auto const sv2 = StringView(u8"F");
            REQUIRE(sv.Contains(sv2));
        }

        SECTION("G")
        {
            auto const sv2 = StringView(u8"G");
            REQUIRE(!sv.Contains(sv2));
        }
    }

    SECTION("Count")
    {
        auto const sv = StringView(u8"ABCDEF");

        SECTION("Empty")
        {
            auto const sv2 = StringView();
            REQUIRE(sv.Count(sv2) == 0);
        }

        SECTION("ABCDEF")
        {
            auto const sv2 = StringView(u8"ABCDEF");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("ABC")
        {
            auto const sv2 = StringView(u8"ABC");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("BCD")
        {
            auto const sv2 = StringView(u8"BCD");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("CDE")
        {
            auto const sv2 = StringView(u8"CDE");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("DEF")
        {
            auto const sv2 = StringView(u8"DEF");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("EFG")
        {
            auto const sv2 = StringView(u8"EFG");
            REQUIRE(sv.Count(sv2) == 0);
        }

        SECTION("F")
        {
            auto const sv2 = StringView(u8"F");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("G")
        {
            auto const sv2 = StringView(u8"G");
            REQUIRE(sv.Count(sv2) == 0);
        }

        SECTION("AB")
        {
            auto const sv2 = StringView(u8"AB");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("BC")
        {
            auto const sv2 = StringView(u8"BC");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("CD")
        {
            auto const sv2 = StringView(u8"CD");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("DE")
        {
            auto const sv2 = StringView(u8"DE");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("EF")
        {
            auto const sv2 = StringView(u8"EF");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("FG")
        {
            auto const sv2 = StringView(u8"FG");
            REQUIRE(sv.Count(sv2) == 0);
        }
    }

    SECTION("Count")
    {
        auto const sv = StringView(u8"AAAAA");

        SECTION("Empty")
        {
            auto const sv2 = StringView();
            REQUIRE(sv.Count(sv2) == 0);
        }

        SECTION("A")
        {
            auto const sv2 = StringView(u8"A");
            REQUIRE(sv.Count(sv2) == 5);
        }

        SECTION("AA")
        {
            auto const sv2 = StringView(u8"AA");
            REQUIRE(sv.Count(sv2) == 2);
        }

        SECTION("AAA")
        {
            auto const sv2 = StringView(u8"AAA");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("AAAA")
        {
            auto const sv2 = StringView(u8"AAAA");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("AAAAA")
        {
            auto const sv2 = StringView(u8"AAAAA");
            REQUIRE(sv.Count(sv2) == 1);
        }

        SECTION("AAAAAA")
        {
            auto const sv2 = StringView(u8"AAAAAA");
            REQUIRE(sv.Count(sv2) == 0);
        }
    }

    SECTION("StartsWith")
    {
        auto const sv = StringView(u8"ABCDEF");

        SECTION("Empty")
        {
            auto const sv2 = StringView();
            REQUIRE(sv.StartsWith(sv2));
        }

        SECTION("ABCDEF")
        {
            auto const sv2 = StringView(u8"ABCDEF");
            REQUIRE(sv.StartsWith(sv2));
        }

        SECTION("ABC")
        {
            auto const sv2 = StringView(u8"ABC");
            REQUIRE(sv.StartsWith(sv2));
        }

        SECTION("BCD")
        {
            auto const sv2 = StringView(u8"BCD");
            REQUIRE(!sv.StartsWith(sv2));
        }

        SECTION("CDE")
        {
            auto const sv2 = StringView(u8"CDE");
            REQUIRE(!sv.StartsWith(sv2));
        }

        SECTION("DEF")
        {
            auto const sv2 = StringView(u8"DEF");
            REQUIRE(!sv.StartsWith(sv2));
        }

        SECTION("EFG")
        {
            auto const sv2 = StringView(u8"EFG");
            REQUIRE(!sv.StartsWith(sv2));
        }

        SECTION("F")
        {
            auto const sv2 = StringView(u8"F");
            REQUIRE(!sv.StartsWith(sv2));
        }

        SECTION("G")
        {
            auto const sv2 = StringView(u8"G");
            REQUIRE(!sv.StartsWith(sv2));
        }
    }

    SECTION("EndsWith")
    {
        auto const sv = StringView(u8"ABCDEF");

        SECTION("Empty")
        {
            auto const sv2 = StringView();
            REQUIRE(sv.EndsWith(sv2));
        }

        SECTION("ABCDEF")
        {
            auto const sv2 = StringView(u8"ABCDEF");
            REQUIRE(sv.EndsWith(sv2));
        }

        SECTION("ABC")
        {
            auto const sv2 = StringView(u8"ABC");
            REQUIRE(!sv.EndsWith(sv2));
        }

        SECTION("BCD")
        {
            auto const sv2 = StringView(u8"BCD");
            REQUIRE(!sv.EndsWith(sv2));
        }

        SECTION("CDE")
        {
            auto const sv2 = StringView(u8"CDE");
            REQUIRE(!sv.EndsWith(sv2));
        }

        SECTION("DEF")
        {
            auto const sv2 = StringView(u8"DEF");
            REQUIRE(sv.EndsWith(sv2));
        }

        SECTION("EFG")
        {
            auto const sv2 = StringView(u8"EFG");
            REQUIRE(!sv.EndsWith(sv2));
        }

        SECTION("F")
        {
            auto const sv2 = StringView(u8"F");
            REQUIRE(sv.EndsWith(sv2));
        }

        SECTION("G")
        {
            auto const sv2 = StringView(u8"G");
            REQUIRE(!sv.EndsWith(sv2));
        }
    }
}
