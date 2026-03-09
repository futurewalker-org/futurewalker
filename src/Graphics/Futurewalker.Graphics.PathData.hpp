// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.PathDataType.hpp"
#include "Futurewalker.Graphics.PathType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
class PathData : NonCopyable
{
public:
    static auto Make() -> Shared<PathData>;

    virtual ~PathData() = 0;
    virtual auto MoveTo(Point<Dp> const& point) -> void = 0;
    virtual auto LineTo(Point<Dp> const& point) -> void = 0;
    virtual auto QuadTo(Point<Dp> const& p1, Point<Dp> const& p2) -> void = 0;
    virtual auto CubicTo(Point<Dp> const& p1, Point<Dp> const& p2, Point<Dp> const& p3) -> void = 0;
    virtual auto Close() -> void = 0;
    virtual auto AddRect(Rect<Dp> const& rect) -> void = 0;
    virtual auto AddRoundRect(RoundRect<Dp> const& roundRect) -> void = 0;
    virtual auto AddPath(Path const& path) -> void = 0;
    virtual auto Clone() const -> Shared<PathData> = 0;
    virtual auto Translate(Offset<Dp> const& offset) -> void = 0;
    virtual auto IsEmpty() const -> Bool = 0;
    virtual auto IsEqual(PathData const& other) const -> Bool = 0;
};
}
}
