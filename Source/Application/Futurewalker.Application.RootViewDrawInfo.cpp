// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.RootViewDrawInfo.hpp"
#include "Futurewalker.Application.ViewLayer.hpp"
#include "Futurewalker.Application.PlatformViewLayerContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
auto RootViewDrawInfo::Initialize() -> void
{
    _layerContext = Locator::Resolve<PlatformViewLayerContext>();
    _layer = ViewLayer::Make(_layerContext->MakeViewLayer());
}

auto RootViewDrawInfo::GetLayer() -> ViewLayer&
{
    return *_layer;
}

auto RootViewDrawInfo::GetLayer() const -> ViewLayer const&
{
    return *_layer;
}

auto RootViewDrawInfo::SetParentLayer(Shared<ViewLayer> parent) -> void
{
    auto currentParent = _parent.Lock();

    if (currentParent != parent)
    {
        if (currentParent)
        {
            currentParent->RemoveChild(_layer);
        }

        _parent = parent;

        if (parent)
        {
            parent->AddChild(_layer, nullptr);
        }
    }
}
}
