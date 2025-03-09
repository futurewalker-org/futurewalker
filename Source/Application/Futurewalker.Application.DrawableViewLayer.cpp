// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.DrawableViewLayer.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"
#include "Futurewalker.Application.PlatformDrawableViewLayerControl.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make view layer.
///
/// @param platformLayer PlatformViewLayer object which provides PlatformDrawableViewLayerControl.
///
auto DrawableViewLayer::Make(Shared<PlatformViewLayer> const& platformLayer) -> Shared<DrawableViewLayer>
{
    return ViewLayer::MakeDerived<DrawableViewLayer>(platformLayer);
}

///
/// @brief Draw display list.
///
/// @param displayList
/// @param offset
/// @param scale
///
auto DrawableViewLayer::Draw(Shared<DisplayList> const& displayList, Offset<Dp> const& offset, Float64 const scale) -> Bool
{
    if (_control)
    {
        return _control->Draw(displayList, offset, scale);
    }
    return false;
}

///
/// @brief Constructor.
///
DrawableViewLayer::DrawableViewLayer(PassKey<ViewLayer> key, Shared<PlatformViewLayer> const& layer)
  : ViewLayer(key, layer)
{
    _control = layer->GetControl().As<PlatformDrawableViewLayerControl>();
}
}
