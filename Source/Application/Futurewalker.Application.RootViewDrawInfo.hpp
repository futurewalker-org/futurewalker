// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.RootViewDrawInfoType.hpp"
#include "Futurewalker.Application.ViewLayerType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"
#include "Futurewalker.Application.PlatformDrawableViewLayerContextType.hpp"

#include "Futurewalker.Geometry.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Draw info of RootView.
///
class RootViewDrawInfo
{
public:
    auto Initialize() -> void;

    auto GetLayer() -> ViewLayer&;
    auto GetLayer() const -> ViewLayer const&;

    auto SetParentLayer(Shared<ViewLayer> parent) -> void;

private:
    Shared<PlatformDrawableViewLayerContext> _layerContext;
    Shared<ViewLayer> _layer;
    Weak<ViewLayer> _parent;
};
}
}
