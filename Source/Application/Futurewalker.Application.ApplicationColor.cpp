// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ApplicationColor.hpp"

#include <utility>
#include <array> 

namespace FW_DETAIL_NS
{
namespace
{
struct Entry
{
    int64_t idx = 0;
    int64_t hex = 0;
};

auto constexpr hexPrimary = std::array {
  Entry {0, 0x000000FF},
  Entry {5, 0x180003FF},
  Entry {10, 0x250059FF},
  Entry {15, 0x320073FF},
  Entry {20, 0x38008dFF},
  Entry {20, 0x38008dFF},
  Entry {25, 0x4B169BFF},
  Entry {30, 0x5727A6FF},
  Entry {35, 0x6336B3FF},
  Entry {40, 0x6F43C0FF},
  Entry {50, 0x895EDBFF},
  Entry {60, 0xA478F7FF},
  Entry {70, 0xBC99FFFF},
  Entry {80, 0xD3BBFFFF},
  Entry {90, 0xEBDDFFFF},
  Entry {95, 0xF7EDFFFF},
  Entry {98, 0xFEF7FFFF},
  Entry {99, 0xFFFBFFFF},
  Entry {100, 0xFFFFFFFF},
};

auto constexpr hexSecondary = std::array {
  Entry {0, 0x000000FF},
  Entry {5, 0x150C25FF},
  Entry {10, 0x201731FF},
  Entry {15, 0x2A213BFF},
  Entry {20, 0x352C47FF},
  Entry {25, 0x403752FF},
  Entry {30, 0x4C425EFF},
  Entry {35, 0x584E6AFF},
  Entry {40, 0x645977FF},
  Entry {50, 0x7D7291FF},
  Entry {60, 0x978BABFF},
  Entry {70, 0xB2A6C7FF},
  Entry {80, 0xCEC1E3FF},
  Entry {90, 0xEBDDFFFF},
  Entry {95, 0xF7EDFFFF},
  Entry {98, 0xFEF7FFFF},
  Entry {99, 0xFFFBFFFF},
  Entry {100, 0xFFFFFFFF},
};

auto constexpr hexTertiary = std::array {
  Entry {0, 0x000000FF},
  Entry {5, 0x290210FF},
  Entry {10, 0x370B1BFF},
  Entry {15, 0x441525FF},
  Entry {20, 0x512030FF},
  Entry {25, 0x5E2B3BFF},
  Entry {30, 0x6C3646FF},
  Entry {35, 0x7A4152FF},
  Entry {40, 0x874D5DFF},
  Entry {50, 0xA46576FF},
  Entry {60, 0xC17E8FFF},
  Entry {70, 0xDE97AAFF},
  Entry {80, 0xFDB2C5FF},
  Entry {90, 0xFFD9E1FF},
  Entry {95, 0xFFECEFFF},
  Entry {98, 0xFFF8F8FF},
  Entry {99, 0xFFFBFFFF},
  Entry {100, 0xFFFFFFFF},
};

auto constexpr hexError = std::array {
  Entry {0, 0x000000FF},
  Entry {5, 0x2D0001FF},
  Entry {10, 0x410002FF},
  Entry {15, 0x540003FF},
  Entry {20, 0x690005FF},
  Entry {25, 0x7E0007FF},
  Entry {30, 0x93000AFF},
  Entry {35, 0xA80710FF},
  Entry {40, 0xBA1A1AFF},
  Entry {50, 0xDE3730FF},
  Entry {60, 0xFF5449FF},
  Entry {70, 0xFF897DFF},
  Entry {80, 0xFF84ABFF},
  Entry {90, 0xFFDAD6FF},
  Entry {95, 0xFFEDEAFF},
  Entry {98, 0xFFF8F7FF},
  Entry {99, 0xFFFBFFFF},
  Entry {100, 0xFFFFFFFF},
};

auto constexpr hexNeutral = std::array {
  Entry {0, 0x000000FF},
  Entry {5, 0x121914FF},
  Entry {10, 0x1D1B1EFF},
  Entry {15, 0x272529FF},
  Entry {20, 0x323033FF},
  Entry {25, 0x3D3A3EFF},
  Entry {30, 0x48464AFF},
  Entry {35, 0x545155FF},
  Entry {40, 0x605D61FF},
  Entry {50, 0x79767AFF},
  Entry {60, 0x938F94FF},
  Entry {70, 0xAEAAAEFF},
  Entry {80, 0x938F94FF},
  Entry {90, 0xE6E1E6FF},
  Entry {95, 0xF5EFF4FF},
  Entry {98, 0xFEF8FDFF},
  Entry {99, 0xFFFBFFFF},
  Entry {100, 0xFFFFFFFF},
};

auto constexpr hexNeutralVariant = std::array {
  Entry {0, 0x000000FF},
  Entry {5, 0x121017FF},
  Entry {10, 0x1D1A22FF},
  Entry {15, 0x28242DFF},
  Entry {20, 0x322F37FF},
  Entry {25, 0x3E3A43FF},
  Entry {30, 0x49454EFF},
  Entry {35, 0x55515AFF},
  Entry {40, 0x615D66FF},
  Entry {50, 0x7A757FFF},
  Entry {60, 0x948F99FF},
  Entry {70, 0xAFA9B4FF},
  Entry {80, 0xCBC4CFFF},
  Entry {90, 0xE7E0EBFF},
  Entry {95, 0xF6EEFAFF},
  Entry {98, 0xFEF7FFFF},
  Entry {99, 0xFFFBFFFF},
  Entry {100, 0xFFFFFFFF},
};

consteval auto MakeRGBA(auto r, auto g, auto b, auto a)
{
    return RGBAColor::MakeFromU8(UInt8(r), UInt8(g), UInt8(b), UInt8(a));
}

consteval auto HexToRGBA(auto const hex)
{
    return MakeRGBA( //
      (static_cast<uint32_t>(hex) & 0xFF000000) >> 24,
      (static_cast<uint32_t>(hex) & 0x00FF0000) >> 16,
      (static_cast<uint32_t>(hex) & 0x0000FF00) >> 8,
      (static_cast<uint32_t>(hex) & 0x000000FF) >> 0);
}

consteval auto GetHexRange(auto const hexTable, auto const index) -> std::pair<Entry, Entry>
{
    auto const hexTableSize = std::ssize(hexTable);
    for (auto i = 0z; i < hexTableSize; ++i)
    {
        if (hexTable[i].idx == index)
        {
            return {hexTable[i], hexTable[i]};
        }
        else if (hexTable[i].idx > index)
        {
            return {hexTable[std::max(0z, i - 1)], hexTable[i]};
        }
    }
    return {hexTable[hexTableSize], hexTable[hexTableSize]};
}


consteval auto LerpRGBA(RGBAColor const a, RGBAColor const b, Float64 const t)
{
    auto const cR = std::lerp(static_cast<float64_t>(a.GetRed().GetF64()), static_cast<float64_t>(b.GetRed().GetF64()), static_cast<float64_t>(t));
    auto const cG = std::lerp(static_cast<float64_t>(a.GetGreen().GetF64()), static_cast<float64_t>(b.GetGreen().GetF64()), static_cast<float64_t>(t));
    auto const cB = std::lerp(static_cast<float64_t>(a.GetBlue().GetF64()), static_cast<float64_t>(b.GetBlue().GetF64()), static_cast<float64_t>(t));
    auto const cA = std::lerp(static_cast<float64_t>(a.GetAlpha().GetF64()), static_cast<float64_t>(b.GetAlpha().GetF64()), static_cast<float64_t>(t));
    return RGBAColor(cR, cG, cB, cA);
}

consteval auto GetRGBAFromHex(auto const hexTable, auto const index)
{
    auto const [lo, hi] = GetHexRange(hexTable, index);
    if (lo.idx == hi.idx)
    {
        return HexToRGBA(lo.hex);
    }
    auto const r = float64_t(index - lo.idx) / float64_t(hi.idx - lo.idx);
    return LerpRGBA(HexToRGBA(lo.hex), HexToRGBA(hi.hex), r);
}
}

StaticAttribute<RGBAColor> const ApplicationColor::Primary0 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,0));
StaticAttribute<RGBAColor> const ApplicationColor::Primary1 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,1));
StaticAttribute<RGBAColor> const ApplicationColor::Primary2 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,2));
StaticAttribute<RGBAColor> const ApplicationColor::Primary3 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,3));
StaticAttribute<RGBAColor> const ApplicationColor::Primary4 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,4));
StaticAttribute<RGBAColor> const ApplicationColor::Primary5 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,5));
StaticAttribute<RGBAColor> const ApplicationColor::Primary6 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,6));
StaticAttribute<RGBAColor> const ApplicationColor::Primary7 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,7));
StaticAttribute<RGBAColor> const ApplicationColor::Primary8 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,8));
StaticAttribute<RGBAColor> const ApplicationColor::Primary9 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,9));
StaticAttribute<RGBAColor> const ApplicationColor::Primary10 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,10));
StaticAttribute<RGBAColor> const ApplicationColor::Primary11 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,11));
StaticAttribute<RGBAColor> const ApplicationColor::Primary12 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,12));
StaticAttribute<RGBAColor> const ApplicationColor::Primary13 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,13));
StaticAttribute<RGBAColor> const ApplicationColor::Primary14 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,14));
StaticAttribute<RGBAColor> const ApplicationColor::Primary15 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,15));
StaticAttribute<RGBAColor> const ApplicationColor::Primary16 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,16));
StaticAttribute<RGBAColor> const ApplicationColor::Primary17 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,17));
StaticAttribute<RGBAColor> const ApplicationColor::Primary18 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,18));
StaticAttribute<RGBAColor> const ApplicationColor::Primary19 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,19));
StaticAttribute<RGBAColor> const ApplicationColor::Primary20 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,20));
StaticAttribute<RGBAColor> const ApplicationColor::Primary25 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,25));
StaticAttribute<RGBAColor> const ApplicationColor::Primary30 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,30));
StaticAttribute<RGBAColor> const ApplicationColor::Primary40 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,40));
StaticAttribute<RGBAColor> const ApplicationColor::Primary50 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,50));
StaticAttribute<RGBAColor> const ApplicationColor::Primary60 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,60));
StaticAttribute<RGBAColor> const ApplicationColor::Primary70 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,70));
StaticAttribute<RGBAColor> const ApplicationColor::Primary80 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,80));
StaticAttribute<RGBAColor> const ApplicationColor::Primary81 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,81));
StaticAttribute<RGBAColor> const ApplicationColor::Primary82 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,82));
StaticAttribute<RGBAColor> const ApplicationColor::Primary83 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,83));
StaticAttribute<RGBAColor> const ApplicationColor::Primary84 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,84));
StaticAttribute<RGBAColor> const ApplicationColor::Primary85 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,85));
StaticAttribute<RGBAColor> const ApplicationColor::Primary86 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,86));
StaticAttribute<RGBAColor> const ApplicationColor::Primary87 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,87));
StaticAttribute<RGBAColor> const ApplicationColor::Primary88 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,88));
StaticAttribute<RGBAColor> const ApplicationColor::Primary89 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,89));
StaticAttribute<RGBAColor> const ApplicationColor::Primary90 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,90));
StaticAttribute<RGBAColor> const ApplicationColor::Primary91 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,91));
StaticAttribute<RGBAColor> const ApplicationColor::Primary92 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,92));
StaticAttribute<RGBAColor> const ApplicationColor::Primary93 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,93));
StaticAttribute<RGBAColor> const ApplicationColor::Primary94 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,94));
StaticAttribute<RGBAColor> const ApplicationColor::Primary95 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,95));
StaticAttribute<RGBAColor> const ApplicationColor::Primary96 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,96));
StaticAttribute<RGBAColor> const ApplicationColor::Primary97 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,97));
StaticAttribute<RGBAColor> const ApplicationColor::Primary98 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,98));
StaticAttribute<RGBAColor> const ApplicationColor::Primary99 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,99));
StaticAttribute<RGBAColor> const ApplicationColor::Primary100 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexPrimary,100));

