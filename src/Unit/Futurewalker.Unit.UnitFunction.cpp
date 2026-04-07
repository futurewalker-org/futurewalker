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
    return Size<Vp>(ConvertPxToVp(dp.width, scale), ConvertPxToVp(dp.height, scale));
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
    return Point<Vp>(ConvertPxToVp(dp.x, scale), ConvertPxToVp(dp.y, scale));
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
    return Rect<Vp>(ConvertPxToVp(dp.x0, scale), ConvertPxToVp(dp.y0, scale), ConvertPxToVp(dp.x1, scale), ConvertPxToVp(dp.y1, scale));
}

///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpRound(Px const& px, BackingScale const& scale) -> Vp
{
    return Vp::Round(ConvertPxToVp(px, scale));
}
///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpRound(Size<Px> const& px, BackingScale const& scale) -> Size<Vp>
{
    return {ConvertPxToVpRound(px.width, scale), ConvertPxToVpRound(px.height, scale)};
}
///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpRound(Point<Px> const& px, BackingScale const& scale) -> Point<Vp>
{
    return {ConvertPxToVpRound(px.x, scale), ConvertPxToVpRound(px.y, scale)};
}
///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpRound(Rect<Px> const& px, BackingScale const& scale) -> Rect<Vp>
{
    return {ConvertPxToVpRound(px.x0, scale), ConvertPxToVpRound(px.y0, scale), ConvertPxToVpRound(px.x1, scale), ConvertPxToVpRound(px.y1, scale)};
}

///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpCeil(Px const& px, BackingScale const& scale) -> Vp
{
    return Vp::Ceil(ConvertPxToVp(px, scale));
}
///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpCeil(Size<Px> const& px, BackingScale const& scale) -> Size<Vp>
{
    return {ConvertPxToVpCeil(px.width, scale), ConvertPxToVpCeil(px.height, scale)};
}
///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpCeil(Point<Px> const& px, BackingScale const& scale) -> Point<Vp>
{
    return {ConvertPxToVpCeil(px.x, scale), ConvertPxToVpCeil(px.y, scale)};
}
///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpCeil(Rect<Px> const& px, BackingScale const& scale) -> Rect<Vp>
{
    return {ConvertPxToVpCeil(px.x0, scale), ConvertPxToVpCeil(px.y0, scale), ConvertPxToVpCeil(px.x1, scale), ConvertPxToVpCeil(px.y1, scale)};
}

///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpFloor(Px const& px, BackingScale const& scale) -> Vp
{
    return Vp::Floor(ConvertPxToVp(px, scale));
}
///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpFloor(Size<Px> const& px, BackingScale const& scale) -> Size<Vp>
{
    return {ConvertPxToVpFloor(px.width, scale), ConvertPxToVpFloor(px.height, scale)};
}
///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpFloor(Point<Px> const& px, BackingScale const& scale) -> Point<Vp>
{
    return {ConvertPxToVpFloor(px.x, scale), ConvertPxToVpFloor(px.y, scale)};
}
///
/// @brief 
///
/// @param px 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToVpFloor(Rect<Px> const& px, BackingScale const& scale) -> Rect<Vp>
{
    return {ConvertPxToVpFloor(px.x0, scale), ConvertPxToVpFloor(px.y0, scale), ConvertPxToVpFloor(px.x1, scale), ConvertPxToVpFloor(px.y1, scale)};
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
    return Size<Px>(ConvertVpToPx(sp.width, scale), ConvertVpToPx(sp.height, scale));
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
    return Point<Px>(ConvertVpToPx(sp.x, scale), ConvertVpToPx(sp.y, scale));
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
    return Rect<Px>::Make(ConvertVpToPx(sp.GetCorner<0, 0>(), scale), ConvertVpToPx(sp.GetSize(), scale));
}

