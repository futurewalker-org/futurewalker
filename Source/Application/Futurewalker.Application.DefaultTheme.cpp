// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.DefaultTheme.hpp"
#include "Futurewalker.Application.ApplicationColor.hpp"
#include "Futurewalker.Application.ApplicationStyle.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp" 

#include "Futurewalker.Color.hpp"

namespace FW_DETAIL_NS
{
namespace
{
consteval auto GetHexRange(auto const hexTable, auto const index)
{
    auto const hexTableSize = std::ssize(hexTable);
    for (auto i = 0z; i < hexTableSize; ++i)
    {
        if (hexTable[i].idx == index)
        {
            return std::make_pair(hexTable[i], hexTable[i]);
        }
        else if (hexTable[i].idx > index)
        {
            return std::make_pair(hexTable[std::max(0z, i - 1)], hexTable[i]);
        }
    }
    return std::make_pair(hexTable[hexTableSize], hexTable[hexTableSize]);
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
        return RGBAColor::MakeFromHex(lo.hex);
    }
    auto const r = float64_t(index - lo.idx) / float64_t(hi.idx - lo.idx);
    return LerpRGBA(RGBAColor::MakeFromHex(lo.hex), RGBAColor::MakeFromHex(hi.hex), r);
}
}

DefaultTheme::DefaultTheme(ThemeBrightness brightness)
  : _brightness(brightness)
{
}

auto DefaultTheme::Apply(AttributeNode& target) -> void
{
    ApplyApplicationColor(target);
    ApplyApplicationStyle(target);
}

