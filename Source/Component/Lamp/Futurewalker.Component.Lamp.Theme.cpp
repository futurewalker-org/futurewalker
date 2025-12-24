// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.Theme.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"
#include "Futurewalker.Component.Lamp.Color.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp" 

#include "Futurewalker.Color.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
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

Theme::Theme(ThemeBrightness brightness)
  : _brightness(brightness)
{
}

auto Theme::Apply(AttributeNode& target) -> void
{
    ApplyApplicationColor(target);
    ApplyApplicationStyle(target);
}

auto Theme::ApplyApplicationColor(AttributeNode& target) -> void
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

    AttributeNode::SetValue<&Color::Primary0>(target, GetRGBAFromHex(palettes.primary, 0));
    AttributeNode::SetValue<&Color::Primary1>(target, GetRGBAFromHex(palettes.primary, 1));
    AttributeNode::SetValue<&Color::Primary2>(target, GetRGBAFromHex(palettes.primary, 2));
    AttributeNode::SetValue<&Color::Primary3>(target, GetRGBAFromHex(palettes.primary, 3));
    AttributeNode::SetValue<&Color::Primary4>(target, GetRGBAFromHex(palettes.primary, 4));
    AttributeNode::SetValue<&Color::Primary5>(target, GetRGBAFromHex(palettes.primary, 5));
    AttributeNode::SetValue<&Color::Primary6>(target, GetRGBAFromHex(palettes.primary, 6));
    AttributeNode::SetValue<&Color::Primary7>(target, GetRGBAFromHex(palettes.primary, 7));
    AttributeNode::SetValue<&Color::Primary8>(target, GetRGBAFromHex(palettes.primary, 8));
    AttributeNode::SetValue<&Color::Primary9>(target, GetRGBAFromHex(palettes.primary, 9));
    AttributeNode::SetValue<&Color::Primary10>(target, GetRGBAFromHex(palettes.primary, 10));
    AttributeNode::SetValue<&Color::Primary11>(target, GetRGBAFromHex(palettes.primary, 11));
    AttributeNode::SetValue<&Color::Primary12>(target, GetRGBAFromHex(palettes.primary, 12));
    AttributeNode::SetValue<&Color::Primary13>(target, GetRGBAFromHex(palettes.primary, 13));
    AttributeNode::SetValue<&Color::Primary14>(target, GetRGBAFromHex(palettes.primary, 14));
    AttributeNode::SetValue<&Color::Primary15>(target, GetRGBAFromHex(palettes.primary, 15));
    AttributeNode::SetValue<&Color::Primary16>(target, GetRGBAFromHex(palettes.primary, 16));
    AttributeNode::SetValue<&Color::Primary17>(target, GetRGBAFromHex(palettes.primary, 17));
    AttributeNode::SetValue<&Color::Primary18>(target, GetRGBAFromHex(palettes.primary, 18));
    AttributeNode::SetValue<&Color::Primary19>(target, GetRGBAFromHex(palettes.primary, 19));
    AttributeNode::SetValue<&Color::Primary20>(target, GetRGBAFromHex(palettes.primary, 20));
    AttributeNode::SetValue<&Color::Primary25>(target, GetRGBAFromHex(palettes.primary, 25));
    AttributeNode::SetValue<&Color::Primary30>(target, GetRGBAFromHex(palettes.primary, 30));
    AttributeNode::SetValue<&Color::Primary40>(target, GetRGBAFromHex(palettes.primary, 40));
    AttributeNode::SetValue<&Color::Primary50>(target, GetRGBAFromHex(palettes.primary, 50));
    AttributeNode::SetValue<&Color::Primary60>(target, GetRGBAFromHex(palettes.primary, 60));
    AttributeNode::SetValue<&Color::Primary70>(target, GetRGBAFromHex(palettes.primary, 70));
    AttributeNode::SetValue<&Color::Primary80>(target, GetRGBAFromHex(palettes.primary, 80));
    AttributeNode::SetValue<&Color::Primary81>(target, GetRGBAFromHex(palettes.primary, 81));
    AttributeNode::SetValue<&Color::Primary82>(target, GetRGBAFromHex(palettes.primary, 82));
    AttributeNode::SetValue<&Color::Primary83>(target, GetRGBAFromHex(palettes.primary, 83));
    AttributeNode::SetValue<&Color::Primary84>(target, GetRGBAFromHex(palettes.primary, 84));
    AttributeNode::SetValue<&Color::Primary85>(target, GetRGBAFromHex(palettes.primary, 85));
    AttributeNode::SetValue<&Color::Primary86>(target, GetRGBAFromHex(palettes.primary, 86));
    AttributeNode::SetValue<&Color::Primary87>(target, GetRGBAFromHex(palettes.primary, 87));
    AttributeNode::SetValue<&Color::Primary88>(target, GetRGBAFromHex(palettes.primary, 88));
    AttributeNode::SetValue<&Color::Primary89>(target, GetRGBAFromHex(palettes.primary, 89));
    AttributeNode::SetValue<&Color::Primary90>(target, GetRGBAFromHex(palettes.primary, 90));
    AttributeNode::SetValue<&Color::Primary91>(target, GetRGBAFromHex(palettes.primary, 91));
    AttributeNode::SetValue<&Color::Primary92>(target, GetRGBAFromHex(palettes.primary, 92));
    AttributeNode::SetValue<&Color::Primary93>(target, GetRGBAFromHex(palettes.primary, 93));
    AttributeNode::SetValue<&Color::Primary94>(target, GetRGBAFromHex(palettes.primary, 94));
    AttributeNode::SetValue<&Color::Primary95>(target, GetRGBAFromHex(palettes.primary, 95));
    AttributeNode::SetValue<&Color::Primary96>(target, GetRGBAFromHex(palettes.primary, 96));
    AttributeNode::SetValue<&Color::Primary97>(target, GetRGBAFromHex(palettes.primary, 97));
    AttributeNode::SetValue<&Color::Primary98>(target, GetRGBAFromHex(palettes.primary, 98));
    AttributeNode::SetValue<&Color::Primary99>(target, GetRGBAFromHex(palettes.primary, 99));
    AttributeNode::SetValue<&Color::Primary100>(target, GetRGBAFromHex(palettes.primary, 100));

    AttributeNode::SetValue<&Color::Secondary0>(target, GetRGBAFromHex(palettes.secondary, 0));
    AttributeNode::SetValue<&Color::Secondary1>(target, GetRGBAFromHex(palettes.secondary, 1));
    AttributeNode::SetValue<&Color::Secondary2>(target, GetRGBAFromHex(palettes.secondary, 2));
    AttributeNode::SetValue<&Color::Secondary3>(target, GetRGBAFromHex(palettes.secondary, 3));
    AttributeNode::SetValue<&Color::Secondary4>(target, GetRGBAFromHex(palettes.secondary, 4));
    AttributeNode::SetValue<&Color::Secondary5>(target, GetRGBAFromHex(palettes.secondary, 5));
    AttributeNode::SetValue<&Color::Secondary6>(target, GetRGBAFromHex(palettes.secondary, 6));
    AttributeNode::SetValue<&Color::Secondary7>(target, GetRGBAFromHex(palettes.secondary, 7));
    AttributeNode::SetValue<&Color::Secondary8>(target, GetRGBAFromHex(palettes.secondary, 8));
    AttributeNode::SetValue<&Color::Secondary9>(target, GetRGBAFromHex(palettes.secondary, 9));
    AttributeNode::SetValue<&Color::Secondary10>(target, GetRGBAFromHex(palettes.secondary, 10));
    AttributeNode::SetValue<&Color::Secondary11>(target, GetRGBAFromHex(palettes.secondary, 11));
    AttributeNode::SetValue<&Color::Secondary12>(target, GetRGBAFromHex(palettes.secondary, 12));
    AttributeNode::SetValue<&Color::Secondary13>(target, GetRGBAFromHex(palettes.secondary, 13));
    AttributeNode::SetValue<&Color::Secondary14>(target, GetRGBAFromHex(palettes.secondary, 14));
    AttributeNode::SetValue<&Color::Secondary15>(target, GetRGBAFromHex(palettes.secondary, 15));
    AttributeNode::SetValue<&Color::Secondary16>(target, GetRGBAFromHex(palettes.secondary, 16));
    AttributeNode::SetValue<&Color::Secondary17>(target, GetRGBAFromHex(palettes.secondary, 17));
    AttributeNode::SetValue<&Color::Secondary18>(target, GetRGBAFromHex(palettes.secondary, 18));
    AttributeNode::SetValue<&Color::Secondary19>(target, GetRGBAFromHex(palettes.secondary, 19));
    AttributeNode::SetValue<&Color::Secondary20>(target, GetRGBAFromHex(palettes.secondary, 20));
    AttributeNode::SetValue<&Color::Secondary25>(target, GetRGBAFromHex(palettes.secondary, 25));
    AttributeNode::SetValue<&Color::Secondary30>(target, GetRGBAFromHex(palettes.secondary, 30));
    AttributeNode::SetValue<&Color::Secondary40>(target, GetRGBAFromHex(palettes.secondary, 40));
    AttributeNode::SetValue<&Color::Secondary50>(target, GetRGBAFromHex(palettes.secondary, 50));
    AttributeNode::SetValue<&Color::Secondary60>(target, GetRGBAFromHex(palettes.secondary, 60));
    AttributeNode::SetValue<&Color::Secondary70>(target, GetRGBAFromHex(palettes.secondary, 70));
    AttributeNode::SetValue<&Color::Secondary80>(target, GetRGBAFromHex(palettes.secondary, 80));
    AttributeNode::SetValue<&Color::Secondary81>(target, GetRGBAFromHex(palettes.secondary, 81));
    AttributeNode::SetValue<&Color::Secondary82>(target, GetRGBAFromHex(palettes.secondary, 82));
    AttributeNode::SetValue<&Color::Secondary83>(target, GetRGBAFromHex(palettes.secondary, 83));
    AttributeNode::SetValue<&Color::Secondary84>(target, GetRGBAFromHex(palettes.secondary, 84));
    AttributeNode::SetValue<&Color::Secondary85>(target, GetRGBAFromHex(palettes.secondary, 85));
    AttributeNode::SetValue<&Color::Secondary86>(target, GetRGBAFromHex(palettes.secondary, 86));
    AttributeNode::SetValue<&Color::Secondary87>(target, GetRGBAFromHex(palettes.secondary, 87));
    AttributeNode::SetValue<&Color::Secondary88>(target, GetRGBAFromHex(palettes.secondary, 88));
    AttributeNode::SetValue<&Color::Secondary89>(target, GetRGBAFromHex(palettes.secondary, 89));
    AttributeNode::SetValue<&Color::Secondary90>(target, GetRGBAFromHex(palettes.secondary, 90));
    AttributeNode::SetValue<&Color::Secondary91>(target, GetRGBAFromHex(palettes.secondary, 91));
    AttributeNode::SetValue<&Color::Secondary92>(target, GetRGBAFromHex(palettes.secondary, 92));
    AttributeNode::SetValue<&Color::Secondary93>(target, GetRGBAFromHex(palettes.secondary, 93));
    AttributeNode::SetValue<&Color::Secondary94>(target, GetRGBAFromHex(palettes.secondary, 94));
    AttributeNode::SetValue<&Color::Secondary95>(target, GetRGBAFromHex(palettes.secondary, 95));
    AttributeNode::SetValue<&Color::Secondary96>(target, GetRGBAFromHex(palettes.secondary, 96));
    AttributeNode::SetValue<&Color::Secondary97>(target, GetRGBAFromHex(palettes.secondary, 97));
    AttributeNode::SetValue<&Color::Secondary98>(target, GetRGBAFromHex(palettes.secondary, 98));
    AttributeNode::SetValue<&Color::Secondary99>(target, GetRGBAFromHex(palettes.secondary, 99));
    AttributeNode::SetValue<&Color::Secondary100>(target, GetRGBAFromHex(palettes.secondary, 100));

    AttributeNode::SetValue<&Color::Tertiary0>(target, GetRGBAFromHex(palettes.tertiary, 0));
    AttributeNode::SetValue<&Color::Tertiary1>(target, GetRGBAFromHex(palettes.tertiary, 1));
    AttributeNode::SetValue<&Color::Tertiary2>(target, GetRGBAFromHex(palettes.tertiary, 2));
    AttributeNode::SetValue<&Color::Tertiary3>(target, GetRGBAFromHex(palettes.tertiary, 3));
    AttributeNode::SetValue<&Color::Tertiary4>(target, GetRGBAFromHex(palettes.tertiary, 4));
    AttributeNode::SetValue<&Color::Tertiary5>(target, GetRGBAFromHex(palettes.tertiary, 5));
    AttributeNode::SetValue<&Color::Tertiary6>(target, GetRGBAFromHex(palettes.tertiary, 6));
    AttributeNode::SetValue<&Color::Tertiary7>(target, GetRGBAFromHex(palettes.tertiary, 7));
    AttributeNode::SetValue<&Color::Tertiary8>(target, GetRGBAFromHex(palettes.tertiary, 8));
    AttributeNode::SetValue<&Color::Tertiary9>(target, GetRGBAFromHex(palettes.tertiary, 9));
    AttributeNode::SetValue<&Color::Tertiary10>(target, GetRGBAFromHex(palettes.tertiary, 10));
    AttributeNode::SetValue<&Color::Tertiary11>(target, GetRGBAFromHex(palettes.tertiary, 11));
    AttributeNode::SetValue<&Color::Tertiary12>(target, GetRGBAFromHex(palettes.tertiary, 12));
    AttributeNode::SetValue<&Color::Tertiary13>(target, GetRGBAFromHex(palettes.tertiary, 13));
    AttributeNode::SetValue<&Color::Tertiary14>(target, GetRGBAFromHex(palettes.tertiary, 14));
    AttributeNode::SetValue<&Color::Tertiary15>(target, GetRGBAFromHex(palettes.tertiary, 15));
    AttributeNode::SetValue<&Color::Tertiary16>(target, GetRGBAFromHex(palettes.tertiary, 16));
    AttributeNode::SetValue<&Color::Tertiary17>(target, GetRGBAFromHex(palettes.tertiary, 17));
    AttributeNode::SetValue<&Color::Tertiary18>(target, GetRGBAFromHex(palettes.tertiary, 18));
    AttributeNode::SetValue<&Color::Tertiary19>(target, GetRGBAFromHex(palettes.tertiary, 19));
    AttributeNode::SetValue<&Color::Tertiary20>(target, GetRGBAFromHex(palettes.tertiary, 20));
    AttributeNode::SetValue<&Color::Tertiary25>(target, GetRGBAFromHex(palettes.tertiary, 25));
    AttributeNode::SetValue<&Color::Tertiary30>(target, GetRGBAFromHex(palettes.tertiary, 30));
    AttributeNode::SetValue<&Color::Tertiary40>(target, GetRGBAFromHex(palettes.tertiary, 40));
    AttributeNode::SetValue<&Color::Tertiary50>(target, GetRGBAFromHex(palettes.tertiary, 50));
    AttributeNode::SetValue<&Color::Tertiary60>(target, GetRGBAFromHex(palettes.tertiary, 60));
    AttributeNode::SetValue<&Color::Tertiary70>(target, GetRGBAFromHex(palettes.tertiary, 70));
    AttributeNode::SetValue<&Color::Tertiary80>(target, GetRGBAFromHex(palettes.tertiary, 80));
    AttributeNode::SetValue<&Color::Tertiary81>(target, GetRGBAFromHex(palettes.tertiary, 81));
    AttributeNode::SetValue<&Color::Tertiary82>(target, GetRGBAFromHex(palettes.tertiary, 82));
    AttributeNode::SetValue<&Color::Tertiary83>(target, GetRGBAFromHex(palettes.tertiary, 83));
    AttributeNode::SetValue<&Color::Tertiary84>(target, GetRGBAFromHex(palettes.tertiary, 84));
    AttributeNode::SetValue<&Color::Tertiary85>(target, GetRGBAFromHex(palettes.tertiary, 85));
    AttributeNode::SetValue<&Color::Tertiary86>(target, GetRGBAFromHex(palettes.tertiary, 86));
    AttributeNode::SetValue<&Color::Tertiary87>(target, GetRGBAFromHex(palettes.tertiary, 87));
    AttributeNode::SetValue<&Color::Tertiary88>(target, GetRGBAFromHex(palettes.tertiary, 88));
    AttributeNode::SetValue<&Color::Tertiary89>(target, GetRGBAFromHex(palettes.tertiary, 89));
    AttributeNode::SetValue<&Color::Tertiary90>(target, GetRGBAFromHex(palettes.tertiary, 90));
    AttributeNode::SetValue<&Color::Tertiary91>(target, GetRGBAFromHex(palettes.tertiary, 91));
    AttributeNode::SetValue<&Color::Tertiary92>(target, GetRGBAFromHex(palettes.tertiary, 92));
    AttributeNode::SetValue<&Color::Tertiary93>(target, GetRGBAFromHex(palettes.tertiary, 93));
    AttributeNode::SetValue<&Color::Tertiary94>(target, GetRGBAFromHex(palettes.tertiary, 94));
    AttributeNode::SetValue<&Color::Tertiary95>(target, GetRGBAFromHex(palettes.tertiary, 95));
    AttributeNode::SetValue<&Color::Tertiary96>(target, GetRGBAFromHex(palettes.tertiary, 96));
    AttributeNode::SetValue<&Color::Tertiary97>(target, GetRGBAFromHex(palettes.tertiary, 97));
    AttributeNode::SetValue<&Color::Tertiary98>(target, GetRGBAFromHex(palettes.tertiary, 98));
    AttributeNode::SetValue<&Color::Tertiary99>(target, GetRGBAFromHex(palettes.tertiary, 99));
    AttributeNode::SetValue<&Color::Tertiary100>(target, GetRGBAFromHex(palettes.tertiary, 100));

    AttributeNode::SetValue<&Color::Error0>(target, GetRGBAFromHex(palettes.error, 0));
    AttributeNode::SetValue<&Color::Error5>(target, GetRGBAFromHex(palettes.error, 5));
    AttributeNode::SetValue<&Color::Error10>(target, GetRGBAFromHex(palettes.error, 10));
    AttributeNode::SetValue<&Color::Error15>(target, GetRGBAFromHex(palettes.error, 15));
    AttributeNode::SetValue<&Color::Error20>(target, GetRGBAFromHex(palettes.error, 20));
    AttributeNode::SetValue<&Color::Error25>(target, GetRGBAFromHex(palettes.error, 25));
    AttributeNode::SetValue<&Color::Error30>(target, GetRGBAFromHex(palettes.error, 30));
    AttributeNode::SetValue<&Color::Error35>(target, GetRGBAFromHex(palettes.error, 35));
    AttributeNode::SetValue<&Color::Error40>(target, GetRGBAFromHex(palettes.error, 40));
    AttributeNode::SetValue<&Color::Error50>(target, GetRGBAFromHex(palettes.error, 50));
    AttributeNode::SetValue<&Color::Error60>(target, GetRGBAFromHex(palettes.error, 60));
    AttributeNode::SetValue<&Color::Error70>(target, GetRGBAFromHex(palettes.error, 70));
    AttributeNode::SetValue<&Color::Error80>(target, GetRGBAFromHex(palettes.error, 80));
    AttributeNode::SetValue<&Color::Error90>(target, GetRGBAFromHex(palettes.error, 90));
    AttributeNode::SetValue<&Color::Error95>(target, GetRGBAFromHex(palettes.error, 95));
    AttributeNode::SetValue<&Color::Error98>(target, GetRGBAFromHex(palettes.error, 98));
    AttributeNode::SetValue<&Color::Error99>(target, GetRGBAFromHex(palettes.error, 99));
    AttributeNode::SetValue<&Color::Error100>(target, GetRGBAFromHex(palettes.error, 100));

    AttributeNode::SetValue<&Color::Neutral0>(target, GetRGBAFromHex(palettes.neutral, 0));
    AttributeNode::SetValue<&Color::Neutral1>(target, GetRGBAFromHex(palettes.neutral, 1));
    AttributeNode::SetValue<&Color::Neutral2>(target, GetRGBAFromHex(palettes.neutral, 2));
    AttributeNode::SetValue<&Color::Neutral3>(target, GetRGBAFromHex(palettes.neutral, 3));
    AttributeNode::SetValue<&Color::Neutral4>(target, GetRGBAFromHex(palettes.neutral, 4));
    AttributeNode::SetValue<&Color::Neutral5>(target, GetRGBAFromHex(palettes.neutral, 5));
    AttributeNode::SetValue<&Color::Neutral6>(target, GetRGBAFromHex(palettes.neutral, 6));
    AttributeNode::SetValue<&Color::Neutral7>(target, GetRGBAFromHex(palettes.neutral, 7));
    AttributeNode::SetValue<&Color::Neutral8>(target, GetRGBAFromHex(palettes.neutral, 8));
    AttributeNode::SetValue<&Color::Neutral9>(target, GetRGBAFromHex(palettes.neutral, 9));
    AttributeNode::SetValue<&Color::Neutral10>(target, GetRGBAFromHex(palettes.neutral, 10));
    AttributeNode::SetValue<&Color::Neutral11>(target, GetRGBAFromHex(palettes.neutral, 11));
    AttributeNode::SetValue<&Color::Neutral12>(target, GetRGBAFromHex(palettes.neutral, 12));
    AttributeNode::SetValue<&Color::Neutral13>(target, GetRGBAFromHex(palettes.neutral, 13));
    AttributeNode::SetValue<&Color::Neutral14>(target, GetRGBAFromHex(palettes.neutral, 14));
    AttributeNode::SetValue<&Color::Neutral15>(target, GetRGBAFromHex(palettes.neutral, 15));
    AttributeNode::SetValue<&Color::Neutral16>(target, GetRGBAFromHex(palettes.neutral, 16));
    AttributeNode::SetValue<&Color::Neutral17>(target, GetRGBAFromHex(palettes.neutral, 17));
    AttributeNode::SetValue<&Color::Neutral18>(target, GetRGBAFromHex(palettes.neutral, 18));
    AttributeNode::SetValue<&Color::Neutral19>(target, GetRGBAFromHex(palettes.neutral, 19));
    AttributeNode::SetValue<&Color::Neutral20>(target, GetRGBAFromHex(palettes.neutral, 20));
    AttributeNode::SetValue<&Color::Neutral21>(target, GetRGBAFromHex(palettes.neutral, 21));
    AttributeNode::SetValue<&Color::Neutral22>(target, GetRGBAFromHex(palettes.neutral, 22));
    AttributeNode::SetValue<&Color::Neutral23>(target, GetRGBAFromHex(palettes.neutral, 23));
    AttributeNode::SetValue<&Color::Neutral24>(target, GetRGBAFromHex(palettes.neutral, 24));
    AttributeNode::SetValue<&Color::Neutral25>(target, GetRGBAFromHex(palettes.neutral, 25));
    AttributeNode::SetValue<&Color::Neutral26>(target, GetRGBAFromHex(palettes.neutral, 26));
    AttributeNode::SetValue<&Color::Neutral27>(target, GetRGBAFromHex(palettes.neutral, 27));
    AttributeNode::SetValue<&Color::Neutral28>(target, GetRGBAFromHex(palettes.neutral, 28));
    AttributeNode::SetValue<&Color::Neutral29>(target, GetRGBAFromHex(palettes.neutral, 29));
    AttributeNode::SetValue<&Color::Neutral30>(target, GetRGBAFromHex(palettes.neutral, 30));
    AttributeNode::SetValue<&Color::Neutral40>(target, GetRGBAFromHex(palettes.neutral, 40));
    AttributeNode::SetValue<&Color::Neutral50>(target, GetRGBAFromHex(palettes.neutral, 50));
    AttributeNode::SetValue<&Color::Neutral60>(target, GetRGBAFromHex(palettes.neutral, 60));
    AttributeNode::SetValue<&Color::Neutral70>(target, GetRGBAFromHex(palettes.neutral, 70));
    AttributeNode::SetValue<&Color::Neutral71>(target, GetRGBAFromHex(palettes.neutral, 71));
    AttributeNode::SetValue<&Color::Neutral72>(target, GetRGBAFromHex(palettes.neutral, 72));
    AttributeNode::SetValue<&Color::Neutral73>(target, GetRGBAFromHex(palettes.neutral, 73));
    AttributeNode::SetValue<&Color::Neutral74>(target, GetRGBAFromHex(palettes.neutral, 74));
    AttributeNode::SetValue<&Color::Neutral75>(target, GetRGBAFromHex(palettes.neutral, 75));
    AttributeNode::SetValue<&Color::Neutral76>(target, GetRGBAFromHex(palettes.neutral, 76));
    AttributeNode::SetValue<&Color::Neutral77>(target, GetRGBAFromHex(palettes.neutral, 77));
    AttributeNode::SetValue<&Color::Neutral78>(target, GetRGBAFromHex(palettes.neutral, 78));
    AttributeNode::SetValue<&Color::Neutral79>(target, GetRGBAFromHex(palettes.neutral, 79));
    AttributeNode::SetValue<&Color::Neutral80>(target, GetRGBAFromHex(palettes.neutral, 80));
    AttributeNode::SetValue<&Color::Neutral81>(target, GetRGBAFromHex(palettes.neutral, 81));
    AttributeNode::SetValue<&Color::Neutral82>(target, GetRGBAFromHex(palettes.neutral, 82));
    AttributeNode::SetValue<&Color::Neutral83>(target, GetRGBAFromHex(palettes.neutral, 83));
    AttributeNode::SetValue<&Color::Neutral84>(target, GetRGBAFromHex(palettes.neutral, 84));
    AttributeNode::SetValue<&Color::Neutral85>(target, GetRGBAFromHex(palettes.neutral, 85));
    AttributeNode::SetValue<&Color::Neutral86>(target, GetRGBAFromHex(palettes.neutral, 86));
    AttributeNode::SetValue<&Color::Neutral87>(target, GetRGBAFromHex(palettes.neutral, 87));
    AttributeNode::SetValue<&Color::Neutral88>(target, GetRGBAFromHex(palettes.neutral, 88));
    AttributeNode::SetValue<&Color::Neutral89>(target, GetRGBAFromHex(palettes.neutral, 89));
    AttributeNode::SetValue<&Color::Neutral90>(target, GetRGBAFromHex(palettes.neutral, 90));
    AttributeNode::SetValue<&Color::Neutral91>(target, GetRGBAFromHex(palettes.neutral, 91));
    AttributeNode::SetValue<&Color::Neutral92>(target, GetRGBAFromHex(palettes.neutral, 92));
    AttributeNode::SetValue<&Color::Neutral93>(target, GetRGBAFromHex(palettes.neutral, 93));
    AttributeNode::SetValue<&Color::Neutral94>(target, GetRGBAFromHex(palettes.neutral, 94));
    AttributeNode::SetValue<&Color::Neutral95>(target, GetRGBAFromHex(palettes.neutral, 95));
    AttributeNode::SetValue<&Color::Neutral96>(target, GetRGBAFromHex(palettes.neutral, 96));
    AttributeNode::SetValue<&Color::Neutral97>(target, GetRGBAFromHex(palettes.neutral, 97));
    AttributeNode::SetValue<&Color::Neutral98>(target, GetRGBAFromHex(palettes.neutral, 98));
    AttributeNode::SetValue<&Color::Neutral99>(target, GetRGBAFromHex(palettes.neutral, 99));
    AttributeNode::SetValue<&Color::Neutral100>(target, GetRGBAFromHex(palettes.neutral, 100));

    AttributeNode::SetValue<&Color::NeutralVariant0>(target, GetRGBAFromHex(palettes.neutralVariant, 0));
    AttributeNode::SetValue<&Color::NeutralVariant1>(target, GetRGBAFromHex(palettes.neutralVariant, 1));
    AttributeNode::SetValue<&Color::NeutralVariant2>(target, GetRGBAFromHex(palettes.neutralVariant, 2));
    AttributeNode::SetValue<&Color::NeutralVariant3>(target, GetRGBAFromHex(palettes.neutralVariant, 3));
    AttributeNode::SetValue<&Color::NeutralVariant4>(target, GetRGBAFromHex(palettes.neutralVariant, 4));
    AttributeNode::SetValue<&Color::NeutralVariant5>(target, GetRGBAFromHex(palettes.neutralVariant, 5));
    AttributeNode::SetValue<&Color::NeutralVariant6>(target, GetRGBAFromHex(palettes.neutralVariant, 6));
    AttributeNode::SetValue<&Color::NeutralVariant7>(target, GetRGBAFromHex(palettes.neutralVariant, 7));
    AttributeNode::SetValue<&Color::NeutralVariant8>(target, GetRGBAFromHex(palettes.neutralVariant, 8));
    AttributeNode::SetValue<&Color::NeutralVariant9>(target, GetRGBAFromHex(palettes.neutralVariant, 9));
    AttributeNode::SetValue<&Color::NeutralVariant10>(target, GetRGBAFromHex(palettes.neutralVariant, 10));
    AttributeNode::SetValue<&Color::NeutralVariant11>(target, GetRGBAFromHex(palettes.neutralVariant, 11));
    AttributeNode::SetValue<&Color::NeutralVariant12>(target, GetRGBAFromHex(palettes.neutralVariant, 12));
    AttributeNode::SetValue<&Color::NeutralVariant13>(target, GetRGBAFromHex(palettes.neutralVariant, 13));
    AttributeNode::SetValue<&Color::NeutralVariant14>(target, GetRGBAFromHex(palettes.neutralVariant, 14));
    AttributeNode::SetValue<&Color::NeutralVariant15>(target, GetRGBAFromHex(palettes.neutralVariant, 15));
    AttributeNode::SetValue<&Color::NeutralVariant16>(target, GetRGBAFromHex(palettes.neutralVariant, 16));
    AttributeNode::SetValue<&Color::NeutralVariant17>(target, GetRGBAFromHex(palettes.neutralVariant, 17));
    AttributeNode::SetValue<&Color::NeutralVariant18>(target, GetRGBAFromHex(palettes.neutralVariant, 18));
    AttributeNode::SetValue<&Color::NeutralVariant19>(target, GetRGBAFromHex(palettes.neutralVariant, 19));
    AttributeNode::SetValue<&Color::NeutralVariant20>(target, GetRGBAFromHex(palettes.neutralVariant, 20));
    AttributeNode::SetValue<&Color::NeutralVariant21>(target, GetRGBAFromHex(palettes.neutralVariant, 21));
    AttributeNode::SetValue<&Color::NeutralVariant22>(target, GetRGBAFromHex(palettes.neutralVariant, 22));
    AttributeNode::SetValue<&Color::NeutralVariant23>(target, GetRGBAFromHex(palettes.neutralVariant, 23));
    AttributeNode::SetValue<&Color::NeutralVariant24>(target, GetRGBAFromHex(palettes.neutralVariant, 24));
    AttributeNode::SetValue<&Color::NeutralVariant25>(target, GetRGBAFromHex(palettes.neutralVariant, 25));
    AttributeNode::SetValue<&Color::NeutralVariant26>(target, GetRGBAFromHex(palettes.neutralVariant, 26));
    AttributeNode::SetValue<&Color::NeutralVariant27>(target, GetRGBAFromHex(palettes.neutralVariant, 27));
    AttributeNode::SetValue<&Color::NeutralVariant28>(target, GetRGBAFromHex(palettes.neutralVariant, 28));
    AttributeNode::SetValue<&Color::NeutralVariant29>(target, GetRGBAFromHex(palettes.neutralVariant, 29));
    AttributeNode::SetValue<&Color::NeutralVariant30>(target, GetRGBAFromHex(palettes.neutralVariant, 30));
    AttributeNode::SetValue<&Color::NeutralVariant40>(target, GetRGBAFromHex(palettes.neutralVariant, 40));
    AttributeNode::SetValue<&Color::NeutralVariant50>(target, GetRGBAFromHex(palettes.neutralVariant, 50));
    AttributeNode::SetValue<&Color::NeutralVariant60>(target, GetRGBAFromHex(palettes.neutralVariant, 60));
    AttributeNode::SetValue<&Color::NeutralVariant70>(target, GetRGBAFromHex(palettes.neutralVariant, 70));
    AttributeNode::SetValue<&Color::NeutralVariant71>(target, GetRGBAFromHex(palettes.neutralVariant, 71));
    AttributeNode::SetValue<&Color::NeutralVariant72>(target, GetRGBAFromHex(palettes.neutralVariant, 72));
    AttributeNode::SetValue<&Color::NeutralVariant73>(target, GetRGBAFromHex(palettes.neutralVariant, 73));
    AttributeNode::SetValue<&Color::NeutralVariant74>(target, GetRGBAFromHex(palettes.neutralVariant, 74));
    AttributeNode::SetValue<&Color::NeutralVariant75>(target, GetRGBAFromHex(palettes.neutralVariant, 75));
    AttributeNode::SetValue<&Color::NeutralVariant76>(target, GetRGBAFromHex(palettes.neutralVariant, 76));
    AttributeNode::SetValue<&Color::NeutralVariant77>(target, GetRGBAFromHex(palettes.neutralVariant, 77));
    AttributeNode::SetValue<&Color::NeutralVariant78>(target, GetRGBAFromHex(palettes.neutralVariant, 78));
    AttributeNode::SetValue<&Color::NeutralVariant79>(target, GetRGBAFromHex(palettes.neutralVariant, 79));
    AttributeNode::SetValue<&Color::NeutralVariant80>(target, GetRGBAFromHex(palettes.neutralVariant, 80));
    AttributeNode::SetValue<&Color::NeutralVariant81>(target, GetRGBAFromHex(palettes.neutralVariant, 81));
    AttributeNode::SetValue<&Color::NeutralVariant82>(target, GetRGBAFromHex(palettes.neutralVariant, 82));
    AttributeNode::SetValue<&Color::NeutralVariant83>(target, GetRGBAFromHex(palettes.neutralVariant, 83));
    AttributeNode::SetValue<&Color::NeutralVariant84>(target, GetRGBAFromHex(palettes.neutralVariant, 84));
    AttributeNode::SetValue<&Color::NeutralVariant85>(target, GetRGBAFromHex(palettes.neutralVariant, 85));
    AttributeNode::SetValue<&Color::NeutralVariant86>(target, GetRGBAFromHex(palettes.neutralVariant, 86));
    AttributeNode::SetValue<&Color::NeutralVariant87>(target, GetRGBAFromHex(palettes.neutralVariant, 87));
    AttributeNode::SetValue<&Color::NeutralVariant88>(target, GetRGBAFromHex(palettes.neutralVariant, 88));
    AttributeNode::SetValue<&Color::NeutralVariant89>(target, GetRGBAFromHex(palettes.neutralVariant, 89));
    AttributeNode::SetValue<&Color::NeutralVariant90>(target, GetRGBAFromHex(palettes.neutralVariant, 90));
    AttributeNode::SetValue<&Color::NeutralVariant91>(target, GetRGBAFromHex(palettes.neutralVariant, 91));
    AttributeNode::SetValue<&Color::NeutralVariant92>(target, GetRGBAFromHex(palettes.neutralVariant, 92));
    AttributeNode::SetValue<&Color::NeutralVariant93>(target, GetRGBAFromHex(palettes.neutralVariant, 93));
    AttributeNode::SetValue<&Color::NeutralVariant94>(target, GetRGBAFromHex(palettes.neutralVariant, 94));
    AttributeNode::SetValue<&Color::NeutralVariant95>(target, GetRGBAFromHex(palettes.neutralVariant, 95));
    AttributeNode::SetValue<&Color::NeutralVariant96>(target, GetRGBAFromHex(palettes.neutralVariant, 96));
    AttributeNode::SetValue<&Color::NeutralVariant97>(target, GetRGBAFromHex(palettes.neutralVariant, 97));
    AttributeNode::SetValue<&Color::NeutralVariant98>(target, GetRGBAFromHex(palettes.neutralVariant, 98));
    AttributeNode::SetValue<&Color::NeutralVariant99>(target, GetRGBAFromHex(palettes.neutralVariant, 99));
    AttributeNode::SetValue<&Color::NeutralVariant100>(target, GetRGBAFromHex(palettes.neutralVariant, 100));

    AttributeNode::SetValue<&Color::Black>(target, RGBAColor::MakeFromHex(0x0000FF));
    AttributeNode::SetValue<&Color::White>(target, RGBAColor::MakeFromHex(0xFFFFFF));
}

