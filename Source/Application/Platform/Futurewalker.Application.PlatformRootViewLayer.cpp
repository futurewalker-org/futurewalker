// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformRootViewLayer.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualContext.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisual.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualUpdater.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualPropertyUpdater.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace
{
auto FindViewLayerById(PlatformViewLayerId const id, Shared<PlatformViewLayer> const& layer) -> Shared<PlatformViewLayer>
{
    if (layer->GetId() == id)
    {
        return layer;
    }

    for (auto const& child : layer->GetChildren())
    {
        if (auto const found = FindViewLayerById(id, child.As<PlatformViewLayer>()))
        {
            return found;
        }
    }
    return {};
}

auto GetRasterizingLayer(Shared<PlatformViewLayer> const& layer) -> Shared<PlatformViewLayer>
{
    auto current = layer;
    while (current)
    {
        if (current->ShouldRasterize())
        {
            return current;
        }
        current = current->GetParent();
    }
    return {};
}

auto GetRasterizingBaseLayer(Shared<PlatformViewLayer> const& layer) -> Shared<PlatformViewLayer>
{
    if (layer)
    {
        return GetRasterizingLayer(layer->GetParent());
    }
    return {};
}
}

PlatformRootViewLayer::PlatformRootViewLayer(PassKey<PlatformViewLayer> key, Shared<PlatformViewLayerVisualContext> const& visualContext)
  : PlatformViewLayer(key)
  , _visualContext(visualContext)
{
    if (!_visualContext)
    {
        FW_DEBUG_LOG_ERROR("PlatformRootViewLayer: visualContext is null");
        throw Exception(ErrorCode::InvalidArgument);
    }
}

auto PlatformRootViewLayer::Initialize() -> void
{
    _visual = _visualContext->CreateVisual();
    _visual->SetBaseLayerId(GetId());
    SetClipMode(ViewClipMode::Bounds);
    RebuildVisual(GetSelf());
}

auto PlatformRootViewLayer::Render() -> void
{
    RenderBegin();

    for (auto const& layerId : _layersToRebuild)
    {
        // Check if the layer still exists.
        // TODO: We can cache weak references to avoid tree traversal.
        auto const layer = FindViewLayerById(layerId, GetSelf());
        if (layer)
        {
            RebuildVisual(layer);
        }
    }
    _layersToRebuild.clear();

    if (_shouldUpdateLayer)
    {
        // TODO: Improve algo so that we don't need to O(n) traverse all layers.
        UpdateVisual();
        _shouldUpdateLayer = false;
    }

    auto traverseVisual = [this](this auto self, Shared<PlatformViewLayerVisual> const& visual) -> void {
        if (visual)
        {
            RenderVisual(visual);

            for (auto const& child : visual->GetChildren())
            {
                self(child);
            }
        }
    };
    traverseVisual(_visual);

    RenderEnd();
}

auto PlatformRootViewLayer::RootOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    QueueUpdateLayer();
}

auto PlatformRootViewLayer::RootSizeChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    QueueUpdateLayer();
}

auto PlatformRootViewLayer::RootClipModeChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    QueueUpdateLayer();
}

auto PlatformRootViewLayer::RootOpacityChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    QueueUpdateLayer();
}

auto PlatformRootViewLayer::RootRenderFlagsChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    if (auto const baseLayer = GetRasterizingBaseLayer(layer))
    {
        QueueRebuildLayer(baseLayer->GetId());
    }
}

auto PlatformRootViewLayer::RootDisplayListChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    // TODO: This should not require full update.
    (void)layer;
    QueueUpdateLayer();
}

auto PlatformRootViewLayer::RootDisplayListOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    // TODO: This should not require full update.
    (void)layer;
    QueueUpdateLayer();
}

auto PlatformRootViewLayer::RootChildAdded(Shared<PlatformViewLayer> const& child) -> void
{
    if (auto const baseLayer = GetRasterizingBaseLayer(child))
    {
        QueueRebuildLayer(baseLayer->GetId());
    }
}

auto PlatformRootViewLayer::RootChildRemoved(Shared<PlatformViewLayer> const& parent) -> void
{
    if (auto const layer = GetRasterizingLayer(parent))
    {
        QueueRebuildLayer(layer->GetId());
    }
}

auto PlatformRootViewLayer::FindBaseVisualByBaseLayerId(PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisual>
{
    auto traverse = [](this auto self, Shared<PlatformViewLayerVisual> const& visual, PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisual> {
        if (visual)
        {
            if (visual->GetBaseLayerId() == layerId)
            {
                return visual;
            }

            for (auto const& child : visual->GetChildren())
            {
                if (auto const found = self(child, layerId))
                {
                    return found;
                }
            }
        }
        return {};
    };
    return traverse(_visual, layerId);
}

auto PlatformRootViewLayer::QueueRebuildLayer(PlatformViewLayerId const layerId) -> void
{
    for (auto const& id : _layersToRebuild)
    {
        if (id == layerId)
        {
            return;
        }
    }
    _layersToRebuild.push_back(layerId);
}

auto PlatformRootViewLayer::QueueUpdateLayer() -> void
{
    _shouldUpdateLayer = true;
}

auto PlatformRootViewLayer::RebuildVisual(Shared<PlatformViewLayer> const& baseLayer) -> void
{
    // find visual corresponding to base layer.
    auto const baseVisual = FindBaseVisualByBaseLayerId(baseLayer->GetId());
    // rebuild with builder.
    if (auto updater = Shared<PlatformViewLayerVisualUpdater>::Make(_visualContext))
    {
        updater->Update(baseVisual, baseLayer);
    }
}

auto PlatformRootViewLayer::UpdateVisual() -> void
{
    if (auto updater = Shared<PlatformViewLayerVisualPropertyUpdater>::Make())
    {
        updater->Update(_visual, GetSelf());
    }
}
}
