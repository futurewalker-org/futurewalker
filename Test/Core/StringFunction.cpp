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
}
