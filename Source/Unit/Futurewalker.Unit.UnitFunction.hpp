// SPDX-License-Identifier: MIT
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
    static auto ConvertPxToSp(Px const& dp, BackingScale const& scale) -> Sp;
    static auto ConvertPxToSp(Size<Px> const& dp, BackingScale const& scale) -> Size<Sp>;
    static auto ConvertPxToSp(Point<Px> const& dp, BackingScale const& scale) -> Point<Sp>;
    static auto ConvertPxToSp(Rect<Px> const& dp, BackingScale const& scale) -> Rect<Sp>;

    static auto ConvertSpToPx(Sp const& sp, BackingScale const& scale) -> Px;
    static auto ConvertSpToPx(Size<Sp> const& sp, BackingScale const& scale) -> Size<Px>;
    static auto ConvertSpToPx(Point<Sp> const& sp, BackingScale const& scale) -> Point<Px>;
    static auto ConvertSpToPx(Rect<Sp> const& sp, BackingScale const& scale) -> Rect<Px>;

    static auto ConvertSpToDp(Sp const& sp, DisplayScale const& scale) -> Dp;
    static auto ConvertSpToDp(Size<Sp> const& sp, DisplayScale const& scale) -> Size<Dp>;
    static auto ConvertSpToDp(Point<Sp> const& sp, DisplayScale const& scale) -> Point<Dp>;
    static auto ConvertSpToDp(Rect<Sp> const& sp, DisplayScale const& scale) -> Rect<Dp>;

    static auto ConvertDpToSp(Dp const& dp, DisplayScale const& scale) -> Sp;
    static auto ConvertDpToSp(Size<Dp> const& dp, DisplayScale const& scale) -> Size<Sp>;
    static auto ConvertDpToSp(Point<Dp> const& dp, DisplayScale const& scale) -> Point<Sp>;
    static auto ConvertDpToSp(Rect<Dp> const& dp, DisplayScale const& scale) -> Rect<Sp>;

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
