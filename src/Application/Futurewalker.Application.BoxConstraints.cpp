// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.BoxConstraints.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto BoxConstraints::MakeUnconstrained() -> BoxConstraints
{
    auto const unconstrained = AxisConstraints::MakeUnconstrained();
    return BoxConstraints(unconstrained, unconstrained);
}

///
/// @brief
///
/// @param width
/// @param height
///
auto BoxConstraints::MakeExact(Dp const width, Dp const height) -> BoxConstraints
{
    return BoxConstraints(AxisConstraints::MakeExact(width), AxisConstraints::MakeExact(height));
}

///
/// @brief
///
/// @param size
///
auto BoxConstraints::MakeExact(Size<Dp> const& size) -> BoxConstraints
{
    return BoxConstraints(AxisConstraints::MakeExact(size.width), AxisConstraints::MakeExact(size.height));
}

///
/// @brief
///
/// @param min
/// @param max
///
auto BoxConstraints::MakeMinMax(Dp const min, Dp const max) -> BoxConstraints
{
    auto const cs = AxisConstraints::MakeMinMax(min, max);
    return BoxConstraints(cs, cs);
}


///
/// @brief
///
/// @param minSize
/// @param maxSize
///
auto BoxConstraints::MakeMinMax(Size<Dp> const& minSize, Size<Dp> const& maxSize) -> BoxConstraints
{
    return BoxConstraints(AxisConstraints::MakeMinMax(minSize.width, maxSize.width), AxisConstraints::MakeMinMax(minSize.height, maxSize.height));
}


///
/// @brief
///
/// @param min
///
auto BoxConstraints::MakeUnbounded(Dp const min) -> BoxConstraints
{
    auto const cs = AxisConstraints::MakeUnbounded(min);
    return BoxConstraints(cs, cs);
}

///
/// @brief
///
/// @param minSize
///
auto BoxConstraints::MakeUnbounded(Size<Dp> const& minSize) -> BoxConstraints
{
    return BoxConstraints(AxisConstraints::MakeUnbounded(minSize.width), AxisConstraints::MakeUnbounded(minSize.height));
}

///
/// @brief
///
/// @param cs
/// @param size
///
auto BoxConstraints::Constrain(BoxConstraints const& cs, Size<Dp> const& size) -> Size<Dp>
{
    return Size<Dp>(AxisConstraints::Constrain(cs._width, size.width), AxisConstraints::Constrain(cs._height, size.height));
}

///
/// @brief Offset the box constraints by the given horizontal and vertical insets.
///
/// @param cs The box constraints to offset.
/// @param horizontal The horizontal offset.
/// @param vertical The vertical offset.
///
/// @return The offset box constraints.
///
auto BoxConstraints::Offset(BoxConstraints const& cs, Dp const horizontal, Dp const vertical) -> BoxConstraints
{
    return BoxConstraints(AxisConstraints::Offset(cs._width, horizontal), AxisConstraints::Offset(cs._height, vertical));
}

///
/// @brief
///
/// @return
///
auto BoxConstraints::GetWidthConstraints() const -> AxisConstraints const&
{
    return _width;
}

///
/// @brief
///
/// @param width
///
/// @return
///
auto BoxConstraints::SetWidthConstraints(AxisConstraints const& width) -> void
{
    _width = width;
}

///
/// @brief
///
/// @return
///
auto BoxConstraints::GetHeightConstraints() const -> AxisConstraints const&
{
    return _height;
}

///
/// @brief
///
/// @param height
///
/// @return
///
auto BoxConstraints::SetHeightConstraints(AxisConstraints const& height) -> void
{
    _height = height;
}

///
/// @brief
///
/// @return
///
auto BoxConstraints::GetMaxSize() const -> Size<Dp>
{
    return Size<Dp>(_width.GetMax(), _height.GetMax());
}

///
/// @brief
///
/// @return
///
auto BoxConstraints::GetMinSize() const -> Size<Dp>
{
    return Size<Dp>(_width.GetMin(), _height.GetMin());
}

///
/// @brief
///
/// @param width
/// @param height
///
BoxConstraints::BoxConstraints(AxisConstraints const& width, AxisConstraints const& height)
  : _width {width}
  , _height {height}
{
}
}
