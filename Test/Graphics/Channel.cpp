#include <catch2/catch_all.hpp>

#include <Futurewalker.Graphics.Color.hpp>

using namespace Futurewalker;

TEST_CASE("Channel")
{
    SECTION("Init")
    {
        SECTION("")
        {
            auto ch = AlphaChannel();
            REQUIRE(ch == 0.0);
        }

        SECTION("")
        {
            auto ch = AlphaChannel(0);
            REQUIRE(ch == 0.0);
            ch = 42;
            REQUIRE(ch == 42);
        }

        SECTION("")
        {
            auto ch = AlphaChannel(0.0);
            REQUIRE(ch == 0.0);
            ch = 3.14;
            REQUIRE(ch == 3.14);
        }
    }

    SECTION("u8")
    {
        auto ch = AlphaChannel::MakeFromU8(UInt8(255));
        REQUIRE(ch == 1.0);
        ch = AlphaChannel::MakeFromU8(UInt8(0));
        REQUIRE(ch == 0.0);
    }

    SECTION("u16")
    {
        auto ch = AlphaChannel::MakeFromU16(std::numeric_limits<uint16_t>::max());
        REQUIRE(ch == 1.0);
        ch = AlphaChannel::MakeFromU16(UInt16(0));
        REQUIRE(ch == 0.0);
    }

    SECTION("u32")
    {
        auto ch = AlphaChannel::MakeFromU32(std::numeric_limits<uint32_t>::max());
        REQUIRE(ch == 1.0);
        ch = AlphaChannel::MakeFromU32(UInt32(0));
        REQUIRE(ch == 0.0);
    }
}