StaticAttribute<RGBAColor> const ApplicationColor::Secondary0 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,0));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary1 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,1));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary2 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,2));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary3 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,3));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary4 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,4));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary5 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,5));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary6 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,6));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary7 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,7));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary8 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,8));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary9 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,9));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary10 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,10));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary11 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,11));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary12 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,12));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary13 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,13));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary14 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,14));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary15 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,15));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary16 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,16));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary17 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,17));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary18 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,18));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary19 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,19));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary20 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,20));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary25 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,25));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary30 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,30));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary40 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,40));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary50 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,50));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary60 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,60));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary70 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,70));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary80 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,80));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary81 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,81));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary82 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,82));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary83 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,83));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary84 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,84));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary85 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,85));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary86 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,86));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary87 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,87));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary88 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,88));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary89 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,89));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary90 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,90));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary91 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,91));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary92 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,92));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary93 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,93));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary94 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,94));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary95 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,95));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary96 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,96));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary97 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,97));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary98 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,98));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary99 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,99));
StaticAttribute<RGBAColor> const ApplicationColor::Secondary100 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexSecondary,100));

StaticAttribute<RGBAColor> const ApplicationColor::Tertiary0 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,0));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary1 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,1));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary2 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,2));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary3 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,3));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary4 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,4));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary5 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,5));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary6 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,6));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary7 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,7));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary8 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,8));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary9 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,9));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary10 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,10));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary11 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,11));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary12 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,12));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary13 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,13));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary14 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,14));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary15 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,15));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary16 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,16));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary17 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,17));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary18 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,18));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary19 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,19));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary20 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,20));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary25 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,25));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary30 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,30));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary40 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,40));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary50 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,50));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary60 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,60));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary70 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,70));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary80 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,80));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary81 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,81));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary82 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,82));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary83 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,83));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary84 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,84));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary85 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,85));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary86 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,86));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary87 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,87));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary88 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,88));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary89 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,89));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary90 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,90));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary91 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,91));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary92 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,92));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary93 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,93));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary94 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,94));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary95 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,95));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary96 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,96));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary97 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,97));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary98 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,98));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary99 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,99));
StaticAttribute<RGBAColor> const ApplicationColor::Tertiary100 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexTertiary,100));

