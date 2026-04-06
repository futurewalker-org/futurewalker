// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.PathType.hpp"
#include "Futurewalker.Graphics.PathDataType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.Memory.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Path object.
///
class Path
{
public:
    Path() = default;
    Path(Path const&) = default;
    auto operator=(Path const&) -> Path& = default;

    auto MoveTo(Dp const x, Dp const y) -> Path&;
    auto MoveTo(Point<Dp> const& point) -> Path&;

    auto LineTo(Dp const x, Dp const y) -> Path&;
    auto LineTo(Point<Dp> const& point) -> Path&;

    auto QuadTo(Dp const x1, Dp const y1, Dp const x2, Dp const y2) -> Path&;
    auto QuadTo(Point<Dp> const& p1, Point<Dp> const& p2) -> Path&;

    auto CubicTo(Dp const x1, Dp const y1, Dp const x2, Dp const y2, Dp const x3, Dp const y3) -> Path&;
    auto CubicTo(Point<Dp> const& p1, Point<Dp> const& p2, Point<Dp> const& p3) -> Path&;

    auto Close() -> Path&;

    auto Translate(Dp const dx, Dp const dy) -> Path&;
    auto Translate(Vector<Dp> const& offset) -> Path&;

    auto AddRect(Rect<Dp> const& rect) -> Path&;
    auto AddRoundRect(RoundRect<Dp> const& roundRect) -> Path&;
    auto AddPath(Path const& path) -> Path&;

    auto IsEmpty() const -> Bool;

    auto GetData() const -> Shared<PathData const>;

    auto operator==(Path const& other) const noexcept -> bool;
    auto operator!=(Path const& other) const noexcept -> bool = default;;

private:
    auto IsUnique() const -> Bool;
    auto GetMutableState() -> PathData&;

private:
    Shared<PathData> _data;
};
}
}
