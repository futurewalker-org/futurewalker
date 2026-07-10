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
    auto const x = AlignToPixelGridByRound(point.x, view);
    auto const y = AlignToPixelGridByRound(point.y, view);
    return Point<Dp>(x, y);
}

auto ViewLayoutFunction::AlignToPixelGridByRound(Size<Dp> const& size, View const& view) -> Size<Dp>
{
    auto const w = AlignToPixelGridByRound(size.width, view);
    auto const h = AlignToPixelGridByRound(size.height, view);
    return Size<Dp>(w, h);
}

auto ViewLayoutFunction::AlignToPixelGridByRound(Rect<Dp> const& rect, View const& view) -> Rect<Dp>
{
    auto const position = AlignToPixelGridByRound(rect.GetPosition(), view);
    auto const size = AlignToPixelGridByRound(rect.GetSize(), view);
    return Rect<Dp>::Make(position, size);
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
    auto const x = AlignToPixelGridByFloor(point.x, view);
    auto const y = AlignToPixelGridByFloor(point.y, view);
    return Point<Dp>(x, y);
}

auto ViewLayoutFunction::AlignToPixelGridByFloor(Size<Dp> const& size, View const& view) -> Size<Dp>
{
    auto const w = AlignToPixelGridByFloor(size.width, view);
    auto const h = AlignToPixelGridByFloor(size.height, view);
    return Size<Dp>(w, h);
}

auto ViewLayoutFunction::AlignToPixelGridByFloor(Rect<Dp> const& rect, View const& view) -> Rect<Dp>
{
    auto const position = AlignToPixelGridByFloor(rect.GetPosition(), view);
    auto const size = AlignToPixelGridByFloor(rect.GetSize(), view);
    return Rect<Dp>::Make(position, size);
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
    auto const x = AlignToPixelGridByCeil(point.x, view);
    auto const y = AlignToPixelGridByCeil(point.y, view);
    return Point<Dp>(x, y);
}

auto ViewLayoutFunction::AlignToPixelGridByCeil(Size<Dp> const& size, View const& view) -> Size<Dp>
{
    auto const w = AlignToPixelGridByCeil(size.width, view);
    auto const h = AlignToPixelGridByCeil(size.height, view);
    return Size<Dp>(w, h);
}

auto ViewLayoutFunction::AlignToPixelGridByCeil(Rect<Dp> const& rect, View const& view) -> Rect<Dp>
{
    auto const position = AlignToPixelGridByCeil(rect.GetPosition(), view);
    auto const size = AlignToPixelGridByCeil(rect.GetSize(), view);
    return Rect<Dp>::Make(position, size);
}

///
/// @brief Calculate popup position.
///
/// @param sourceRect Rectangle of the source view in global coordinate space.
/// @param popupSize Size of the popup menu.
/// @param screenRect Rectangle of the screen's work area.
/// @param edge Edge of the source rectangle to which the popup is anchored.
/// @param alignment Alignment of the popup relative to the anchor edge.
/// @param rtl True if layout direction is right-to-left.
///
auto ViewLayoutFunction::CalcPopupPosition(
  Rect<Vp> const& sourceRect,
  Size<Vp> const& popupSize,
  Rect<Vp> const& screenRect,
  PopupAnchorEdge const edge,
  PopupAnchorAlignment const alignment,
  Bool const rtl) -> Point<Vp>
{
    if (edge == PopupAnchorEdge::Leading || edge == PopupAnchorEdge::Trailing)
    {
        auto x = Vp(0);
        if ((!rtl && edge == PopupAnchorEdge::Leading) || (rtl && edge == PopupAnchorEdge::Trailing))
        {
            x = sourceRect.x0 - popupSize.width;
            x = Vp::Max(screenRect.x0, x);
        }
        else if ((!rtl && edge == PopupAnchorEdge::Trailing) || (rtl && edge == PopupAnchorEdge::Leading))
        {
            x = sourceRect.x1;
            x = Vp::Min(x, screenRect.x1 - popupSize.width);
        }

        if (rtl)
        {
            x = Vp::Min(x, screenRect.x1 - popupSize.width);
        }
        else
        {
            x = Vp::Max(x, screenRect.x0);
        }

        auto y = Vp(0);
        if (alignment == PopupAnchorAlignment::Start)
        {
            y = sourceRect.y0;
        }
        else if (alignment == PopupAnchorAlignment::Center)
        {
            y = (sourceRect.y0 + sourceRect.y1 - popupSize.height) / 2;
        }
        else if (alignment == PopupAnchorAlignment::End)
        {
            y = sourceRect.y1 - popupSize.height;
            y = Vp::Min(y, screenRect.y1 - popupSize.height);
        }
        y = Vp::Max(y, screenRect.y0);

        return {x, y};
    }
    else if (edge == PopupAnchorEdge::Top || edge == PopupAnchorEdge::Bottom)
    {
        auto y = Vp(0);
        if (edge == PopupAnchorEdge::Top)
        {
            y = sourceRect.y0 - popupSize.height;
        }
        else if (edge == PopupAnchorEdge::Bottom)
        {
            y = sourceRect.y1;
            y = Vp::Min(y, screenRect.y1 - popupSize.height);
        }
        y = Vp::Max(y, screenRect.y0);

        auto x = Vp(0);
        if ((!rtl && alignment == PopupAnchorAlignment::Start) || (rtl && alignment == PopupAnchorAlignment::End))
        {
            x = sourceRect.x0;
            x = Vp::Min(x, screenRect.x1 - popupSize.width);
        }
        else if ((!rtl && alignment == PopupAnchorAlignment::End) || (rtl && alignment == PopupAnchorAlignment::Start))
        {
            x = sourceRect.x1 - popupSize.width;
            x = Vp::Max(x, screenRect.x0);
        }
        else if (alignment == PopupAnchorAlignment::Center)
        {
            x = (sourceRect.x0 + sourceRect.x1 - popupSize.width) / 2;
        }

        if (rtl)
        {
            x = Vp::Min(x, screenRect.x1 - popupSize.width);
        }
        else
        {
            x = Vp::Max(x, screenRect.x0);
        }

        return {x, y};
    }
    else
    {
        FW_DEBUG_LOG_ERROR("Invalid PopupAnchorEdge");
        FW_DEBUG_ASSERT(false);
    }
    return {};
}
}
