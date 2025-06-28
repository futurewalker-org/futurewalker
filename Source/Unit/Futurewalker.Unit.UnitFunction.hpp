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

    static auto ConvertVpToPx(Vp const& vp, BackingScale const& scale) -> Px;
    static auto ConvertVpToPx(Size<Vp> const& vp, BackingScale const& scale) -> Size<Px>;
    static auto ConvertVpToPx(Point<Vp> const& vp, BackingScale const& scale) -> Point<Px>;
    static auto ConvertVpToPx(Rect<Vp> const& vp, BackingScale const& scale) -> Rect<Px>;

    static auto ConvertVpToDp(Vp const& vp, DisplayScale const& scale) -> Dp;
    static auto ConvertVpToDp(Size<Vp> const& vp, DisplayScale const& scale) -> Size<Dp>;
    static auto ConvertVpToDp(Point<Vp> const& vp, DisplayScale const& scale) -> Point<Dp>;
    static auto ConvertVpToDp(Rect<Vp> const& vp, DisplayScale const& scale) -> Rect<Dp>;

    static auto ConvertDpToVp(Dp const& dp, DisplayScale const& scale) -> Vp;
    static auto ConvertDpToVp(Size<Dp> const& dp, DisplayScale const& scale) -> Size<Vp>;
    static auto ConvertDpToVp(Point<Dp> const& dp, DisplayScale const& scale) -> Point<Vp>;
    static auto ConvertDpToVp(Rect<Dp> const& dp, DisplayScale const& scale) -> Rect<Vp>;

    static auto ConvertDpToPx(Dp const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Px;
    static auto ConvertDpToPx(Size<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Size<Px>;
    static auto ConvertDpToPx(Point<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Point<Px>;
    static auto ConvertDpToPx(Rect<Dp> const& dp, DisplayScale const& displayScale, BackingScale const& backingScale) -> Rect<Px>;

    static auto ConvertPxToDp(Px const& px,       BackingScale const& backingScale, DisplayScale const& displayScale) -> Dp;
    static auto ConvertPxToDp(Size<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Size<Dp>;
    static auto ConvertPxToDp(Point<Px> const& px,BackingScale const& backingScale, DisplayScale const& displayScale) -> Point<Dp>;
    static auto ConvertPxToDp(Rect<Px> const& px, BackingScale const& backingScale, DisplayScale const& displayScale) -> Rect<Dp>;

    static auto ConvertDegreeToRadian(Degree const& degree) -> Radian;
    static auto ConvertRadianToDegree(Radian const& radian) -> Degree;
};
}
}