StaticAttribute<RGBAColor> const ApplicationColor::Error0 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 0));
StaticAttribute<RGBAColor> const ApplicationColor::Error5 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 5));
StaticAttribute<RGBAColor> const ApplicationColor::Error10 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 10));
StaticAttribute<RGBAColor> const ApplicationColor::Error15 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 15));
StaticAttribute<RGBAColor> const ApplicationColor::Error20 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 20));
StaticAttribute<RGBAColor> const ApplicationColor::Error25 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 25));
StaticAttribute<RGBAColor> const ApplicationColor::Error30 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 30));
StaticAttribute<RGBAColor> const ApplicationColor::Error35 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 35));
StaticAttribute<RGBAColor> const ApplicationColor::Error40 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 40));
StaticAttribute<RGBAColor> const ApplicationColor::Error50 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 50));
StaticAttribute<RGBAColor> const ApplicationColor::Error60 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 60));
StaticAttribute<RGBAColor> const ApplicationColor::Error70 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 70));
StaticAttribute<RGBAColor> const ApplicationColor::Error80 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 80));
StaticAttribute<RGBAColor> const ApplicationColor::Error90 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 90));
StaticAttribute<RGBAColor> const ApplicationColor::Error95 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 95));
StaticAttribute<RGBAColor> const ApplicationColor::Error98 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 98));
StaticAttribute<RGBAColor> const ApplicationColor::Error99 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 99));
StaticAttribute<RGBAColor> const ApplicationColor::Error100 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexError, 100));

