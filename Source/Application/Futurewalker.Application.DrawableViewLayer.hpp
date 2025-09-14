// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.DrawableViewLayerType.hpp"
#include "Futurewalker.Application.ViewLayer.hpp"
#include "Futurewalker.Application.PlatformDrawableViewLayerControl.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Drawable view layer.
///
class DrawableViewLayer : public ViewLayer
{
public:
    static auto Make(Shared<PlatformViewLayer> const& platformLayer) -> Shared<DrawableViewLayer>;

    auto Draw(Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& offset) -> Bool;

    DrawableViewLayer(PassKey<ViewLayer> key, Shared<PlatformViewLayer> const& platformLayer);

private:
    Shared<PlatformDrawableViewLayerControl> _control;
};
}
}
