// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Unit.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Unit related utility functions.
///
class UnitFunction : NonConstructible
{
public:
    static auto ConvertPxToVp(Px const& dp, BackingScale const& scale) -> Vp;
    static auto ConvertPxToVp(Size<Px> const& dp, BackingScale const& scale) -> Size<Vp>;
    static auto ConvertPxToVp(Point<Px> const& dp, BackingScale const& scale) -> Point<Vp>;
    static auto ConvertPxToVp(Rect<Px> const& dp, BackingScale const& scale) -> Rect<Vp>;

    static auto ConvertPxToVpRound(Px const& dp, BackingScale const& scale) -> Vp;
    static auto ConvertPxToVpRound(Size<Px> const& dp, BackingScale const& scale) -> Size<Vp>;
    static auto ConvertPxToVpRound(Point<Px> const& dp, BackingScale const& scale) -> Point<Vp>;
    static auto ConvertPxToVpRound(Rect<Px> const& dp, BackingScale const& scale) -> Rect<Vp>;

    static auto ConvertPxToVpCeil(Px const& dp, BackingScale const& scale) -> Vp;
    static auto ConvertPxToVpCeil(Size<Px> const& dp, BackingScale const& scale) -> Size<Vp>;
    static auto ConvertPxToVpCeil(Point<Px> const& dp, BackingScale const& scale) -> Point<Vp>;
    static auto ConvertPxToVpCeil(Rect<Px> const& dp, BackingScale const& scale) -> Rect<Vp>;

    static auto ConvertPxToVpFloor(Px const& dp, BackingScale const& scale) -> Vp;
    static auto ConvertPxToVpFloor(Size<Px> const& dp, BackingScale const& scale) -> Size<Vp>;
    static auto ConvertPxToVpFloor(Point<Px> const& dp, BackingScale const& scale) -> Point<Vp>;
    static auto ConvertPxToVpFloor(Rect<Px> const& dp, BackingScale const& scale) -> Rect<Vp>;

    static auto ConvertVpToPx(Vp const& vp, BackingScale const& scale) -> Px;
    static auto ConvertVpToPx(Size<Vp> const& vp, BackingScale const& scale) -> Size<Px>;
    static auto ConvertVpToPx(Point<Vp> const& vp, BackingScale const& scale) -> Point<Px>;
    static auto ConvertVpToPx(Rect<Vp> const& vp, BackingScale const& scale) -> Rect<Px>;

    static auto ConvertVpToRound(Vp const& vp, BackingScale const& scale) -> Px;
    static auto ConvertVpToRound(Size<Vp> const& vp, BackingScale const& scale) -> Size<Px>;
    static auto ConvertVpToRound(Point<Vp> const& vp, BackingScale const& scale) -> Point<Px>;
    static auto ConvertVpToRound(Rect<Vp> const& vp, BackingScale const& scale) -> Rect<Px>;

    static auto ConvertVpToCeil(Vp const& vp, BackingScale const& scale) -> Px;
    static auto ConvertVpToCeil(Size<Vp> const& vp, BackingScale const& scale) -> Size<Px>;
    static auto ConvertVpToCeil(Point<Vp> const& vp, BackingScale const& scale) -> Point<Px>;
    static auto ConvertVpToCeil(Rect<Vp> const& vp, BackingScale const& scale) -> Rect<Px>;

    static auto ConvertVpToFloor(Vp const& vp, BackingScale const& scale) -> Px;
    static auto ConvertVpToFloor(Size<Vp> const& vp, BackingScale const& scale) -> Size<Px>;
    static auto ConvertVpToFloor(Point<Vp> const& vp, BackingScale const& scale) -> Point<Px>;
    static auto ConvertVpToFloor(Rect<Vp> const& vp, BackingScale const& scale) -> Rect<Px>;

    static auto ConvertVpToDp(Vp const& vp, DisplayScale const& scale) -> Dp;
    static auto ConvertVpToDp(Size<Vp> const& vp, DisplayScale const& scale) -> Size<Dp>;
    static auto ConvertVpToDp(Point<Vp> const& vp, DisplayScale const& scale) -> Point<Dp>;
    static auto ConvertVpToDp(Rect<Vp> const& vp, DisplayScale const& scale) -> Rect<Dp>;

    static auto ConvertVpToRound(Vp const& vp, DisplayScale const& scale) -> Dp;
    static auto ConvertVpToRound(Size<Vp> const& vp, DisplayScale const& scale) -> Size<Dp>;
    static auto ConvertVpToRound(Point<Vp> const& vp, DisplayScale const& scale) -> Point<Dp>;
    static auto ConvertVpToRound(Rect<Vp> const& vp, DisplayScale const& scale) -> Rect<Dp>;

    static auto ConvertVpToCeil(Vp const& vp, DisplayScale const& scale) -> Dp;
    static auto ConvertVpToCeil(Size<Vp> const& vp, DisplayScale const& scale) -> Size<Dp>;
    static auto ConvertVpToCeil(Point<Vp> const& vp, DisplayScale const& scale) -> Point<Dp>;
    static auto ConvertVpToCeil(Rect<Vp> const& vp, DisplayScale const& scale) -> Rect<Dp>;