StaticAttribute<RGBAColor> const ApplicationColor::Neutral0 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 0));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral1 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 1));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral2 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 2));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral3 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 3));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral4 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 4));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral5 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 5));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral6 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 6));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral7 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 7));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral8 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 8));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral9 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 9));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral10 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 10));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral11 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 11));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral12 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 12));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral13 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 13));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral14 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 14));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral15 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 15));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral16 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 16));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral17 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 17));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral18 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 18));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral19 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 19));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral20 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 20));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral21 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 21));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral22 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 22));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral23 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 23));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral24 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 24));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral25 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 25));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral26 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 26));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral27 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 27));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral28 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 28));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral29 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 29));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral30 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 30));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral40 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 40));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral50 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 50));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral60 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 60));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral70 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 70));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral71 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 71));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral72 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 72));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral73 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 73));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral74 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 74));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral75 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 75));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral76 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 76));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral77 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 77));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral78 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 78));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral79 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 79));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral80 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 80));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral81 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 81));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral82 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 82));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral83 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 83));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral84 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 84));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral85 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 85));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral86 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 86));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral87 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 87));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral88 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 88));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral89 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 89));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral90 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 90));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral91 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 91));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral92 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 92));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral93 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 93));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral94 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 94));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral95 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 95));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral96 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 96));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral97 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 97));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral98 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 98));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral99 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 99));
StaticAttribute<RGBAColor> const ApplicationColor::Neutral100 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutral, 100));

StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant0 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 0));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant1 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 1));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant2 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 2));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant3 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 3));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant4 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 4));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant5 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 5));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant6 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 6));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant7 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 7));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant8 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 8));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant9 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 9));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant10 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 10));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant11 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 11));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant12 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 12));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant13 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 13));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant14 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 14));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant15 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 15));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant16 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 16));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant17 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 17));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant18 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 18));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant19 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 19));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant20 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 20));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant21 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 21));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant22 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 22));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant23 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 23));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant24 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 24));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant25 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 25));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant26 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 26));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant27 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 27));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant28 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 28));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant29 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 29));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant30 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 30));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant40 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 40));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant50 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 50));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant60 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 60));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant70 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 70));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant71 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 71));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant72 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 72));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant73 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 73));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant74 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 74));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant75 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 75));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant76 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 76));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant77 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 77));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant78 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 78));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant79 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 79));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant80 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 80));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant81 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 81));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant82 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 82));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant83 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 83));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant84 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 84));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant85 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 85));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant86 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 86));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant87 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 87));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant88 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 88));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant89 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 89));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant90 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 90));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant91 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 91));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant92 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 92));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant93 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 93));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant94 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 94));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant95 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 95));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant96 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 96));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant97 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 97));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant98 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 98));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant99 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 99));
StaticAttribute<RGBAColor> const ApplicationColor::NeutralVariant100 = StaticAttribute<RGBAColor>::MakeWithDefaultValue(GetRGBAFromHex(hexNeutralVariant, 100));

StaticAttribute<RGBAColor> const ApplicationColor::Black = StaticAttribute<RGBAColor>::MakeWithDefaultValue(HexToRGBA(0x0000FF));
StaticAttribute<RGBAColor> const ApplicationColor::White = StaticAttribute<RGBAColor>::MakeWithDefaultValue(HexToRGBA(0xFFFFFF));
}
