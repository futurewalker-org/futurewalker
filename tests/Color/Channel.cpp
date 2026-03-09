#include <catch2/catch_all.hpp>

#include <Futurewalker.Color.hpp>

using namespace Futurewalker;

TEST_CASE("Channel")
{
    SECTION("Init")
    {
        SECTION("")
        {
            auto ch = Channel();
            REQUIRE(ch == 0.0);
        }

        SECTION("")
        {
            auto ch = Channel(0);
            REQUIRE(ch == 0.0);
            ch = 42;
            REQUIRE(ch == 42);
        }

        SECTION("")
        {
            auto ch = Channel(0.0);
            REQUIRE(ch == 0.0);
            ch = 3.14;
            REQUIRE(ch == 3.14);
        }
    }

    SECTION("u8")
    {
        auto ch = Channel::MakeFromU8(UInt8(255));
        REQUIRE(ch == 1.0);
        ch = Channel::MakeFromU8(UInt8(0));
        REQUIRE(ch == 0.0);
    }

    SECTION("u16")
    {
        auto ch = Channel::MakeFromU16(std::numeric_limits<uint16_t>::max());
        REQUIRE(ch == 1.0);
        ch = Channel::MakeFromU16(UInt16(0));
        REQUIRE(ch == 0.0);
    }

    SECTION("u32")
    {
        auto ch = Channel::MakeFromU32(std::numeric_limits<uint32_t>::max());
        REQUIRE(ch == 1.0);
        ch = Channel::MakeFromU32(UInt32(0));
        REQUIRE(ch == 0.0);
    }

    SECTION("get/set")
    {
        auto ch = Channel(0.0);
        ch.GetValue();
        ch.GetU8();
        ch.GetU16();
        ch.GetU32();
        ch.GetF32();
        ch.GetF64();
        ch.SetValue(0.0);
        ch.SetU8(UInt8(0));
        ch.SetU16(UInt16(0));
        ch.SetU32(UInt32(0));
        ch.SetF32(0.f);
        ch.SetF64(0.0);
    }
}