///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToRound(Vp const& vp, BackingScale const& scale) -> Px
{
    return Px::Round(ConvertVpToPx(vp, scale));
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToRound(Size<Vp> const& vp, BackingScale const& scale) -> Size<Px>
{
    return {ConvertVpToRound(vp.width, scale), ConvertVpToRound(vp.height, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToRound(Point<Vp> const& vp, BackingScale const& scale) -> Point<Px>
{
    return {ConvertVpToRound(vp.x, scale), ConvertVpToRound(vp.y, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToRound(Rect<Vp> const& vp, BackingScale const& scale) -> Rect<Px>
{
    return {ConvertVpToRound(vp.x0, scale), ConvertVpToRound(vp.y0, scale), ConvertVpToRound(vp.x1, scale), ConvertVpToRound(vp.y1, scale)};
}

///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToCeil(Vp const& vp, BackingScale const& scale) -> Px
{
    return Px::Ceil(ConvertVpToPx(vp, scale));
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToCeil(Size<Vp> const& vp, BackingScale const& scale) -> Size<Px>
{
    return {ConvertVpToCeil(vp.width, scale), ConvertVpToCeil(vp.height, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToCeil(Point<Vp> const& vp, BackingScale const& scale) -> Point<Px>
{
    return {ConvertVpToCeil(vp.x, scale), ConvertVpToCeil(vp.y, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToCeil(Rect<Vp> const& vp, BackingScale const& scale) -> Rect<Px>
{
    return {ConvertVpToCeil(vp.x0, scale), ConvertVpToCeil(vp.y0, scale), ConvertVpToCeil(vp.x1, scale), ConvertVpToCeil(vp.y1, scale)};
}

///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToFloor(Vp const& vp, BackingScale const& scale) -> Px
{
    return Px::Floor(ConvertVpToPx(vp, scale));
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToFloor(Size<Vp> const& vp, BackingScale const& scale) -> Size<Px>
{
    return {ConvertVpToFloor(vp.width, scale), ConvertVpToFloor(vp.height, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToFloor(Point<Vp> const& vp, BackingScale const& scale) -> Point<Px>
{
    return {ConvertVpToFloor(vp.x, scale), ConvertVpToFloor(vp.y, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToFloor(Rect<Vp> const& vp, BackingScale const& scale) -> Rect<Px>
{
    return {ConvertVpToFloor(vp.x0, scale), ConvertVpToFloor(vp.y0, scale), ConvertVpToFloor(vp.x1, scale), ConvertVpToFloor(vp.y1, scale)};
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
    return Size<Dp>(ConvertVpToDp(sp.width, scale), ConvertVpToDp(sp.height, scale));
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
    return Point<Dp>(ConvertVpToDp(sp.x, scale), ConvertVpToDp(sp.y, scale));
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
    return Rect<Dp>(ConvertVpToDp(sp.x0, scale), ConvertVpToDp(sp.y0, scale), ConvertVpToDp(sp.x1, scale), ConvertVpToDp(sp.y1, scale));
}

///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToRound(Vp const& vp, DisplayScale const& scale) -> Dp
{
    return Dp::Round(ConvertVpToDp(vp, scale));
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToRound(Size<Vp> const& vp, DisplayScale const& scale) -> Size<Dp>
{
    return {ConvertVpToRound(vp.width, scale), ConvertVpToRound(vp.height, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToRound(Point<Vp> const& vp, DisplayScale const& scale) -> Point<Dp>
{
    return {ConvertVpToRound(vp.x, scale), ConvertVpToRound(vp.y, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToRound(Rect<Vp> const& vp, DisplayScale const& scale) -> Rect<Dp>
{
    return {ConvertVpToRound(vp.x0, scale), ConvertVpToRound(vp.y0, scale), ConvertVpToRound(vp.x1, scale), ConvertVpToRound(vp.y1, scale)};
}

///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToCeil(Vp const& vp, DisplayScale const& scale) -> Dp
{
    return Dp::Ceil(ConvertVpToDp(vp, scale));
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToCeil(Size<Vp> const& vp, DisplayScale const& scale) -> Size<Dp>
{
    return {ConvertVpToCeil(vp.width, scale), ConvertVpToCeil(vp.height, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToCeil(Point<Vp> const& vp, DisplayScale const& scale) -> Point<Dp>
{
    return {ConvertVpToCeil(vp.x, scale), ConvertVpToCeil(vp.y, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToCeil(Rect<Vp> const& vp, DisplayScale const& scale) -> Rect<Dp>
{
    return {ConvertVpToCeil(vp.x0, scale), ConvertVpToCeil(vp.y0, scale), ConvertVpToCeil(vp.x1, scale), ConvertVpToCeil(vp.y1, scale)};
}

///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToFloor(Vp const& vp, DisplayScale const& scale) -> Dp
{
    return Dp::Floor(ConvertVpToDp(vp, scale));
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToFloor(Size<Vp> const& vp, DisplayScale const& scale) -> Size<Dp>
{
    return {ConvertVpToFloor(vp.width, scale), ConvertVpToFloor(vp.height, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToFloor(Point<Vp> const& vp, DisplayScale const& scale) -> Point<Dp>
{
    return {ConvertVpToFloor(vp.x, scale), ConvertVpToFloor(vp.y, scale)};
}
///
/// @brief 
///
/// @param vp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertVpToFloor(Rect<Vp> const& vp, DisplayScale const& scale) -> Rect<Dp>
{
    return {ConvertVpToFloor(vp.x0, scale), ConvertVpToFloor(vp.y0, scale), ConvertVpToFloor(vp.x1, scale), ConvertVpToFloor(vp.y1, scale)};
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
    return Size<Vp>(ConvertDpToVp(dp.width, scale), ConvertDpToVp(dp.height, scale));
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
    return Point<Vp>(ConvertDpToVp(dp.x, scale), ConvertDpToVp(dp.y, scale));
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
    return Rect<Vp>(ConvertDpToVp(dp.x0, scale), ConvertDpToVp(dp.y0, scale), ConvertDpToVp(dp.x1, scale), ConvertDpToVp(dp.y1, scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpRound(Dp const& dp, DisplayScale const& scale) -> Vp
{
    return Vp::Round(ConvertDpToVp(dp, scale));
}
///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpRound(Size<Dp> const& dp, DisplayScale const& scale) -> Size<Vp>
{
    return {ConvertDpToVpRound(dp.width, scale), ConvertDpToVpRound(dp.height, scale)};
}
///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpRound(Point<Dp> const& dp, DisplayScale const& scale) -> Point<Vp>
{
    return {ConvertDpToVpRound(dp.x, scale), ConvertDpToVpRound(dp.y, scale)};
}
///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpRound(Rect<Dp> const& dp, DisplayScale const& scale) -> Rect<Vp>
{
    return {ConvertDpToVpRound(dp.x0, scale), ConvertDpToVpRound(dp.y0, scale), ConvertDpToVpRound(dp.x1, scale), ConvertDpToVpRound(dp.y1, scale)};
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpCeil(Dp const& dp, DisplayScale const& scale) -> Vp
{
    return Vp::Ceil(ConvertDpToVp(dp, scale));
}
///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpCeil(Size<Dp> const& dp, DisplayScale const& scale) -> Size<Vp>
{
    return {ConvertDpToVpCeil(dp.width, scale), ConvertDpToVpCeil(dp.height, scale)};
}
///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpCeil(Point<Dp> const& dp, DisplayScale const& scale) -> Point<Vp>
{
    return {ConvertDpToVpCeil(dp.x, scale), ConvertDpToVpCeil(dp.y, scale)};
}
///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpCeil(Rect<Dp> const& dp, DisplayScale const& scale) -> Rect<Vp>
{
    return {ConvertDpToVpCeil(dp.x0, scale), ConvertDpToVpCeil(dp.y0, scale), ConvertDpToVpCeil(dp.x1, scale), ConvertDpToVpCeil(dp.y1, scale)};
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpFloor(Dp const& dp, DisplayScale const& scale) -> Vp
{
    return Vp::Floor(ConvertDpToVp(dp, scale));
}
///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpFloor(Size<Dp> const& dp, DisplayScale const& scale) -> Size<Vp>
{
    return {ConvertDpToVpFloor(dp.width, scale), ConvertDpToVpFloor(dp.height, scale)};
}
///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpFloor(Point<Dp> const& dp, DisplayScale const& scale) -> Point<Vp>
{
    return {ConvertDpToVpFloor(dp.x, scale), ConvertDpToVpFloor(dp.y, scale)};
}
///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToVpFloor(Rect<Dp> const& dp, DisplayScale const& scale) -> Rect<Vp>
{
    return {ConvertDpToVpFloor(dp.x0, scale), ConvertDpToVpFloor(dp.y0, scale), ConvertDpToVpFloor(dp.x1, scale), ConvertDpToVpFloor(dp.y1, scale)};
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
/// @param dp 
/// @param displayScale 
/// @param backingScale 
///
/// @return 
///
auto UnitFunction::ConvertDpToPxRound(Dp const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Px
{
    return Px::Round(ConvertDpToPx(dp, displayScale, backingScale));
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
auto UnitFunction::ConvertDpToPxRound(Size<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Size<Px>
{
    return {ConvertDpToPxRound(dp.width, displayScale, backingScale), ConvertDpToPxRound(dp.height, displayScale, backingScale)};
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
auto UnitFunction::ConvertDpToPxRound(Point<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Point<Px>
{
    return {ConvertDpToPxRound(dp.x, displayScale, backingScale), ConvertDpToPxRound(dp.y, displayScale, backingScale)};
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
auto UnitFunction::ConvertDpToPxRound(Rect<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Rect<Px>
{
    return {ConvertDpToPxRound(dp.x0, displayScale, backingScale), ConvertDpToPxRound(dp.y0, displayScale, backingScale), ConvertDpToPxRound(dp.x1, displayScale, backingScale), ConvertDpToPxRound(dp.y1, displayScale, backingScale)};
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
auto UnitFunction::ConvertDpToPxCeil(Dp const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Px
{
    return Px::Ceil(ConvertDpToPx(dp, displayScale, backingScale));
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
auto UnitFunction::ConvertDpToPxCeil(Size<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Size<Px>
{
    return {ConvertDpToPxCeil(dp.width, displayScale, backingScale), ConvertDpToPxCeil(dp.height, displayScale, backingScale)};
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
auto UnitFunction::ConvertDpToPxCeil(Point<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Point<Px>
{
    return {ConvertDpToPxCeil(dp.x, displayScale, backingScale), ConvertDpToPxCeil(dp.y, displayScale, backingScale)};
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
auto UnitFunction::ConvertDpToPxCeil(Rect<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Rect<Px>
{
    return {ConvertDpToPxCeil(dp.x0, displayScale, backingScale), ConvertDpToPxCeil(dp.y0, displayScale, backingScale), ConvertDpToPxCeil(dp.x1, displayScale, backingScale), ConvertDpToPxCeil(dp.y1, displayScale, backingScale)};
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
auto UnitFunction::ConvertDpToPxFloor(Dp const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Px
{
    return Px::Floor(ConvertDpToPx(dp, displayScale, backingScale));
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
auto UnitFunction::ConvertDpToPxFloor(Size<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Size<Px>
{
    return {ConvertDpToPxFloor(dp.width, displayScale, backingScale), ConvertDpToPxFloor(dp.height, displayScale, backingScale)};
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
auto UnitFunction::ConvertDpToPxFloor(Point<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Point<Px>
{
    return {ConvertDpToPxFloor(dp.x, displayScale, backingScale), ConvertDpToPxFloor(dp.y, displayScale, backingScale)};
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
auto UnitFunction::ConvertDpToPxFloor(Rect<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Rect<Px>
{
    return {ConvertDpToPxFloor(dp.x0, displayScale, backingScale), ConvertDpToPxFloor(dp.y0, displayScale, backingScale), ConvertDpToPxFloor(dp.x1, displayScale, backingScale), ConvertDpToPxFloor(dp.y1, displayScale, backingScale)};
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
/// @param px 
/// @param backingScale 
/// @param displayScale 
///
/// @return 
///
auto UnitFunction::ConvertPxToRound(Px const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Dp
{
    return Dp::Round(ConvertPxToDp(px, backingScale, displayScale));
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
auto UnitFunction::ConvertPxToRound(Size<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Size<Dp>
{
    return {ConvertPxToRound(px.width, backingScale, displayScale), ConvertPxToRound(px.height, backingScale, displayScale)};
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
auto UnitFunction::ConvertPxToRound(Point<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Point<Dp>
{
    return {ConvertPxToRound(px.x, backingScale, displayScale), ConvertPxToRound(px.y, backingScale, displayScale)};
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
auto UnitFunction::ConvertPxToRound(Rect<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Rect<Dp>
{
    return {ConvertPxToRound(px.x0, backingScale, displayScale), ConvertPxToRound(px.y0, backingScale, displayScale), ConvertPxToRound(px.x1, backingScale, displayScale), ConvertPxToRound(px.y1, backingScale, displayScale)};
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
auto UnitFunction::ConvertPxToCeil(Px const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Dp
{
    return Dp::Ceil(ConvertPxToDp(px, backingScale, displayScale));
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
auto UnitFunction::ConvertPxToCeil(Size<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Size<Dp>
{
    return {ConvertPxToCeil(px.width, backingScale, displayScale), ConvertPxToCeil(px.height, backingScale, displayScale)};
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
auto UnitFunction::ConvertPxToCeil(Point<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Point<Dp>
{
    return {ConvertPxToCeil(px.x, backingScale, displayScale), ConvertPxToCeil(px.y, backingScale, displayScale)};
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
auto UnitFunction::ConvertPxToCeil(Rect<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Rect<Dp>
{
    return {ConvertPxToCeil(px.x0, backingScale, displayScale), ConvertPxToCeil(px.y0, backingScale, displayScale), ConvertPxToCeil(px.x1, backingScale, displayScale), ConvertPxToCeil(px.y1, backingScale, displayScale)};
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
auto UnitFunction::ConvertPxToFloor(Px const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Dp
{
    return Dp::Floor(ConvertPxToDp(px, backingScale, displayScale));
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
auto UnitFunction::ConvertPxToFloor(Size<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Size<Dp>
{
    return {ConvertPxToFloor(px.width, backingScale, displayScale), ConvertPxToFloor(px.height, backingScale, displayScale)};
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
auto UnitFunction::ConvertPxToFloor(Point<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Point<Dp>
{
    return {ConvertPxToFloor(px.x, backingScale, displayScale), ConvertPxToFloor(px.y, backingScale, displayScale)};
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
auto UnitFunction::ConvertPxToFloor(Rect<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Rect<Dp>
{
    return {ConvertPxToFloor(px.x0, backingScale, displayScale), ConvertPxToFloor(px.y0, backingScale, displayScale), ConvertPxToFloor(px.x1, backingScale, displayScale), ConvertPxToFloor(px.y1, backingScale, displayScale)};
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