    static auto ConvertVpToFloor(Vp const& vp, DisplayScale const& scale) -> Dp;
    static auto ConvertVpToFloor(Size<Vp> const& vp, DisplayScale const& scale) -> Size<Dp>;
    static auto ConvertVpToFloor(Point<Vp> const& vp, DisplayScale const& scale) -> Point<Dp>;
    static auto ConvertVpToFloor(Rect<Vp> const& vp, DisplayScale const& scale) -> Rect<Dp>;

    static auto ConvertDpToVp(Dp const& dp, DisplayScale const& scale) -> Vp;
    static auto ConvertDpToVp(Size<Dp> const& dp, DisplayScale const& scale) -> Size<Vp>;
    static auto ConvertDpToVp(Point<Dp> const& dp, DisplayScale const& scale) -> Point<Vp>;
    static auto ConvertDpToVp(Rect<Dp> const& dp, DisplayScale const& scale) -> Rect<Vp>;

    static auto ConvertDpToVpRound(Dp const& dp, DisplayScale const& scale) -> Vp;
    static auto ConvertDpToVpRound(Size<Dp> const& dp, DisplayScale const& scale) -> Size<Vp>;
    static auto ConvertDpToVpRound(Point<Dp> const& dp, DisplayScale const& scale) -> Point<Vp>;
    static auto ConvertDpToVpRound(Rect<Dp> const& dp, DisplayScale const& scale) -> Rect<Vp>;

    static auto ConvertDpToVpCeil(Dp const& dp, DisplayScale const& scale) -> Vp;
    static auto ConvertDpToVpCeil(Size<Dp> const& dp, DisplayScale const& scale) -> Size<Vp>;
    static auto ConvertDpToVpCeil(Point<Dp> const& dp, DisplayScale const& scale) -> Point<Vp>;
    static auto ConvertDpToVpCeil(Rect<Dp> const& dp, DisplayScale const& scale) -> Rect<Vp>;

    static auto ConvertDpToVpFloor(Dp const& dp, DisplayScale const& scale) -> Vp;
    static auto ConvertDpToVpFloor(Size<Dp> const& dp, DisplayScale const& scale) -> Size<Vp>;
    static auto ConvertDpToVpFloor(Point<Dp> const& dp, DisplayScale const& scale) -> Point<Vp>;
    static auto ConvertDpToVpFloor(Rect<Dp> const& dp, DisplayScale const& scale) -> Rect<Vp>;

    static auto ConvertDpToPx(Dp const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Px;
    static auto ConvertDpToPx(Size<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Size<Px>;
    static auto ConvertDpToPx(Point<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Point<Px>;
    static auto ConvertDpToPx(Rect<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Rect<Px>;

    static auto ConvertDpToPxRound(Dp const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Px;
    static auto ConvertDpToPxRound(Size<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Size<Px>;
    static auto ConvertDpToPxRound(Point<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Point<Px>;
    static auto ConvertDpToPxRound(Rect<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Rect<Px>;

    static auto ConvertDpToPxCeil(Dp const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Px;
    static auto ConvertDpToPxCeil(Size<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Size<Px>;
    static auto ConvertDpToPxCeil(Point<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Point<Px>;
    static auto ConvertDpToPxCeil(Rect<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Rect<Px>;

    static auto ConvertDpToPxFloor(Dp const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Px;
    static auto ConvertDpToPxFloor(Size<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Size<Px>;
    static auto ConvertDpToPxFloor(Point<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Point<Px>;
    static auto ConvertDpToPxFloor(Rect<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Rect<Px>;

    static auto ConvertPxToDp(Px const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Dp;
    static auto ConvertPxToDp(Size<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Size<Dp>;
    static auto ConvertPxToDp(Point<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Point<Dp>;
    static auto ConvertPxToDp(Rect<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Rect<Dp>;

    static auto ConvertPxToRound(Px const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Dp;
    static auto ConvertPxToRound(Size<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Size<Dp>;
    static auto ConvertPxToRound(Point<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Point<Dp>;
    static auto ConvertPxToRound(Rect<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Rect<Dp>;

    static auto ConvertPxToCeil(Px const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Dp;
    static auto ConvertPxToCeil(Size<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Size<Dp>;
    static auto ConvertPxToCeil(Point<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Point<Dp>;
    static auto ConvertPxToCeil(Rect<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Rect<Dp>;

    static auto ConvertPxToFloor(Px const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Dp;
    static auto ConvertPxToFloor(Size<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Size<Dp>;
    static auto ConvertPxToFloor(Point<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Point<Dp>;
    static auto ConvertPxToFloor(Rect<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Rect<Dp>;

    static auto ConvertDegreeToRadian(Degree const& degree) -> Radian;
    static auto ConvertRadianToDegree(Radian const& radian) -> Degree;
};
}
}
