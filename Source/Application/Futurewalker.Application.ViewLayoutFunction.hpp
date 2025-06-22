// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"
#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Layout related utility functions.
///
class ViewLayoutFunction : NonConstructible
{
public:
    static auto AlignToPixelGridByRound(Dp const length, View const& view) -> Dp;
    static auto AlignToPixelGridByRound(Point<Dp> const& point, View const& view) -> Point<Dp>;
    static auto AlignToPixelGridByRound(Size<Dp> const& size, View const& view) -> Size<Dp>;
    static auto AlignToPixelGridByRound(Rect<Dp> const& rect, View const& view) -> Rect<Dp>;

    static auto AlignToPixelGridByFloor(Dp const length, View const& view) -> Dp;
    static auto AlignToPixelGridByFloor(Point<Dp> const& point, View const& view) -> Point<Dp>;
    static auto AlignToPixelGridByFloor(Size<Dp> const& size, View const& view) -> Size<Dp>;
    static auto AlignToPixelGridByFloor(Rect<Dp> const& rect, View const& view) -> Rect<Dp>;

    static auto AlignToPixelGridByCeil(Dp const length, View const& view) -> Dp;
    static auto AlignToPixelGridByCeil(Point<Dp> const& point, View const& view) -> Point<Dp>;
    static auto AlignToPixelGridByCeil(Size<Dp> const& size, View const& view) -> Size<Dp>;
    static auto AlignToPixelGridByCeil(Rect<Dp> const& rect, View const& view) -> Rect<Dp>;
};
}
}