auto DefaultTheme::ApplyApplicationColor(AttributeNode& target) -> void
{
    struct Entry
    {
        int64_t idx = 0;
        int64_t hex = 0;
    };

    struct ColorPalettes
    {
        std::array<Entry, 18> primary;
        std::array<Entry, 18> secondary;
        std::array<Entry, 18> tertiary;
        std::array<Entry, 18> error;
        std::array<Entry, 28> neutral;
        std::array<Entry, 18> neutralVariant;
    };

    // We should match Material 3 default color palettes but it is unclear where these values come from.
    // Even official M3 Theme Builder exports inconsistent values.
    // M3 Design Kit seems to be relatively consistent so we reference it here.
    auto constexpr palettes = ColorPalettes {
    // clang-format off
        .primary = {{
            {0, 0x000000FF},
            {5, 0x160041FF},
            {10, 0x22005DFF},
            {15, 0x2D1067FF},
            {20, 0x381E72FF},
            {25, 0x432B7EFF},
            {30, 0x4F378AFF},
            {35, 0x5B4397FF},
            {40, 0x6750A4FF},
            {50, 0x7F67BEFF},
            {60, 0x9A82DBFF},
            {70, 0xB69DF8FF},
            {80, 0xD0BCFFFF},
            {90, 0xEADDFFFF},
            {95, 0xF6EDFFFF},
            {98, 0xFEF7FFFF},
            {99, 0xFFFBFEFF},
            {100, 0xFFFFFFFF},
        }},
        .secondary = {{
            {0, 0x000000FF},
            {5, 0x130E20FF},
            {10, 0x1D192BFF},
            {15, 0x282336FF},
            {20, 0x332D41FF},
            {25, 0x3E384DFF},
            {30, 0x4A4458FF},
            {35, 0x564F65FF},
            {40, 0x625B71FF},
            {50, 0x7A7289FF},
            {60, 0x958DA5FF},
            {70, 0xB0A7C0FF},
            {80, 0xCCC2DCFF},
            {90, 0xE8DEF8FF},
            {95, 0xF6EDFFFF},
            {98, 0xFEF7FFFF},
            {99, 0xFFFBFEFF},
            {100, 0xFFFFFFFF},
        }},
        .tertiary = {{
            {0, 0x000000FF},
            {5, 0x240612FF},
            {10, 0x31111DFF},
            {15, 0x3D1B27FF},
            {20, 0x492532FF},
            {25, 0x56303DFF},
            {30, 0x633B48FF},
            {35, 0x704654FF},
            {40, 0x7D5260FF},
            {50, 0x986977FF},
            {60, 0xB58392FF},
            {70, 0xD29DACFF},
            {80, 0xEFB8C8FF},
            {90, 0xFFD8E4FF},
            {95, 0xFFECF1FF},
            {98, 0xFFF8F8FF},
            {99, 0xFFFBFAFF},
            {100, 0xFFFFFFFF},
        }},
        .error = {{
            {0, 0x000000FF},
            {5, 0x2D0002FF},
            {10, 0x410004FF},
            {15, 0x540006FF},
            {20, 0x601410FF},
            {25, 0x751617FF},
            {30, 0x8C1D18FF},
            {35, 0x952E2BFF},
            {40, 0xB3261EFF},
            {50, 0xDC362EFF},
            {60, 0xE46962FF},
            {70, 0xEC928EFF},
            {80, 0xF2B8B5FF},
            {90, 0xF9DEDCFF},
            {95, 0xFCEEEEFF},
            {98, 0xFFF8F7FF},
            {99, 0xFFFBF9FF},
            {100, 0xFFFFFFFF},
        }},
        .neutral = {{
            {0, 0x000000FF},
            {4, 0x0F0D13FF},
            {5, 0x111111FF},
            {6, 0x141218FF},
            {10, 0x1D1B20FF},
            {12, 0x211F26FF},
            {15, 0x262526FF},
            {17, 0x2B2930FF},
            {20, 0x322F35FF},
            {22, 0x36343BFF},
            {24, 0x3B383EFF},
            {25, 0x3C3B3BFF},
            {30, 0x48464CFF},
            {35, 0x545252FF},
            {40, 0x605D64FF},
            {50, 0x79767DFF},
            {60, 0x938F96FF},
            {70, 0xAEA9B1FF},
            {80, 0xCAC5CDFF},
            {87, 0xDED8E1FF},
            {90, 0xE6E0E9FF},
            {92, 0xECE6F0FF},
            {94, 0xF3EDF7FF},
            {95, 0xF5EFF7FF},
            {96, 0xF7F2FAFF},
            {98, 0xFEF7FFFF},
            {99, 0xFFFBFFFF},
            {100, 0xFFFFFFFF},
        }},
        .neutralVariant = {{
            {0, 0x000000FF},
            {5, 0x111112FF},
            {10, 0x1D1A22FF},
            {15, 0x262526FF},
            {20, 0x322F37FF},
            {25, 0x3C3B3CFF},
            {30, 0x49454FFF},
            {35, 0x545253FF},
            {40, 0x605D66FF},
            {50, 0x79747EFF},
            {60, 0x938F99FF},
            {70, 0xAEA9B4FF},
            {80, 0xCAC4D0FF},
            {90, 0xE7E0ECFF},
            {95, 0xF5EEFAFF},
            {98, 0xFDF7FFFF},
            {99, 0xFFFBFEFF},
            {100, 0xFFFFFFFF},
        }},
    // clang-format on
    };

    AttributeNode::SetValue<&ApplicationColor::Primary0>(target, GetRGBAFromHex(palettes.primary, 0));
    AttributeNode::SetValue<&ApplicationColor::Primary1>(target, GetRGBAFromHex(palettes.primary, 1));
    AttributeNode::SetValue<&ApplicationColor::Primary2>(target, GetRGBAFromHex(palettes.primary, 2));
    AttributeNode::SetValue<&ApplicationColor::Primary3>(target, GetRGBAFromHex(palettes.primary, 3));
    AttributeNode::SetValue<&ApplicationColor::Primary4>(target, GetRGBAFromHex(palettes.primary, 4));
    AttributeNode::SetValue<&ApplicationColor::Primary5>(target, GetRGBAFromHex(palettes.primary, 5));
    AttributeNode::SetValue<&ApplicationColor::Primary6>(target, GetRGBAFromHex(palettes.primary, 6));
    AttributeNode::SetValue<&ApplicationColor::Primary7>(target, GetRGBAFromHex(palettes.primary, 7));
    AttributeNode::SetValue<&ApplicationColor::Primary8>(target, GetRGBAFromHex(palettes.primary, 8));
    AttributeNode::SetValue<&ApplicationColor::Primary9>(target, GetRGBAFromHex(palettes.primary, 9));
    AttributeNode::SetValue<&ApplicationColor::Primary10>(target, GetRGBAFromHex(palettes.primary, 10));
    AttributeNode::SetValue<&ApplicationColor::Primary11>(target, GetRGBAFromHex(palettes.primary, 11));
    AttributeNode::SetValue<&ApplicationColor::Primary12>(target, GetRGBAFromHex(palettes.primary, 12));
    AttributeNode::SetValue<&ApplicationColor::Primary13>(target, GetRGBAFromHex(palettes.primary, 13));
    AttributeNode::SetValue<&ApplicationColor::Primary14>(target, GetRGBAFromHex(palettes.primary, 14));
    AttributeNode::SetValue<&ApplicationColor::Primary15>(target, GetRGBAFromHex(palettes.primary, 15));
    AttributeNode::SetValue<&ApplicationColor::Primary16>(target, GetRGBAFromHex(palettes.primary, 16));
    AttributeNode::SetValue<&ApplicationColor::Primary17>(target, GetRGBAFromHex(palettes.primary, 17));
    AttributeNode::SetValue<&ApplicationColor::Primary18>(target, GetRGBAFromHex(palettes.primary, 18));
    AttributeNode::SetValue<&ApplicationColor::Primary19>(target, GetRGBAFromHex(palettes.primary, 19));
    AttributeNode::SetValue<&ApplicationColor::Primary20>(target, GetRGBAFromHex(palettes.primary, 20));
    AttributeNode::SetValue<&ApplicationColor::Primary25>(target, GetRGBAFromHex(palettes.primary, 25));
    AttributeNode::SetValue<&ApplicationColor::Primary30>(target, GetRGBAFromHex(palettes.primary, 30));
    AttributeNode::SetValue<&ApplicationColor::Primary40>(target, GetRGBAFromHex(palettes.primary, 40));
    AttributeNode::SetValue<&ApplicationColor::Primary50>(target, GetRGBAFromHex(palettes.primary, 50));
    AttributeNode::SetValue<&ApplicationColor::Primary60>(target, GetRGBAFromHex(palettes.primary, 60));
    AttributeNode::SetValue<&ApplicationColor::Primary70>(target, GetRGBAFromHex(palettes.primary, 70));
    AttributeNode::SetValue<&ApplicationColor::Primary80>(target, GetRGBAFromHex(palettes.primary, 80));
    AttributeNode::SetValue<&ApplicationColor::Primary81>(target, GetRGBAFromHex(palettes.primary, 81));
    AttributeNode::SetValue<&ApplicationColor::Primary82>(target, GetRGBAFromHex(palettes.primary, 82));
    AttributeNode::SetValue<&ApplicationColor::Primary83>(target, GetRGBAFromHex(palettes.primary, 83));
    AttributeNode::SetValue<&ApplicationColor::Primary84>(target, GetRGBAFromHex(palettes.primary, 84));
    AttributeNode::SetValue<&ApplicationColor::Primary85>(target, GetRGBAFromHex(palettes.primary, 85));
    AttributeNode::SetValue<&ApplicationColor::Primary86>(target, GetRGBAFromHex(palettes.primary, 86));
    AttributeNode::SetValue<&ApplicationColor::Primary87>(target, GetRGBAFromHex(palettes.primary, 87));
    AttributeNode::SetValue<&ApplicationColor::Primary88>(target, GetRGBAFromHex(palettes.primary, 88));
    AttributeNode::SetValue<&ApplicationColor::Primary89>(target, GetRGBAFromHex(palettes.primary, 89));
    AttributeNode::SetValue<&ApplicationColor::Primary90>(target, GetRGBAFromHex(palettes.primary, 90));
    AttributeNode::SetValue<&ApplicationColor::Primary91>(target, GetRGBAFromHex(palettes.primary, 91));
    AttributeNode::SetValue<&ApplicationColor::Primary92>(target, GetRGBAFromHex(palettes.primary, 92));
    AttributeNode::SetValue<&ApplicationColor::Primary93>(target, GetRGBAFromHex(palettes.primary, 93));
    AttributeNode::SetValue<&ApplicationColor::Primary94>(target, GetRGBAFromHex(palettes.primary, 94));
    AttributeNode::SetValue<&ApplicationColor::Primary95>(target, GetRGBAFromHex(palettes.primary, 95));
    AttributeNode::SetValue<&ApplicationColor::Primary96>(target, GetRGBAFromHex(palettes.primary, 96));
    AttributeNode::SetValue<&ApplicationColor::Primary97>(target, GetRGBAFromHex(palettes.primary, 97));
    AttributeNode::SetValue<&ApplicationColor::Primary98>(target, GetRGBAFromHex(palettes.primary, 98));
    AttributeNode::SetValue<&ApplicationColor::Primary99>(target, GetRGBAFromHex(palettes.primary, 99));
    AttributeNode::SetValue<&ApplicationColor::Primary100>(target, GetRGBAFromHex(palettes.primary, 100));

    AttributeNode::SetValue<&ApplicationColor::Secondary0>(target, GetRGBAFromHex(palettes.secondary, 0));
    AttributeNode::SetValue<&ApplicationColor::Secondary1>(target, GetRGBAFromHex(palettes.secondary, 1));
    AttributeNode::SetValue<&ApplicationColor::Secondary2>(target, GetRGBAFromHex(palettes.secondary, 2));
    AttributeNode::SetValue<&ApplicationColor::Secondary3>(target, GetRGBAFromHex(palettes.secondary, 3));
    AttributeNode::SetValue<&ApplicationColor::Secondary4>(target, GetRGBAFromHex(palettes.secondary, 4));
    AttributeNode::SetValue<&ApplicationColor::Secondary5>(target, GetRGBAFromHex(palettes.secondary, 5));
    AttributeNode::SetValue<&ApplicationColor::Secondary6>(target, GetRGBAFromHex(palettes.secondary, 6));
    AttributeNode::SetValue<&ApplicationColor::Secondary7>(target, GetRGBAFromHex(palettes.secondary, 7));
    AttributeNode::SetValue<&ApplicationColor::Secondary8>(target, GetRGBAFromHex(palettes.secondary, 8));
    AttributeNode::SetValue<&ApplicationColor::Secondary9>(target, GetRGBAFromHex(palettes.secondary, 9));
    AttributeNode::SetValue<&ApplicationColor::Secondary10>(target, GetRGBAFromHex(palettes.secondary, 10));
    AttributeNode::SetValue<&ApplicationColor::Secondary11>(target, GetRGBAFromHex(palettes.secondary, 11));
    AttributeNode::SetValue<&ApplicationColor::Secondary12>(target, GetRGBAFromHex(palettes.secondary, 12));
    AttributeNode::SetValue<&ApplicationColor::Secondary13>(target, GetRGBAFromHex(palettes.secondary, 13));
    AttributeNode::SetValue<&ApplicationColor::Secondary14>(target, GetRGBAFromHex(palettes.secondary, 14));
    AttributeNode::SetValue<&ApplicationColor::Secondary15>(target, GetRGBAFromHex(palettes.secondary, 15));
    AttributeNode::SetValue<&ApplicationColor::Secondary16>(target, GetRGBAFromHex(palettes.secondary, 16));
    AttributeNode::SetValue<&ApplicationColor::Secondary17>(target, GetRGBAFromHex(palettes.secondary, 17));
    AttributeNode::SetValue<&ApplicationColor::Secondary18>(target, GetRGBAFromHex(palettes.secondary, 18));
    AttributeNode::SetValue<&ApplicationColor::Secondary19>(target, GetRGBAFromHex(palettes.secondary, 19));
    AttributeNode::SetValue<&ApplicationColor::Secondary20>(target, GetRGBAFromHex(palettes.secondary, 20));
    AttributeNode::SetValue<&ApplicationColor::Secondary25>(target, GetRGBAFromHex(palettes.secondary, 25));
    AttributeNode::SetValue<&ApplicationColor::Secondary30>(target, GetRGBAFromHex(palettes.secondary, 30));
    AttributeNode::SetValue<&ApplicationColor::Secondary40>(target, GetRGBAFromHex(palettes.secondary, 40));
    AttributeNode::SetValue<&ApplicationColor::Secondary50>(target, GetRGBAFromHex(palettes.secondary, 50));
    AttributeNode::SetValue<&ApplicationColor::Secondary60>(target, GetRGBAFromHex(palettes.secondary, 60));
    AttributeNode::SetValue<&ApplicationColor::Secondary70>(target, GetRGBAFromHex(palettes.secondary, 70));
    AttributeNode::SetValue<&ApplicationColor::Secondary80>(target, GetRGBAFromHex(palettes.secondary, 80));
    AttributeNode::SetValue<&ApplicationColor::Secondary81>(target, GetRGBAFromHex(palettes.secondary, 81));
    AttributeNode::SetValue<&ApplicationColor::Secondary82>(target, GetRGBAFromHex(palettes.secondary, 82));
    AttributeNode::SetValue<&ApplicationColor::Secondary83>(target, GetRGBAFromHex(palettes.secondary, 83));
    AttributeNode::SetValue<&ApplicationColor::Secondary84>(target, GetRGBAFromHex(palettes.secondary, 84));
    AttributeNode::SetValue<&ApplicationColor::Secondary85>(target, GetRGBAFromHex(palettes.secondary, 85));
    AttributeNode::SetValue<&ApplicationColor::Secondary86>(target, GetRGBAFromHex(palettes.secondary, 86));
    AttributeNode::SetValue<&ApplicationColor::Secondary87>(target, GetRGBAFromHex(palettes.secondary, 87));
    AttributeNode::SetValue<&ApplicationColor::Secondary88>(target, GetRGBAFromHex(palettes.secondary, 88));
    AttributeNode::SetValue<&ApplicationColor::Secondary89>(target, GetRGBAFromHex(palettes.secondary, 89));
    AttributeNode::SetValue<&ApplicationColor::Secondary90>(target, GetRGBAFromHex(palettes.secondary, 90));
    AttributeNode::SetValue<&ApplicationColor::Secondary91>(target, GetRGBAFromHex(palettes.secondary, 91));
    AttributeNode::SetValue<&ApplicationColor::Secondary92>(target, GetRGBAFromHex(palettes.secondary, 92));
    AttributeNode::SetValue<&ApplicationColor::Secondary93>(target, GetRGBAFromHex(palettes.secondary, 93));
    AttributeNode::SetValue<&ApplicationColor::Secondary94>(target, GetRGBAFromHex(palettes.secondary, 94));
    AttributeNode::SetValue<&ApplicationColor::Secondary95>(target, GetRGBAFromHex(palettes.secondary, 95));
    AttributeNode::SetValue<&ApplicationColor::Secondary96>(target, GetRGBAFromHex(palettes.secondary, 96));
    AttributeNode::SetValue<&ApplicationColor::Secondary97>(target, GetRGBAFromHex(palettes.secondary, 97));
    AttributeNode::SetValue<&ApplicationColor::Secondary98>(target, GetRGBAFromHex(palettes.secondary, 98));
    AttributeNode::SetValue<&ApplicationColor::Secondary99>(target, GetRGBAFromHex(palettes.secondary, 99));
    AttributeNode::SetValue<&ApplicationColor::Secondary100>(target, GetRGBAFromHex(palettes.secondary, 100));

    AttributeNode::SetValue<&ApplicationColor::Tertiary0>(target, GetRGBAFromHex(palettes.tertiary, 0));
    AttributeNode::SetValue<&ApplicationColor::Tertiary1>(target, GetRGBAFromHex(palettes.tertiary, 1));
    AttributeNode::SetValue<&ApplicationColor::Tertiary2>(target, GetRGBAFromHex(palettes.tertiary, 2));
    AttributeNode::SetValue<&ApplicationColor::Tertiary3>(target, GetRGBAFromHex(palettes.tertiary, 3));
    AttributeNode::SetValue<&ApplicationColor::Tertiary4>(target, GetRGBAFromHex(palettes.tertiary, 4));
    AttributeNode::SetValue<&ApplicationColor::Tertiary5>(target, GetRGBAFromHex(palettes.tertiary, 5));
    AttributeNode::SetValue<&ApplicationColor::Tertiary6>(target, GetRGBAFromHex(palettes.tertiary, 6));
    AttributeNode::SetValue<&ApplicationColor::Tertiary7>(target, GetRGBAFromHex(palettes.tertiary, 7));
    AttributeNode::SetValue<&ApplicationColor::Tertiary8>(target, GetRGBAFromHex(palettes.tertiary, 8));
    AttributeNode::SetValue<&ApplicationColor::Tertiary9>(target, GetRGBAFromHex(palettes.tertiary, 9));
    AttributeNode::SetValue<&ApplicationColor::Tertiary10>(target, GetRGBAFromHex(palettes.tertiary, 10));
    AttributeNode::SetValue<&ApplicationColor::Tertiary11>(target, GetRGBAFromHex(palettes.tertiary, 11));
    AttributeNode::SetValue<&ApplicationColor::Tertiary12>(target, GetRGBAFromHex(palettes.tertiary, 12));
    AttributeNode::SetValue<&ApplicationColor::Tertiary13>(target, GetRGBAFromHex(palettes.tertiary, 13));
    AttributeNode::SetValue<&ApplicationColor::Tertiary14>(target, GetRGBAFromHex(palettes.tertiary, 14));
    AttributeNode::SetValue<&ApplicationColor::Tertiary15>(target, GetRGBAFromHex(palettes.tertiary, 15));
    AttributeNode::SetValue<&ApplicationColor::Tertiary16>(target, GetRGBAFromHex(palettes.tertiary, 16));
    AttributeNode::SetValue<&ApplicationColor::Tertiary17>(target, GetRGBAFromHex(palettes.tertiary, 17));
    AttributeNode::SetValue<&ApplicationColor::Tertiary18>(target, GetRGBAFromHex(palettes.tertiary, 18));
    AttributeNode::SetValue<&ApplicationColor::Tertiary19>(target, GetRGBAFromHex(palettes.tertiary, 19));
    AttributeNode::SetValue<&ApplicationColor::Tertiary20>(target, GetRGBAFromHex(palettes.tertiary, 20));
    AttributeNode::SetValue<&ApplicationColor::Tertiary25>(target, GetRGBAFromHex(palettes.tertiary, 25));
    AttributeNode::SetValue<&ApplicationColor::Tertiary30>(target, GetRGBAFromHex(palettes.tertiary, 30));
    AttributeNode::SetValue<&ApplicationColor::Tertiary40>(target, GetRGBAFromHex(palettes.tertiary, 40));
    AttributeNode::SetValue<&ApplicationColor::Tertiary50>(target, GetRGBAFromHex(palettes.tertiary, 50));
    AttributeNode::SetValue<&ApplicationColor::Tertiary60>(target, GetRGBAFromHex(palettes.tertiary, 60));
    AttributeNode::SetValue<&ApplicationColor::Tertiary70>(target, GetRGBAFromHex(palettes.tertiary, 70));
    AttributeNode::SetValue<&ApplicationColor::Tertiary80>(target, GetRGBAFromHex(palettes.tertiary, 80));
    AttributeNode::SetValue<&ApplicationColor::Tertiary81>(target, GetRGBAFromHex(palettes.tertiary, 81));
    AttributeNode::SetValue<&ApplicationColor::Tertiary82>(target, GetRGBAFromHex(palettes.tertiary, 82));
    AttributeNode::SetValue<&ApplicationColor::Tertiary83>(target, GetRGBAFromHex(palettes.tertiary, 83));
    AttributeNode::SetValue<&ApplicationColor::Tertiary84>(target, GetRGBAFromHex(palettes.tertiary, 84));
    AttributeNode::SetValue<&ApplicationColor::Tertiary85>(target, GetRGBAFromHex(palettes.tertiary, 85));
    AttributeNode::SetValue<&ApplicationColor::Tertiary86>(target, GetRGBAFromHex(palettes.tertiary, 86));
    AttributeNode::SetValue<&ApplicationColor::Tertiary87>(target, GetRGBAFromHex(palettes.tertiary, 87));
    AttributeNode::SetValue<&ApplicationColor::Tertiary88>(target, GetRGBAFromHex(palettes.tertiary, 88));
    AttributeNode::SetValue<&ApplicationColor::Tertiary89>(target, GetRGBAFromHex(palettes.tertiary, 89));
    AttributeNode::SetValue<&ApplicationColor::Tertiary90>(target, GetRGBAFromHex(palettes.tertiary, 90));
    AttributeNode::SetValue<&ApplicationColor::Tertiary91>(target, GetRGBAFromHex(palettes.tertiary, 91));
    AttributeNode::SetValue<&ApplicationColor::Tertiary92>(target, GetRGBAFromHex(palettes.tertiary, 92));
    AttributeNode::SetValue<&ApplicationColor::Tertiary93>(target, GetRGBAFromHex(palettes.tertiary, 93));
    AttributeNode::SetValue<&ApplicationColor::Tertiary94>(target, GetRGBAFromHex(palettes.tertiary, 94));
    AttributeNode::SetValue<&ApplicationColor::Tertiary95>(target, GetRGBAFromHex(palettes.tertiary, 95));
    AttributeNode::SetValue<&ApplicationColor::Tertiary96>(target, GetRGBAFromHex(palettes.tertiary, 96));
    AttributeNode::SetValue<&ApplicationColor::Tertiary97>(target, GetRGBAFromHex(palettes.tertiary, 97));
    AttributeNode::SetValue<&ApplicationColor::Tertiary98>(target, GetRGBAFromHex(palettes.tertiary, 98));
    AttributeNode::SetValue<&ApplicationColor::Tertiary99>(target, GetRGBAFromHex(palettes.tertiary, 99));
    AttributeNode::SetValue<&ApplicationColor::Tertiary100>(target, GetRGBAFromHex(palettes.tertiary, 100));

    AttributeNode::SetValue<&ApplicationColor::Error0>(target, GetRGBAFromHex(palettes.error, 0));
    AttributeNode::SetValue<&ApplicationColor::Error5>(target, GetRGBAFromHex(palettes.error, 5));
    AttributeNode::SetValue<&ApplicationColor::Error10>(target, GetRGBAFromHex(palettes.error, 10));
    AttributeNode::SetValue<&ApplicationColor::Error15>(target, GetRGBAFromHex(palettes.error, 15));
    AttributeNode::SetValue<&ApplicationColor::Error20>(target, GetRGBAFromHex(palettes.error, 20));
    AttributeNode::SetValue<&ApplicationColor::Error25>(target, GetRGBAFromHex(palettes.error, 25));
    AttributeNode::SetValue<&ApplicationColor::Error30>(target, GetRGBAFromHex(palettes.error, 30));
    AttributeNode::SetValue<&ApplicationColor::Error35>(target, GetRGBAFromHex(palettes.error, 35));
    AttributeNode::SetValue<&ApplicationColor::Error40>(target, GetRGBAFromHex(palettes.error, 40));
    AttributeNode::SetValue<&ApplicationColor::Error50>(target, GetRGBAFromHex(palettes.error, 50));
    AttributeNode::SetValue<&ApplicationColor::Error60>(target, GetRGBAFromHex(palettes.error, 60));
    AttributeNode::SetValue<&ApplicationColor::Error70>(target, GetRGBAFromHex(palettes.error, 70));
    AttributeNode::SetValue<&ApplicationColor::Error80>(target, GetRGBAFromHex(palettes.error, 80));
    AttributeNode::SetValue<&ApplicationColor::Error90>(target, GetRGBAFromHex(palettes.error, 90));
    AttributeNode::SetValue<&ApplicationColor::Error95>(target, GetRGBAFromHex(palettes.error, 95));
    AttributeNode::SetValue<&ApplicationColor::Error98>(target, GetRGBAFromHex(palettes.error, 98));
    AttributeNode::SetValue<&ApplicationColor::Error99>(target, GetRGBAFromHex(palettes.error, 99));
    AttributeNode::SetValue<&ApplicationColor::Error100>(target, GetRGBAFromHex(palettes.error, 100));

    AttributeNode::SetValue<&ApplicationColor::Neutral0>(target, GetRGBAFromHex(palettes.neutral, 0));
    AttributeNode::SetValue<&ApplicationColor::Neutral1>(target, GetRGBAFromHex(palettes.neutral, 1));
    AttributeNode::SetValue<&ApplicationColor::Neutral2>(target, GetRGBAFromHex(palettes.neutral, 2));
    AttributeNode::SetValue<&ApplicationColor::Neutral3>(target, GetRGBAFromHex(palettes.neutral, 3));
    AttributeNode::SetValue<&ApplicationColor::Neutral4>(target, GetRGBAFromHex(palettes.neutral, 4));
    AttributeNode::SetValue<&ApplicationColor::Neutral5>(target, GetRGBAFromHex(palettes.neutral, 5));
    AttributeNode::SetValue<&ApplicationColor::Neutral6>(target, GetRGBAFromHex(palettes.neutral, 6));
    AttributeNode::SetValue<&ApplicationColor::Neutral7>(target, GetRGBAFromHex(palettes.neutral, 7));
    AttributeNode::SetValue<&ApplicationColor::Neutral8>(target, GetRGBAFromHex(palettes.neutral, 8));
    AttributeNode::SetValue<&ApplicationColor::Neutral9>(target, GetRGBAFromHex(palettes.neutral, 9));
    AttributeNode::SetValue<&ApplicationColor::Neutral10>(target, GetRGBAFromHex(palettes.neutral, 10));
    AttributeNode::SetValue<&ApplicationColor::Neutral11>(target, GetRGBAFromHex(palettes.neutral, 11));
    AttributeNode::SetValue<&ApplicationColor::Neutral12>(target, GetRGBAFromHex(palettes.neutral, 12));
    AttributeNode::SetValue<&ApplicationColor::Neutral13>(target, GetRGBAFromHex(palettes.neutral, 13));
    AttributeNode::SetValue<&ApplicationColor::Neutral14>(target, GetRGBAFromHex(palettes.neutral, 14));
    AttributeNode::SetValue<&ApplicationColor::Neutral15>(target, GetRGBAFromHex(palettes.neutral, 15));
    AttributeNode::SetValue<&ApplicationColor::Neutral16>(target, GetRGBAFromHex(palettes.neutral, 16));
    AttributeNode::SetValue<&ApplicationColor::Neutral17>(target, GetRGBAFromHex(palettes.neutral, 17));
    AttributeNode::SetValue<&ApplicationColor::Neutral18>(target, GetRGBAFromHex(palettes.neutral, 18));
    AttributeNode::SetValue<&ApplicationColor::Neutral19>(target, GetRGBAFromHex(palettes.neutral, 19));
    AttributeNode::SetValue<&ApplicationColor::Neutral20>(target, GetRGBAFromHex(palettes.neutral, 20));
    AttributeNode::SetValue<&ApplicationColor::Neutral21>(target, GetRGBAFromHex(palettes.neutral, 21));
    AttributeNode::SetValue<&ApplicationColor::Neutral22>(target, GetRGBAFromHex(palettes.neutral, 22));
    AttributeNode::SetValue<&ApplicationColor::Neutral23>(target, GetRGBAFromHex(palettes.neutral, 23));
    AttributeNode::SetValue<&ApplicationColor::Neutral24>(target, GetRGBAFromHex(palettes.neutral, 24));
    AttributeNode::SetValue<&ApplicationColor::Neutral25>(target, GetRGBAFromHex(palettes.neutral, 25));
    AttributeNode::SetValue<&ApplicationColor::Neutral26>(target, GetRGBAFromHex(palettes.neutral, 26));
    AttributeNode::SetValue<&ApplicationColor::Neutral27>(target, GetRGBAFromHex(palettes.neutral, 27));
    AttributeNode::SetValue<&ApplicationColor::Neutral28>(target, GetRGBAFromHex(palettes.neutral, 28));
    AttributeNode::SetValue<&ApplicationColor::Neutral29>(target, GetRGBAFromHex(palettes.neutral, 29));
    AttributeNode::SetValue<&ApplicationColor::Neutral30>(target, GetRGBAFromHex(palettes.neutral, 30));
    AttributeNode::SetValue<&ApplicationColor::Neutral40>(target, GetRGBAFromHex(palettes.neutral, 40));
    AttributeNode::SetValue<&ApplicationColor::Neutral50>(target, GetRGBAFromHex(palettes.neutral, 50));
    AttributeNode::SetValue<&ApplicationColor::Neutral60>(target, GetRGBAFromHex(palettes.neutral, 60));
    AttributeNode::SetValue<&ApplicationColor::Neutral70>(target, GetRGBAFromHex(palettes.neutral, 70));
    AttributeNode::SetValue<&ApplicationColor::Neutral71>(target, GetRGBAFromHex(palettes.neutral, 71));
    AttributeNode::SetValue<&ApplicationColor::Neutral72>(target, GetRGBAFromHex(palettes.neutral, 72));
    AttributeNode::SetValue<&ApplicationColor::Neutral73>(target, GetRGBAFromHex(palettes.neutral, 73));
    AttributeNode::SetValue<&ApplicationColor::Neutral74>(target, GetRGBAFromHex(palettes.neutral, 74));
    AttributeNode::SetValue<&ApplicationColor::Neutral75>(target, GetRGBAFromHex(palettes.neutral, 75));
    AttributeNode::SetValue<&ApplicationColor::Neutral76>(target, GetRGBAFromHex(palettes.neutral, 76));
    AttributeNode::SetValue<&ApplicationColor::Neutral77>(target, GetRGBAFromHex(palettes.neutral, 77));
    AttributeNode::SetValue<&ApplicationColor::Neutral78>(target, GetRGBAFromHex(palettes.neutral, 78));
    AttributeNode::SetValue<&ApplicationColor::Neutral79>(target, GetRGBAFromHex(palettes.neutral, 79));
    AttributeNode::SetValue<&ApplicationColor::Neutral80>(target, GetRGBAFromHex(palettes.neutral, 80));
    AttributeNode::SetValue<&ApplicationColor::Neutral81>(target, GetRGBAFromHex(palettes.neutral, 81));
    AttributeNode::SetValue<&ApplicationColor::Neutral82>(target, GetRGBAFromHex(palettes.neutral, 82));
    AttributeNode::SetValue<&ApplicationColor::Neutral83>(target, GetRGBAFromHex(palettes.neutral, 83));
    AttributeNode::SetValue<&ApplicationColor::Neutral84>(target, GetRGBAFromHex(palettes.neutral, 84));
    AttributeNode::SetValue<&ApplicationColor::Neutral85>(target, GetRGBAFromHex(palettes.neutral, 85));
    AttributeNode::SetValue<&ApplicationColor::Neutral86>(target, GetRGBAFromHex(palettes.neutral, 86));
    AttributeNode::SetValue<&ApplicationColor::Neutral87>(target, GetRGBAFromHex(palettes.neutral, 87));
    AttributeNode::SetValue<&ApplicationColor::Neutral88>(target, GetRGBAFromHex(palettes.neutral, 88));
    AttributeNode::SetValue<&ApplicationColor::Neutral89>(target, GetRGBAFromHex(palettes.neutral, 89));
    AttributeNode::SetValue<&ApplicationColor::Neutral90>(target, GetRGBAFromHex(palettes.neutral, 90));
    AttributeNode::SetValue<&ApplicationColor::Neutral91>(target, GetRGBAFromHex(palettes.neutral, 91));
    AttributeNode::SetValue<&ApplicationColor::Neutral92>(target, GetRGBAFromHex(palettes.neutral, 92));
    AttributeNode::SetValue<&ApplicationColor::Neutral93>(target, GetRGBAFromHex(palettes.neutral, 93));
    AttributeNode::SetValue<&ApplicationColor::Neutral94>(target, GetRGBAFromHex(palettes.neutral, 94));
    AttributeNode::SetValue<&ApplicationColor::Neutral95>(target, GetRGBAFromHex(palettes.neutral, 95));
    AttributeNode::SetValue<&ApplicationColor::Neutral96>(target, GetRGBAFromHex(palettes.neutral, 96));
    AttributeNode::SetValue<&ApplicationColor::Neutral97>(target, GetRGBAFromHex(palettes.neutral, 97));
    AttributeNode::SetValue<&ApplicationColor::Neutral98>(target, GetRGBAFromHex(palettes.neutral, 98));
    AttributeNode::SetValue<&ApplicationColor::Neutral99>(target, GetRGBAFromHex(palettes.neutral, 99));
    AttributeNode::SetValue<&ApplicationColor::Neutral100>(target, GetRGBAFromHex(palettes.neutral, 100));

    AttributeNode::SetValue<&ApplicationColor::NeutralVariant0>(target, GetRGBAFromHex(palettes.neutralVariant, 0));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant1>(target, GetRGBAFromHex(palettes.neutralVariant, 1));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant2>(target, GetRGBAFromHex(palettes.neutralVariant, 2));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant3>(target, GetRGBAFromHex(palettes.neutralVariant, 3));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant4>(target, GetRGBAFromHex(palettes.neutralVariant, 4));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant5>(target, GetRGBAFromHex(palettes.neutralVariant, 5));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant6>(target, GetRGBAFromHex(palettes.neutralVariant, 6));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant7>(target, GetRGBAFromHex(palettes.neutralVariant, 7));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant8>(target, GetRGBAFromHex(palettes.neutralVariant, 8));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant9>(target, GetRGBAFromHex(palettes.neutralVariant, 9));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant10>(target, GetRGBAFromHex(palettes.neutralVariant, 10));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant11>(target, GetRGBAFromHex(palettes.neutralVariant, 11));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant12>(target, GetRGBAFromHex(palettes.neutralVariant, 12));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant13>(target, GetRGBAFromHex(palettes.neutralVariant, 13));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant14>(target, GetRGBAFromHex(palettes.neutralVariant, 14));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant15>(target, GetRGBAFromHex(palettes.neutralVariant, 15));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant16>(target, GetRGBAFromHex(palettes.neutralVariant, 16));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant17>(target, GetRGBAFromHex(palettes.neutralVariant, 17));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant18>(target, GetRGBAFromHex(palettes.neutralVariant, 18));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant19>(target, GetRGBAFromHex(palettes.neutralVariant, 19));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant20>(target, GetRGBAFromHex(palettes.neutralVariant, 20));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant21>(target, GetRGBAFromHex(palettes.neutralVariant, 21));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant22>(target, GetRGBAFromHex(palettes.neutralVariant, 22));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant23>(target, GetRGBAFromHex(palettes.neutralVariant, 23));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant24>(target, GetRGBAFromHex(palettes.neutralVariant, 24));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant25>(target, GetRGBAFromHex(palettes.neutralVariant, 25));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant26>(target, GetRGBAFromHex(palettes.neutralVariant, 26));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant27>(target, GetRGBAFromHex(palettes.neutralVariant, 27));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant28>(target, GetRGBAFromHex(palettes.neutralVariant, 28));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant29>(target, GetRGBAFromHex(palettes.neutralVariant, 29));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant30>(target, GetRGBAFromHex(palettes.neutralVariant, 30));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant40>(target, GetRGBAFromHex(palettes.neutralVariant, 40));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant50>(target, GetRGBAFromHex(palettes.neutralVariant, 50));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant60>(target, GetRGBAFromHex(palettes.neutralVariant, 60));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant70>(target, GetRGBAFromHex(palettes.neutralVariant, 70));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant71>(target, GetRGBAFromHex(palettes.neutralVariant, 71));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant72>(target, GetRGBAFromHex(palettes.neutralVariant, 72));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant73>(target, GetRGBAFromHex(palettes.neutralVariant, 73));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant74>(target, GetRGBAFromHex(palettes.neutralVariant, 74));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant75>(target, GetRGBAFromHex(palettes.neutralVariant, 75));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant76>(target, GetRGBAFromHex(palettes.neutralVariant, 76));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant77>(target, GetRGBAFromHex(palettes.neutralVariant, 77));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant78>(target, GetRGBAFromHex(palettes.neutralVariant, 78));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant79>(target, GetRGBAFromHex(palettes.neutralVariant, 79));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant80>(target, GetRGBAFromHex(palettes.neutralVariant, 80));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant81>(target, GetRGBAFromHex(palettes.neutralVariant, 81));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant82>(target, GetRGBAFromHex(palettes.neutralVariant, 82));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant83>(target, GetRGBAFromHex(palettes.neutralVariant, 83));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant84>(target, GetRGBAFromHex(palettes.neutralVariant, 84));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant85>(target, GetRGBAFromHex(palettes.neutralVariant, 85));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant86>(target, GetRGBAFromHex(palettes.neutralVariant, 86));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant87>(target, GetRGBAFromHex(palettes.neutralVariant, 87));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant88>(target, GetRGBAFromHex(palettes.neutralVariant, 88));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant89>(target, GetRGBAFromHex(palettes.neutralVariant, 89));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant90>(target, GetRGBAFromHex(palettes.neutralVariant, 90));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant91>(target, GetRGBAFromHex(palettes.neutralVariant, 91));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant92>(target, GetRGBAFromHex(palettes.neutralVariant, 92));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant93>(target, GetRGBAFromHex(palettes.neutralVariant, 93));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant94>(target, GetRGBAFromHex(palettes.neutralVariant, 94));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant95>(target, GetRGBAFromHex(palettes.neutralVariant, 95));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant96>(target, GetRGBAFromHex(palettes.neutralVariant, 96));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant97>(target, GetRGBAFromHex(palettes.neutralVariant, 97));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant98>(target, GetRGBAFromHex(palettes.neutralVariant, 98));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant99>(target, GetRGBAFromHex(palettes.neutralVariant, 99));
    AttributeNode::SetValue<&ApplicationColor::NeutralVariant100>(target, GetRGBAFromHex(palettes.neutralVariant, 100));

    AttributeNode::SetValue<&ApplicationColor::Black>(target, RGBAColor::MakeFromHex(0x0000FF));
    AttributeNode::SetValue<&ApplicationColor::White>(target, RGBAColor::MakeFromHex(0xFFFFFF));
}