auto Theme::ApplyApplicationStyle(AttributeNode& target) -> void
{
    if (_brightness == ThemeBrightness::Light)
    {
        AttributeNode::SetReference<&Style::ColorPrimary>(target, Color::Primary40);
        AttributeNode::SetReference<&Style::ColorPrimaryContainer>(target, Color::Primary90);
        AttributeNode::SetReference<&Style::ColorOnPrimary>(target, Color::Primary100);
        AttributeNode::SetReference<&Style::ColorOnPrimaryContainer>(target, Color::Primary30);
        AttributeNode::SetReference<&Style::ColorSecondary>(target, Color::Secondary40);
        AttributeNode::SetReference<&Style::ColorSecondaryContainer>(target, Color::Secondary90);
        AttributeNode::SetReference<&Style::ColorOnSecondary>(target, Color::Secondary100);
        AttributeNode::SetReference<&Style::ColorOnSecondaryContainer>(target, Color::Secondary30);
        AttributeNode::SetReference<&Style::ColorTertiary>(target, Color::Tertiary40);
        AttributeNode::SetReference<&Style::ColorTertiaryContainer>(target, Color::Tertiary90);
        AttributeNode::SetReference<&Style::ColorOnTertiary>(target, Color::Tertiary100);
        AttributeNode::SetReference<&Style::ColorOnTertiaryContainer>(target, Color::Tertiary30);
        AttributeNode::SetReference<&Style::ColorSurface>(target, Color::Neutral98);
        AttributeNode::SetReference<&Style::ColorSurfaceVariant>(target, Color::NeutralVariant90);
        AttributeNode::SetReference<&Style::ColorSurfaceDim>(target, Color::Neutral87);
        AttributeNode::SetReference<&Style::ColorSurfaceBright>(target, Color::Neutral98);
        AttributeNode::SetReference<&Style::ColorSurfaceContainerLowest>(target, Color::Neutral100);
        AttributeNode::SetReference<&Style::ColorSurfaceContainerLow>(target, Color::Neutral96);
        AttributeNode::SetReference<&Style::ColorSurfaceContainer>(target, Color::Neutral94);
        AttributeNode::SetReference<&Style::ColorSurfaceContainerHigh>(target, Color::Neutral92);
        AttributeNode::SetReference<&Style::ColorSurfaceContainerHighest>(target, Color::Neutral90);
        AttributeNode::SetReference<&Style::ColorOnSurface>(target, Color::Neutral10);
        AttributeNode::SetReference<&Style::ColorOnSurfaceVariant>(target, Color::Neutral30);
        AttributeNode::SetReference<&Style::ColorOutline>(target, Color::NeutralVariant50);
        AttributeNode::SetReference<&Style::ColorOutlineVariant>(target, Color::NeutralVariant90);
        AttributeNode::SetReference<&Style::ColorError>(target, Color::Error40);
        AttributeNode::SetReference<&Style::ColorErrorContainer>(target, Color::Error90);
        AttributeNode::SetReference<&Style::ColorOnError>(target, Color::Error100);
        AttributeNode::SetReference<&Style::ColorOnErrorContainer>(target, Color::Error30);
    }
    else
    {
        AttributeNode::SetReference<&Style::ColorPrimary>(target, Color::Primary80);
        AttributeNode::SetReference<&Style::ColorPrimaryContainer>(target, Color::Primary30);
        AttributeNode::SetReference<&Style::ColorOnPrimary>(target, Color::Primary20);
        AttributeNode::SetReference<&Style::ColorOnPrimaryContainer>(target, Color::Primary90);
        AttributeNode::SetReference<&Style::ColorSecondary>(target, Color::Secondary80);
        AttributeNode::SetReference<&Style::ColorSecondaryContainer>(target, Color::Secondary30);
        AttributeNode::SetReference<&Style::ColorOnSecondary>(target, Color::Secondary20);
        AttributeNode::SetReference<&Style::ColorOnSecondaryContainer>(target, Color::Secondary90);
        AttributeNode::SetReference<&Style::ColorTertiary>(target, Color::Tertiary90);
        AttributeNode::SetReference<&Style::ColorTertiaryContainer>(target, Color::Tertiary30);
        AttributeNode::SetReference<&Style::ColorOnTertiary>(target, Color::Tertiary20);
        AttributeNode::SetReference<&Style::ColorOnTertiaryContainer>(target, Color::Tertiary90);
        AttributeNode::SetReference<&Style::ColorSurface>(target, Color::Neutral6);
        AttributeNode::SetReference<&Style::ColorSurfaceVariant>(target, Color::NeutralVariant30);
        AttributeNode::SetReference<&Style::ColorSurfaceDim>(target, Color::Neutral6);
        AttributeNode::SetReference<&Style::ColorSurfaceBright>(target, Color::Neutral24);
        AttributeNode::SetReference<&Style::ColorSurfaceContainerLowest>(target, Color::Neutral4);
        AttributeNode::SetReference<&Style::ColorSurfaceContainerLow>(target, Color::Neutral10);
        AttributeNode::SetReference<&Style::ColorSurfaceContainer>(target, Color::Neutral12);
        AttributeNode::SetReference<&Style::ColorSurfaceContainerHigh>(target, Color::Neutral17);
        AttributeNode::SetReference<&Style::ColorSurfaceContainerHighest>(target, Color::Neutral22);
        AttributeNode::SetReference<&Style::ColorOnSurface>(target, Color::Neutral90);
        AttributeNode::SetReference<&Style::ColorOnSurfaceVariant>(target, Color::NeutralVariant80);
        AttributeNode::SetReference<&Style::ColorOutline>(target, Color::NeutralVariant60);
        AttributeNode::SetReference<&Style::ColorOutlineVariant>(target, Color::NeutralVariant30);
        AttributeNode::SetReference<&Style::ColorError>(target, Color::Error80);
        AttributeNode::SetReference<&Style::ColorErrorContainer>(target, Color::Error30);
        AttributeNode::SetReference<&Style::ColorOnError>(target, Color::Error20);
        AttributeNode::SetReference<&Style::ColorOnErrorContainer>(target, Color::Error90);
    }

    AttributeNode::SetValue<&Style::AlphaHighlightHover>(target, 0.08);
    AttributeNode::SetValue<&Style::AlphaHighlightPress>(target, 0.10);

    AttributeNode::SetValue<&Style::AlphaEnabled>(target, 1.0);
    AttributeNode::SetValue<&Style::AlphaDisabled>(target, 0.38);

    AttributeNode::SetValue<&Style::FontSizeBodySmall>(target, 11);
    AttributeNode::SetValue<&Style::FontSizeBodyMedium>(target, 14);
    AttributeNode::SetValue<&Style::FontSizeBodyLarge>(target, 16);
    AttributeNode::SetValue<&Style::FontSizeLabelSmall>(target, 11);
    AttributeNode::SetValue<&Style::FontSizeLabelMedium>(target, 12);
    AttributeNode::SetValue<&Style::FontSizeLabelLarge>(target, 14);
    AttributeNode::SetValue<&Style::FontSizeTitleSmall>(target, 14);
    AttributeNode::SetValue<&Style::FontSizeTitleMedium>(target, 16);
    AttributeNode::SetValue<&Style::FontSizeTitleLarge>(target, 22);
    AttributeNode::SetValue<&Style::FontSizeHeadlineSmall>(target, 24);
    AttributeNode::SetValue<&Style::FontSizeHeadlineMedium>(target, 28);
    AttributeNode::SetValue<&Style::FontSizeHeadlineLarge>(target, 32);
    AttributeNode::SetValue<&Style::FontSizeDisplaySmall>(target, 36);
    AttributeNode::SetValue<&Style::FontSizeDisplayMedium>(target, 45);
    AttributeNode::SetValue<&Style::FontSizeDisplayLarge>(target, 57);

    AttributeNode::SetValue<&Style::FontWeightBodySmall>(target, 400);
    AttributeNode::SetValue<&Style::FontWeightBodyMedium>(target, 400);
    AttributeNode::SetValue<&Style::FontWeightBodyLarge>(target, 400);
    AttributeNode::SetValue<&Style::FontWeightLabelSmall>(target, 500);
    AttributeNode::SetValue<&Style::FontWeightLabelMedium>(target, 500);
    AttributeNode::SetValue<&Style::FontWeightLabelLarge>(target, 500);
    AttributeNode::SetValue<&Style::FontWeightTitleSmall>(target, 500);
    AttributeNode::SetValue<&Style::FontWeightTitleMedium>(target, 500);
    AttributeNode::SetValue<&Style::FontWeightTitleLarge>(target, 400);
    AttributeNode::SetValue<&Style::FontWeightHeadlineSmall>(target, 400);
    AttributeNode::SetValue<&Style::FontWeightHeadlineMedium>(target, 400);
    AttributeNode::SetValue<&Style::FontWeightHeadlineLarge>(target, 400);
    AttributeNode::SetValue<&Style::FontWeightDisplaySmall>(target, 400);
    AttributeNode::SetValue<&Style::FontWeightDisplayMedium>(target, 400);
    AttributeNode::SetValue<&Style::FontWeightDisplayLarge>(target, 400);

    AttributeNode::SetValue<&Style::CornerRadiusNone>(target, CornerRadius::MakeUniform(0));
    AttributeNode::SetValue<&Style::CornerRadiusExtraSmall>(target, CornerRadius::MakeUniform(4));
    AttributeNode::SetValue<&Style::CornerRadiusSmall>(target, CornerRadius::MakeUniform(8));
    AttributeNode::SetValue<&Style::CornerRadiusMedium>(target, CornerRadius::MakeUniform(12));
    AttributeNode::SetValue<&Style::CornerRadiusLarge>(target, CornerRadius::MakeUniform(16));
    AttributeNode::SetValue<&Style::CornerRadiusLargeIncreased>(target, CornerRadius::MakeUniform(20));
    AttributeNode::SetValue<&Style::CornerRadiusExtraLarge>(target, CornerRadius::MakeUniform(28));
    AttributeNode::SetValue<&Style::CornerRadiusExtraLargeIncreased>(target, CornerRadius::MakeUniform(32));
    AttributeNode::SetValue<&Style::CornerRadiusLargest>(target, CornerRadius::MakeLargest());

    AttributeNode::SetValue<&Style::IconSizeNone>(target, 0);
    AttributeNode::SetValue<&Style::IconSizeSmall>(target, 20);
    AttributeNode::SetValue<&Style::IconSizeMedium>(target, 24);
    AttributeNode::SetValue<&Style::IconSizeLarge>(target, 40);
    AttributeNode::SetValue<&Style::IconSizeExtraLarge>(target, 48);

    AttributeNode::SetValue<&Style::SpaceNone>(target, 0);
    AttributeNode::SetValue<&Style::SpaceExtraSmall>(target, 4);
    AttributeNode::SetValue<&Style::SpaceSmall>(target, 8);
    AttributeNode::SetValue<&Style::SpaceMedium>(target, 16);
    AttributeNode::SetValue<&Style::SpaceLarge>(target, 24);
    AttributeNode::SetValue<&Style::SpaceExtraLarge>(target, 32);

    AttributeNode::SetValue<&Style::PaddingNone>(target, EdgeInsets::MakeUniform(0));
    AttributeNode::SetValue<&Style::PaddingExtraSmall>(target, EdgeInsets::MakeUniform(4));
    AttributeNode::SetValue<&Style::PaddingSmall>(target, EdgeInsets::MakeUniform(8));
    AttributeNode::SetValue<&Style::PaddingMedium>(target, EdgeInsets::MakeUniform(16));
    AttributeNode::SetValue<&Style::PaddingLarge>(target, EdgeInsets::MakeUniform(24));
    AttributeNode::SetValue<&Style::PaddingExtraLarge>(target, EdgeInsets::MakeUniform(32));
}
}
}
