#include <catch2/catch_all.hpp>

#include <Futurewalker.Graphics.Color.hpp>

using namespace Futurewalker;

TEST_CASE("RGBColor")
{
    auto c = RGBColor(0, 0.5, 1);
    REQUIRE(c.GetRed() == 0);
    REQUIRE(c.GetGreen() == 0.5);
    REQUIRE(c.GetBlue() == 1);
    REQUIRE(c == c);
    REQUIRE(c != RGBColor(0, 0, 0));

    c.GetRed();
    c.GetRedU8();
    c.GetRedU16();
    c.GetRedU32();
    c.GetRedF32();
    c.GetRedF64();
    c.SetRed(RGBChannel(0.0));
    c.SetRedU8(UInt8(0));
    c.SetRedU16(UInt16(0));
    c.SetRedU32(UInt32(0));
    c.SetRedF32(Float32(0));
    c.SetRedF64(Float64(0));

    c.GetGreen();
    c.GetGreenU8();
    c.GetGreenU16();
    c.GetGreenU32();
    c.GetGreenF32();
    c.GetGreenF64();
    c.SetGreen(RGBChannel(0.0));
    c.SetGreenU8(UInt8(0));
    c.SetGreenU16(UInt16(0));
    c.SetGreenU32(UInt32(0));
    c.SetGreenF32(Float32(0));
    c.SetGreenF64(Float64(0));

    c.GetBlue();
    c.GetBlueU8();
    c.GetBlueU16();
    c.GetBlueU32();
    c.GetBlueF32();
    c.GetBlueF64();
    c.SetBlue(RGBChannel(0.0));
    c.SetBlueU8(UInt8(0));
    c.SetBlueU16(UInt16(0));
    c.SetBlueU32(UInt32(0));
    c.SetBlueF32(Float32(0));
    c.SetBlueF64(Float64(0));
}
