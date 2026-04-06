// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Path.hpp"
#include "Futurewalker.Graphics.PathData.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
auto Path::MoveTo(Dp const x, Dp const y) -> Path&
{
    GetMutableState().MoveTo({x, y});
    return *this;
}

auto Path::MoveTo(Point<Dp> const& point) -> Path&
{
    GetMutableState().MoveTo(point);
    return *this;
}

auto Path::LineTo(Dp const x, Dp const y) -> Path&
{
    GetMutableState().LineTo({x, y});
    return *this;
}

auto Path::LineTo(Point<Dp> const& point) -> Path&
{
    GetMutableState().LineTo(point);
    return *this;
}

auto Path::QuadTo(Dp const x1, Dp const y1, Dp const x2, Dp const y2) -> Path&
{
    GetMutableState().QuadTo({x1, y1}, {x2, y2});
    return *this;
}

auto Path::QuadTo(Point<Dp> const& p1, Point<Dp> const& p2) -> Path&
{
    GetMutableState().QuadTo(p1, p2);
    return *this;
}

auto Path::CubicTo(Dp const x1, Dp const y1, Dp const x2, Dp const y2, Dp const x3, Dp const y3) -> Path&
{
    GetMutableState().CubicTo({x1, y1}, {x2, y2}, {x3, y3});
    return *this;
}

auto Path::CubicTo(Point<Dp> const& p1, Point<Dp> const& p2, Point<Dp> const& p3) -> Path&
{
    GetMutableState().CubicTo(p1, p2, p3);
    return *this;
}

auto Path::Close() -> Path&
{
    GetMutableState().Close();
    return *this;
}

auto Path::Translate(Dp const dx, Dp const dy) -> Path&
{
    GetMutableState().Translate({dx, dy});
    return *this;
}

auto Path::Translate(Vector<Dp> const& offset) -> Path&
{
    GetMutableState().Translate(offset);
    return *this;
}

auto Path::AddRect(Rect<Dp> const& rect) -> Path&
{
    GetMutableState().AddRect(rect);
    return *this;
}

auto Path::AddRoundRect(RoundRect<Dp> const& roundRect) -> Path&
{
    GetMutableState().AddRoundRect(roundRect);
    return *this;
}

auto Path::AddPath(Path const& path) -> Path&
{
    GetMutableState().AddPath(path);
    return *this;
}

auto Path::IsEmpty() const -> Bool
{
    if (_data)
    {
        return _data->IsEmpty();
    }
    return true;
}

auto Path::GetData() const -> Shared<PathData const>
{
    return _data;
}

auto Path::operator==(Path const& other) const noexcept -> bool
{
    if (_data == other._data)
    {
        return true;
    }

    if (_data && other._data)
    {
        return static_cast<bool>(_data->IsEqual(*other._data));
    }
    return false;
}

auto Path::IsUnique() const -> Bool
{
    return _data.GetUseCount() == 1;
}

auto Path::GetMutableState() -> PathData&
{
    if (!IsUnique())
    {
        _data = _data ? _data->Clone() : PathData::Make();
    }
    return *_data;
}
}
