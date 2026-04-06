// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.SkiaPathDataType.hpp"
#include "Futurewalker.Graphics.PathData.hpp"
#include "Futurewalker.Graphics.PathType.hpp"

#include <include/core/SkPathBuilder.h>

namespace FW_GRAPHICS_DETAIL_NS
{
class SkiaPathData final : public PathData
{
public:
    auto MoveTo(Point<Dp> const& point) -> void override;
    auto LineTo(Point<Dp> const& point) -> void override;
    auto QuadTo(Point<Dp> const& p1, Point<Dp> const& p2) -> void override;
    auto CubicTo(Point<Dp> const& p1, Point<Dp> const& p2, Point<Dp> const& p3) -> void override;
    auto Close() -> void override;
    auto AddRect(Rect<Dp> const& rect) -> void override;
    auto AddRoundRect(RoundRect<Dp> const& roundRect) -> void override;
    auto AddPath(Path const& path) -> void override;
    auto Clone() const -> Shared<PathData> override;
    auto Translate(Vector<Dp> const& offset) -> void override;
    auto IsEmpty() const -> Bool override;
    auto IsEqual(PathData const& other) const noexcept -> Bool override;

    auto GetSkPath() const -> SkPath; 

private:
    SkPathBuilder _path;
};
}
