// SPDX-License-Identifier: MPL-2.0

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
///
auto DrawableViewLayer::Draw(Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& offset) -> Bool
{
    if (_control)
    {
        return _control->Draw(displayList, offset);
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
