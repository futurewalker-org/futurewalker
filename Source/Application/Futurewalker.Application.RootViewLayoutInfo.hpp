// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.RootViewLayoutInfoType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Layout info of RootView.
///
class RootViewLayoutInfo
{
public:
    auto SetSize(Size<Dp> const& size) -> Bool;
    auto GetSize() const -> Size<Dp>;

private:
    Size<Dp> _size;
};
}
