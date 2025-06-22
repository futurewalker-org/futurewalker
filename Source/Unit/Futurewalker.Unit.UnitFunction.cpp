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
auto UnitFunction::ConvertPxToSp(Px const& dp, BackingScale const& scale) -> Sp
{
    return Sp(static_cast<float64_t>(dp) / static_cast<float64_t>(scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToSp(Size<Px> const& dp, BackingScale const& scale) -> Size<Sp>
{
    return Size<Sp>(ConvertPxToSp(dp.GetWidth(), scale), ConvertPxToSp(dp.GetHeight(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToSp(Point<Px> const& dp, BackingScale const& scale) -> Point<Sp>
{
    return Point<Sp>(ConvertPxToSp(dp.GetX(), scale), ConvertPxToSp(dp.GetY(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertPxToSp(Rect<Px> const& dp, BackingScale const& scale) -> Rect<Sp>
{
    return Rect<Sp>(ConvertPxToSp(dp.GetLeft(), scale), ConvertPxToSp(dp.GetTop(), scale), ConvertPxToSp(dp.GetRight(), scale), ConvertPxToSp(dp.GetBottom(), scale));
}

///
/// @brief 
///
/// @param sp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertSpToPx(Sp const& sp, BackingScale const& scale) -> Px
{
    return Px(static_cast<float64_t>(sp) * static_cast<float64_t>(scale));
}

///
/// @brief 
///
/// @param sp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertSpToPx(Size<Sp> const& sp, BackingScale const& scale) -> Size<Px>
{
    return Size<Px>(ConvertSpToPx(sp.GetWidth(), scale), ConvertSpToPx(sp.GetHeight(), scale));
}

///
/// @brief 
///
/// @param sp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertSpToPx(Point<Sp> const& sp, BackingScale const& scale) -> Point<Px>
{
    return Point<Px>(ConvertSpToPx(sp.GetX(), scale), ConvertSpToPx(sp.GetY(), scale));
}

///
/// @brief 
///
/// @param sp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertSpToPx(Rect<Sp> const& sp, BackingScale const& scale) -> Rect<Px>
{
    return Rect<Px>(ConvertSpToPx(sp.GetTopLeft(), scale), ConvertSpToPx(sp.GetSize(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertSpToDp(Sp const& sp, DisplayScale const& scale) -> Dp
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
auto UnitFunction::ConvertSpToDp(Size<Sp> const& sp, DisplayScale const& scale) -> Size<Dp>
{
    return Size<Dp>(ConvertSpToDp(sp.GetWidth(), scale), ConvertSpToDp(sp.GetHeight(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertSpToDp(Point<Sp> const& sp, DisplayScale const& scale) -> Point<Dp>
{
    return Point<Dp>(ConvertSpToDp(sp.GetX(), scale), ConvertSpToDp(sp.GetY(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertSpToDp(Rect<Sp> const& sp, DisplayScale const& scale) -> Rect<Dp>
{
    return Rect<Dp>(ConvertSpToDp(sp.GetLeft(), scale), ConvertSpToDp(sp.GetTop(), scale), ConvertSpToDp(sp.GetRight(), scale), ConvertSpToDp(sp.GetBottom(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToSp(Dp const& dp, DisplayScale const& scale) -> Sp
{
    return Sp(static_cast<float64_t>(dp) * static_cast<float64_t>(scale));
}

///
/// @brief
///
/// @param dp
/// @param scale
///
/// @return
///
auto UnitFunction::ConvertDpToSp(Size<Dp> const& dp, DisplayScale const& scale) -> Size<Sp>
{
    return Size<Sp>(ConvertDpToSp(dp.GetWidth(), scale), ConvertDpToSp(dp.GetHeight(), scale));
}

///
/// @brief 
///
/// @param dp 
/// @param scale 
///
/// @return 
///
auto UnitFunction::ConvertDpToSp(Point<Dp> const& dp, DisplayScale const& scale) -> Point<Sp>
{
    return Point<Sp>(ConvertDpToSp(dp.GetX(), scale), ConvertDpToSp(dp.GetY(), scale));
}

///
/// @brief
///
/// @param dp
/// @param scale
///
/// @return
///
auto UnitFunction::ConvertDpToSp(Rect<Dp> const& dp, DisplayScale const& scale) -> Rect<Sp>
{
    return Rect<Sp>(ConvertDpToSp(dp.GetLeft(), scale), ConvertDpToSp(dp.GetTop(), scale), ConvertDpToSp(dp.GetRight(), scale), ConvertDpToSp(dp.GetBottom(), scale));
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
    return ConvertSpToPx(ConvertDpToSp(dp, displayScale), backingScale);
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
    return ConvertSpToPx(ConvertDpToSp(dp, displayScale), backingScale);
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
    return ConvertSpToPx(ConvertDpToSp(dp, displayScale), backingScale);
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
    return ConvertSpToPx(ConvertDpToSp(dp, displayScale), backingScale);
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
    return ConvertSpToDp(ConvertPxToSp(px, backingScale), displayScale);
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
    return ConvertSpToDp(ConvertPxToSp(px, backingScale), displayScale);
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
    return ConvertSpToDp(ConvertPxToSp(px, backingScale), displayScale);
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
    return ConvertSpToDp(ConvertPxToSp(px, backingScale), displayScale);
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
