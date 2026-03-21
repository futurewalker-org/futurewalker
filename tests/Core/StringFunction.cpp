#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.StringFunction.hpp>

using namespace Futurewalker;

TEST_CASE("StringFunction")
{
    SECTION("Format")
    {
        String result;

        SECTION("Arithmetic")
        {
            int32_t v1 = 1;
            int64_t v2 = 2;
            uint32_t v3 = 3;
            uint64_t v4 = 4;
            float32_t v5 = 5;
            float64_t v6 = 6;
            SInt32 v7 = 7;
            SInt64 v8 = 8;
            UInt32 v9 = 9u;
            UInt64 v10 = 10u;
            Float32 v11 = 11.0f;
            Float64 v12 = 12.0f;
            int8_t v13 = 13;
            uint8_t v14 = 14;

            REQUIRE_NOTHROW(result = StringFunction::Format(u8"{}{}{}{}{}{}{}{}{}{}{}{}{}{}", v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14));
            REQUIRE(result == u8"1234567891011121314");
        }

        SECTION("Boolean")
        {
            REQUIRE_NOTHROW(result = StringFunction::Format(u8"{}{}", true, false));
            REQUIRE(result == u8"truefalse");
        }

        SECTION("String")
        {
            SECTION("")
            {
                REQUIRE_NOTHROW(result = StringFunction::Format(u8"{}", u8"42"));
                REQUIRE(result == u8"42");
            }

            SECTION("")
            {
                REQUIRE_NOTHROW(result = StringFunction::Format(u8"{}", String(u8"42")));
                REQUIRE(result == u8"42");
            }

            SECTION("")
            {
                REQUIRE_NOTHROW(result = StringFunction::Format(u8"{}", StringView(u8"42")));
                REQUIRE(result == u8"42");
            }

            SECTION("")
            {
                REQUIRE_NOTHROW(result = StringFunction::Format(u8"{}{}", u8'4', u8'2'));
                REQUIRE(result == u8"42");
            }

            SECTION("")
            {
                REQUIRE_NOTHROW(result = StringFunction::Format(u8"{}{}{}{}", u8"1", String(u8"2"), StringView(u8"3"), u8'4'));
                REQUIRE(result == String(u8"1234"));
            }
        }

        SECTION("Error")
        {
            SECTION("Extra")
            {
                result = u8"error";
                REQUIRE_NOTHROW(result = StringFunction::Format(u8"", 42)); // Unused parameter should be OK.
                REQUIRE(result == u8"");
            }

            SECTION("Missing")
            {
                REQUIRE_THROWS_AS(StringFunction::Format(u8"{}"), Exception); // Missing parameter should throw exception.
            }
        }
    }

    SECTION("Join")
    {
        SECTION("0")
        {
            std::vector<String> parts = {};
            REQUIRE(StringFunction::Join(u8",", parts) == u8"");
        }

        SECTION("1")
        {
            std::vector<String> parts = {u8"one"};
            REQUIRE(StringFunction::Join(u8",", parts) == u8"one");
        }

        SECTION("3")
        {
            std::vector<String> parts = {u8"one", u8"two", u8"three"};
            REQUIRE(StringFunction::Join(u8",", parts) == u8"one,two,three");
        }
    }

    SECTION("StripTrailingBreakAndSpace")
    {
        SECTION("No trailing break or space")
        {
            String text(u8"hello");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello");
        }

        SECTION("Trailing single space")
        {
            String text(u8"hello ");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello");
        }

        SECTION("Trailing multiple spaces")
        {
            String text(u8"hello   ");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello");
        }

        SECTION("Trailing line feed")
        {
            String text(u8"hello\n");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello");
        }

        SECTION("Trailing carriage return")
        {
            String text(u8"hello\r");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello");
        }

        SECTION("Trailing CRLF")
        {
            String text(u8"hello\r\n");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello");
        }

        SECTION("Trailing mixed breaks and spaces")
        {
            String text(u8"hello \n \r\n ");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello");
        }

        SECTION("All spaces returns empty")
        {
            String text(u8"   ");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"");
        }

        SECTION("All breaks returns empty")
        {
            String text(u8"\n\r\n");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"");
        }

        SECTION("Single character preserved")
        {
            String text(u8"x");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"x");
        }

        SECTION("Interior spaces preserved")
        {
            String text(u8"hello world  ");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello world");
        }

        SECTION("Interior breaks preserved")
        {
            String text(u8"hello\nworld\n");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello\nworld");
        }

        SECTION("Tab is not stripped")
        {
            String text(u8"hello\t");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello\t");
        }

        SECTION("No-break space U+00A0 is stripped")
        {
            String text(u8"hello\u00A0");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello");
        }

        SECTION("Unicode next line U+0085 is stripped")
        {
            String text(u8"hello\u0085");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello");
        }

        SECTION("Unicode line separator U+2028 is stripped")
        {
            String text(u8"hello\u2028");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello");
        }

        SECTION("Unicode paragraph separator U+2029 is stripped")
        {
            String text(u8"hello\u2029");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"hello");
        }

        SECTION("Subrange from middle to end")
        {
            String text(u8"hello world\n");
            auto begin = text.GetNext(text.GetBegin(), 6);
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, begin, text.GetEnd()) == u8"world");
        }

        SECTION("Subrange excluding trailing content")
        {
            String text(u8"hello world\nfoo");
            auto end = text.GetNext(text.GetBegin(), 12);
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), end) == u8"hello world");
        }

        SECTION("Subrange with trailing spaces")
        {
            String text(u8"hello world  \nfoo");
            auto begin = text.GetNext(text.GetBegin(), 6);
            auto end = text.GetNext(text.GetBegin(), 14);
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, begin, end) == u8"world");
        }

        SECTION("Multibyte content with trailing break")
        {
            String text(u8"\u00E9\u00E0\n");
            REQUIRE(StringFunction::StripTrailingBreakAndSpace(text, text.GetBegin(), text.GetEnd()) == u8"\u00E9\u00E0");
        }
    }
}
