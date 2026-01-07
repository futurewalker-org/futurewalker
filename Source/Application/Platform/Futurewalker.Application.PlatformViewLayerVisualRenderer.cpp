// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformViewLayerVisualRenderer.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualContext.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisual.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualUpdater.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualPropertyUpdater.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"

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

PlatformViewLayerVisualRenderer::PlatformViewLayerVisualRenderer(Delegate const& delegate, Shared<PlatformViewLayer> const& layer, Shared<PlatformViewLayerVisualContext> const& context)
  : _delegate {delegate}
  , _layer {layer}
  , _context {context}
{
    if (context && layer)
    {
        _visual = context->CreateVisual();
        _visual->SetBaseLayerId(layer->GetId());
        RebuildVisual(GetLayer());
    }
    else
    {
        FW_DEBUG_LOG_ERROR("PlatformRootViewLayer: visualContext is null");
        throw Exception(ErrorCode::InvalidArgument);
    }
}

auto PlatformViewLayerVisualRenderer::Render() -> void
{
    RenderBegin();

    auto c1 = _visual->GetFragmentCount();
    if (_shouldRebuildLayer)
    {
        RebuildVisual(GetLayer());
        _shouldRebuildLayer = false;
    }
    c1 = _visual->GetFragmentCount();

    if (_shouldUpdateLayer)
    {
        // TODO: Improve algo so that we don't need to O(n) traverse all layers.
        UpdateVisual();
        _shouldUpdateLayer = false;
    }
    c1 = _visual->GetFragmentCount();

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

auto PlatformViewLayerVisualRenderer::RequestRebuildLayer() -> void
{
    _shouldRebuildLayer = true;
}

auto PlatformViewLayerVisualRenderer::RequestUpdateLayer() -> void
{
    _shouldUpdateLayer = true;
}

auto PlatformViewLayerVisualRenderer::GetLayer() -> Shared<PlatformViewLayer>
{
    return _layer.Lock();
}

auto PlatformViewLayerVisualRenderer::FindBaseVisualByBaseLayerId(PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisual>
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

auto PlatformViewLayerVisualRenderer::RebuildVisual(Shared<PlatformViewLayer> const& baseLayer) -> void
{
    // find visual corresponding to base layer.
    auto const baseVisual = FindBaseVisualByBaseLayerId(baseLayer->GetId());
    // rebuild with builder.
    if (auto updater = Shared<PlatformViewLayerVisualUpdater>::Make(_context))
    {
        updater->Update(baseVisual, baseLayer);
    }
}

auto PlatformViewLayerVisualRenderer::UpdateVisual() -> void
{
    if (auto updater = Shared<PlatformViewLayerVisualPropertyUpdater>::Make())
    {
        updater->Update(_visual, GetLayer());
    }
}

auto PlatformViewLayerVisualRenderer::RenderBegin() -> void
{
    if (_delegate.renderBegin)
    {
        _delegate.renderBegin();
    }
}

auto PlatformViewLayerVisualRenderer::RenderEnd() -> void
{
    if (_delegate.renderEnd)
    {
        _delegate.renderEnd();
    }
}

auto PlatformViewLayerVisualRenderer::RenderVisual(Shared<PlatformViewLayerVisual> const& visual) -> void
{
    if (_delegate.renderVisual)
    {
        _delegate.renderVisual(visual);
    }
}
}
