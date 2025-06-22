// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ViewLayoutFunction.hpp"

#include "Futurewalker.Unit.hpp" 

namespace FW_DETAIL_NS
{
auto ViewLayoutFunction::AlignToPixelGridByRound(Dp const length, View const& view) -> Dp
{
    if (Dp::IsFinite(length))
    {
        auto const displayScale = view.GetDisplayScale();
        auto const backingScale = view.GetBackingScale();
        auto const pixelLength = UnitFunction::ConvertDpToPx(length, displayScale, backingScale);
        return UnitFunction::ConvertPxToDp(Px::Round(pixelLength), backingScale, displayScale);
    }
    return length;
}

auto ViewLayoutFunction::AlignToPixelGridByRound(Point<Dp> const& point, View const& view) -> Point<Dp>
{
    auto const x = AlignToPixelGridByRound(point.GetX(), view);
    auto const y = AlignToPixelGridByRound(point.GetY(), view);
    return Point<Dp>(x, y);
}

auto ViewLayoutFunction::AlignToPixelGridByRound(Size<Dp> const& size, View const& view) -> Size<Dp>
{
    auto const w = AlignToPixelGridByRound(size.GetWidth(), view);
    auto const h = AlignToPixelGridByRound(size.GetHeight(), view);
    return Size<Dp>(w, h);
}

auto ViewLayoutFunction::AlignToPixelGridByRound(Rect<Dp> const& rect, View const& view) -> Rect<Dp>
{
    auto const position = AlignToPixelGridByRound(rect.GetPosition(), view);
    auto const size = AlignToPixelGridByRound(rect.GetSize(), view);
    return Rect<Dp>(position, size);
}

auto ViewLayoutFunction::AlignToPixelGridByFloor(Dp const length, View const& view) -> Dp
{
    if (Dp::IsFinite(length))
    {
        auto const displayScale = view.GetDisplayScale();
        auto const backingScale = view.GetBackingScale();
        auto const pixelLength = UnitFunction::ConvertDpToPx(length, displayScale, backingScale);
        auto const pixelLengthRound = Px::Round(pixelLength);
        if (Px::IsNearlyEqual(pixelLength, pixelLengthRound))
        {
            return UnitFunction::ConvertPxToDp(pixelLengthRound, backingScale, displayScale);
        }
        return UnitFunction::ConvertPxToDp(Px::Floor(pixelLength), backingScale, displayScale);
    }
    return length;
}

auto ViewLayoutFunction::AlignToPixelGridByFloor(Point<Dp> const& point, View const& view) -> Point<Dp>
{
    auto const x = AlignToPixelGridByFloor(point.GetX(), view);
    auto const y = AlignToPixelGridByFloor(point.GetY(), view);
    return Point<Dp>(x, y);
}

auto ViewLayoutFunction::AlignToPixelGridByFloor(Size<Dp> const& size, View const& view) -> Size<Dp>
{
    auto const w = AlignToPixelGridByFloor(size.GetWidth(), view);
    auto const h = AlignToPixelGridByFloor(size.GetHeight(), view);
    return Size<Dp>(w, h);
}

auto ViewLayoutFunction::AlignToPixelGridByFloor(Rect<Dp> const& rect, View const& view) -> Rect<Dp>
{
    auto const position = AlignToPixelGridByFloor(rect.GetPosition(), view);
    auto const size = AlignToPixelGridByFloor(rect.GetSize(), view);
    return Rect<Dp>(position, size);
}

auto ViewLayoutFunction::AlignToPixelGridByCeil(Dp const length, View const& view) -> Dp
{
    if (Dp::IsFinite(length))
    {
        auto const displayScale = view.GetDisplayScale();
        auto const backingScale = view.GetBackingScale();
        auto const pixelLength = UnitFunction::ConvertDpToPx(length, displayScale, backingScale);
        auto const pixelLengthRound = Px::Round(pixelLength);
        if (Px::IsNearlyEqual(pixelLength, pixelLengthRound))
        {
            return UnitFunction::ConvertPxToDp(pixelLengthRound, backingScale, displayScale);
        }
        return UnitFunction::ConvertPxToDp(Px::Ceil(pixelLength), backingScale, displayScale);
    }
    return length;
}

auto ViewLayoutFunction::AlignToPixelGridByCeil(Point<Dp> const& point, View const& view) -> Point<Dp>
{
    auto const x = AlignToPixelGridByCeil(point.GetX(), view);
    auto const y = AlignToPixelGridByCeil(point.GetY(), view);
    return Point<Dp>(x, y);
}

auto ViewLayoutFunction::AlignToPixelGridByCeil(Size<Dp> const& size, View const& view) -> Size<Dp>
{
    auto const w = AlignToPixelGridByCeil(size.GetWidth(), view);
    auto const h = AlignToPixelGridByCeil(size.GetHeight(), view);
    return Size<Dp>(w, h);
}

auto ViewLayoutFunction::AlignToPixelGridByCeil(Rect<Dp> const& rect, View const& view) -> Rect<Dp>
{
    auto const position = AlignToPixelGridByCeil(rect.GetPosition(), view);
    auto const size = AlignToPixelGridByCeil(rect.GetSize(), view);
    return Rect<Dp>(position, size);
}
}
