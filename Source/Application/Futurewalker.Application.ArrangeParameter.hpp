// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.ArrangeParameterType.hpp"

#include "Futurewalker.Graphics.Rect.hpp"
#include "Futurewalker.Graphics.GraphicsUnits.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief 
///
class ArrangeParameter
{
public:
    auto GetOldFrameRect() const -> Rect<Dp>;
    auto SetOldFrameRect(Rect<Dp> const& frameRect) -> void;

    auto GetNewFrameRect() const -> Rect<Dp>;
    auto SetNewFrameRect(Rect<Dp> const& frameRect) -> void;

private:
    Rect<Dp> _oldFrameRect;
    Rect<Dp> _newFrameRect;
};
}
}
