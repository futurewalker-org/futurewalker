#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.String.hpp>

using namespace Futurewalker;

TEST_CASE("String")
{
    SECTION("Empty")
    {
        auto str = String();
        REQUIRE(str.IsEmpty());
        REQUIRE(str.GetView().GetSize() == 0);
        REQUIRE(str == str);
        REQUIRE_NOTHROW(str.Clear());
    }

    SECTION("Empty")
    {
        auto str = String(u8"");
        REQUIRE(str.IsEmpty());
        REQUIRE(str.GetView().GetSize() == 0);
        REQUIRE(str == str);
        REQUIRE_NOTHROW(str.Clear());
    }

    SECTION("Ctor")
    {
        auto str = String(U'\U0001F600');
        REQUIRE(str == String(u8"\xF0\x9F\x98\x80"));
        REQUIRE(str.GetView().GetSize() == 4);
    }

    SECTION("Ctor")
    {
        auto str = String(u8'a');
        REQUIRE(!str.IsEmpty());
        REQUIRE(str.GetView().GetSize() == 1);
        REQUIRE(str == str);
        REQUIRE(str.GetView()[0] == u8'a');
    }

    SECTION("Ctor")
    {
        auto str = String(u8"abcde");
        REQUIRE(str.GetView().GetSize() == 5);
        REQUIRE(str.GetView()[4] == u8'e');
    }

    SECTION("Ctor")
    {
        auto str = String(u8"0123456789");
        REQUIRE(str.GetView().GetSize() == 10);
        auto str2 = str;
        REQUIRE((int)str2.GetView().GetSize() == (int)str.GetView().GetSize());
        REQUIRE(str2 == str);
    }

    SECTION("Ctor")
    {
        auto str = String(u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
        auto str2 = str;
        REQUIRE(str2.GetView().GetSize() == str.GetView().GetSize());
        REQUIRE(str2 == str);
    }

    SECTION("Move")
    {
        auto str = String();

        SECTION("small")
        {
            str = u8"abcdef";
        }

        SECTION("large")
        {
            str = u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
        }

        auto str1 = str;
        auto str2 = std::move(str);
        REQUIRE(!str1.IsEmpty());
        REQUIRE(!str2.IsEmpty());
        REQUIRE(str.IsEmpty());

        auto str3 = String();
        str3 = str1;
        REQUIRE(!str1.IsEmpty());
        REQUIRE(!str3.IsEmpty());

        str3 = std::move(str2);
        REQUIRE(str2.IsEmpty());
        REQUIRE(!str3.IsEmpty());
    }

    SECTION("Clear")
    {
        auto str = String();

        SECTION("Small")
        {
            str = String(u8"abcdef");
        }

        SECTION("Large")
        {
            str = String(u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
        }

        auto str2 = str;
        str.Clear();
        REQUIRE(str.IsEmpty());
        REQUIRE(!str2.IsEmpty());
    }

    SECTION("Erase")
    {
        SECTION("Small")
        {
            auto str = String(u8"abc");

            str.Erase(String::IndexType(0), String::IndexType(0));
            REQUIRE(str == String(u8"abc"));
            REQUIRE(str.GetView().GetSize() == 3);

            auto str2 = str;
            str.Erase(String::IndexType(0), String::IndexType(1));
            REQUIRE(str == String(u8"bc"));
            REQUIRE(str.GetView().GetSize() == 2);
            REQUIRE(str2.GetView().GetSize() == 3);

            auto str3 = str;
            str.Erase(String::IndexType(1), String::IndexType(2));
            REQUIRE(str == String(u8"b"));
            REQUIRE(str.GetView().GetSize() == 1);
            REQUIRE(str3.GetView().GetSize() == 2);

            str.Erase(String::IndexType(0), String::IndexType(str.GetView().GetSize()));
            REQUIRE(str.GetView().GetSize() == 0);

            str.Erase(String::IndexType(0), String::IndexType(0));
            REQUIRE(str.GetView().GetSize() == 0);
        }

        SECTION("Large")
        {
            auto str = String(u8"The quick brown fox jumps over the lazy dog 01234567890");
            str.Erase(String::IndexType(4), String::IndexType(20));
            REQUIRE(str == String(u8"The jumps over the lazy dog 01234567890"));
            str.Erase(String::IndexType(4), String::IndexType(str.GetView().GetSize()));
            REQUIRE(str == String(u8"The "));
            str.Erase(String::IndexType(0), String::IndexType(3));
            REQUIRE(str == String(u8" "));
            str.Erase(String::IndexType(0), String::IndexType(1));
            REQUIRE(str == String(u8""));
        }
    }

    SECTION("Replace")
    {
        const char8_t* s = nullptr;

        SECTION("Empty")
        {
            s = u8"";
        }

        SECTION("Small")
        {
            s = u8"012345678901234567890";
        }

        SECTION("Large")
        {
            s = u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
        }

        auto str1 = String(s);
        auto str2 = std::u8string(s);

        auto testEquality = [&] { return str1 == String(StringView(str2.data(), SInt64(str2.size()))); };

        REQUIRE_NOTHROW(str1.Replace(String::IndexType(0), String::IndexType(0), u8" "));
        REQUIRE_NOTHROW(str2.replace(0, 0, u8" "));
        REQUIRE(testEquality());

        REQUIRE_NOTHROW(str1.Replace(String::IndexType(0), String::IndexType(1), u8" "));
        REQUIRE_NOTHROW(str2.replace(0, 1, u8" "));
        REQUIRE(testEquality());

        REQUIRE_NOTHROW(str1.Replace(String::IndexType(str1.GetView().GetSize() - 1), String::IndexType(str1.GetView().GetSize()), u8" "));
        REQUIRE_NOTHROW(str2.replace(str2.length() - 1, 1, u8" "));
        REQUIRE(testEquality());

        REQUIRE_NOTHROW(str1.Replace(String::IndexType(str1.GetView().GetSize()), String::IndexType(str1.GetView().GetSize()), u8"a"));
        REQUIRE_NOTHROW(str2.replace(str2.end(), str2.end(), u8"a"));
        REQUIRE(testEquality());

        REQUIRE_NOTHROW(str1.Replace(String::IndexType(0), String::IndexType(str1.GetView().GetSize()), u8""));
        REQUIRE_NOTHROW(str2.replace(str2.begin(), str2.end(), u8""));
        REQUIRE(testEquality());

        if (str1.GetView().GetSize() >= 5)
        {
            REQUIRE_NOTHROW(str1.Replace(String::IndexType(3), String::IndexType(5), u8"12345"));
            REQUIRE_NOTHROW(str2.replace(3, 2, u8"12345"));
            REQUIRE(testEquality());

            REQUIRE_NOTHROW(str1.Replace(String::IndexType(3), String::IndexType(5), u8"x"));
            REQUIRE_NOTHROW(str2.replace(3, 2, u8"x"));
            REQUIRE(testEquality());
        }

        if (str1.GetView().GetSize() >= 1)
        {
            REQUIRE_NOTHROW(str1.Replace(String::IndexType(str1.GetView().GetSize() - 1), String::IndexType(str1.GetView().GetSize()), u8""));
            REQUIRE_NOTHROW(str2.replace(str2.length() - 1, 1, u8""));
            REQUIRE(testEquality());

            REQUIRE_NOTHROW(str1.Replace(String::IndexType(str1.GetView().GetSize() - 1), String::IndexType(str1.GetView().GetSize()), s));
            REQUIRE_NOTHROW(str2.replace(str2.length() - 1, 1, s));
            REQUIRE(testEquality());
        }
    }

    SECTION("Insert")
    {
        auto str = String(u8"abc");
        str.Insert(String::IndexType(0), u8"");
        REQUIRE(str == String(u8"abc"));
        str.Insert(String::IndexType(0), u8"012");
        REQUIRE(str == String(u8"012abc"));
        str.Insert(String::IndexType(str.GetView().GetSize()), u8"345");
        REQUIRE(str == String(u8"012abc345"));
        str.Insert(String::IndexType(6), u8"def");
        REQUIRE(str == String(u8"012abcdef345"));
    }

    SECTION("Substring")
    {
        auto str = String();

        SECTION("Small")
        {
            str = String(u8"abc123");
        }

        SECTION("Large")
        {
            str = String(u8"abc123zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
        }

        REQUIRE(str.GetSubString(String::IndexType(0), String::IndexType(3)) == String(u8"abc"));
        REQUIRE(str.GetSubString(String::IndexType(3), String::IndexType(6)) == String(u8"123"));
        REQUIRE(str.GetSubString(String::IndexType(0), String::IndexType(0)) == String(u8""));
        REQUIRE(str.GetSubString(String::IndexType(str.GetView().GetSize()), String::IndexType(str.GetView().GetSize())).IsEmpty());
    }

    SECTION("Swap")
    {
        auto const small = String(u8"abcdef");
        auto const large = String(u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");

        auto str1 = small;
        auto str2 = large;

        str1.Swap(str2);
        REQUIRE(str1 == large);
        REQUIRE(str2 == small);

        str1.Swap(str2);
        REQUIRE(str1 == small);
        REQUIRE(str2 == large);
    }
}
