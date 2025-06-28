// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Unit.UnitFunction.hpp"

#include <numbers>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVp(Px const& dp, BackingScale const& scale) -> Vp
{
    return Vp(static_cast<float64_t>(dp) / static_cast<float64_t>(scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVp(Size<Px> const& dp, BackingScale const& scale) -> Size<Vp>
{
    return Size<Vp>(ConvertPxToVp(dp.GetWidth(), scale), ConvertPxToVp(dp.GetHeight(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVp(Point<Px> const& dp, BackingScale const& scale) -> Point<Vp>
{
    return Point<Vp>(ConvertPxToVp(dp.GetX(), scale), ConvertPxToVp(dp.GetY(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVp(Rect<Px> const& dp, BackingScale const& scale) -> Rect<Vp>
{
    return Rect<Vp>(ConvertPxToVp(dp.GetLeft(), scale), ConvertPxToVp(dp.GetTop(), scale), ConvertPxToVp(dp.GetRight(), scale), ConvertPxToVp(dp.GetBottom(), scale));
}

///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToPx(Vp const& sp, BackingScale const& scale) -> Px
{
    return Px(static_cast<float64_t>(sp) * static_cast<float64_t>(scale));
}

///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToPx(Size<Vp> const& sp, BackingScale const& scale) -> Size<Px>
{
    return Size<Px>(ConvertVpToPx(sp.GetWidth(), scale), ConvertVpToPx(sp.GetHeight(), scale));
}

///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToPx(Point<Vp> const& sp, BackingScale const& scale) -> Point<Px>
{
    return Point<Px>(ConvertVpToPx(sp.GetX(), scale), ConvertVpToPx(sp.GetY(), scale));
}

///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToPx(Rect<Vp> const& sp, BackingScale const& scale) -> Rect<Px>
{
    return Rect<Px>(ConvertVpToPx(sp.GetTopLeft(), scale), ConvertVpToPx(sp.GetSize(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToDp(Vp const& sp, DisplayScale const& scale) -> Dp
{
    return Dp(static_cast<float64_t>(sp) / static_cast<float64_t>(scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToDp(Size<Vp> const& sp, DisplayScale const& scale) -> Size<Dp>
{
    return Size<Dp>(ConvertVpToDp(sp.GetWidth(), scale), ConvertVpToDp(sp.GetHeight(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToDp(Point<Vp> const& sp, DisplayScale const& scale) -> Point<Dp>
{
    return Point<Dp>(ConvertVpToDp(sp.GetX(), scale), ConvertVpToDp(sp.GetY(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToDp(Rect<Vp> const& sp, DisplayScale const& scale) -> Rect<Dp>
{
    return Rect<Dp>(ConvertVpToDp(sp.GetLeft(), scale), ConvertVpToDp(sp.GetTop(), scale), ConvertVpToDp(sp.GetRight(), scale), ConvertVpToDp(sp.GetBottom(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVp(Dp const& dp, DisplayScale const& scale) -> Vp
{
    return Vp(static_cast<float64_t>(dp) * static_cast<float64_t>(scale));
}

///
/// @brief
///
/// @param dp
/// @param scale
///
/// @return
///
auto UnitFunction::ConvertDpToVp(Size<Dp> const& dp, DisplayScale const& scale) -> Size<Vp>
{
    return Size<Vp>(ConvertDpToVp(dp.GetWidth(), scale), ConvertDpToVp(dp.GetHeight(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVp(Point<Dp> const& dp, DisplayScale const& scale) -> Point<Vp>
{
    return Point<Vp>(ConvertDpToVp(dp.GetX(), scale), ConvertDpToVp(dp.GetY(), scale));
}

///
/// @brief
///
/// @param dp
/// @param scale
///
/// @return
///
auto UnitFunction::ConvertDpToVp(Rect<Dp> const& dp, DisplayScale const& scale) -> Rect<Vp>
{
    return Rect<Vp>(ConvertDpToVp(dp.GetLeft(), scale), ConvertDpToVp(dp.GetTop(), scale), ConvertDpToVp(dp.GetRight(), scale), ConvertDpToVp(dp.GetBottom(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param displayScale 
/// @param backingScale 
///
/// @return 
///
auto UnitFunction::ConvertDpToPx(Dp const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Px
{
    return ConvertVpToPx(ConvertDpToVp(dp, displayScale), backingScale);
}

///
/// @brief 
///
/// @param dp 
/// @param displayScale 
/// @param backingScale 
///
/// @return 
///
auto UnitFunction::ConvertDpToPx(Size<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Size<Px>
{
    return ConvertVpToPx(ConvertDpToVp(dp, displayScale), backingScale);
}

///
/// @brief 
///
/// @param dp 
/// @param displayScale 
/// @param backingScale 
///
/// @return 
///
auto UnitFunction::ConvertDpToPx(Point<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Point<Px>
{
    return ConvertVpToPx(ConvertDpToVp(dp, displayScale), backingScale);
}

///
/// @brief 
///
/// @param dp 
/// @param displayScale 
/// @param backingScale 
///
/// @return 
///
auto UnitFunction::ConvertDpToPx(Rect<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Rect<Px>
{
    return ConvertVpToPx(ConvertDpToVp(dp, displayScale), backingScale);
}

///
/// @brief 
///
/// @param px 
/// @param backingScale 
/// @param displayScale 
///
/// @return 
///
auto UnitFunction::ConvertPxToDp(Px const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Dp
{
    return ConvertVpToDp(ConvertPxToVp(px, backingScale), displayScale);
}

///
/// @brief 
///
/// @param px 
/// @param backingScale 
/// @param displayScale 
///
/// @return 
///
auto UnitFunction::ConvertPxToDp(Size<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Size<Dp>
{
    return ConvertVpToDp(ConvertPxToVp(px, backingScale), displayScale);
}

///
/// @brief 
///
/// @param px 
/// @param backingScale 
/// @param displayScale 
///
/// @return 
///
auto UnitFunction::ConvertPxToDp(Point<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Point<Dp>
{
    return ConvertVpToDp(ConvertPxToVp(px, backingScale), displayScale);
}

///
/// @brief 
///
/// @param px 
/// @param backingScale 
/// @param displayScale 
///
/// @return 
///
auto UnitFunction::ConvertPxToDp(Rect<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Rect<Dp>
{
    return ConvertVpToDp(ConvertPxToVp(px, backingScale), displayScale);
}

///
/// @brief
///
/// @param degree
///
/// @return
///
auto UnitFunction::ConvertDegreeToRadian(Degree const& degree) -> Radian
{
    return static_cast<float64_t>(degree) * std::numbers::pi_v<float64_t> / 180.0;
}

///
/// @brief
///
/// @param radian
///
/// @return
///
auto UnitFunction::ConvertRadianToDegree(Radian const& radian) -> Degree
{
    return static_cast<float64_t>(radian) * 180.0 / std::numbers::pi_v<float64_t>;
}
}
}
