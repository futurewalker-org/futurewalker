#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.String.hpp>

#pragma clang diagnostic ignored "-Wself-move"

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

    //SECTION("Ctor")
    //{
    //    auto str = String(U'\U0001F600');
    //    REQUIRE(str == String(u8"\xF0\x9F\x98\x80"));
    //    REQUIRE(str.GetView().GetSize() == 4);
    //}

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

    SECTION("Swap with empty")
    {
        auto empty = String();
        auto str = String(u8"hello");

        empty.Swap(str);
        REQUIRE(empty == String(u8"hello"));
        REQUIRE(str.IsEmpty());

        empty.Swap(str);
        REQUIRE(empty.IsEmpty());
        REQUIRE(str == String(u8"hello"));
    }

    SECTION("Comparison")
    {
        SECTION("Equality")
        {
            REQUIRE(String(u8"abc") == String(u8"abc"));
            REQUIRE_FALSE(String(u8"abc") == String(u8"abd"));
            REQUIRE_FALSE(String(u8"abc") == String(u8"ab"));
            REQUIRE_FALSE(String(u8"abc") == String(u8"abcd"));
            REQUIRE_FALSE(String(u8"abc") == String());
            REQUIRE(String() == String());
        }

        SECTION("Ordering")
        {
            REQUIRE((String(u8"abc") <=> String(u8"abc")) == std::strong_ordering::equal);
            REQUIRE((String(u8"abc") <=> String(u8"abd")) == std::strong_ordering::less);
            REQUIRE((String(u8"abd") <=> String(u8"abc")) == std::strong_ordering::greater);
            REQUIRE((String(u8"ab") <=> String(u8"abc")) == std::strong_ordering::less);
            REQUIRE((String(u8"abc") <=> String(u8"ab")) == std::strong_ordering::greater);
            REQUIRE((String() <=> String()) == std::strong_ordering::equal);
            REQUIRE((String() <=> String(u8"a")) == std::strong_ordering::less);
            REQUIRE((String(u8"a") <=> String()) == std::strong_ordering::greater);
        }

        SECTION("Relational operators")
        {
            REQUIRE(String(u8"abc") != String(u8"def"));
            REQUIRE_FALSE(String(u8"abc") != String(u8"abc"));
            REQUIRE(String(u8"abc") < String(u8"abd"));
            REQUIRE(String(u8"abc") <= String(u8"abc"));
            REQUIRE(String(u8"abd") > String(u8"abc"));
            REQUIRE(String(u8"abc") >= String(u8"abc"));
        }
    }

    SECTION("Append")
    {
        SECTION("To empty")
        {
            auto str = String();
            str.Append(u8"hello");
            REQUIRE(str == String(u8"hello"));
        }

        SECTION("Small to small")
        {
            auto str = String(u8"abc");
            str.Append(u8"def");
            REQUIRE(str == String(u8"abcdef"));
            REQUIRE(str.GetView().GetSize() == 6);
        }

        SECTION("Empty append")
        {
            auto str = String(u8"abc");
            str.Append(u8"");
            REQUIRE(str == String(u8"abc"));
        }

        SECTION("Growing small to large")
        {
            auto str = String(u8"abc");
            str.Append(u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore");
            REQUIRE(str.GetView()[0] == u8'a');
            REQUIRE(str.GetView()[1] == u8'b');
            REQUIRE(str.GetView()[2] == u8'c');
            REQUIRE(str.GetView()[3] == u8'L');
        }

        SECTION("COW on append")
        {
            auto str1 = String(u8"abc");
            auto str2 = str1;
            str1.Append(u8"def");
            REQUIRE(str1 == String(u8"abcdef"));
            REQUIRE(str2 == String(u8"abc"));
        }

        SECTION("Multiple appends")
        {
            auto str = String();
            str.Append(u8"a");
            str.Append(u8"b");
            str.Append(u8"c");
            REQUIRE(str == String(u8"abc"));
        }
    }

    SECTION("GetBegin and GetEnd")
    {
        SECTION("Empty")
        {
            auto str = String();
            REQUIRE(str.GetBegin() == str.GetEnd());
        }

        SECTION("Non-empty")
        {
            auto str = String(u8"abc");
            REQUIRE(str.GetBegin() == String::IndexType(0));
            REQUIRE(str.GetEnd() == String::IndexType(3));
            REQUIRE(str.GetBegin() != str.GetEnd());
        }

        SECTION("Multi-byte")
        {
            auto str = String(U'\U0001F600'); // 4-byte emoji
            REQUIRE(str.GetBegin() == String::IndexType(0));
            REQUIRE(str.GetEnd() == String::IndexType(4));
        }
    }

    SECTION("GetNext and GetPrev")
    {
        SECTION("ASCII")
        {
            auto str = String(u8"abc");
            auto idx = str.GetBegin();
            REQUIRE(idx == String::IndexType(0));

            idx = str.GetNext(idx);
            REQUIRE(idx == String::IndexType(1));

            idx = str.GetNext(idx);
            REQUIRE(idx == String::IndexType(2));

            idx = str.GetNext(idx);
            REQUIRE(idx == String::IndexType(3));
            REQUIRE(idx == str.GetEnd());

            // Walk backwards
            idx = str.GetPrev(idx);
            REQUIRE(idx == String::IndexType(2));

            idx = str.GetPrev(idx);
            REQUIRE(idx == String::IndexType(1));

            idx = str.GetPrev(idx);
            REQUIRE(idx == String::IndexType(0));
            REQUIRE(idx == str.GetBegin());
        }

        SECTION("Multi-byte UTF-8")
        {
            // U+00E9 = é (2 bytes: C3 A9)
            // U+4E16 = 世 (3 bytes: E4 B8 96)
            // U+1F600 = 😀 (4 bytes: F0 9F 98 80)
            auto str = String(u8"\u00E9\u4E16\U0001F600");
            // Total: 2 + 3 + 4 = 9 bytes
            REQUIRE(str.GetView().GetSize() == 9);

            auto idx = str.GetBegin();
            REQUIRE(idx == String::IndexType(0));

            idx = str.GetNext(idx); // skip 2-byte é
            REQUIRE(idx == String::IndexType(2));

            idx = str.GetNext(idx); // skip 3-byte 世
            REQUIRE(idx == String::IndexType(5));

            idx = str.GetNext(idx); // skip 4-byte 😀
            REQUIRE(idx == String::IndexType(9));
            REQUIRE(idx == str.GetEnd());

            // Walk backwards
            idx = str.GetPrev(idx);
            REQUIRE(idx == String::IndexType(5));

            idx = str.GetPrev(idx);
            REQUIRE(idx == String::IndexType(2));

            idx = str.GetPrev(idx);
            REQUIRE(idx == String::IndexType(0));
        }

        SECTION("Offset greater than 1")
        {
            auto str = String(u8"abcde");
            auto idx = str.GetBegin();
            idx = str.GetNext(idx, 3);
            REQUIRE(idx == String::IndexType(3));

            idx = str.GetPrev(idx, 2);
            REQUIRE(idx == String::IndexType(1));
        }
    }

    SECTION("GetChar")
    {
        SECTION("ASCII")
        {
            auto str = String(u8"abc");
            String::CharType ch = 0;
            REQUIRE(str.GetChar(String::IndexType(0), ch));
            REQUIRE(ch == U'a');
            REQUIRE(str.GetChar(String::IndexType(1), ch));
            REQUIRE(ch == U'b');
            REQUIRE(str.GetChar(String::IndexType(2), ch));
            REQUIRE(ch == U'c');
        }

        SECTION("Multi-byte")
        {
            auto str = String(U'\U0001F600');
            String::CharType ch = 0;
            REQUIRE(str.GetChar(String::IndexType(0), ch));
            REQUIRE(ch == U'\U0001F600');
        }

        SECTION("Mixed ASCII and multi-byte")
        {
            auto str = String(u8"a\u00E9b"); // a + é + b
            String::CharType ch = 0;

            REQUIRE(str.GetChar(String::IndexType(0), ch));
            REQUIRE(ch == U'a');

            REQUIRE(str.GetChar(String::IndexType(1), ch)); // é at byte 1
            REQUIRE(ch == U'\u00E9');

            REQUIRE(str.GetChar(String::IndexType(3), ch)); // b at byte 3
            REQUIRE(ch == U'b');
        }

        SECTION("Out of bounds")
        {
            auto str = String(u8"abc");
            String::CharType ch = 0;
            REQUIRE_FALSE(str.GetChar(String::IndexType(3), ch));
            REQUIRE_FALSE(str.GetChar(String::IndexType(100), ch));
        }

        SECTION("GetChar with next index")
        {
            auto str = String(u8"a\u00E9\U0001F600"); // a(1) + é(2) + 😀(4)
            String::CharType ch = 0;
            String::IndexType next(0);

            REQUIRE(str.GetChar(String::IndexType(0), ch, next));
            REQUIRE(ch == U'a');
            REQUIRE(next == String::IndexType(1));

            REQUIRE(str.GetChar(next, ch, next));
            REQUIRE(ch == U'\u00E9');
            REQUIRE(next == String::IndexType(3));

            REQUIRE(str.GetChar(next, ch, next));
            REQUIRE(ch == U'\U0001F600');
            REQUIRE(next == String::IndexType(7));
            REQUIRE(next == str.GetEnd());
        }

        SECTION("Iterate all code points")
        {
            auto str = String(u8"Hello");
            String::CharType ch = 0;
            String::IndexType idx = str.GetBegin();
            std::u32string result;

            while (str.GetChar(idx, ch, idx))
            {
                result += ch;
            }

            REQUIRE(result == U"Hello");
        }
    }

    SECTION("StringView conversion")
    {
        auto str = String(u8"hello");
        StringView sv = str;
        REQUIRE(sv.GetSize() == 5);
        REQUIRE(sv[0] == u8'h');
        REQUIRE(sv[4] == u8'o');
    }

    SECTION("Construct from StringView")
    {
        auto sv = StringView(u8"world");
        auto str = String(sv);
        REQUIRE(str == String(u8"world"));
        REQUIRE(str.GetView().GetSize() == 5);
    }

    SECTION("Construct with pointer and size")
    {
        const char8_t* data = u8"abcdefgh";
        auto str = String(data, SInt64(5));
        REQUIRE(str == String(u8"abcde"));
        REQUIRE(str.GetView().GetSize() == 5);
    }

    SECTION("Self-assignment")
    {
        SECTION("Copy self-assignment")
        {
            auto str = String(u8"hello");
            auto& ref = str;
            str = ref;
            REQUIRE(str == String(u8"hello"));
        }

        SECTION("Move self-assignment")
        {
            auto str = String(u8"hello");
            str = std::move(str);
            // After self-move, the string should still be in a valid state
            REQUIRE_NOTHROW(str.IsEmpty());
        }
    }

    SECTION("COW semantics")
    {
        SECTION("Copy does not affect original on Erase")
        {
            auto str1 = String(u8"abcdef");
            auto str2 = str1;
            str2.Erase(String::IndexType(0), String::IndexType(3));
            REQUIRE(str1 == String(u8"abcdef"));
            REQUIRE(str2 == String(u8"def"));
        }

        SECTION("Copy does not affect original on Replace")
        {
            auto str1 = String(u8"abcdef");
            auto str2 = str1;
            str2.Replace(String::IndexType(0), String::IndexType(3), u8"XYZ");
            REQUIRE(str1 == String(u8"abcdef"));
            REQUIRE(str2 == String(u8"XYZdef"));
        }

        SECTION("Copy does not affect original on Insert")
        {
            auto str1 = String(u8"abc");
            auto str2 = str1;
            str2.Insert(String::IndexType(1), u8"X");
            REQUIRE(str1 == String(u8"abc"));
            REQUIRE(str2 == String(u8"aXbc"));
        }

        SECTION("Copy does not affect original on Clear")
        {
            auto str1 = String(u8"abc");
            auto str2 = str1;
            str2.Clear();
            REQUIRE(str1 == String(u8"abc"));
            REQUIRE(str2.IsEmpty());
        }

        SECTION("Large string COW")
        {
            auto str1 = String(u8"The quick brown fox jumps over the lazy dog");
            auto str2 = str1;
            str2.Erase(String::IndexType(0), String::IndexType(10));
            REQUIRE(str1 == String(u8"The quick brown fox jumps over the lazy dog"));
            REQUIRE(str2 == String(u8"brown fox jumps over the lazy dog"));
        }
    }

    SECTION("UTF-8 multi-byte")
    {
        SECTION("2-byte characters")
        {
            auto str = String(u8"\u00C0\u00FF"); // À and ÿ
            REQUIRE(str.GetView().GetSize() == 4);

            String::CharType ch = 0;
            REQUIRE(str.GetChar(String::IndexType(0), ch));
            REQUIRE(ch == U'\u00C0');
            REQUIRE(str.GetChar(String::IndexType(2), ch));
            REQUIRE(ch == U'\u00FF');
        }

        SECTION("3-byte characters")
        {
            auto str = String(u8"\u4E16\u754C"); // 世界
            REQUIRE(str.GetView().GetSize() == 6);

            String::CharType ch = 0;
            REQUIRE(str.GetChar(String::IndexType(0), ch));
            REQUIRE(ch == U'\u4E16');
            REQUIRE(str.GetChar(String::IndexType(3), ch));
            REQUIRE(ch == U'\u754C');
        }

        SECTION("4-byte characters")
        {
            auto str = String(u8"\U0001F600\U0001F601"); // 😀😁
            REQUIRE(str.GetView().GetSize() == 8);

            String::CharType ch = 0;
            REQUIRE(str.GetChar(String::IndexType(0), ch));
            REQUIRE(ch == U'\U0001F600');
            REQUIRE(str.GetChar(String::IndexType(4), ch));
            REQUIRE(ch == U'\U0001F601');
        }

        SECTION("Mixed byte-width characters")
        {
            auto str = String(u8"A\u00E9\u4E16\U0001F600Z");
            // A(1) + é(2) + 世(3) + 😀(4) + Z(1) = 11 bytes
            REQUIRE(str.GetView().GetSize() == 11);

            String::CharType ch = 0;
            auto idx = str.GetBegin();

            REQUIRE(str.GetChar(idx, ch, idx));
            REQUIRE(ch == U'A');

            REQUIRE(str.GetChar(idx, ch, idx));
            REQUIRE(ch == U'\u00E9');

            REQUIRE(str.GetChar(idx, ch, idx));
            REQUIRE(ch == U'\u4E16');

            REQUIRE(str.GetChar(idx, ch, idx));
            REQUIRE(ch == U'\U0001F600');

            REQUIRE(str.GetChar(idx, ch, idx));
            REQUIRE(ch == U'Z');

            REQUIRE(idx == str.GetEnd());
        }

        SECTION("Erase multi-byte character")
        {
            auto str = String(u8"a\u00E9b"); // a + é(2 bytes) + b
            str.Erase(String::IndexType(1), String::IndexType(3)); // erase é
            REQUIRE(str == String(u8"ab"));
        }

        SECTION("Insert multi-byte character")
        {
            auto str = String(u8"ab");
            str.Insert(String::IndexType(1), String(U'\u00E9'));
            REQUIRE(str.GetView().GetSize() == 4); // a(1) + é(2) + b(1)

            String::CharType ch = 0;
            auto idx = str.GetBegin();
            REQUIRE(str.GetChar(idx, ch, idx));
            REQUIRE(ch == U'a');
            REQUIRE(str.GetChar(idx, ch, idx));
            REQUIRE(ch == U'\u00E9');
            REQUIRE(str.GetChar(idx, ch, idx));
            REQUIRE(ch == U'b');
        }

        SECTION("Replace with multi-byte")
        {
            auto str = String(u8"abc");
            str.Replace(String::IndexType(1), String::IndexType(2), String(U'\U0001F600'));
            // a(1) + 😀(4) + c(1) = 6 bytes
            REQUIRE(str.GetView().GetSize() == 6);

            String::CharType ch = 0;
            auto idx = str.GetBegin();
            REQUIRE(str.GetChar(idx, ch, idx));
            REQUIRE(ch == U'a');
            REQUIRE(str.GetChar(idx, ch, idx));
            REQUIRE(ch == U'\U0001F600');
            REQUIRE(str.GetChar(idx, ch, idx));
            REQUIRE(ch == U'c');
        }

        SECTION("Substring of multi-byte")
        {
            auto str = String(u8"a\u00E9\u4E16z"); // a(1) + é(2) + 世(3) + z(1)
            auto sub = str.GetSubString(String::IndexType(1), String::IndexType(6)); // é + 世
            REQUIRE(sub.GetView().GetSize() == 5);

            String::CharType ch = 0;
            auto idx = sub.GetBegin();
            REQUIRE(sub.GetChar(idx, ch, idx));
            REQUIRE(ch == U'\u00E9');
            REQUIRE(sub.GetChar(idx, ch, idx));
            REQUIRE(ch == U'\u4E16');
            REQUIRE(idx == sub.GetEnd());
        }
    }

    SECTION("Insert into empty string")
    {
        auto str = String();
        str.Insert(String::IndexType(0), u8"hello");
        REQUIRE(str == String(u8"hello"));
    }

    SECTION("Erase entire string")
    {
        SECTION("Small")
        {
            auto str = String(u8"abc");
            str.Erase(String::IndexType(0), String::IndexType(str.GetView().GetSize()));
            REQUIRE(str.IsEmpty());
            REQUIRE(str.GetView().GetSize() == 0);
        }

        SECTION("Large")
        {
            auto str = String(u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit");
            str.Erase(String::IndexType(0), String::IndexType(str.GetView().GetSize()));
            REQUIRE(str.IsEmpty());
            REQUIRE(str.GetView().GetSize() == 0);
        }
    }

    SECTION("Replace in empty string")
    {
        auto str = String();
        str.Replace(String::IndexType(0), String::IndexType(0), u8"hello");
        REQUIRE(str == String(u8"hello"));
    }

    SECTION("Replace with same content")
    {
        auto str = String(u8"abc");
        str.Replace(String::IndexType(0), String::IndexType(3), u8"abc");
        REQUIRE(str == String(u8"abc"));
    }

    SECTION("Replace entire string")
    {
        auto str = String(u8"abc");
        str.Replace(String::IndexType(0), String::IndexType(str.GetView().GetSize()), u8"xyz");
        REQUIRE(str == String(u8"xyz"));
    }

    SECTION("Chained mutations")
    {
        auto str = String(u8"Hello World");
        str.Erase(String::IndexType(5), String::IndexType(11));
        REQUIRE(str == String(u8"Hello"));
        str.Append(u8", ");
        REQUIRE(str == String(u8"Hello, "));
        str.Append(u8"World!");
        REQUIRE(str == String(u8"Hello, World!"));
        str.Insert(String::IndexType(7), u8"Beautiful ");
        REQUIRE(str == String(u8"Hello, Beautiful World!"));
        str.Replace(String::IndexType(7), String::IndexType(16), u8"Wonderful");
        REQUIRE(str == String(u8"Hello, Wonderful World!"));
    }

    SECTION("Construct from char32_t")
    {
        SECTION("BMP character")
        {
            auto str = String(U'\u00E9'); // é
            REQUIRE(str.GetView().GetSize() == 2);
            String::CharType ch = 0;
            REQUIRE(str.GetChar(String::IndexType(0), ch));
            REQUIRE(ch == U'\u00E9');
        }

        SECTION("Supplementary plane character")
        {
            auto str = String(U'\U0001F4A9'); // 💩
            REQUIRE(str.GetView().GetSize() == 4);
            String::CharType ch = 0;
            REQUIRE(str.GetChar(String::IndexType(0), ch));
            REQUIRE(ch == U'\U0001F4A9');
        }

        SECTION("ASCII via char32_t")
        {
            auto str = String(U'A');
            REQUIRE(str.GetView().GetSize() == 1);
            REQUIRE(str == String(u8"A"));
        }

        SECTION("Invalid code point produces replacement character")
        {
            auto str = String(char32_t(0x110000)); // beyond Unicode range
            String::CharType ch = 0;
            REQUIRE(str.GetChar(String::IndexType(0), ch));
            REQUIRE(ch == U'\uFFFD');
        }
    }

    SECTION("Large string operations")
    {
        SECTION("Build large string with appends")
        {
            auto str = String();
            for (int i = 0; i < 100; ++i)
            {
                str.Append(u8"abcdefghij");
            }
            REQUIRE(str.GetView().GetSize() == 1000);
        }

        SECTION("Copy and modify large string")
        {
            auto str = String(u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
            auto copy = str;
            copy.Append(u8" Extra text.");
            REQUIRE(str.GetView().GetSize() == 123);
            REQUIRE(copy.GetView().GetSize() == 135);
            REQUIRE_FALSE(str == copy);
        }
    }

    SECTION("Single character edge cases")
    {
        auto str = String(u8"x");
        REQUIRE(str.GetView().GetSize() == 1);
        REQUIRE(!str.IsEmpty());
        REQUIRE(str.GetView()[0] == u8'x');

        str.Erase(String::IndexType(0), String::IndexType(1));
        REQUIRE(str.IsEmpty());
        REQUIRE(str.GetView().GetSize() == 0);
    }

    SECTION("GetView returns consistent data")
    {
        auto str = String(u8"test");
        auto view1 = str.GetView();
        auto view2 = str.GetView();
        REQUIRE(view1.GetSize() == view2.GetSize());
        REQUIRE(view1 == view2);

        // After mutation, view should reflect new state
        str.Append(u8"!");
        auto view3 = str.GetView();
        REQUIRE(view3.GetSize() == 5);
        REQUIRE(view3[4] == u8'!');
    }

    SECTION("Multiple copies with independent mutations")
    {
        auto str = String(u8"original");
        auto copy1 = str;
        auto copy2 = str;
        auto copy3 = str;

        copy1.Append(u8"_1");
        copy2.Insert(String::IndexType(0), u8"pre_");
        copy3.Erase(String::IndexType(0), String::IndexType(4));

        REQUIRE(str == String(u8"original"));
        REQUIRE(copy1 == String(u8"original_1"));
        REQUIRE(copy2 == String(u8"pre_original"));
        REQUIRE(copy3 == String(u8"inal"));
    }
}
