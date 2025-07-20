// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.BoxConstraints.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief 
///
/// @return 
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
/// @return 
///
auto BoxConstraints::MakeExact(Dp const width, Dp const height) -> BoxConstraints
{
    return BoxConstraints(AxisConstraints::MakeExact(width), AxisConstraints::MakeExact(height));
}

///
/// @brief 
///
/// @param horizontal 
/// @param vertical 
///
/// @return 
///
auto BoxConstraints::MakeMinMax(AxisConstraints const& horizontal, AxisConstraints const& vertical) -> BoxConstraints
{
    return BoxConstraints(horizontal, vertical);
}

///
/// @brief 
///
/// @return 
///
auto BoxConstraints::GetWidthConstraints() const -> AxisConstraints
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
auto BoxConstraints::GetHeightConstraints() const -> AxisConstraints
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
/// @param size 
///
/// @return 
///
auto BoxConstraints::ConstrainSize(Size<Dp> const& size) const -> Size<Dp>
{
    return size; //TODO.
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
