// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaPathData.hpp"
#include "Futurewalker.Graphics.SkiaFunction.hpp"
#include "Futurewalker.Graphics.Path.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
auto SkiaPathData::MoveTo(Point<Dp> const& point) -> void
{
    auto const x = static_cast<SkScalar>(point.GetX());
    auto const y = static_cast<SkScalar>(point.GetY());
    _path.moveTo(x, y);
}

auto SkiaPathData::LineTo(Point<Dp> const& point) -> void
{
    auto const x = static_cast<SkScalar>(point.GetX());
    auto const y = static_cast<SkScalar>(point.GetY());
    _path.lineTo(x, y);
}

auto SkiaPathData::QuadTo(Point<Dp> const& p1, Point<Dp> const& p2) -> void
{
    auto const x1 = static_cast<SkScalar>(p1.GetX());
    auto const y1 = static_cast<SkScalar>(p1.GetY());
    auto const x2 = static_cast<SkScalar>(p2.GetX());
    auto const y2 = static_cast<SkScalar>(p2.GetY());
    _path.quadTo(x1, y1, x2, y2);
}

auto SkiaPathData::CubicTo(Point<Dp> const& p1, Point<Dp> const& p2, Point<Dp> const& p3) -> void
{
    auto const x1 = static_cast<SkScalar>(p1.GetX());
    auto const y1 = static_cast<SkScalar>(p1.GetY());
    auto const x2 = static_cast<SkScalar>(p2.GetX());
    auto const y2 = static_cast<SkScalar>(p2.GetY());
    auto const x3 = static_cast<SkScalar>(p3.GetX());
    auto const y3 = static_cast<SkScalar>(p3.GetY());
    _path.cubicTo(x1, y1, x2, y2, x3, y3);
}

auto SkiaPathData::Close() -> void
{
    _path.close();
}

auto SkiaPathData::AddRect(Rect<Dp> const& rect) -> void
{
    auto const skRect = SkiaFunction::RectToSkRect(rect);
    _path.addRect(skRect);
}

auto SkiaPathData::AddRoundRect(RoundRect<Dp> const& roundRect) -> void
{
    auto const skRRect = SkiaFunction::RoundRectToSkRRect(roundRect);
    _path.addRRect(skRRect);
}

auto SkiaPathData::AddPath(Path const& path) -> void
{
    if (auto const otherSkiaPath = path.GetData().TryAs<SkiaPathData const>())
    {
        _path.addPath(otherSkiaPath->GetSkPath());
    }
}

auto SkiaPathData::Clone() const -> Shared<PathData>
{
    auto clone = Shared<SkiaPathData>::Make();
    clone->_path = _path;
    return clone;
}

auto SkiaPathData::Translate(Vector<Dp> const& offset) -> void
{
    auto const x = static_cast<SkScalar>(offset.x);
    auto const y = static_cast<SkScalar>(offset.y);
    _path.offset(x, y);
}

auto SkiaPathData::IsEmpty() const -> Bool
{
    return _path.isEmpty();
}

auto SkiaPathData::IsEqual(PathData const& other) const noexcept -> Bool
{
    if (auto const otherSkiaPath = Pointer(&other).TryAs<SkiaPathData const>())
    {
        return _path == otherSkiaPath->_path;
    }
    return false;
}

auto SkiaPathData::GetSkPath() const -> SkPath
{
    return _path.snapshot();
}
}