auto DefaultTheme::ApplyApplicationStyle(AttributeNode& target) -> void
{
    if (_brightness == ThemeBrightness::Light)
    {
        AttributeNode::SetReference<&ApplicationStyle::ColorPrimary>(target, ApplicationColor::Primary40);
        AttributeNode::SetReference<&ApplicationStyle::ColorPrimaryContainer>(target, ApplicationColor::Primary90);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnPrimary>(target, ApplicationColor::Primary100);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnPrimaryContainer>(target, ApplicationColor::Primary30);
        AttributeNode::SetReference<&ApplicationStyle::ColorSecondary>(target, ApplicationColor::Secondary40);
        AttributeNode::SetReference<&ApplicationStyle::ColorSecondaryContainer>(target, ApplicationColor::Secondary90);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnSecondary>(target, ApplicationColor::Secondary100);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnSecondaryContainer>(target, ApplicationColor::Secondary30);
        AttributeNode::SetReference<&ApplicationStyle::ColorTertiary>(target, ApplicationColor::Tertiary40);
        AttributeNode::SetReference<&ApplicationStyle::ColorTertiaryContainer>(target, ApplicationColor::Tertiary90);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnTertiary>(target, ApplicationColor::Tertiary100);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnTertiaryContainer>(target, ApplicationColor::Tertiary30);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurface>(target, ApplicationColor::Neutral98);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceVariant>(target, ApplicationColor::NeutralVariant90);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceDim>(target, ApplicationColor::Neutral87);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceBright>(target, ApplicationColor::Neutral98);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceContainerLowest>(target, ApplicationColor::Neutral100);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceContainerLow>(target, ApplicationColor::Neutral96);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceContainer>(target, ApplicationColor::Neutral94);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceContainerHigh>(target, ApplicationColor::Neutral92);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceContainerHighest>(target, ApplicationColor::Neutral90);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnSurface>(target, ApplicationColor::Neutral10);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnSurfaceVariant>(target, ApplicationColor::Neutral30);
        AttributeNode::SetReference<&ApplicationStyle::ColorOutline>(target, ApplicationColor::NeutralVariant50);
        AttributeNode::SetReference<&ApplicationStyle::ColorOutlineVariant>(target, ApplicationColor::NeutralVariant90);
        AttributeNode::SetReference<&ApplicationStyle::ColorError>(target, ApplicationColor::Error40);
        AttributeNode::SetReference<&ApplicationStyle::ColorErrorContainer>(target, ApplicationColor::Error90);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnError>(target, ApplicationColor::Error100);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnErrorContainer>(target, ApplicationColor::Error30);
    }
    else
    {
        AttributeNode::SetReference<&ApplicationStyle::ColorPrimary>(target, ApplicationColor::Primary80);
        AttributeNode::SetReference<&ApplicationStyle::ColorPrimaryContainer>(target, ApplicationColor::Primary30);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnPrimary>(target, ApplicationColor::Primary20);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnPrimaryContainer>(target, ApplicationColor::Primary90);
        AttributeNode::SetReference<&ApplicationStyle::ColorSecondary>(target, ApplicationColor::Secondary80);
        AttributeNode::SetReference<&ApplicationStyle::ColorSecondaryContainer>(target, ApplicationColor::Secondary30);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnSecondary>(target, ApplicationColor::Secondary20);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnSecondaryContainer>(target, ApplicationColor::Secondary90);
        AttributeNode::SetReference<&ApplicationStyle::ColorTertiary>(target, ApplicationColor::Tertiary90);
        AttributeNode::SetReference<&ApplicationStyle::ColorTertiaryContainer>(target, ApplicationColor::Tertiary30);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnTertiary>(target, ApplicationColor::Tertiary20);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnTertiaryContainer>(target, ApplicationColor::Tertiary90);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurface>(target, ApplicationColor::Neutral6);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceVariant>(target, ApplicationColor::NeutralVariant30);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceDim>(target, ApplicationColor::Neutral6);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceBright>(target, ApplicationColor::Neutral24);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceContainerLowest>(target, ApplicationColor::Neutral4);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceContainerLow>(target, ApplicationColor::Neutral10);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceContainer>(target, ApplicationColor::Neutral12);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceContainerHigh>(target, ApplicationColor::Neutral17);
        AttributeNode::SetReference<&ApplicationStyle::ColorSurfaceContainerHighest>(target, ApplicationColor::Neutral22);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnSurface>(target, ApplicationColor::Neutral90);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnSurfaceVariant>(target, ApplicationColor::NeutralVariant80);
        AttributeNode::SetReference<&ApplicationStyle::ColorOutline>(target, ApplicationColor::NeutralVariant60);
        AttributeNode::SetReference<&ApplicationStyle::ColorOutlineVariant>(target, ApplicationColor::NeutralVariant30);
        AttributeNode::SetReference<&ApplicationStyle::ColorError>(target, ApplicationColor::Error80);
        AttributeNode::SetReference<&ApplicationStyle::ColorErrorContainer>(target, ApplicationColor::Error30);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnError>(target, ApplicationColor::Error20);
        AttributeNode::SetReference<&ApplicationStyle::ColorOnErrorContainer>(target, ApplicationColor::Error90);
    }

    AttributeNode::SetValue<&ApplicationStyle::FontSizeBodySmall>(target, 11);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeBodyMedium>(target, 14);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeBodyLarge>(target, 16);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeLabelSmall>(target, 11);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeLabelMedium>(target, 12);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeLabelLarge>(target, 14);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeTitleSmall>(target, 14);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeTitleMedium>(target, 16);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeTitleLarge>(target, 22);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeHeadlineSmall>(target, 24);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeHeadlineMedium>(target, 28);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeHeadlineLarge>(target, 32);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeDisplaySmall>(target, 36);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeDisplayMedium>(target, 45);
    AttributeNode::SetValue<&ApplicationStyle::FontSizeDisplayLarge>(target, 57);

    AttributeNode::SetValue<&ApplicationStyle::FontWeightBodySmall>(target, 400);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightBodyMedium>(target, 400);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightBodyLarge>(target, 400);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightLabelSmall>(target, 500);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightLabelMedium>(target, 500);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightLabelLarge>(target, 500);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightTitleSmall>(target, 500);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightTitleMedium>(target, 500);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightTitleLarge>(target, 400);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightHeadlineSmall>(target, 400);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightHeadlineMedium>(target, 400);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightHeadlineLarge>(target, 400);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightDisplaySmall>(target, 400);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightDisplayMedium>(target, 400);
    AttributeNode::SetValue<&ApplicationStyle::FontWeightDisplayLarge>(target, 400);
}
}
